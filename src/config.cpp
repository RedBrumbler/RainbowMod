#include "config.hpp"

static ModInfo modInfo = {ID, VERSION};
extern const Logger& getLogger();
config_t Config;

Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

void SaveConfig() {
    getLogger().info("Saving Configuration...");
    getConfig().config.RemoveAllMembers();
    getConfig().config.SetObject();
    rapidjson::Document::AllocatorType& allocator = getConfig().config.GetAllocator();
    getConfig().config.AddMember("Lights", Config.Lights, allocator);
    getConfig().config.AddMember("Walls", Config.Walls, allocator);
    getConfig().config.AddMember("Sabers", Config.Sabers, allocator);
    getConfig().config.AddMember("Trails", Config.Trails, allocator);
    getConfig().config.AddMember("Notes", Config.Notes, allocator);
    getConfig().config.AddMember("SaberASpeed", Config.SaberASpeed, allocator);
    getConfig().config.AddMember("SaberBSpeed", Config.SaberBSpeed, allocator);
    getConfig().config.AddMember("SabersStartDiff", Config.SabersStartDiff, allocator);
    getConfig().config.AddMember("LightASpeed", Config.LightASpeed, allocator);
    getConfig().config.AddMember("LightBSpeed", Config.LightBSpeed, allocator);
    getConfig().config.AddMember("LightsStartDiff", Config.LightsStartDiff, allocator);
    getConfig().config.AddMember("BoostValue", Config.BoostValue, allocator);
    getConfig().config.AddMember("BoostSaturation", Config.BoostSaturation, allocator);
    getConfig().config.AddMember("WallsSpeed", Config.WallsSpeed, allocator);
    getConfig().Write();
    getLogger().info("Saved Configuration!");
}

bool LoadConfig() { 
    getLogger().info("Loading Configuration...");
    getConfig().Load();
    bool foundEverything = true;
    if(getConfig().config.HasMember("Lights") && getConfig().config["Lights"].IsBool()){
        Config.Lights = getConfig().config["Lights"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("Walls") && getConfig().config["Walls"].IsBool()){
        Config.Walls = getConfig().config["Walls"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("Sabers") && getConfig().config["Sabers"].IsBool()){
        Config.Sabers = getConfig().config["Sabers"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("Trails") && getConfig().config["Trails"].IsBool()){
        Config.Trails = getConfig().config["Trails"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("Notes") && getConfig().config["Notes"].IsBool()){
        Config.Notes = getConfig().config["Notes"].GetBool();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("SaberASpeed") && getConfig().config["SaberASpeed"].IsDouble()){
        Config.SaberASpeed = getConfig().config["SaberASpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("SaberBSpeed") && getConfig().config["SaberBSpeed"].IsDouble()){
        Config.SaberBSpeed = getConfig().config["SaberBSpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("SabersStartDiff") && getConfig().config["SabersStartDiff"].IsDouble()){
        Config.SabersStartDiff = getConfig().config["SabersStartDiff"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("WallsSpeed") && getConfig().config["WallsSpeed"].IsDouble()){
        Config.WallsSpeed = getConfig().config["WallsSpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("LightASpeed") && getConfig().config["LightASpeed"].IsDouble()){
        Config.LightASpeed = getConfig().config["LightASpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("LightBSpeed") && getConfig().config["LightBSpeed"].IsDouble()){
        Config.LightBSpeed = getConfig().config["LightBSpeed"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("BoostValue") && getConfig().config["BoostValue"].IsDouble()){
        Config.BoostValue = getConfig().config["BoostValue"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("BoostSaturation") && getConfig().config["BoostSaturation"].IsDouble()){
        Config.BoostSaturation = getConfig().config["BoostSaturation"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(getConfig().config.HasMember("LightsStartDiff") && getConfig().config["LightsStartDiff"].IsDouble()){
        Config.LightsStartDiff = getConfig().config["LightsStartDiff"].GetDouble();    
    }else{
        foundEverything = false;
    }
    if(foundEverything){
        getLogger().info("Loaded Configuration!");
        return true;
    }
    return false;
}