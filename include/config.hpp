#pragma once 
#include "../extern/beatsaber-hook/shared/config/rapidjson-utils.hpp"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/rapidjson/include/rapidjson/document.h"
#include "modloader/shared/modloader.hpp"

typedef struct _Config_t {
    bool Lights = false;
    bool Walls = false;
    bool Sabers = false;
    bool Trails = false;
    bool Notes = false;
    bool Qbloqs = false;
    bool Qwalls = false;
    bool QSabers = false;
    double SaberASpeed = 1.0;
    double SaberBSpeed = 1.0;
    double SabersStartDiff = 180.0;
    double LightASpeed = 2.0;
    double LightBSpeed = 2.0;
    double LightsStartDiff = 180.0;
    double BoostValue = 1.0f;
    double BoostSaturation = 0.7f;
    double WallsSpeed = 2.0;
} config_t;

void SaveConfig();
bool LoadConfig();
Configuration& getConfig();