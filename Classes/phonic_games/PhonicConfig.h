#ifndef __PhonicConfig__
#define __PhonicConfig__
#define PHONIC_MANAGER PhonicConfig::getInstance()

#include <stdio.h>
#include "cocos2d.h"
#include "PhonicInfoModel.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
using namespace cocos2d;
using namespace std;

class PhonicConfig {
public:
    static PhonicConfig * getInstance();
    static PhonicConfig * __instance;
    
    std::string getGameFontName(std::string game_type = GAME_TYPE_PHONIC);
    bool isAutoPlayGame();
    
    void releaseUnusedAssets();
    void clearCacheGaf();
    void cacheGAF(std::string key, std::string gaf_path);
    std::map<std::string, gaf::GAFAsset *> gaf_cache;
    std::string getPhonicGameName(int game_id);
    void getGameMap();
private:
    std::map<int, std::string> game_map;
};

#endif
