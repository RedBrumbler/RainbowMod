#include "RainbowColorSchemeContainer.hpp"
#include "UnityEngine/ScriptableObject.hpp"
#include "config.hpp"

DEFINE_CLASS(RainbowMod::RainbowColorSchemeContainer);
extern config_t Config;

float RainbowMod::RainbowColorSchemeContainer::saberColorAHue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::saberColorBHue = Config.SabersStartDiff;
float RainbowMod::RainbowColorSchemeContainer::obstaclesColorHue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::environmentColor0Hue = 0.0f;
float RainbowMod::RainbowColorSchemeContainer::environmentColor1Hue = Config.LightsStartDiff;
bool RainbowMod::RainbowColorSchemeContainer::enabled = false;

UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::saberAColor;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::saberBColor;
UnityEngine::Color RainbowMod::RainbowColorSchemeContainer::obstaclesColor;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentColor0SO;
GlobalNamespace::SimpleColorSO* RainbowMod::RainbowColorSchemeContainer::environmentColor1SO;

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
        environmentColor0SO->SetColor(saberAColor);
        environmentColor1SO->SetColor(saberBColor);
    }

    void RainbowColorSchemeContainer::Awake()
    {
        SetDefaults();
    }

    void RainbowColorSchemeContainer::Update()
    {
        if (!enabled) return;
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

            environmentColor0SO->SetColor(UnityEngine::Color::HSVToRGB(environmentColor0Hue / 360.0f, 1.0f, 1.0f));
            environmentColor1SO->SetColor(UnityEngine::Color::HSVToRGB(environmentColor1Hue / 360.0f, 1.0f, 1.0f));
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

}