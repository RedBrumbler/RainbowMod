#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "RainbowColorSchemeContainer.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/NoteDebris.hpp"

DECLARE_CLASS_CODEGEN(RainbowMod, RainbowManager, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, SetSaberColor);
    DECLARE_METHOD(void, SetWallColor);
    DECLARE_METHOD(void, SetNoteColor);
    DECLARE_METHOD(void, SetDebrisColor);
    DECLARE_METHOD(void, SetQSaberColor);

    DECLARE_STATIC_FIELD(bool, enabled);

    DECLARE_INSTANCE_FIELD_DEFAULT(int, managerType, 0);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::ObstacleController*, wallController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::GameNoteController*, noteController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberModelController*, saberController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::NoteDebris*, noteDebris, nullptr);
    DECLARE_STATIC_FIELD(RainbowMod::RainbowColorSchemeContainer*, colorSchemeContainer);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::ColorType, colorType, 0);

    REGISTER_FUNCTION(RainbowManager,
        REGISTER_METHOD(Update);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(SetSaberColor);
        REGISTER_METHOD(SetQSaberColor);
        REGISTER_METHOD(SetDebrisColor);
        REGISTER_METHOD(SetNoteColor);
        REGISTER_METHOD(SetWallColor);
        REGISTER_FIELD(managerType);
        REGISTER_FIELD(wallController);
        REGISTER_FIELD(noteController);
        REGISTER_FIELD(saberController);
        REGISTER_FIELD(noteDebris);
        REGISTER_FIELD(colorType);
        REGISTER_FIELD(colorSchemeContainer);
        REGISTER_FIELD(enabled);
    )
)