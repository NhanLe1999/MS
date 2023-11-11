#include "AppDelegate.h"
#include "mjlib/Common.h"
#include "CocoStudioMacro.h"
#include "HSLib.h"

#include "TruyenTranhPage.h"
#include "TextStory.h"
#include "PageMenuControl.h"
#include "StoryInfoCover.h"
#include "StoryEndingPage.h"
#include "StoryPageVC.h"
#include "StoryRatePopUp.h"
#include "StoryAlert.h"
//#include "MSAISpeakingPermissionPopup.h"
#include "MSAISpeakingSharePopup.h"
#include "StoryWelcomePopUp.h"
#include "StoryLevelInfo.h"
#include "StoryReadingLevel.h"
#include "StoryLevelInfo_Guide.h"

#include "MJPayPopup.h"
#include "MJPayPopup_OnePay.h"
#include "MJPayPopup_COD.h"
#include "MJPayPopup_BankTransfer.h"
#include "MJPayPopup_Store.h"
#include "MJLicensePopup.h"
#include "MJSubscriptionPage.h"
#include "MJSubscriptionItem.h"
#include "MJFirebaseListener.h"

#include "StoryParentPage_Landscape.h"
#include "StoryParentHeader_Landscape.h"
#include "StoryParentItem_Landscape.h"
#include "StoryParentSupport_Landscape.h"
#include "StoryParentLicense_Landscape.h"
#include "StoryParentCoupon_Landscape.h"
#include "AppInbox.h"

#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryLanguageManager.h"
#include "StoryTheEndPage.h"
#include "StoryDataModel.h"
#include "MSAudiobookPlayer.h"
#include "IAPManager.h"
#include "MJVerifyStoreAPI.h"
#include "AISpeakingScene.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)||(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "AppsFlyer/AppsFlyerX.h"
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MacCppBridge.h"
#endif

#include "MJPlatformConfig.h"
#include "MJDefinitions.h"

#include "GameMakeAMatch.h"
#include "GameLandThePlane.h"
#include "GameJumpingFrog.h"
#include "GameQA.h"

#include "MJPay.h"

#include "MJAPIRefresh.h"
#include "MJ_PageLoadResource.h"
#include "MJPayGuide.h"
#include "StoryFeedback.h"
#include "MJPayPopup_BankInfo.h"
#include "MSIntro.h"
#include "MSIntroGrade.h"
#include "story/popup/ViralPopup/ViralPopup.h"
#include "story/popup/ViralPopup/ViralPopup_Item.h"
#include "story/popup/ViralPopup/ViralReceiveGift.h"

#include "MSEventLogger.h"
#include "MMQRScan.h"
#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "APProfileManager.h"
#include "MSPlaylistHelper.h"
#include "MSLanguageSelection.h"
#include "StoryPageVC.h"
#include "MSLessonPreview.h"
#include "MSTabSubscribe.h"
#include "MSHome.h"
#include "APOTPPopup.h"
#include "ProScore.h"
#include "StoryPageVC.h"
#include "AppsFlyerManager.h"
#include "SplashScene.h"
#include "MSNewFirebase.h"

// this function will be called when the app is active again
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "firebase/analytics.h"
#include "firebase/dynamic_links.h"
#endif
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    MJDEFAULT->deleteValueForKey(key_error_url_stories);
    MJDEFAULT->deleteValueForKey(key_Limit_download_thumb);
    MJDEFAULT->deleteValueForKey(key_count_retry);
    MJDEFAULT->deleteValueForKey(key_speed_internet);
    MJDEFAULT->deleteValueForKey(key_push_event_crash_to_fibase);
#endif
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

void AppDelegate::registerCSBReaders() {
    REGISTER_READER(TruyenTranhPageReader);
    REGISTER_READER(TextStoryReader);
    REGISTER_READER(PageMenuControlReader);
    REGISTER_READER(StoryInfoCoverReader);
    REGISTER_READER(StoryEndingPageReader);
    REGISTER_READER(StoryPageVCReader);
    REGISTER_READER(StoryRatePopUpReader);
    REGISTER_READER(StoryAlertReader);
    REGISTER_READER(StoryWelcomePopUpReader);
    REGISTER_READER(StoryLevelInfoReader);
    REGISTER_READER(StoryReadingLevelReader);
    REGISTER_READER(StoryLevelInfo_GuideReader);

    //REGISTER_READER(MSAISpeakingPermissionPopupReader);
    //REGISTER_READER(MSAISpeakingSharePopupReader);

    REGISTER_READER(MJPayPopupReader);
    REGISTER_READER(MJPayPopup_OnePayReader);
    REGISTER_READER(MJPayPopup_CODReader);
    REGISTER_READER(MJPayPopup_BankTransferReader);
    REGISTER_READER(MJPayPopup_StoreReader);
    REGISTER_READER(MJLicensePopupReader);
    REGISTER_READER(MJSubscriptionPageReader);
    REGISTER_READER(MJSubscriptionItemReader);
    REGISTER_READER(MJPayPopup_BankInfoReader);

    REGISTER_READER(StoryParentPage_LandscapeReader);
    REGISTER_READER(StoryParentHeader_LandscapeReader);
    REGISTER_READER(StoryParentItem_LandscapeReader);
    
    REGISTER_READER(StoryTheEndPageReader);

    StoryParentSubPage_Landscape::initReader();
    
    REGISTER_READER(MJPayGuideReader);
    REGISTER_READER(StoryFeedbackReader);
    
    REGISTER_CSB_READER(ViralPopup);
    REGISTER_CSB_READER(ViralPopup_Item);
    REGISTER_CSB_READER(ViralReceiveGift);
    
//    MJGameMakeAMatch::initReader();
//    MJGameLandThePlane::initReader();
//    MJGameJumpingFrog::initReader();
//    MJGameQA::initReader();

#pragma mark - READER
}

bool AppDelegate::applicationDidFinishLaunching() {
    CONFIG_MANAGER;
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    int Width = 1024 * 1.25;
    int Height = 768 * 1.25;

    mj::PlatformConfig::getInstance().getDeviceInfoShort();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if !DISABLE_APPSFLYER
    AppsFlyerManager::GetInstance()->startAppsFlyer();
#endif

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto path = FileUtils::getInstance()->getWritablePath();
    if (!mj::helper::isWriablePathValid(path)) {
        path = mj::helper::createWindowValidPath(path);//update path
    }
    CCLOG("new path: %s", path.c_str());
    std::string app_name_appdata = CONFIG_MANAGER->isAppVietNamStories?"VMonkey/":"MonkeyStories/";
    

    if (path.find(app_name_appdata) == std::string::npos){
        if (!FileUtils::getInstance()->isDirectoryExist(path+app_name_appdata)){
            FileUtils::getInstance()->createDirectory(path+app_name_appdata);
        }
        FileUtils::getInstance()->setWritablePath(path + app_name_appdata);
    }
    

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    Width = MAC_CPP_BRIDGE->getScreenSize().width;
    Height = MAC_CPP_BRIDGE->getScreenSize().height;
    std::string app_name_appdata = CONFIG_MANAGER->isAppVietNamStories ? "VMonkey/" : "MonkeyStories/";
    auto path = FileUtils::getInstance()->getWritablePath();
    if (path.find(app_name_appdata) == std::string::npos) {
        if (!FileUtils::getInstance()->isDirectoryExist(FileUtils::getInstance()->getWritablePath() + app_name_appdata)) {
            FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + app_name_appdata);
}
        FileUtils::getInstance()->setWritablePath(FileUtils::getInstance()->getWritablePath() + app_name_appdata);
    }

    //    MAC_CPP_BRIDGE->createContainerDataPath();
    //    std::string app_name_appdata = CONFIG_MANAGER->isVietnameseStories()?"VMonkey/":"MonkeyStories/";
    //    if (!FileUtils::getInstance()->isDirectoryExist(FileUtils::getInstance()->getWritablePath()+app_name_appdata)){
    //        FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath()+app_name_appdata);
    //    }
    //    FileUtils::getInstance()->setWritablePath(FileUtils::getInstance()->getWritablePath()+app_name_appdata);
#endif
    CCLOG("Documents: %s", FileUtils::getInstance()->getWritablePath().c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    {
        Width = GetSystemMetrics(SM_CXFULLSCREEN);
        Height = GetSystemMetrics(SM_CYFULLSCREEN);
    }
#endif
    
    if (Height <= 800) {
	    const auto ratio = Width * 1.f / Height;
        Height = 600;
        Width = Height * ratio;
    } else {
        auto ratio = Width * 1.f / Height;
        Height = 800;
        Width = Height * ratio;
    }
    auto app_name = mj::PlatformConfig::getInstance().getAppName();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
        if (MJDEFAULT->getBoolForKey("debugkey_fake_ipx", false)) {
            glview = GLViewImpl::createWithRect(app_name, Rect(0, 0, 1040, 480));
        } else {
            
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
			std::vector<cocos2d::Rect> sizes = {
                cocos2d::Rect(0, 0, 1024, 768),
                cocos2d::Rect(0, 0, 1366, 768),
                cocos2d::Rect(0, 0, 1280, 720)
            };
			glview = GLViewImpl::createWithRect(app_name, sizes.at(cocos2d::random(0, (int)sizes.size() - 1)), 1, true);
#else
			glview = GLViewImpl::createWithRect(app_name, Rect(0, 0, Width, Height), 1, true);
#endif
        }
#else
        glview = GLViewImpl::create(app_name);
#endif
        director->setOpenGLView(glview);
    }
    
    glview->setDesignResolutionSize(1024,768,ResolutionPolicy::FIXED_HEIGHT);
    
    registerCSBReaders();
    FileUtils::getInstance()->setPopupNotify(false);

    setupSearchPath();

    fixV220();
    
    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0 / 30);
    mj::helper::SetFpsDefault();
    setConfigApp();
    
    MJDEFAULT->setBoolForKey(key_event_go_to_lessons_page, false);
    MJDEFAULT->setBoolForKey(key_event_go_to_audiobooks_page, false);
    MJDEFAULT->setBoolForKey(key_event_go_to_subscribe_page, false);
    MJDEFAULT->setBoolForKey(key_event_go_to_free_page, false);
    
    MJDEFAULT->setBoolForKey("is_user_logout", false);
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    MJDEFAULT->setBoolForKey("show_hoc_van", true);
//    MJDEFAULT->setBoolForKey("debugkey_product_environment", false);
//    MJDEFAULT->setBoolForKey("mjdk_full_features", false);
  //  MJDEFAULT->setBoolForKey(key_is_submit, false);
  //  MJDEFAULT->setBoolForKey("debugkey_dev_environment", false);
 //    MJDEFAULT->setBoolForKey("debugkey_all_lessons", false);
//    if(MJDEFAULT->getBoolForKey("is_language_selection_show",false))
//    {
//        MJDEFAULT->setBoolForKey("is_language_selection_show",true);//this key for language display check, if the selection language show -> we not use the language key ion
//    }
#endif
        MJDEFAULT->setBoolForKey("key_update_3_day", false);
    if (MJDEFAULT->getBoolForKey("key_push_event_firestore", false)) {
        MSFIRESTORE->init();
    }

    // create a scene. it's an autorelease object
    CCLOG("Document: %s", FileUtils::getInstance()->getWritablePath().c_str());
    // run
    CCLOG("XML: %s",cocos2d::UserDefault::getInstance()->getXMLFilePath().c_str());
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

//CleverTapManager::GetInstance()->initDelegate();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity","deleteFilePng", FileUtils::getInstance()->getWritablePath());

#endif

    director->runWithScene(MJDEFAULT->getBoolForKey("is_language_selection_show",false) ? MSIntro::createScene() : MSLanguageSelection::createScene());

    MJDEFAULT->setBoolForKey(key_show_parent_lock, true);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    if (!MJ_PLATFORM_CONFIG.getDeviceId().empty()) {
        mj::FirebaseMessagingHelper::getInstance().initFirebaseCloudMessaging();
    }
#endif

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    auto time_current = int(utils::getTimeInMilliseconds() / 1000);
    auto last_time_launched = MJDEFAULT->getIntegerForKey("key.time.launched", 0);
    if (time_current - last_time_launched >= 24 * 60 * 60) {
        FREE_STORY_MANAGER->setPercentActionFreePlayed(0);
    }
    MJDEFAULT->setIntegerForKey("key.time.launched", time_current);
#endif
    IAPManager::getInstance()->iapInit();

    mj::api::VerifyStoreAPI::refresh();

    ms::EventLogger::getInstance();
    ms::EventLogger::getInstance().setUserProperties("click_to_join_fb_group", MJDEFAULT->getBoolForKey("user.clicked.to.join.group", false) ? "yes" : "no");

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
    if(MJDEFAULT->getBoolForKey(SHOULD_LOGIN_DUMMY_CT, true)){
        CleverTapManager::GetInstance()->loginDummyProfile();
        MJDEFAULT->setBoolForKey(SHOULD_LOGIN_DUMMY_CT, false);
    }
#endif

    return true;
} 

void AppDelegate::setupSearchPath()
{
    std::vector<std::string> search_paths;
    auto old_searchpaths = FileUtils::getInstance()->getSearchPaths();
    search_paths.push_back("fonts");
    search_paths.push_back("sounds");
    search_paths.push_back("sounds/quizz");
    search_paths.push_back("plist");
    search_paths.push_back("csb");
    search_paths.push_back("json");
    search_paths.push_back(StringUtils::format("%s", FileUtils::getInstance()->getWritablePath().c_str()));
    
   // Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA4444);
    
    if (MJDEFAULT->getIntegerForKey(key_device_retina, -1) == -1) {
        //not set
        if (MJDEFAULT->getStringForKey(kstoryversion, "").empty()) {
            //không phải là upgrade
            if (mj::helper::isRetina()) {
                search_paths.push_back(StringUtils::format("%s/hdr", FileUtils::getInstance()->getWritablePath().c_str()));
                search_paths.push_back("hdr");
                search_paths.push_back("hdr/games");
                search_paths.push_back("hdr/mjstory");
                search_paths.push_back("hdr/mjstory/avatar");
				search_paths.push_back("hdr/account");
                Director::getInstance()->setContentScaleFactor(2);
                StoryData::getInstance()->isHdr = true;
                search_paths.push_back("sample_game/hdr");
            } else {
                search_paths.push_back(StringUtils::format("%s/hd", FileUtils::getInstance()->getWritablePath().c_str()));
                search_paths.push_back("hd");
                search_paths.push_back("sample_game/hd");
                search_paths.push_back("hd/games");
                search_paths.push_back("hd/mjstory");
                search_paths.push_back("hd/mjstory/avatar");
				search_paths.push_back("hd/account");
                StoryData::getInstance()->isHdr = false;
            }
        } else {
            //là upgrade từ version cũ
            #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
            search_paths.push_back(StringUtils::format("%s/hdr", FileUtils::getInstance()->getWritablePath().c_str()));
            search_paths.push_back("hdr");
            search_paths.push_back("sample_game/hdr");
            search_paths.push_back("hdr/games");
            search_paths.push_back("hdr/mjstory");
            search_paths.push_back("hdr/mjstory/avatar");
            search_paths.push_back("hdr/account");
            StoryData::getInstance()->isHdr = true;
            Director::getInstance()->setContentScaleFactor(2);
            #else
            auto actual_size = Director::getInstance()->getOpenGLView()->getFrameSize();
            if (actual_size.height > 1000) {
                search_paths.push_back(StringUtils::format("%s/hdr", FileUtils::getInstance()->getWritablePath().c_str()));
                search_paths.push_back("hdr");
                Director::getInstance()->setContentScaleFactor(2);
                StoryData::getInstance()->isHdr = true;
                search_paths.push_back("sample_game/hdr");
                search_paths.push_back("hdr/games");
                search_paths.push_back("hdr/mjstory");
                search_paths.push_back("hdr/mjstory/avatar");
				search_paths.push_back("hdr/account");
            } else {
                search_paths.push_back(StringUtils::format("%s/hd", FileUtils::getInstance()->getWritablePath().c_str()));
                search_paths.push_back("hd");
                search_paths.push_back("hd/mjstory");
                search_paths.push_back("hd/mjstory/avatar");
				search_paths.push_back("hd/account");
                StoryData::getInstance()->isHdr = false;
            }
            #endif
        }
    } else {
        //đã được set
        if (mj::helper::isRetina()) {
            search_paths.push_back(StringUtils::format("%s/hdr", FileUtils::getInstance()->getWritablePath().c_str()));
            search_paths.push_back("hdr");
            search_paths.push_back("hdr/games");
            search_paths.push_back("hdr/mjstory");
            search_paths.push_back("hdr/mjstory/avatar");
			search_paths.push_back("hdr/account");
            Director::getInstance()->setContentScaleFactor(2);
            StoryData::getInstance()->isHdr = true;
            search_paths.push_back("sample_game/hdr");
        } else {
            search_paths.push_back(StringUtils::format("%s/hd", FileUtils::getInstance()->getWritablePath().c_str()));
            search_paths.push_back("hd");
            search_paths.push_back("sample_game/hd");
            search_paths.push_back("hd/games");
            search_paths.push_back("hd/mjstory");
            search_paths.push_back("hd/mjstory/avatar");
			search_paths.push_back("hd/account");
            StoryData::getInstance()->isHdr = false;
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    MAC_CPP_BRIDGE->fabricInit();
#endif
    
    search_paths.push_back(StringUtils::format("%s/games", FileUtils::getInstance()->getWritablePath().c_str()));
    search_paths.push_back("sample_game/sounds");
    search_paths.push_back("sample_game");
    search_paths.push_back("sample_game/mjstory");
    for (auto path : old_searchpaths) {
        search_paths.push_back(path);
    }
    
    search_paths.insert(search_paths.begin(), FileUtils::getInstance()->getWritablePath() + "az_test");
    search_paths.insert(search_paths.begin(), FileUtils::getInstance()->getWritablePath() + "mjstory");
    
    FileUtils::getInstance()->setSearchPaths(search_paths);
}

void AppDelegate::fixV220() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto str_version = MJDEFAULT->getStringForKey(kstoryversion);
    if (str_version == "2.2.0") {
        //xóa device id, yêu cầu chạy lại api từ đầu, reset lại các bài đã học
        MJDEFAULT->setBoolForKey(key_first_api_called, true);
        MJDEFAULT->setStringForKey(key_device_identifier, "");
        StoryDataModel::getInstance()->resetLessonLearned();
    }
#endif
}

void AppDelegate::setConfigApp(){
    FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "cached");
    FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "cached/zip");
    
    FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "audiobooks");
    FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "audiobooks/audio");
    
    this->copytodocument();
    
    if (MJDEFAULT->getStringForKey("language") == "") {
        MJDEFAULT->setStringForKey("language", "[1,4]");
    }
    
    if (MJDEFAULT->getStringForKey(key_list_lang) == "")
    {        
        if (CONFIG_MANAGER->isVietnameseStories())
        {
            MJDEFAULT->setStringForKey(key_list_lang, "[4]");
            MJDEFAULT->setIntegerForKey(key_current_lang, 4);
        } else {
            MJDEFAULT->setStringForKey(key_list_lang, "[1]");
            MJDEFAULT->setIntegerForKey(key_current_lang, 1);
        }
        MJDEFAULT->setIntegerForKey(key_lang_display, mj::PlatformConfig::getInstance().getDefaultLangCode());
        LANGUAGE_MANAGER->updateLanguageWithID(mj::PlatformConfig::getInstance().getDefaultLangCode());
    }
    
    
    MJDEFAULT->setBoolForKey(key_loadhome_done, false);
    MJDEFAULT->setBoolForKey(key_loadexplore_done, false);
}

void AppDelegate::copytodocument(){
    FileUtils *fileManager = FileUtils::getInstance();
    string database_path = fileManager->getWritablePath() + "database.sqlite";
    
    if(!fileManager->isFileExist(database_path)) {
        FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + "imgcache");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC )//&& (!defined COCOS2D_DEBUG || COCOS2D_DEBUG == 0))
//        MAC_CPP_BRIDGE->createContainerDataPath();
//        MAC_CPP_BRIDGE->createMacPathFileByString(fileManager->getWritablePath());
//        MAC_CPP_BRIDGE->createMacPathFileByString(fileManager->getWritablePath()+"mjstory");
//        MAC_CPP_BRIDGE->createMacPathFileByString(fileManager->getWritablePath()+"imgcache");
//        MAC_CPP_BRIDGE->createMacPathFileByString(fileManager->getWritablePath()+"games");
        MAC_CPP_BRIDGE->removeUserDefault();
        
#endif
        Data db_data = fileManager->getDataFromFile("database.sqlite");
        fileManager->writeDataToFile(db_data, database_path);
        
        string mjdatabase_path = fileManager->getWritablePath() + "mjdefault.sqlite";
        Data mj_data = fileManager->getDataFromFile("mjdefault.sqlite");
        fileManager->writeDataToFile(mj_data, mjdatabase_path);
//
//        std::ifstream src(FileUtils::getInstance()->fullPathForFilename("mjdefault.sqlite"), std::ios::binary);
//        std::ofstream dst(mjdatabase_path, std::ios::binary);
//        dst << src.rdbuf();
//        src.close();
//        dst.close();
//
//        std::ifstream db_src(FileUtils::getInstance()->fullPathForFilename("database.sqlite"), std::ios::binary);
//        std::ofstream db_dst(database_path, std::ios::binary);
//        db_dst << db_src.rdbuf();
//        db_dst.close();
//        db_src.close();
        
        string save_path1 = FileUtils::getInstance()->getWritablePath() + "mjstory";
        if(!FileUtils::getInstance()->isDirectoryExist(save_path1)) {
            FileUtils::getInstance()->createDirectory(save_path1);
            save_path1 = FileUtils::getInstance()->getWritablePath() + "games";
            FileUtils::getInstance()->createDirectory(save_path1);
        }
        
        MJDEFAULT->setStringForKey(kstoryversion, WebService::getInstance()->sub_version);
        
    }
    
    upgradeVersionStory();
}
void AppDelegate::upgradeVersionStory(){
    CCLOG("app_ver = %s", WebService::getInstance()->sub_version.c_str());
    auto str_version = MJDEFAULT->getStringForKey(kstoryversion);
    if (str_version == std::string("")) {
        MJDEFAULT->setStringForKey(kstoryversion, WebService::getInstance()->sub_version);
    }
    //về sau xử lý upgrade ở đây!
    CCLOG("version default = %d", atoi(MJDEFAULT->getStringForKey(kstoryversion).c_str()));
    auto arr_version_tach = GlobalFunc::getInstance()->explode(str_version, '.');
    if ((int)arr_version_tach.size() < 3) {
        for (int i = (int)arr_version_tach.size(); i < 3; i++) {
            arr_version_tach.push_back(string("0"));
        }
    }
    
    CCLOG("WebService::getInstance()->sub_version = %s", WebService::getInstance()->sub_version.c_str());
    auto arr_websubversion_tach = GlobalFunc::getInstance()->explode(WebService::getInstance()->sub_version, '.');
    if ((int)arr_websubversion_tach.size() < 3) {
        for (int i = (int)arr_websubversion_tach.size(); i < 3; i++) {
            arr_websubversion_tach.push_back(string("0"));
        }
    }
    
    if (str_version == "2.2.0") {
        //update từ phiên bản 2.2.0
    }
    
    if (atoi(arr_version_tach[0].c_str()) == 0) {//ver0.0.0
        MJDEFAULT->setStringForKey(kstoryversion, WebService::getInstance()->sub_version);
    }else{
        if (checkVersionLess(arr_version_tach, arr_websubversion_tach)) {
            CCLOG("version lưu bé hơn => vừa update lên ver mới!");
            MJDEFAULT->setStringForKey(kstoryversion, WebService::getInstance()->sub_version);
            MJDEFAULT->setBoolForKey(key_update_show, false);
            MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
            MJDEFAULT->setIntegerForKey("key_rate_action", 0);
            MJDEFAULT->setBoolForKey(key_first_api_called, true);
//            MJDEFAULT->setStringForKey(key_device_identifier, "");
            MJDEFAULT->setIntegerForKey("key_time_to_show", MJDEFAULT->getIntegerForKey(key_time_curent));
            MJDEFAULT->setIntegerForKey("key_count_to_show", 0);
//            MJDEFAULT->setIntegerForKey(key_api_version_story_local, 0);
//            MJDEFAULT->setIntegerForKey(key_api_appinfo_called, 0);
            MJDEFAULT->setBoolForKey(key_first_api_called, true);
        }else{//đang ở phiên bản mới rồi!
            CCLOG("đang ở phiên bản mới rồi!");
        }
    }
    if (!CONFIG_MANAGER->isAppVietNamStories){
         if (atoi(arr_version_tach[0].c_str()) < 2) {
                //update from story v1
                FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH);
                FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH);
                
        #if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
        //        MAC_CPP_BRIDGE->createMacPathFileByString(FileUtils::getInstance()->getWritablePath()+"imgcache");
        #endif
            }
	}
	else {

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH);
			FileUtils::getInstance()->createDirectory(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH);

#endif

	}
   
    
    //gọi lại api để lấy file size
    if (!CONFIG_MANAGER->isAppVNStories()) {
        auto arr_reset_version = GlobalFunc::getInstance()->explode("2.3.0", '.');
        if(checkVersionLess(arr_version_tach, arr_reset_version)){
           // mj::helper::resetDatabase();
        }
    }else{
        auto arr_reset_version = GlobalFunc::getInstance()->explode("1.1.4", '.');
        if(checkVersionLess(arr_version_tach, arr_reset_version)){
            mj::helper::resetDatabase();
        }
    }

}

bool AppDelegate::checkVersionLess(std::vector<string> ver1, std::vector<string> ver2){
    for (auto i = 0; i < 3; i++) {
        auto v1 = atoi(ver1[i].c_str());
        auto v2 = atoi(ver2[i].c_str());
        if (v1 != v2) {
            return v1 < v2;
        }
    }
    return false;
}

#pragma mark - RATE_APP
void AppDelegate::displayCheckRateApp(){
    if (MJDEFAULT->getBoolForKey(key_rate_app) && !MJDEFAULT->getBoolForKey(key_rate_app_won)) {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.showrateconfirm");
    }
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {

    CRASH_AUTO_LOG;
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)||(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #if !DISABLE_APPSFLYER
        AppsFlyerX::didEnterBackground();
    #endif

    #endif
    addMessageAppinbox();

    auto runningScene = Director::getInstance()->getRunningScene();
//
    if (auto storyPageVC = dynamic_cast<StoryPageVC*>(runningScene))
    {
        int timeUserExitApp = int(utils::getTimeInMilliseconds() / 1000);
        MJDEFAULT->setIntegerForKey(key_time_user_exit_app, timeUserExitApp);
    }
    

    
    ms::EventLogger::getInstance().logEvent("applicationDidEnterBackground", { });
    
    MMQRScan::Is_parsing_deeplink = false;

    Director::getInstance()->stopAnimation();
    cocos2d::UserDefault::getInstance()->flush();
    
    if (CONFIG_MANAGER->isAppActive()) {
        auto scene = Director::getInstance()->getRunningScene();
        auto player = utils::findChild(scene, "audiobook_player");
        if (!player) {
            AUDIO_ENGINE->pauseAllAudio();
        } else {
            MJDEFAULT->setIntegerForKey("onboard.screensaver.show.count", 5);
        }
    } else {
        AUDIO_ENGINE->pauseAllAudio();
        
        if (auto scene = Director::getInstance()->getRunningScene()) {
            if (auto player = utils::findChild<MSAudiobookPlayer *>(scene, "audiobook_player")) {
                player->onPlayerPause(nullptr);
            }
        }
    }
   
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.app.enter.background");
}


void AppDelegate::applicationWillEnterForeground()
{
    CRASH_AUTO_LOG;
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #if !DISABLE_APPSFLYER
        AppsFlyerManager::GetInstance()->startAppsFlyer();
    #endif

    #endif
    auto runningScene = Director::getInstance()->getRunningScene();
    if (auto storyPageVC = dynamic_cast<StoryPageVC*>(runningScene))
    {
        int timeUserExitApp = int(utils::getTimeInMilliseconds() / 1000) - MJDEFAULT->getIntegerForKey(key_time_user_exit_app, 0);
        int sumTime = MJDEFAULT->getIntegerForKey(key_sum_time_user_exit_app, 0) + timeUserExitApp;
        MJDEFAULT->setIntegerForKey(key_sum_time_user_exit_app, sumTime);
    }
       
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    firebase::dynamic_links::Fetch();
#endif

    ms::EventLogger::getInstance().logEvent("applicationWillEnterForeground", { });
    
    MJDEFAULT->setBoolForKey(key_show_parent_lock, true);

    Director::getInstance()->startAnimation();
    AUDIO_ENGINE->resumAllAudio();

    auto scene = Director::getInstance()->getRunningScene();
    auto player = utils::findChild(scene, "audiobook_player");
    if (!player)
        AUDIO_ENGINE->resumAllAudio();

    CCLOG("App enter foreground");

    displayCheckRateApp();

    ms::playlist::ABPlayer::getInstance().onAppDidEnterForeground();
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_app_active_again);
}

#pragma mark - REQUEST_API
void AppDelegate::pushTokenToServer(){
}

void AppDelegate::onRequestSuccess(int _tag, rapidjson::Value data) {
    if (_tag == kTag_API_pushToken){
        MJDEFAULT->setBoolForKey(key_token_sent, true);
    }
    
    if (_tag == kTag_API_loadupdate) {
//        callbackLoadUpdate(data.GetObject());
    }
    
    if (_tag == kTag_API_appinfo) {
//        callbackAppInfo(data.GetObject());
    }
}

void AppDelegate::onRequestFailed(int _tag, int error_code, std::string error_string, rapidjson::Value data)
{
    CCLOG("Faill Response for URL %s: \n", error_string.c_str());
}


void AppDelegate::loadNotification(){
}

//Tells the delegate that the app is about to become inactive.

void AppDelegate::applicationWillResignActive() {
    
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)){
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC

    auto runningScene = Director::getInstance()->getRunningScene();
    if (!runningScene) {
        return;
    }
//    for (auto& child : runningScene->getChildren())
//    {
//        if(auto monkeyTalking = dynamic_cast<AISpeakingScene*>(child)) {
//
//            monkeyTalking->onScenePause();
//
//            break;
//        }
//    }
#endif // CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
    }
}

void AppDelegate::applicationDidBecomeActive() {
    
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)){
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC

//    auto runningScene = Director::getInstance()->getRunningScene();
//    if (!runningScene) {
//        return;
//    }
//    for (auto& child : runningScene->getChildren())
//    {
//        if(auto monkeyTalking = dynamic_cast<AISpeakingScene*>(child)) {
//            
//            monkeyTalking->onSceneResume();
//
//            break;
//        }
//    }
#endif

}
}

void AppDelegate::applicationWillTerminate()
{
    CRASH_AUTO_LOG;
    MJDEFAULT->deleteValueForKey(key_error_url_stories);
    MJDEFAULT->deleteValueForKey(key_Limit_download_thumb);
    MJDEFAULT->deleteValueForKey(key_count_retry);
    MJDEFAULT->deleteValueForKey(key_speed_internet);

    MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, false);
    
    //vi hien tai dang crash o cho push event khi kill app nen tam thoi comment
    
//    auto runningScene = Director::getInstance()->getRunningScene();
//    if (auto storyPageVC = dynamic_cast<StoryPageVC*>(runningScene))
//    {
//        storyPageVC->pushEventInteractiveQuestion();
//        storyPageVC->pushCleverTapTrackingData();
//    }
//
//    if (auto lessonPreview = dynamic_cast<MSLessonPreview*>(utils::findChild(runningScene, "lesson_preview")))
//    {
//        lessonPreview->pushCleverTapTrackingData();
//    }

//    if (auto audiobookPlayer = dynamic_cast<MSAudiobookPlayer*>(utils::findChild(runningScene, mjhelper_name_audiobookplayer)))
//    {
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//        audiobookPlayer->pushCleverTapTrackingData();
//#else
//        audiobookPlayer->pushCleverTapTrackingDataForWinMac();
//#endif
//
//    }
//    if (auto mSTabSubscribe = dynamic_cast<MSTabSubscribe*>(utils::findChild(runningScene, mshome_name_tabsubscribe)))
//    {
//        mSTabSubscribe->pushCleverTapEvent();
//    }
//    if (auto fireBaseEventGameAI = dynamic_cast<ProScore*>(utils::findChild(runningScene, "interactive_question_gameAI")))
//    {
//        fireBaseEventGameAI->pushFireBaseEventGameAI();
//    }
//    if (auto fireBaseEventInteractiveQuestion = dynamic_cast<StoryPageVC*>(utils::findChild(runningScene, "story_pageVC")))
//    {
//        fireBaseEventInteractiveQuestion->pushFireBaseInteractiveQuestionActivitySkipped();
//    }
//
//    if (auto splashScene = dynamic_cast<SplashScene*>(runningScene))
//    {
//        splashScene->pushEventTracking();
//    }
//
//    if (auto splashScene = dynamic_cast<SplashScene*>(utils::findChild(runningScene, "SplashScene")))
//    {
//        splashScene->pushEventTracking();
//    }
}

#pragma mark - HANDLE URL SCHEME
#include "MSFirebasePlugin.h"
#include "MSHome.h"

void AppDelegate::applicationHandleOpenURL(const std::string& url)
{
    if (url.empty()) {
        return;
    }
    
    const std::string deeplinkPart= "deeplink=";
    
    auto pos = url.find(deeplinkPart);
    
    if(pos != std::string::npos) {
        
        const std::string newURl = url.substr(pos+deeplinkPart.size());
        
        CCLOG("xxx newURL:%s",newURl.c_str());
        
        ms::analytics::DynamicLinksListener::OnDynamicLinkReceivedAndroid(newURl);
    } else {
        ms::analytics::DynamicLinksListener::OnDynamicLinkReceivedAndroid(url);
    }
}

void AppDelegate::setDataAppinbox(std::vector<std::string> value)
{
    _valueAppinbox = value;
}

std::vector<std::string> AppDelegate::getDataAppinbox()
{
    return _valueAppinbox;
}


void AppDelegate::addMessageAppinbox()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    auto message = mj::appinbox::getMessageAppinbox();
    std::vector<StoryNotificationInfo> messageInbox = {};
    for(int i = 0; i < message.size(); i++)
    {
        auto isMessageDatabase = ap::ProfileManager::getInstance().getDataAppinbox(message[i].title);

        if(isMessageDatabase == "" && message[i].message.length() > 0)
        {
            messageInbox.push_back(message[i]);
            ap::ProfileManager::getInstance().setDataAppinbox(message[i].id, message[i].message, message[i].title, message[i].url, message[i].unread, message[i].time_epoch);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "setTitleAndMessageForNotification", message[i].title, message[i].message);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
            valueNotificationAppinbox.push_back({ message[i].title, cocos2d::Value(message[i].message) });
#endif
        }
    }
#endif
}

std::vector<std::pair<std::string, cocos2d::Value>> AppDelegate::getValueNotificationAppinbox()
{
    return valueNotificationAppinbox;
}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {
JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onDestroyCallback(JNIEnv *env, jclass clazz ) {
        if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
            delegate->applicationWillTerminate();
        }
    }

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_getDataObjectArray(JNIEnv *env, jclass clazz, jobjectArray key) {
    // TODO: implement getDataObjectArray()
    vector<string> stringVec;
    if(key != nullptr)
    {
        int size = env->GetArrayLength(key);

        for (int i=0; i < size; ++i)
        {
            jstring stringValue = static_cast<jstring>(env->GetObjectArrayElement(key, i));

            const jsize strLen = env->GetStringUTFLength(stringValue);
            const char* mayarray = env->GetStringUTFChars(stringValue, 0);
            if(mayarray != nullptr)
            {
                string str(mayarray, strLen);
                stringVec.push_back(str);
                env->ReleaseStringUTFChars(stringValue, mayarray);
            }
            env->DeleteLocalRef(stringValue);
        }
    }
    if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
        delegate->setDataAppinbox(stringVec);
    }
}

JNIEXPORT jint JNICALL Java_org_cocos2dx_cpp_AppActivity_getCountry(JNIEnv *env, jclass clazz) {
    return LANGUAGE_MANAGER->getDisplayLangId();

}


JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_getMessageOTP(JNIEnv *env, jclass clazz, jstring message) {
// TODO: implement getMessageOTP()

    const jsize strLen = env->GetStringUTFLength(message);
    const char* mayarray = env->GetStringUTFChars(message, 0);
    if(mayarray != nullptr)
    {
        string str(mayarray, strLen);
        env->ReleaseStringUTFChars(message, mayarray);
        env->DeleteLocalRef(message);
        if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
            delegate->setMessageOTP(str);

            Director::getInstance()->getScheduler()->performFunctionInCocosThread([](){

                if (auto otpScreen = dynamic_cast<APOTPPopup*> (utils::findChild(Director::getInstance()->getRunningScene(), "APOTPPopup")))
                {
                    otpScreen->autoGetOTP();
                }
            });
        }
    }
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_pushEnvetLostConnectionToClevertab(JNIEnv *env, jclass clazz, jobjectArray value) {
    // TODO: implement pushEnvetLostConnectionToClevertab()
    vector<string> stringValueEvent;

    if(value != nullptr)
    {
        int size = env->GetArrayLength(value);

        for (int i=0; i < size; ++i)
        {
            jstring stringValue = static_cast<jstring>(env->GetObjectArrayElement(value, i));

            const jsize strLen = env->GetStringUTFLength(stringValue);
            const char* mayarray = env->GetStringUTFChars(stringValue, 0);
            if(mayarray != nullptr)
            {
                string str(mayarray, strLen);
                stringValueEvent.push_back(str);
                env->ReleaseStringUTFChars(stringValue, mayarray);
            }
            env->DeleteLocalRef(stringValue);
        }
    }

    mj::helper::pushEvnetLostConnection(stringValueEvent[0], stringValueEvent[1], stringValueEvent[2], stringValueEvent[3], stringValueEvent[4], stringValueEvent[5], stringValueEvent[6], stringValueEvent[7]);

}

}
#endif

void AppDelegate::setMessageOTP(std::string message)
{
    _message = message;
}

std::string AppDelegate::getMessage()
{
    return _message;
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"
JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_MyNewFirebaseManager_callbackCreateNewToken(JNIEnv *env, jobject thiz, jstring token) {
    // TODO: implement asd()
    const jsize strLen = env->GetStringUTFLength(token);
    const char* mayarray = env->GetStringUTFChars(token, 0);
    if(mayarray != nullptr)
    {
        string strToken(mayarray, strLen);
        env->ReleaseStringUTFChars(token, mayarray);
        env->DeleteLocalRef(token);
        if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
            AppsFlyerX::registerUninstall(strToken);
        }
    }
}
#endif

