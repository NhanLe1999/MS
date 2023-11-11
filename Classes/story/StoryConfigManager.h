#ifndef __StoryConfigManager__
#define __StoryConfigManager__
#define CONFIG_MANAGER StoryConfigManager::getInstance()

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class StoryConfigManager {
public:
    static StoryConfigManager * getInstance();
    static StoryConfigManager * __instance;
    static bool isLandscapeScene;
    static bool isAppVietNamStories;
    
    bool isVietnameseStories();
    bool isLearnAllLessons();
    
    bool isTestStory();
    bool isTestSubscribe();
    bool isBetaVersion();
    bool isHomeScreenLandscape();
    bool isCountryVN();
    bool isLicenceEnable();
    bool isQuickPlayGame();
    bool isTestGame();
    bool isAppActive();
    bool isTest7Day();
    bool isTestFreeTimeFake();
//    bool isDomainDaybehoc();
    bool isDevEnvironment();
    bool isProductEnvironment();
    bool isTestActVmonkey();
    bool isGameVMonkey();
    bool isAutoPlay();
    bool isTurnOnABTesting();
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    bool isFixOpenglWin32();
#endif
    bool isAppVNStories();
    
    bool isCountryThailand();
    bool isCountryIndonesia();
};

#endif
