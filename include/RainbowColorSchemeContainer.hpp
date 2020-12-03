#pragma once
#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/Color.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/ColorScheme.hpp"
#include "GlobalNamespace/SimpleColorSO.hpp"

DECLARE_CLASS_CODEGEN(RainbowMod, RainbowColorSchemeContainer, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, Awake);
    public: static void SetDefaults();

    DECLARE_METHOD(UnityEngine::Color, get_saberAColor);
    DECLARE_METHOD(UnityEngine::Color, get_saberBColor);
    DECLARE_METHOD(GlobalNamespace::SimpleColorSO*, get_environmentColor0SO);
    DECLARE_METHOD(GlobalNamespace::SimpleColorSO*, get_environmentColor1SO);
    DECLARE_METHOD(UnityEngine::Color, get_obstaclesColor);

    DECLARE_METHOD(UnityEngine::Color, GetSaberColorForType, GlobalNamespace::ColorType type);

    DECLARE_STATIC_FIELD(float, saberColorAHue);
    DECLARE_STATIC_FIELD(float, saberColorBHue);
    DECLARE_STATIC_FIELD(float, obstaclesColorHue);
    DECLARE_STATIC_FIELD(float, environmentColor0Hue);
    DECLARE_STATIC_FIELD(float, environmentColor1Hue);
    DECLARE_STATIC_FIELD(bool, enabled);
    DECLARE_STATIC_FIELD(UnityEngine::Color, saberAColor);
    DECLARE_STATIC_FIELD(UnityEngine::Color, saberBColor);
    DECLARE_STATIC_FIELD(UnityEngine::Color, obstaclesColor);
    DECLARE_STATIC_FIELD(GlobalNamespace::SimpleColorSO*, environmentColor0SO);
    DECLARE_STATIC_FIELD(GlobalNamespace::SimpleColorSO*, environmentColor1SO);

    REGISTER_FUNCTION(RainbowColorSchemeContainer,
        REGISTER_METHOD(Update);
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(get_saberAColor);
        REGISTER_METHOD(get_saberBColor);
        REGISTER_METHOD(get_environmentColor0SO);
        REGISTER_METHOD(get_environmentColor1SO);
        REGISTER_METHOD(get_obstaclesColor);
        REGISTER_METHOD(GetSaberColorForType);

        REGISTER_FIELD(saberColorAHue);
        REGISTER_FIELD(saberColorBHue);
        REGISTER_FIELD(obstaclesColorHue);
        REGISTER_FIELD(environmentColor0Hue);
        REGISTER_FIELD(environmentColor1Hue);

        REGISTER_FIELD(saberAColor);
        REGISTER_FIELD(saberBColor);
        REGISTER_FIELD(obstaclesColor);
        REGISTER_FIELD(environmentColor0SO);
        REGISTER_FIELD(environmentColor1SO);
        REGISTER_FIELD(enabled);
    )
)