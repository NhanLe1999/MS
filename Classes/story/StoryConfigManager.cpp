#include "StoryConfigManager.h"
#include "MJDefault.h"
#include "NativeBridge.h"
#include "MJPay.h"
#include "MJDebugView.h"
#include "StoryFreeManager.h"
#include "MJPlatformConfig.h"
#include "APPurchaseManager.h"
StoryConfigManager * StoryConfigManager::__instance = nullptr;
bool StoryConfigManager::isLandscapeScene = false;
bool StoryConfigManager::isAppVietNamStories = false;

StoryConfigManager * StoryConfigManager::getInstance()
{
    if (!__instance)
    {
        __instance = new StoryConfigManager();
        isLandscapeScene = NativeBridge::isTabletDevice();
        
        auto package_name = mj::PlatformConfig::getInstance().getPackageName();
        auto found = package_name.find(".vn");
        if (found != std::string::npos) {
            isAppVietNamStories = true;
        }
#if defined(VMONKEY)
		isAppVietNamStories = true;
#endif
    }
    return __instance;
}

bool StoryConfigManager::isCountryVN()
{
    auto saved = MJDEFAULT->getBoolForKey(debugkey_country_gb, false);
    if (saved)
    {
        return !saved;
    }
    CCLOG("isCountryVN = %d", MJDEFAULT->getStringForKey(key_country_code) == "VN");
    return MJDEFAULT->getStringForKey(key_country_code) == "VN";
}

bool StoryConfigManager::isLicenceEnable(){
    auto saved = MJDEFAULT->getBoolForKey(debugkey_license_disable, false);
    if (saved) {
        return !saved;
    }
    return MJDEFAULT->getBoolForKey(key_is_licence);
}


bool StoryConfigManager::isAppActive(){
    if (MJDEFAULT->getBoolForKey(debugkey_test_free, false)) {
        return false;
    }

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    if (MJDEFAULT->getBoolForKey("mjdk_full_features", false))
    {
        return true;
    }

#endif
    auto expire_time = mj::Pay::getInstance().getTimeExpire();
    auto time_current = MJDEFAULT->getIntegerForKey(key_time_curent);

    if (FREE_STORY_MANAGER->getRateCanReadWithTimeExpire() || APPURCHASE_MANAGER->getTypeUser() != IAP::user::TYPE_USERS::USER_EXPRIED)
    {
        return true;
    }
    return false;
}

bool StoryConfigManager::isVietnameseStories() {
    if (isAppVNStories()){
        return true;
    }
    return MJDEFAULT->getBoolForKey(debugkey_language_vn);
}

bool StoryConfigManager::isLearnAllLessons() {
    return MJDEFAULT->getBoolForKey(debugkey_all_lessons);
}

bool StoryConfigManager::isTestActVmonkey() {
    return MJDEFAULT->getBoolForKey(debugkey_test_act_vmonkey);
}
bool StoryConfigManager::isGameVMonkey() {
    if (isVietnameseStories() || isTestActVmonkey()) return true;
    return false;
}

bool StoryConfigManager::isAutoPlay() {
    return MJDEFAULT->getBoolForKey(debugkey_auto_play);
}

bool StoryConfigManager::isTurnOnABTesting()
{
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    return MJDEFAULT->getBoolForKey("debugkey_turn_on_A_B_testing", false);
#else
    return true;
#endif
}


#pragma mark  - SETUP_CONFIG_APP
bool StoryConfigManager::isTestStory(){//Nội dung config.
//    if (isAppVNStories()){
//        return true;
//    }
    return MJDEFAULT->getBoolForKey(debugkey_test_config, false);
}

bool StoryConfigManager::isTestSubscribe(){
    return MJDEFAULT->getBoolForKey(debugkey_full_features, false);
}

bool StoryConfigManager::isHomeScreenLandscape() {
    return true;
//    auto saved = MJDEFAULT->getBoolForKey(debugkey_homescreen_landscape, false);
//    if (saved) {
//        return saved;
//    }
//    return isLandscapeScene;
}

bool StoryConfigManager::isBetaVersion(){
    return false;
}

bool StoryConfigManager::isQuickPlayGame(){
    return false;
}

bool StoryConfigManager::isTestGame(){
    return MJDEFAULT->getBoolForKey(debugkey_quick_play_game, false);
}

bool StoryConfigManager::isTest7Day(){
    return MJDEFAULT->getBoolForKey(debugkey_test_7_days, false);
}

bool StoryConfigManager::isTestFreeTimeFake() {
    return MJDEFAULT->getBoolForKey(debugkey_fake_free_time, false);
}

bool StoryConfigManager::isDevEnvironment() {
    return MJDEFAULT->getBoolForKey(debugkey_dev_environment, false);
}

bool StoryConfigManager::isProductEnvironment() {
    return MJDEFAULT->getBoolForKey(debugkey_product_environment, false);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
bool StoryConfigManager::isFixOpenglWin32(){
    return false;
}
#endif

bool StoryConfigManager::isAppVNStories(){
    if (isAppVietNamStories) {
        return true;
    }
    return MJDEFAULT->getBoolForKey(debugkey_language_vn, false);
}

bool StoryConfigManager::isCountryThailand()
{
    //return true;
    CCLOG("isThaiLand = %d", MJDEFAULT->getStringForKey(key_country_code) == "TH");
    return MJDEFAULT->getStringForKey(key_country_code) == "TH";
}
bool StoryConfigManager::isCountryIndonesia()
{
    return MJDEFAULT->getStringForKey(key_country_code) == "ID";
}
