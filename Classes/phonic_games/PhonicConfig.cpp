#include "PhonicConfig.h"
PhonicConfig * PhonicConfig::__instance = NULL;
PhonicConfig * PhonicConfig::getInstance() {
    if (!__instance) {
        __instance = new PhonicConfig();
    }
    return __instance;
}

std::string PhonicConfig::getGameFontName(std::string game_type){
    if (CONFIG_MANAGER->isGameVMonkey()){
        return "fonts/Linotte Regular.ttf";
    }
    return "fonts/phonicfont.ttf";
}

bool PhonicConfig::isAutoPlayGame(){
    return false;
}

void PhonicConfig::cacheGAF(std::string key, std::string gaf_path){
    auto gaf_asset = gaf::GAFAsset::create(gaf_path, nullptr);
    gaf_asset->retain();
    gaf_cache.insert(std::pair<std::string,gaf::GAFAsset *>(key, gaf_asset));
}

void PhonicConfig::clearCacheGaf(){
    for (auto gaf : gaf_cache) {
        gaf.second->release();
    }
    gaf_cache.clear();
}

void PhonicConfig::releaseUnusedAssets() {
    for (auto it = gaf_cache.begin(); it != gaf_cache.end(); ) {
        gaf::GAFAsset * asset = it->second;
        CCLOG("cocos2d: GAFAnimation: ref count: %d", asset->getReferenceCount());
        if (asset->getReferenceCount() == 1) {
            CCLOG("cocos2d: GAFAnimation: removing unused assets: %s", it->first.c_str());
            asset->release();
            it = gaf_cache.erase(it);
        } else {
            ++it;
        }
    }
}
#include "../external/json/document.h"
void PhonicConfig::getGameMap(){
    std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile("games.json");
    rapidjson::Document document;
    if (document.Parse<0>(str.c_str()).HasParseError())
        return;
    
    auto games_list = document["games"].GetArray();
    for (int i = 0; i < games_list.Size(); i++) {
        auto id = games_list[i]["game_id"].GetInt();
        game_map[id] = games_list[i]["title"].GetString();
    }
}

std::string PhonicConfig::getPhonicGameName(int game_id){
    return game_map[game_id];
}

