#include "RainbowManager.hpp"
#include "config.hpp"

#include "GlobalNamespace/ColorSO.hpp"
#include "GlobalNamespace/GameNoteController_GameNoteType.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorNoteVisuals.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberTrail.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor_PropertyTintColorPair.hpp"
#include "GlobalNamespace/Parametric3SliceSpriteController.hpp"
#include "GlobalNamespace/MaterialPropertyBlockController.hpp"

#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/Shader.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "UnityEngine/SpriteRenderer.hpp"

DEFINE_CLASS(RainbowMod::RainbowManager);

RainbowMod::RainbowColorSchemeContainer* RainbowMod::RainbowManager::colorSchemeContainer = nullptr;
bool RainbowMod::RainbowManager::enabled;

extern config_t Config;

bool IDinited = false;
int colorID, otherColorID, bloomID, glowID, customColorID;

const Logger& getManagerLogger()
{
    static const Logger logger((ModInfo){"RainbowModManager", "0.0.1"});
    return logger;
}

namespace RainbowMod
{
    void RainbowManager::Awake()
    {
        colorID = UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Color"));
        otherColorID = UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_OtherColor"));
        glowID = UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Glow"));
        customColorID = UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_CustomColors"));
        bloomID = UnityEngine::Shader::PropertyToID(il2cpp_utils::createcsstr("_Bloom"));
        cached = false;
        cached00 = false;
        cached01 = false;
        cached10 = false;
        cached11 = false;
        cachedld = false;
        cachedrd = false;
        ccmaterials = nullptr;
        leftDot = nullptr;
        leftArrow = nullptr;
        rightDot = nullptr;
        rightArrow = nullptr;
        leftDebris = nullptr;
        rightDebris = nullptr;

        colorSchemeContainer = UnityEngine::Object::FindObjectOfType<RainbowMod::RainbowColorSchemeContainer*>();
        if (!colorSchemeContainer)
        {
            UnityEngine::GameObject* newObj = UnityEngine::GameObject::New_ctor();
            newObj->DontDestroyOnLoad(newObj);
            newObj->AddComponent<RainbowMod::RainbowColorSchemeContainer*>();
        }
        if (GlobalNamespace::SaberModelController* saber = get_gameObject()->GetComponent<GlobalNamespace::SaberModelController*>())
        {
            saberController = saber;
            this->managerType = 1;
            this->colorType = get_transform()->get_parent()->get_gameObject()->GetComponent<GlobalNamespace::Saber*>()->get_saberType().value;
        }
        else if (GlobalNamespace::GameNoteController* note = get_gameObject()->GetComponent<GlobalNamespace::GameNoteController*>())
        {
            noteController = note;
            this->managerType = 2;
            this->colorType = note->noteData->colorType.value;
        }
        else if (GlobalNamespace::ObstacleController* obstacle = get_gameObject()->GetComponent<GlobalNamespace::ObstacleController*>())
        {
            wallController = obstacle;
            this->managerType = 3;
            this->colorType = 0;
        }
        else if (GlobalNamespace::NoteDebris* debris = get_gameObject()->GetComponent<GlobalNamespace::NoteDebris*>())
        {
            noteDebris = debris;
            this->managerType = 4;
        }
        else if (GlobalNamespace::LightSwitchEventEffect* effect = get_gameObject()->GetComponent<GlobalNamespace::LightSwitchEventEffect*>())
        {
            lightEffect = effect;
            this->managerType = 5;
        }
    }

    void RainbowManager::Update()
    {
        if (!enabled) return;
        switch (this->managerType)
        {
            case 1: // saber
                SetSaberColor();
                break;
            case 2: // note
                SetNoteColor();
                break;
            case 3: // wall
                SetWallColor();
                break;
            case 4:
                SetDebrisColor();
                break;
            case 5:
                SetLightEffectColor();
                break;
        }
    }

    void RainbowManager::SetLightEffectColor()
    {
        if (Config.Lights)
        {
            lightEffect->lightColor0 = colorSchemeContainer->get_environmentColor0SO();
            lightEffect->lightColor1 = colorSchemeContainer->get_environmentColor1SO();
            lightEffect->highlightColor0 = colorSchemeContainer->get_environmentColor0SO();
            lightEffect->highlightColor1 = colorSchemeContainer->get_environmentColor1SO();
        }
    }

    void RainbowManager::SetQSaberColor()
    {
        UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
        if (Config.Sabers)
        {
            SetColorOnRenderers(get_transform()->get_parent()->get_gameObject(), &ccmaterials);
        }
        if (Config.Trails)
        {
            Array<GlobalNamespace::SaberTrail*>* trails = GetComponentsInChildren<GlobalNamespace::SaberTrail*>();

            if (trails)
                for (int i = 0; i < trails->Length(); i++)
                {
                    trails->values[i]->color = theColor.get_linear();
                }
        }
    }

    void RainbowManager::SetSaberColor()
    {
        if (Config.QSabers)
        {
            SetQSaberColor();
            return;
        }
        else
        {
            UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
            if (Config.Sabers)
            {
                Array<GlobalNamespace::SetSaberFakeGlowColor*>* fakeGlowColor = saberController->setSaberFakeGlowColors;
                if (fakeGlowColor)
                {
                    for (int i = 0; i < fakeGlowColor->Length(); i++)
                    {
                        fakeGlowColor->values[i]->parametric3SliceSprite->color = theColor;
                        fakeGlowColor->values[i]->parametric3SliceSprite->Refresh();
                    }
                }

                Array<GlobalNamespace::SetSaberGlowColor*>* glowColor = saberController->setSaberGlowColors;
                if (glowColor)
                {
                    for (int i = 0; i < glowColor->Length(); i++)
                    {
                        Array<GlobalNamespace::SetSaberGlowColor::PropertyTintColorPair*>* pairs = glowColor->values[i]->propertyTintColorPairs;
                        
                        for (int j = 0; j < pairs->Length(); j++)
                        {
                            glowColor->values[i]->materialPropertyBlock->SetColor(pairs->values[j]->property, theColor);
                        }
                        glowColor->values[i]->meshRenderer->SetPropertyBlock(glowColor->values[i]->materialPropertyBlock);
                    }
                }
            }
            if (Config.Trails)
            {
                saberController->saberTrail->color = theColor.get_linear();
            }
        }
        
    }

    void RainbowManager::SetDebrisColor()
    {
        if (Config.Notes)
        {
            UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
            if (Config.Qbloqs)
            {
                SetColorOnRenderers(get_gameObject(), &ccmaterials);
            }
            else
            {
                noteDebris->materialPropertyBlockController->materialPropertyBlock->SetColor(GlobalNamespace::NoteDebris::_get__colorID(), theColor);
            }
        }
    }

    void RainbowManager::SetNoteColor()
    {
        if (Config.Notes)
        {
            UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(noteController->noteData->colorType);
            if (Config.Qbloqs)
            {
                SetColorOnBloqRenderers(get_gameObject());
            }
            else
            {
                GlobalNamespace::ColorNoteVisuals* visuals = GetComponent<GlobalNamespace::ColorNoteVisuals*>();
                UnityEngine::Color arrowCol = theColor;
                UnityEngine::Color restCol = theColor;
                arrowCol.a = visuals->arrowGlowIntensity;
                restCol.a = 1.0f;
                visuals->arrowGlowSpriteRenderer->set_color(arrowCol);
                visuals->circleGlowSpriteRenderer->set_color(theColor);
                Array<GlobalNamespace::MaterialPropertyBlockController*>* materialPropertyBlockControllers = visuals->materialPropertyBlockControllers;
                for (int i = 0; i < materialPropertyBlockControllers->Length(); i++)
                {
                    materialPropertyBlockControllers->values[i]->materialPropertyBlock->SetColor(GlobalNamespace::ColorNoteVisuals::_get__colorId(), restCol);
                    materialPropertyBlockControllers->values[i]->ApplyChanges();
                }
            }
        }
    }

    void RainbowManager::SetWallColor()
    {
        if (Config.Walls)
        {
            if (Config.Qwalls)
            {
                SetColorOnRenderers(get_gameObject(), &ccmaterials);
            }
            else
            {
                float w = wallController->stretchableObstacle->obstacleFrame->width;
                float h = wallController->stretchableObstacle->obstacleFrame->height;
                float l = wallController->stretchableObstacle->obstacleFrame->length;
                wallController->stretchableObstacle->SetSizeAndColor(w, h, l, colorSchemeContainer->get_obstaclesColor());
            }
        }
    }

    bool RainbowManager::ShouldCCMaterial(UnityEngine::Material* mat)
    {
        std::string matName = to_utf8(csstrtostr(mat->get_name()));
        getManagerLogger().info("checking material %s", matName.c_str());
        if (mat->HasProperty(customColorID))
        {
            return (mat->GetFloat(customColorID) > 0.0f);
        }
        else
        {
            if (mat->HasProperty(glowID))
            {
                return (mat->GetFloat(glowID) > 0.0f);
            }
            else
            {
                if (mat->HasProperty(bloomID))
                {
                    return (mat->GetFloat(bloomID) > 0.0f);
                }
                else // if that property does not exist
                {
                    bool hasReplaceName = (matName.find("_replace") != std::string::npos); // if mat has _replace in the name
                    if (hasReplaceName)
                    {
                        if (matName.find("_noCC") == std::string::npos) // if the mat does not have "_noCC" in its name
                            return true;
                    } 
                }
            } 
        }
        return false;
    }

    void RainbowManager::SetColorOnDebrisRenderers(UnityEngine::GameObject* obj)
    {
        std::string lookingFor = "NoteDebrisMesh/custom";
        switch (this->colorType)
        {
            case 0:
                lookingFor += "LeftDebris";
                break;
            case 1:
                lookingFor += "RightDebris";
                break;
            default: return;
        }


        UnityEngine::Transform* customDebris = get_transform()->Find(il2cpp_utils::createcsstr(lookingFor));
        if (!customDebris) // if no custom debris is added to the bloq :)
        {
            UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
            noteDebris->materialPropertyBlockController->materialPropertyBlock->SetColor(GlobalNamespace::NoteDebris::_get__colorID(), theColor);
            return;
        }

        if (this->colorType == 1)
        {
            if (cachedrd)
            {
                cached = true;
                SetColorOnRenderers(customDebris->get_gameObject(), &rightDebris);
            }
            else
            {
                cached = false;
                SetColorOnRenderers(customDebris->get_gameObject(), &rightDebris);
                cachedrd = true;
            }
        }
        else // left debris
        {
            if (cachedld)
            {
                cached = true;
                SetColorOnRenderers(customDebris->get_gameObject(), &leftDebris);
            }
            else
            {
                cached = false;
                SetColorOnRenderers(customDebris->get_gameObject(), &leftDebris);
                cachedld = true;
            }
        }

    }

    void RainbowManager::SetColorOnBloqRenderers(UnityEngine::GameObject* obj)
    {
        std::string lookingFor = "NoteCube/custom";
        int type1 = 0;
        int dot = 0;
        switch (noteController->noteData->colorType)
        {
            case 0:
                lookingFor += "Left";
                type1 = 0;
                break;
            case 1:
                lookingFor += "Right";
                type1 = 1;
                break;
            default: return;
        }

        switch (noteController->noteData->cutDirection)
        {
            case 0: //Up
            case 1: //Down
            case 2: //Left
            case 3: //Right
            case 4: //UpLeft
            case 5: //UpRight
            case 6: //DownLeft
            case 7: //DownRight
            case 9: //None  
                lookingFor += "Arrow";
                break;
            case 8: //Any
                lookingFor += "Dot";
                dot = 1;
                break;
        }

        UnityEngine::Transform* customNote = get_transform()->Find(il2cpp_utils::createcsstr(lookingFor));
        if (!customNote) return; // if not found, don't try to use

        if (type1) // if right
        {
            if (dot)
            {
                if (cached11)
                {
                    cached = true;
                    SetColorOnRenderers(customNote->get_gameObject(), &rightDot);
                }
                else
                {
                    cached = false;
                    SetColorOnRenderers(customNote->get_gameObject(), &rightDot);
                    cached11 = true;
                }
            }
            else // if arrow
            {
                if (cached10)
                {
                    cached = true;
                    SetColorOnRenderers(customNote->get_gameObject(), &rightArrow);
                }
                else
                {
                    cached = false;
                    SetColorOnRenderers(customNote->get_gameObject(), &rightArrow);
                    cached10 = true;
                }
            }
        }
        else // if left
        {
            if (dot)
            {
                if (cached01)
                {
                    cached = true;
                    SetColorOnRenderers(customNote->get_gameObject(), &leftDot);
                }
                else
                {
                    cached = false;
                    SetColorOnRenderers(customNote->get_gameObject(), &leftDot);
                    cached01 = true;
                }
            }
            else // if arrow
            {
                if (cached00)
                {
                    cached = true;
                    SetColorOnRenderers(customNote->get_gameObject(), &leftArrow);
                }
                else
                {
                    cached = false;
                    SetColorOnRenderers(customNote->get_gameObject(), &leftArrow);
                    cached00 = true;
                }
            }
        }
    }

    void RainbowManager::SetColorOnRenderers(UnityEngine::GameObject* obj, Array<UnityEngine::Material*>** cache)
    {
        if (cached) 
        {
            if (!cache) return;
            if (!*cache) return;
            int type1 = noteController ? noteController->noteData->colorType : this->colorType;
            UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(type1);
            UnityEngine::Color theOtherColor = colorSchemeContainer->GetSaberColorForType(1 - type1);
             
            for (int i = 0; i < (*cache)->Length(); i++)
            {
                (*cache)->values[i]->SetColor(colorID, theColor);
                if ((*cache)->values[i]->HasProperty(otherColorID)) (*cache)->values[i]->SetColor(otherColorID, theOtherColor);
            }
            return;
        }
        else
        {
            std::vector<UnityEngine::Material*> ccmats;
            Array<UnityEngine::Renderer*>* renderers = obj->GetComponentsInChildren<UnityEngine::Renderer*>();

            for (int i = 0; i < renderers->Length(); i++)
            {
                Array<UnityEngine::Material*>* materials = renderers->values[i]->get_sharedMaterials();
                for (int j = 0; j < materials->Length(); j++)
                {
                    if (materials->values[j] == nullptr) continue;
                    else if (ShouldCCMaterial(materials->values[j])) 
                                ccmats.push_back(materials->values[j]);
                }
            }

            (*cache) = il2cpp_utils::vectorToArray(ccmats);
            cached = true;
            SetColorOnRenderers(obj, cache);
        }
    }
}