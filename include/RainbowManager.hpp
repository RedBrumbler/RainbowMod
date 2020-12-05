#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "RainbowColorSchemeContainer.hpp"
#include "GlobalNamespace/ObstacleController.hpp"
#include "GlobalNamespace/GameNoteController.hpp"
#include "GlobalNamespace/SaberModelController.hpp"
#include "GlobalNamespace/NoteDebris.hpp"
#include "GlobalNamespace/LightSwitchEventEffect.hpp"
#include "GlobalNamespace/TubeBloomPrePassLight.hpp"
#include "GlobalNamespace/SaberTrail.hpp"


DECLARE_CLASS_CODEGEN(RainbowMod, RainbowManager, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, SetSaberColor);
    DECLARE_METHOD(void, SetWallColor);
    DECLARE_METHOD(void, SetNoteColor);
    DECLARE_METHOD(void, SetDebrisColor);
    DECLARE_METHOD(void, SetQSaberColor);
    DECLARE_METHOD(void, SetColorOnRenderers, UnityEngine::GameObject* obj, Array<UnityEngine::Material*>** cache);
    DECLARE_METHOD(void, SetColorOnBloqRenderers, UnityEngine::GameObject* obj);
    DECLARE_METHOD(void, SetColorOnDebrisRenderers, UnityEngine::GameObject* obj);
    DECLARE_METHOD(bool, ShouldCCMaterial, UnityEngine::Material* mat);

    public: static void ResetCaches();

    DECLARE_STATIC_FIELD(bool, enabled);
    DECLARE_STATIC_FIELD(bool, hasMaster);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, isMaster, false);
    
    DECLARE_INSTANCE_FIELD_DEFAULT(int, managerType, 0);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::ObstacleController*, wallController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::GameNoteController*, noteController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::SaberModelController*, saberController, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::NoteDebris*, noteDebris, nullptr);

    DECLARE_STATIC_FIELD(RainbowMod::RainbowColorSchemeContainer*, colorSchemeContainer);
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::ColorType, colorType, 0);
    DECLARE_INSTANCE_FIELD_DEFAULT(Array<UnityEngine::Material*>*, ccmaterials, nullptr);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, leftDot);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, rightDot);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, leftArrow);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, rightArrow);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, leftDebris);
    DECLARE_STATIC_FIELD(Array<UnityEngine::Material*>*, rightDebris);
    DECLARE_INSTANCE_FIELD_DEFAULT(Array<GlobalNamespace::SaberTrail*>*, saberTrails, nullptr);

    DECLARE_METHOD(void, OnDestroy);

    public: bool cached = false;
    public: static bool cached00;
    public: static bool cached01;
    public: static bool cached10;
    public: static bool cached11;
    public: static bool cachedld;
    public: static bool cachedrd;
    public: static bool allCached;

    REGISTER_FUNCTION(RainbowManager,
        REGISTER_METHOD(Update);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(SetSaberColor);
        REGISTER_METHOD(SetQSaberColor);
        REGISTER_METHOD(SetDebrisColor);
        REGISTER_METHOD(SetNoteColor);
        REGISTER_METHOD(SetWallColor);
        REGISTER_METHOD(SetColorOnRenderers);
        REGISTER_METHOD(SetColorOnBloqRenderers);
        REGISTER_METHOD(ShouldCCMaterial);
        REGISTER_METHOD(OnDestroy);
        REGISTER_FIELD(managerType);
        REGISTER_FIELD(wallController);
        REGISTER_FIELD(noteController);
        REGISTER_FIELD(saberController);
        REGISTER_FIELD(noteDebris);
        REGISTER_FIELD(colorType);
        REGISTER_FIELD(colorSchemeContainer);
        REGISTER_FIELD(enabled);
        REGISTER_FIELD(isMaster);
        REGISTER_FIELD(hasMaster);
        REGISTER_FIELD(saberTrails);
    )
)