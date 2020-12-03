#include "SettingsViewController.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "config.hpp"


#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/RectTransform.hpp"
#include "UnityEngine/Vector2.hpp"
#include "UnityEngine/UI/Image.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/Toggle_ToggleEvent.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/Events/UnityAction.hpp"
#include "UnityEngine/Events/UnityAction_1.hpp"
#include "HMUI/ScrollView.hpp"
#include "HMUI/ModalView.hpp"
#include "HMUI/Touchable.hpp"

#include "questui/shared/BeatSaberUI.hpp"
#include "questui/shared/CustomTypes/Components/ExternalComponents.hpp"
#include "questui/shared/CustomTypes/Components/Backgroundable.hpp"

#include <cstdlib>

using namespace QuestUI;
using namespace UnityEngine;
using namespace UnityEngine::UI;
using namespace UnityEngine::Events;
using namespace HMUI;

extern config_t Config;

DEFINE_CLASS(RainbowMod::SettingsViewController);

void RainbowMod::SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling)
{
    if (firstActivation)
    {
        get_gameObject()->AddComponent<Touchable*>();
        GameObject* container = BeatSaberUI::CreateScrollableSettingsContainer(get_transform());
        BeatSaberUI::CreateToggle(container->get_transform(), "Rainbow Lights", Config.Lights, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](SettingsViewController* view, bool value) { 
                Config.Lights = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Left Light Speed", 2, 0.2f, Config.LightASpeed, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.LightASpeed = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Right Light Speed", 2, 0.2f, Config.LightBSpeed, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.LightBSpeed = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Light Start Difference", 2, 5.0f, Config.LightsStartDiff, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.LightsStartDiff = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateToggle(container->get_transform(), "Rainbow Walls", Config.Walls, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](SettingsViewController* view, bool value) { 
                Config.Walls = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Wall Speed", 2, 0.2f, Config.WallsSpeed, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.WallsSpeed = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateToggle(container->get_transform(), "Rainbow Sabers", Config.Sabers, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](SettingsViewController* view, bool value) { 
                Config.Sabers = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateToggle(container->get_transform(), "Rainbow Trails", Config.Trails, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](SettingsViewController* view, bool value) { 
                Config.Trails = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateToggle(container->get_transform(), "Rainbow Notes", Config.Notes, il2cpp_utils::MakeDelegate<UnityAction_1<bool>*>(classof(UnityAction_1<bool>*), this, +[](SettingsViewController* view, bool value) { 
                Config.Notes = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Left Saber Speed", 2, 0.2f, Config.SaberASpeed, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.SaberASpeed = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Right Saber Speed", 2, 0.2f, Config.SaberBSpeed, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.SaberBSpeed = value;
                SaveConfig();
            }));
        BeatSaberUI::CreateIncrementSetting(container->get_transform(), "Saber Start Difference", 2, 5.0f, Config.SabersStartDiff, il2cpp_utils::MakeDelegate<UnityAction_1<float>*>(classof(UnityAction_1<float>*), this, +[](SettingsViewController* view, float value) {
                Config.SabersStartDiff = value;
                SaveConfig();
            }));
    }
}