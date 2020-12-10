#include "RainbowManager.hpp"
#include "config.hpp"

#include "GlobalNamespace/ColorSO.hpp"
#include "GlobalNamespace/GameNoteController_GameNoteType.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorNoteVisuals.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/Saber.hpp"
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
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::leftDot = nullptr;
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::leftArrow = nullptr;
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::rightDot = nullptr;
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::rightArrow = nullptr;
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::leftDebris = nullptr;
Array<UnityEngine::Material*>* RainbowMod::RainbowManager::rightDebris = nullptr;

bool RainbowMod::RainbowManager::cached00 = false;
bool RainbowMod::RainbowManager::cached01 = false;
bool RainbowMod::RainbowManager::cached10 = false;
bool RainbowMod::RainbowManager::cached11 = false;
bool RainbowMod::RainbowManager::cachedld = false;
bool RainbowMod::RainbowManager::cachedrd = false;
bool RainbowMod::RainbowManager::allCached = false;
bool RainbowMod::RainbowManager::hasMaster = false;

extern config_t Config;

bool IDinited = false;
int colorID, otherColorID, bloomID, glowID, customColorID;
static int colorMod = 0;
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
            this->saberTrails = nullptr;
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

            if (!hasMaster) 
            {
                hasMaster = true;
                isMaster = true;
            }
        }
    }

    void RainbowManager::Update()
    {
        if (!enabled) return;

        if (Config.RainbowSetMod > 1)
        {
            colorMod ++;
            colorMod %= Config.RainbowSetMod;
            if (colorMod != 0) return;
        }


        if ((noteDebris || noteController) && !hasMaster)
        {
            isMaster = true;
            hasMaster = true;
        }

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
            if (!this->saberTrails) saberTrails = get_transform()->get_parent()->get_gameObject()->GetComponentsInChildren<GlobalNamespace::SaberTrail*>();
            //if (this->saberTrails && this->saberTrails->Length() == 0) get_transform()->get_parent()->get_gameObject()->GetComponentsInChildren<GlobalNamespace::SaberTrail*>();
            //saberTrails = get_transform()->get_parent()->get_gameObject()->GetComponentsInChildren<GlobalNamespace::SaberTrail*>();
            
            if (this->saberTrails)
                for (int i = 0; i < this->saberTrails->Length(); i++)
                {
                    this->saberTrails->values[i]->color = theColor.get_linear();
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
                SetColorOnDebrisRenderers(get_gameObject());
            }
            else
            {
                noteDebris->materialPropertyBlockController->materialPropertyBlock->SetColor(GlobalNamespace::NoteDebris::_get__colorID(), theColor);
                noteDebris->materialPropertyBlockController->ApplyChanges();
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
                GlobalNamespace::ColorNoteVisuals* visuals = get_gameObject()->GetComponent<GlobalNamespace::ColorNoteVisuals*>();
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
                getManagerLogger().info("Logging walls getting their color set because hammer is being a dumdum and wants it");
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
        if (!mat->HasProperty(colorID) && !mat->HasProperty(otherColorID)) return false; // if the material does not have a color property, there is no use in checking if it should CC
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
            noteDebris->materialPropertyBlockController->ApplyChanges();
            switch (this->colorType)
            {
                case 0:
                    cachedld = true;
                    break;
                case 1:
                    cachedrd = true;
                    break;
            }
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
            if (noteController || noteDebris)
            {
                if (!isMaster) return;

                UnityEngine::Color leftColor = colorSchemeContainer->GetSaberColorForType(0);
                UnityEngine::Color rightColor = colorSchemeContainer->GetSaberColorForType(1);

                if (cached00 && leftArrow) // left arrow
                {
                    for (int i = 0; i < leftArrow->Length(); i++)
                    {
                        leftArrow->values[i]->SetColor(colorID, leftColor);
                        if (leftArrow->values[i]->HasProperty(otherColorID)) leftArrow->values[i]->SetColor(otherColorID, rightColor);
                    }
                }

                if (cached01 && leftDot) // left dot
                {
                    for (int i = 0; i < leftDot->Length(); i++)
                    {
                        leftDot->values[i]->SetColor(colorID, leftColor);
                        if (leftDot->values[i]->HasProperty(otherColorID)) leftDot->values[i]->SetColor(otherColorID, rightColor);
                    }
                }

                if (cached10 && rightArrow) // right arrow
                {
                    for (int i = 0; i < rightArrow->Length(); i++)
                    {
                        rightArrow->values[i]->SetColor(colorID, rightColor);
                        if (rightArrow->values[i]->HasProperty(otherColorID)) rightArrow->values[i]->SetColor(otherColorID, leftColor);
                    }
                }

                if (cached11 && rightDot) // right dot
                {
                    for (int i = 0; i < rightDot->Length(); i++)
                    {
                        rightDot->values[i]->SetColor(colorID, rightColor);
                        if (rightDot->values[i]->HasProperty(otherColorID)) rightDot->values[i]->SetColor(otherColorID, leftColor);
                    }
                }

                if (cachedld && leftDebris)
                {
                    for (int i = 0; i < leftDebris->Length(); i++)
                    {
                        leftDebris->values[i]->SetColor(colorID, leftColor);
                        if (leftDebris->values[i]->HasProperty(otherColorID)) leftDebris->values[i]->SetColor(otherColorID, rightColor);
                    }
                }

                if (cachedrd && rightDebris)
                {
                    for (int i = 0; i < rightDebris->Length(); i++)
                    {
                        rightDebris->values[i]->SetColor(colorID, rightColor);
                        if (rightDebris->values[i]->HasProperty(otherColorID)) rightDebris->values[i]->SetColor(otherColorID, leftColor);
                    }
                }
                return;
            }
            else
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
        }
        else
        {
            std::vector<UnityEngine::Material*> ccmats;
            Array<UnityEngine::Renderer*>* renderers = obj->GetComponentsInChildren<UnityEngine::Renderer*>();
            if (renderers)
            {
                for (int i = 0; i < renderers->Length(); i++)
                {
                    Array<UnityEngine::Material*>* materials = renderers->values[i]->get_sharedMaterials();
                    if (!materials) continue;
                    for (int j = 0; j < materials->Length(); j++)
                    {
                        if (materials->values[j] == nullptr) continue;
                        else if (ShouldCCMaterial(materials->values[j])) 
                                    ccmats.push_back(materials->values[j]);
                    }
                }
            }
            (*cache) = il2cpp_utils::vectorToArray(ccmats);
            getManagerLogger().info("cache pointer %p", (*cache));
            cached = true;
            SetColorOnRenderers(obj, cache);
        }
    }

    void RainbowManager::OnDisable()
    {
        if (isMaster)
        {
            hasMaster = false;
            isMaster = false;
        }
    }

    void RainbowManager::ResetCaches()
    {
        leftDot = nullptr;
        leftArrow = nullptr;
        rightDot = nullptr;
        rightArrow = nullptr;
        leftDebris = nullptr;
        rightDebris = nullptr;

        cached00 = false;
        cached01 = false;
        cached10 = false;
        cached11 = false;
        cachedld = false;
        cachedrd = false;

        allCached = false;
        hasMaster = false;
        
    }

    void RainbowManager::OnDestroy()
    {
        if (isMaster) hasMaster = false;
    }
}