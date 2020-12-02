#include "RainbowManager.hpp"
#include "config.hpp"

#include "GlobalNamespace/GameNoteController_GameNoteType.hpp"
#include "GlobalNamespace/StretchableObstacle.hpp"
#include "GlobalNamespace/ParametricBoxFrameController.hpp"
#include "GlobalNamespace/Saber.hpp"
#include "GlobalNamespace/SaberType.hpp"
#include "GlobalNamespace/SetSaberFakeGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor.hpp"
#include "GlobalNamespace/SetSaberGlowColor_PropertyTintColorPair.hpp"
#include "GlobalNamespace/Parametric3SliceSpriteController.hpp"
#include "UnityEngine/MaterialPropertyBlock.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "qosmetics/shared/QosmeticsColorSetting.hpp"

DEFINE_CLASS(RainbowMod::RainbowManager);

RainbowMod::RainbowColorSchemeContainer* RainbowMod::RainbowManager::colorSchemeContainer = nullptr;
bool RainbowMod::RainbowManager::enabled = false;

extern config_t Config;
const Logger& getManagerLogger()
{
    static const Logger logger((ModInfo){"RainbowModManager", "0.0.1"});
    return logger;
}

namespace RainbowMod
{
    void RainbowManager::Awake()
    {
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
            this->managerType = 0;
            this->colorType = get_transform()->get_parent()->get_gameObject()->GetComponent<GlobalNamespace::Saber*>()->get_saberType().value;
        }
        if (GlobalNamespace::GameNoteController* note = get_gameObject()->GetComponent<GlobalNamespace::GameNoteController*>())
        {
            noteController = note;
            this->managerType = 1;
            this->colorType = note->get_gameNoteType().value;
        }
        if (GlobalNamespace::ObstacleController* obstacle = get_gameObject()->GetComponent<GlobalNamespace::ObstacleController*>())
        {
            wallController = obstacle;
            this->managerType = 2;
            this->colorType = 0;
        }
    }

    void RainbowManager::Update()
    {
        if (!enabled) return;
        switch (this->managerType)
        {
            case 0: // saber
                SetSaberColor();
                break;
            case 1: // note
                SetNoteColor();
                break;
            case 2: // wall
                SetWallColor();
                break;
        }
    }

    void RainbowManager::SetQSaberColor()
    {
        UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
        if (Config.Sabers)
        {
            switch(this->colorType.value)
            {
                case 0:
                    Qosmetics::QosmeticsColors::SetLeftSaberColor(theColor);
                    break;
                case 1:
                    Qosmetics::QosmeticsColors::SetRightSaberColor(theColor);
                    break;
            }
        }
        if (Config.Trails)
        {
            switch(this->colorType.value)
            {
                case 0:
                    Qosmetics::QosmeticsColors::SetLeftTrailColor(theColor);
                    break;
                case 1:
                    Qosmetics::QosmeticsColors::SetRightTrailColor(theColor);
                    break;
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
            if (Config.Sabers)
            {
                UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
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

            }
        }
        
    }

    void RainbowManager::SetQNoteColor()
    {
        if (Config.Notes)
        {
            
        }
    }

    void RainbowManager::SetNoteColor()
    {
        UnityEngine::Color theColor = colorSchemeContainer->GetSaberColorForType(this->colorType);
        if (Config.Notes)
        {
            if (Config.Qbloqs)
            {
                switch(this->colorType.value)
                {
                    case 0:
                        Qosmetics::QosmeticsColors::SetLeftNoteColor(theColor);
                        break;
                    case 1:
                        Qosmetics::QosmeticsColors::SetRightNoteColor(theColor);
                        break;
                }
            }
            else
            {

            }
        }
    }

    void RainbowManager::SetWallColor()
    {
        if (Config.Walls)
        {
            if (Config.Qwalls)
            {
                Qosmetics::QosmeticsColors::SetObstacleColor(colorSchemeContainer->get_obstaclesColor());
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
}