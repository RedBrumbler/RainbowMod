#include "RainbowColorSchemeContainer.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "UnityEngine/Resources.hpp"
#include "System/Action.hpp"
#include "config.hpp"

DEFINE_CLASS(RainbowMod::RainbowColorSchemeContainer);
extern config_t Config;

static int colorMod;

Array<GlobalNamespace::ColorManager*>* RainbowMod::RainbowColorSchemeContainer::colorManagers;

float RainbowMod::RainbowColorSchemeContainer::saberColorAHue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::saberColorBHue = Config.SabersStartDiff;
float RainbowMod::RainbowColorSchemeContainer::obstaclesColorHue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::environmentColor0Hue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::environmentColor1Hue = Config.LightsStartDiff;
bool RainbowMod::RainbowColorSchemeContainer::enabled = false;

UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::saberAColor;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::saberBColor;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::obstaclesColor;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::environmentColor0;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::environmentColor1;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::environmentColor0Boost;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::environmentColor1Boost;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentColor0SO;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentColor1SO;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentBoost0SO;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentBoost1SO;

const Logger& getContainerLogger()
{
    static const Logger logger((ModInfo){"RainbowModContainer", "0.0.1"});
    return logger;
}

namespace RainbowMod
{
    void RainbowColorSchemeContainer::SetDefaults()
    {
        saberColorAHue = 0.0f;
        saberColorBHue = Config.SabersStartDiff;
        obstaclesColorHue = 0.0f;
        environmentColor0Hue = 0.0f;
        environmentColor1Hue = Config.LightsStartDiff;
        saberAColor = UnityEngine::Color(168.0f/255.0f, 32.0f/255.0f, 32.0f/255.0f, 1.0f); //RED
        saberBColor = UnityEngine::Color(32.0f/255.0f, 100.0f/255.0f, 168.0f/255.0f, 1.0f); //BLUE
        obstaclesColor = saberAColor;
        environmentColor0SO = UnityEngine::ScriptableObject::CreateInstance<GlobalNamespace::SimpleColorSO*>();
        environmentColor1SO = UnityEngine::ScriptableObject::CreateInstance<GlobalNamespace::SimpleColorSO*>();
        environmentBoost0SO = UnityEngine::ScriptableObject::CreateInstance<GlobalNamespace::SimpleColorSO*>();
        environmentBoost1SO = UnityEngine::ScriptableObject::CreateInstance<GlobalNamespace::SimpleColorSO*>();
        environmentColor0SO->SetColor(saberAColor);
        environmentColor1SO->SetColor(saberBColor);

        float h;
        float s;
        float v;
        UnityEngine::Color::RGBToHSV(saberAColor, h, s, v);
        environmentColor0Boost = UnityEngine::Color::HSVToRGB(h, Config.BoostSaturation, Config.BoostValue);
        environmentBoost0SO->SetColor(environmentColor0Boost);

        UnityEngine::Color::RGBToHSV(saberAColor, h, s, v);
        environmentColor1Boost = UnityEngine::Color::HSVToRGB(h, Config.BoostSaturation, Config.BoostValue);
        environmentBoost1SO->SetColor(environmentColor1Boost);
        
    }

    void RainbowColorSchemeContainer::Awake()
    {
        SetDefaults();
    }

    void RainbowColorSchemeContainer::Update()
    {
        if (!enabled) return;
        
        if (Config.RainbowSetMod > 1)
        {
            colorMod ++;
            colorMod %= Config.RainbowSetMod;
            if (colorMod != 0) return;
        }

        if (Config.Sabers || Config.Notes || Config.Trails)
        {
            saberColorAHue = fmod(saberColorAHue+Config.SaberASpeed, 360.0f);
            saberColorBHue = fmod(saberColorBHue+Config.SaberBSpeed, 360.0f);

            saberAColor = UnityEngine::Color::HSVToRGB(saberColorAHue / 360.0f, 1.0f, 1.0f);
            saberBColor = UnityEngine::Color::HSVToRGB(saberColorBHue / 360.0f, 1.0f, 1.0f);
        }
        if (Config.Walls)
        {
            obstaclesColorHue = fmod(obstaclesColorHue+Config.WallsSpeed, 360.0f);

            obstaclesColor = UnityEngine::Color::HSVToRGB(obstaclesColorHue / 360.0f, 1.0f, 1.0f);
        }

        if (Config.Lights)
        {
            environmentColor0Hue = fmod(environmentColor0Hue+Config.LightASpeed, 360.0f);
            environmentColor1Hue = fmod(environmentColor1Hue+Config.LightBSpeed, 360.0f);

            environmentColor0 = UnityEngine::Color::HSVToRGB(environmentColor0Hue / 360.0f, 1.0f, 1.0f);
            environmentColor1 = UnityEngine::Color::HSVToRGB(environmentColor1Hue / 360.0f, 1.0f, 1.0f);
            environmentColor0Boost = UnityEngine::Color::HSVToRGB(environmentColor0Hue / 360.0f, Config.BoostSaturation, Config.BoostValue);
            environmentColor1Boost = UnityEngine::Color::HSVToRGB(environmentColor1Hue / 360.0f, Config.BoostSaturation, Config.BoostValue);

            environmentColor0SO->SetColor(environmentColor0);
            environmentColor1SO->SetColor(environmentColor1);
            environmentBoost0SO->SetColor(environmentColor0Boost);
            environmentBoost1SO->SetColor(environmentColor1Boost);
        }

        if (Config.Notes || Config.Walls || Config.Lights)
        {
            colorManagers = UnityEngine::Object::FindObjectsOfType<GlobalNamespace::ColorManager*>();
            for (int i = 0; i < colorManagers->Length(); i++)
            {
                if (Config.Notes)
                {
                    colorManagers->values[i]->saberAColor->SetColor(saberAColor);
                    colorManagers->values[i]->saberBColor->SetColor(saberBColor);
                }
                if (Config.Walls)
                {
                    colorManagers->values[i]->obstaclesColor->SetColor(obstaclesColor);
                }
                if (Config.Lights)
                {
                    colorManagers->values[i]->environmentColor0->SetColor(environmentColor0);
                    colorManagers->values[i]->environmentColor1->SetColor(environmentColor1);
                    colorManagers->values[i]->environmentColor0Boost->SetColor(environmentColor0Boost);
                    colorManagers->values[i]->environmentColor1Boost->SetColor(environmentColor1Boost);
                }
            }
        }
    }

    UnityEngine::Color RainbowColorSchemeContainer::GetSaberColorForType(GlobalNamespace::ColorType type)
    {
        switch (type.value)
        {
            case 0:
                return get_saberAColor();
                break;
            case 1:
                return get_saberBColor();
                break;
            default:
                return UnityEngine::Color::get_black();
                break;
        }
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_saberAColor()
    {
        return saberAColor;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_saberBColor()
    {   
        return saberBColor;
    }

    GlobalNamespace::SimpleColorSO* RainbowColorSchemeContainer::get_environmentColor0SO()
    {
        return environmentColor0SO;
    }

    GlobalNamespace::SimpleColorSO* RainbowColorSchemeContainer::get_environmentColor1SO()
    {
        return environmentColor1SO;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_obstaclesColor()
    {
        return obstaclesColor;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_environmentColor0()
    {
        return environmentColor0;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_environmentColor1()
    {
        return environmentColor1;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_environmentColor0Boost()
    {
        return environmentColor0Boost;
    }

    UnityEngine::Color RainbowColorSchemeContainer::get_environmentColor1Boost()
    {
        return environmentColor1Boost;
    }

}