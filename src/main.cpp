#include "config.hpp"
#include "SettingsViewController.hpp"
#include "RainbowManager.hpp"
#include "RainbowColorSchemeContainer.hpp"
#include "qosmetics/shared/QosmeticsColorSetting.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/SceneManagement/Scene.hpp"

#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/Saber.hpp"

#include "questui/shared/QuestUI.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "custom-types/shared/register.hpp"

extern config_t Config;

static ModInfo modInfo;

const Logger& getLogger()
{
    static const Logger& logger(modInfo);
    return logger;
}

bool getSceneName(UnityEngine::SceneManagement::Scene scene, std::string& output)
{
    Il2CppString* csString = UnityEngine::SceneManagement::Scene::GetNameInternal(scene.m_Handle);
    RET_0_UNLESS(csString);
    output = to_utf8(csstrtostr(csString));
    return true; 
}

std::string activeSceneName = "";
MAKE_HOOK_OFFSETLESS(SceneManager_SetActiveScene, bool, UnityEngine::SceneManagement::Scene scene)
{
    getSceneName(scene, activeSceneName);
    getLogger().info("Found scene %s", activeSceneName.c_str());
    if (activeSceneName == "GameCore")
    {
        RainbowMod::RainbowColorSchemeContainer::SetDefaults();
        RainbowMod::RainbowManager::enabled = true;
        RainbowMod::RainbowColorSchemeContainer::enabled = true;
    }
    else
    {
        RainbowMod::RainbowManager::enabled = false;
        RainbowMod::RainbowColorSchemeContainer::enabled = false;
    }
    return SceneManager_SetActiveScene(scene);
}

MAKE_HOOK_OFFSETLESS(Saber_Start, void, GlobalNamespace::Saber* self)
{
    Saber_Start(self);
}

MAKE_HOOK_OFFSETLESS(SaberModelController_Init, void, GlobalNamespace::SaberModelController* self, UnityEngine::Transform* parent, GlobalNamespace::Saber* saber)
{
    getLogger().info("SaberModelController Init");
    SaberModelController_Init(self, parent, saber);
    if (activeSceneName == "GameCore")
    {
        if (!self->get_gameObject()->GetComponent<RainbowMod::RainbowManager*>()) 
                self->get_gameObject()->AddComponent<RainbowMod::RainbowManager*>();
    }
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;

    modInfo = info;
}

extern "C" void load()
{
    if(!LoadConfig())
        SaveConfig();
    il2cpp_functions::Init();
    QuestUI::Init();

    getLogger().info("Starting RainbowMod installation...");

    INSTALL_HOOK_OFFSETLESS(SceneManager_SetActiveScene, il2cpp_utils::FindMethodUnsafe("UnityEngine.SceneManagement", "SceneManager", "SetActiveScene", 1));
    //INSTALL_HOOK_OFFSETLESS(TutorialController_Start, il2cpp_utils::FindMethodUnsafe("", "TutorialController", "Start", 0));
    //INSTALL_HOOK_OFFSETLESS(TutorialController_OnDestroy, il2cpp_utils::FindMethodUnsafe("", "TutorialController", "OnDestroy", 0));
    //INSTALL_HOOK_OFFSETLESS(ColorManager_ColorForNoteType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "ColorForType", 1));
    //INSTALL_HOOK_OFFSETLESS(ColorManager_ColorForSaberType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "ColorForSaberType", 1));
    //INSTALL_HOOK_OFFSETLESS(ColorManager_EffectsColorForSaberType, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "EffectsColorForSaberType", 1));
    //INSTALL_HOOK_OFFSETLESS(ColorManager_GetObstacleEffectColor, il2cpp_utils::FindMethodUnsafe("", "ColorManager", "GetObstacleEffectColor", 0));
    //INSTALL_HOOK_OFFSETLESS(SaberManager_RefreshSabers, il2cpp_utils::FindMethodUnsafe("", "SaberManager", "RefreshSabers", 0));
    //INSTALL_HOOK_OFFSETLESS(SaberManager_Update, il2cpp_utils::FindMethodUnsafe("", "SaberManager", "Update", 0));
    INSTALL_HOOK_OFFSETLESS(SaberModelController_Init, il2cpp_utils::FindMethodUnsafe("", "SaberModelController", "Init", 2));
    INSTALL_HOOK_OFFSETLESS(Saber_Start, il2cpp_utils::FindMethodUnsafe("", "Saber", "Start", 0));
    //INSTALL_HOOK_OFFSETLESS(GameNoteController_Update, il2cpp_utils::FindMethodUnsafe("", "GameNoteController", "Update", 0));
    //INSTALL_HOOK_OFFSETLESS(ObstacleController_Update, il2cpp_utils::FindMethodUnsafe("", "ObstacleController", "Update", 0));
    custom_types::Register::RegisterType<RainbowMod::SettingsViewController>();
    custom_types::Register::RegisterType<RainbowMod::RainbowColorSchemeContainer>();
    custom_types::Register::RegisterType<RainbowMod::RainbowManager>();

    RainbowMod::RainbowColorSchemeContainer::SetDefaults();
    RainbowMod::RainbowManager::enabled = false;
    RainbowMod::RainbowColorSchemeContainer::enabled = false;

    QuestUI::Register::RegisterModSettingsViewController<RainbowMod::SettingsViewController*>((ModInfo){"Rainbow Mod", VERSION}, "Rainbow Mod");

    getLogger().info("Successfully installed RainbowMod!");
}