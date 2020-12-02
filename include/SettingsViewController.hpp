#pragma once
#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

DECLARE_CLASS_CODEGEN(RainbowMod, SettingsViewController, HMUI::ViewController,
    // Override DidActivate, which is called whenever you enter the menu
    
    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3), bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling);
    REGISTER_FUNCTION(SettingsViewController,
        REGISTER_METHOD(DidActivate);
    )
)