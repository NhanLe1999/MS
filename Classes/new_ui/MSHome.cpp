//
//  MSHome.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//  Modified by Nguyen Hoang Thien Phuoc
//
//

#include "MSHome.h"
#include "MSSidebar.h"
#include "MSSearchbar.h"
#include "MSTabStories.h"
#include "MSTabLessons.h"
#include "MSGradeManager.h"
#include "MSTabAudiobooks.h"
#include "MSTabFree.h"
#include "MSTabFavourite.h"
#include "MSAppDrawer.h"
#include "StoryParentPage_Landscape.h"
#include "MJSubscriptionPage.h"
#include "MSComplexCollection.h"
#include "StoryDataModel.h"
#include "MSExploreView.h"
#include "MSLessonPreview.h"
#include "MSDownloader.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "MJPlatformConfig.h"
#include "MSLessonEndFX.h"
#include "MJDebugDownloadView.h"
#include "MJPlatformConfig.h"
#include "MSOnboard.h"
#include "StoryConfigManager.h"
#include "MMQRScan.h"
#include "StoryAudioPlayer.h"
#include "APWaitView.h"
#include "APPopup.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MSBookData.h"
#include "MSBookTextInline.h"
#include "MSPlaylistHelper.h"
#include "MJAPIGetAudioSyncData.h"
#include "network/CCDownloader.h"
#include "MSDownloader.h"
#include "network/HttpClient.h"
#include "MJRequest.h"
#include "json/document.h"
#include "json/prettywriter.h"
#include <iostream>
#include "MJHelper.h"
#include "GAFWrapper.h"
#include "MJMailIntro.h"
#include "MJLicenseActiveSuccess.h"
#include "MJAPI_AppInfo_Phonic_Zip.h"
#include "APLoginView.h"
#include "APProfileAvatarView.h"
//#include "DatabaseHelper.h"
#include "APDatabase.h"
#include "APProfileManager.h"
#include "APUpdateUserInfoAPI.h"
#include "APLoadUpdateAPI.h"
#include "MMFirebaseDatabase.h"
#include "manager/GettingUserInfoManager.h"
#include "manager/RatingManager.h"
#include "rating/RatingPopup.h"
#include "popup/GettingUserInfoPopup.h"
#include "popup/RemindingUpdateInfoPopup.h"
#include "MSNotificationPopup.h"
#include "story/HSAudioEngine.h"
#include "story/popup/ViralPopup/ViralGiftPopup.h"
#include "StoryFreeManager.h"
#include "MSFirebasePlugin.h"
#include "DatePicker.h"
#include "MSLanguageSelection.h"
#include "APChooseProfileView.h"
#include "MJDefinitions.h"
#include "APAccountUserInfo.h"
#include "MSCollectionStoryItem.h"
#include "MSCollectionAudiobookItem.h"
#include "MSGradeBox.h"
//#include "RewardPopup.h"
#include "HelperManager.h"
#include "APAccountProfileDetail.h"
#include "APAccountDefinesV3.h"
#include "MJWorksheet.h"
#include "CheatMenu.h"
#include "MsSubscriptionLandscape.h"
#include "F2PManager.h"
#include "MSFreeContentPage.h"
#include "MSIntroLoad.h"
#include "GettingStartedScene.h"
#include "MSCacheManager.h"
#include "CameraHelper.h"

//define
#define FRAME_GRADEBOX_BG "mjstory/new_ui/frame_gradebox_background.png"

USING_NS_CC;
INITIALIZE_READER(MSHome);

std::once_flag ms_home_reader;

//
// Coding Convention
// Please follow here: https://docs.google.com/document/d/1w63J5DSRezBUxIktcyTvB4w90MhrtU-1FiXhTrq--TI/edit?usp=sharing
//

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Scene * MSHome::createScene()
{
    auto scene = Scene::create();
    
    if (scene)
    {
        scene->setName(ms_scene_name_home);
        
        auto view = MSHome::createView(scene);
        view->setName(MSHOME_NAME);
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }
    
    return scene;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MSHome * MSHome::createView(cocos2d::Scene *i_homeScene)
{
    std::call_once(ms_home_reader, [] {
        REGISTER_CSB_READER(MSHome);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/HomeUI.csb");
    
    auto view = reinterpret_cast<MSHome*>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB(i_homeScene);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return nullptr;
}

MSHome::~MSHome()
{
    ms::LessonManager::getInstance().ResetCacheAllLesson();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::didLoadFromCSB(cocos2d::Scene *i_homeScene)
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    this->scheduleOnce([=] (float dt) {
        mj::helper::changeIconAPP();
    }, 1.0f, "delay_time_change_icon_app");
#endif
    // tab Stories hoặc Lessons lần gần nhất được chọn
    auto old_tab_selected = MJDEFAULT->getStringForKey("key_sidebar_tab_select", "lessons");
    
    StoryDataModel::getInstance()->getAllStory();
    StoryDataModel::getInstance()->initAudiobooksWithChapters();
    
    ms::EventLogger::getInstance().setIdentifier(MJ_PLATFORM_CONFIG.getDeviceId());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    intCheatMenu();
#endif
    
    if(CONFIG_MANAGER->isVietnameseStories() && Director::getInstance()->getVisibleSize().width/Director::getInstance()->getVisibleSize().height >= 19.f/9.0f)
    {
        if(auto backgroundImg = utils::findChild(this, "Panel_1"))
        {
            backgroundImg->setIgnoreAnchorPointForPosition(false);
            backgroundImg->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
            backgroundImg->setScale(1.1f);//110%
            backgroundImg->setPosition(backgroundImg->getParent()->getContentSize()/2);
        }
    }
    
    //
    // this is the left bar
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    m_sidebar = MSSidebar::createSidebar();
    m_sidebar->setName("msSideBar");
    m_sidebar->setDelegate(this);
    this->addChild(m_sidebar, kMSHomeZOrder_sideBar);
    
    //time_fix_icon_new
    MJDEFAULT->setBoolForKey(key_time_mark_new_available, time_mark_new_STORY > MJDEFAULT->getIntegerForKey(key_time_curent));
    MJDEFAULT->setBoolForKey(key_time_mark_new_AUDIO_available, time_mark_new_AUDIO > MJDEFAULT->getIntegerForKey(key_time_curent));
    
    //bg_music
    MJDEFAULT->setBoolForKey(key_background_music, false);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    auto content = utils::findChild(this, "tab_content");
    content->setContentSize(Size(Director::getInstance()->getVisibleSize().width - m_sidebar->getContentSize().width, content->getContentSize().height));
    content->setPosition(Point(m_sidebar->getContentSize().width, 0));
    content->setLocalZOrder(4);
    
    PROFILE_MANAGER.setIntegerForKey(key_grade_audiobooks, -1);
    PROFILE_MANAGER.setIntegerForKey(key_grade_lessons, 84);
    PROFILE_MANAGER.setIntegerForKey(key_grade_lessons_phonic,  479);
    PROFILE_MANAGER.setIntegerForKey(key_grade_stories, -1);
    PROFILE_MANAGER.resetFavoriteBooks();
    
    ms::GradeManager::getInstance().updateGrades();
    
    m_tabStories = MSTabStories::createTab(this);
    m_tabStories->setContentSize(content->getContentSize());
    m_tabStories->setVisible(true);
    content->addChild(m_tabStories);
    
    m_tabAudiobooks = MSTabAudiobooks::createTab(this);//gradeBox
    m_tabAudiobooks->setContentSize(content->getContentSize());
    m_tabAudiobooks->setVisible(false);
    content->addChild(m_tabAudiobooks);
    
//    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
//    {
//        _tabAISpeaking = MSTabAISpeaking::createTab(this);
//        _tabAISpeaking->setContentSize(content->getContentSize());
//        _tabAISpeaking->setVisible(false);
//        content->addChild(_tabAISpeaking);
//    }
    m_currentTab    = ms::HomeTab::STORIES;
    m_prevTab       = m_tabStories;
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if (!MJDEFAULT->getBoolForKey(key_onboarding_part_2, false))
    {
        if(i_homeScene)
        {
            auto check_node = i_homeScene->getChildByName("update_popup");
            
            if (check_node)
            {
                return;
            }
            
            auto fakeUpdatePopupNode = Node::create();
            fakeUpdatePopupNode->setName("update_popup");
            i_homeScene->addChild(fakeUpdatePopupNode);
        }
        
        onboarding();
    }
    else
    {
        if (!CONFIG_MANAGER->isVietnameseStories())
        {
            if (old_tab_selected == "lessons") {
                m_sidebar->onReadTabAutoSelect();
            } else {
                m_sidebar->onLessionTabAutoSelect();
            }
        }
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
        #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            CheatMenu::show();
        #endif
        //clevertap login current account.
        int profileId =  PROFILE_MANAGER.getCurrentProfileId();
        if(profileId>0)
        {
            
            CleverTapManager::GetInstance()->pushProfileByID(profileId,CleverTapManager::PUSH_TYPE::LOGIN,nullptr);
            
            CleverTapManager::GetInstance()->initProductConfig();
        }

        // dispatch event Tạo profile đầu tiên, push khi vào home
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_profile_first_created);
        
        // dispatch event Chọn profile
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_profile_choose);
        
        if (MJDEFAULT->getStringForKey("key_push_event_sign_up_type").length() > 0)
        {
            const int k_currentProfileID = MJDEFAULT->getIntegerForKey("key_last_profile_updated");
            auto profile = ap::Database::getInstance().getProfileById(k_currentProfileID);
            CleverTapManager::GetInstance()->pushEvent("sign_up", {
                    {"type", cocos2d::Value(MJDEFAULT->getStringForKey("key_push_event_sign_up_type"))},
            });

            CleverTapManager::GetInstance()->pushEvent("profile_name", {
                    {"name",cocos2d::Value(profile.name)},
                    {"gender",cocos2d::Value(profile.gender)},
                    {"click",cocos2d::Value("continue")},
            });

            CleverTapManager::GetInstance()->pushEvent("profile_age",
                {
                    {"age",cocos2d::Value(profile.age)},
                    {"click",cocos2d::Value("continue")},
                });
            CleverTapManager::GetInstance()->pushEvent("profile_avatar",
                {
                    {"avatar_name",cocos2d::Value(profile.avatar)},
                    {"click",cocos2d::Value("continue")},
                });
            CleverTapManager::GetInstance()->pushEvent("home_screen", {         
                });

            MJDEFAULT->deleteValueForKey("key_push_event_sign_up_type");
        }

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        onPushEventForWinMac();
#endif

        //clevertap appinbox: only for test, we need a button to show this feature.
//         this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
//
//             CleverTapManager::GetInstance()->initAppInbox();
//
//         }),DelayTime::create(3.0f),CallFunc::create([=]() {
//
//             CleverTapManager::GetInstance()->showAppInbox();
//
//         }),nullptr));
                                                        
        //
        if(m_tabLessonSelect)
        {
            m_tabLessonSelect->showOnboard();
        }
                
    }),DelayTime::create(5.0f),CallFunc::create([=]() {
        
        CleverTapManager::GetInstance()->fechProductConfig();
        
    }), nullptr));
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    mm::RealtimeDatabase::getInstance().saveData();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //receive dynamic link from outside scope
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.dynamic.link.received", [] (EventCustom * e)
    {
        auto link = ms::analytics::Firebase::QR_DEEP_LINK;
        auto story_id = mj::helper::parseDynamicLinkQR(link);
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([story_id]
        {
            auto view = MSStoryAudioPlayer::createView(story_id);
            Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
            Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
            ms::analytics::Firebase::QR_DEEP_LINK = "";
            //            MJDEFAULT->setStringForKey(key_qrcode_received, "");
        });
    }), this);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    onShowFreeLessonIcon();
    auto value = MJDEFAULT->getStringForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE, "");
    if (value != "")
    {
        int timeEnd = MJDEFAULT->getIntegerForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE, 0);
        std::vector<std::pair<std::string, cocos2d::Value>> properties = {
            {"click_type", cocos2d::Value(value)},
            {"time_on_screen", cocos2d::Value(timeEnd)},
        };
        CleverTapManager::GetInstance()->pushEvent("ob_choose_account_type", properties);
        MJDEFAULT->deleteValueForKey(CLICK_TYPE_OF_OB_CHOOSE_ACCOUNT_TYPE);
        MJDEFAULT->deleteValueForKey(TIME_OF_EVENT_OB_CHOOSE_ACCOUNT_TYPE);
    }
    //goToSignInForF2PMerge();
}


void MSHome::onEnter()
{
    ms::EventLogger::getInstance().setUserProperties("purchased", mj::Pay::getInstance().isActive() ? "yes" : "no");
    ms::EventLogger::getInstance().setUserProperties("device_id", MJ_PLATFORM_CONFIG.getDeviceId());
    ms::EventLogger::getInstance().setUserProperties("profile_id", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_current_profile_id)));
    ms::EventLogger::getInstance().setUserProperties("uid", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_user_id)));
    
    FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "tmp/");
    FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "tmpaudio/");
    FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "tmpstory/");
    
    FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + "story.zip");
    FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + "appinfo.zip");
    FileUtils::getInstance()->removeFile(FileUtils::getInstance()->getWritablePath() + "audio.zip");
    
    ui::Layout::onEnter();
    mj::helper::SetFpsByNumberFrames(20);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    this->runAction(Sequence::create(DelayTime::create(1.0), CallFunc::create([=]
    {

        //nam popup
        if (MJDEFAULT->getBoolForKey("is_user_skip", false))
        {
            int day_skip = 2;
            
            auto time_skip = MJDEFAULT->getIntegerForKey("time_skip_popup_upgrade_account", 0);
            
            if (((mj::helper::getTime() - time_skip) > day_skip * 24 * 3600) && time_skip != 0)
            {
                MJDEFAULT->setIntegerForKey("time_skip_popup_upgrade_account", mj::helper::getTime());
                //key.account.btn.back
                //ap_register_button_title
                auto popup = AccountPopup::show({
                    LANGUAGE_MANAGER->getDisplayTextByKey("key.upgrade.account.popup.msg"),
                    LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.back"),
                    LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title") },NULL, [this]()
                {
                    auto waiting_layout = Layout::create();
                    waiting_layout->setContentSize(Director::getInstance()->getVisibleSize());
                    waiting_layout->setBackGroundColor(Color3B::BLACK);
                    waiting_layout->setBackGroundColorType(BackGroundColorType::SOLID);
                    waiting_layout->setBackGroundColorOpacity(200);
                    waiting_layout->setTouchEnabled(true);
                    waiting_layout->setName("waiting_logout");
                    Director::getInstance()->getRunningScene()->addChild(waiting_layout);
                    
                    auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
                    waiting_layout->addChild(indicator);
                    indicator->setPosition(waiting_layout->getContentSize() / 2);
                    indicator->setVisible(true);
                    indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
                    
                    //reload data
                    auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event) {
                        MJDEFAULT->setBoolForKey(key_onboarding_license, false);
                        MJDEFAULT->setBoolForKey(key_logged_in, false);
                        //        waiting_layout->removeFromParent();
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                            auto scene = MSIntro::createScene();
                            Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
                        });
                    });
                    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
                   
                    auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom * event) {
                        cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
                        waiting_layout->removeFromParent();
                    });
                    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);
                    ap::Auth::signOut();
                });
                
                //utils::findChild<ImageView*>(popup, "close")->setVisible(false);
            }
        }
        
        bool update_showed = false;
        
        if (!mjupdate_manager)
        {
            mjupdate_manager = MJUpdateManager::createUpdate(mj::PlatformConfig::getInstance().getAppVersion());
            this->addChild(mjupdate_manager, kMSHomeZOrder_popupUpdate);
            update_showed = mjupdate_manager->onShowUpdatePopup();
        }
        else
        {
            update_showed = mjupdate_manager->onShowUpdatePopup();
        }
        
        if (!update_showed)
        {
            //show notification popup
            auto notification_id = MJDEFAULT->getIntegerForKey(key_popup_id_to_show, 0);
            
            if (notification_id == 0)
            {
                return;
            }
            
            auto noti = StoryDataModel::getInstance()->getNotificationById(notification_id);
            
            auto is_first_time = !MJDEFAULT->getBoolForKey("key_passed_first_time", false);
            
            if(noti.m_messages.count("vi") && noti.m_messages.at("vi").find("hủy quyền sử dụng")!= std::string::npos && is_first_time)
            {
               //nếu là lần đầu cài app mà có thông báo hủy quyền sử dụng trước đây thì không hiện
               MJDEFAULT->setBoolForKey(StringUtils::format("%s%d", key_popup_showed, noti.notification_id), true);
               MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
         
            }
            else
            {
                MSNotificationPopup::show(noti);
            }
        }
        
        MJDEFAULT->setBoolForKey("key_passed_first_time", true);
        
    }), NULL));
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    this->scheduleOnce([](float dt)
    {

        mj::helper::purgeTextureData();
    
    }, 1.5, "purge_cached");
    
    //TODO
    //bg music
    if(MJDEFAULT->getBoolForKey(key_background_music))
    {
        if(CONFIG_MANAGER->isVietnameseStories())
        {
            AUDIO_ENGINE->playBackgroundMusic("sounds/bg_music.mp3");
        }
    }
    
    //TODO
    //Event popup
    //    this->runAction(Sequence::create(DelayTime::create(2), CallFunc::create([=]{
    //        bool inEvent = true;
    //        if(inEvent){
    //            MSEventPopup::show();
    //        }
    //
    //    }), NULL));
    
    //TODO
    //popup tặng 7 ngày
    //    if(MJDEFAULT->getBoolForKey("key.is.refer", false)){
    //        this->scheduleOnce([](float dt) {
    //            ViralGiftPopup::show();
    //            MJDEFAULT->setBoolForKey("key.is.refer", false);
    //        }, 1.5, "gifted_popup");
    //    }
    
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onEnterTransitionDidFinish()
{
    ui::Layout::onEnterTransitionDidFinish();
    ScanSubFolderImageCache();
    this->runAction(Sequence::createWithTwoActions(DelayTime::create(0.05f), CallFunc::create([=](){
        
        processQRCode();
        
    })));
}

#pragma mark - Goto a tab/layout/page
void MSHome::openLesson(int gradeId, int lessonId,ms::LessonTab lessonTab)
{
    // VMonkey
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        this->goToLessonsTab();
        
        if (m_comprehensionLesson)
        {
            m_comprehensionLesson->openLessonForce(gradeId, lessonId);
        }
    }
    // Monkey Stories
    else
    {
        this->onSelectedLesson(lessonTab, gradeId, lessonId);
    }
}

void MSHome::openStory(const mj::model::StoryInfo& story_info)
{
    if (story_info.unique_id != 0)
    {
        this->goToStoriesTab();
        
        auto story_item = MSCollectionStoryItem::createItem(story_info);
        mj::helper::playTransitionStoryCover(story_item, nullptr, -1);
    }
    else
    {
        CCLOG("hieunt: ERROR - không có truyện!");
    }
}

void MSHome::clearLayouts()
{
    // có thằng layout nào thì remove hết ở đây, để tránh deep link nhảy tab hoặc sinh layout mới bị thằng cũ che mất
    
    this->removeChildByName(mshome_name_choose_profile_view);
    this->removeChildByName(mshome_name_tabsubscribe);
    this->removeChildByName(mshome_name_account_userinfo);
    this->removeChildByName(mshome_name_storyparentpage_landscape);
    this->removeChildByName(name_account_profile_detail);
     
    // remove các con của running scene
    auto runningScene = Director::getInstance()->getRunningScene();
    
    if (auto scan_screen = (MMQRScan *)runningScene->getChildByName(mmqrscan_name)) {scan_screen->onClose(nullptr);} // MMQRScan layout
    
    runningScene->removeChildByName("storyalert_popup"); // popup StoryAlert
    runningScene->removeChildByName("StoryWelcomePopUp");
    runningScene->removeChildByName("qr_story_player");     // audio book
    runningScene->removeChildByName(mshome_name_appdrawer); // promotion layout
    runningScene->removeChildByName(mjhelper_name_storyinfocover); // StoryInfoCover layout
    runningScene->removeChildByName(mjhelper_name_audiobookplayer); // AudiobookPlayer layout
    runningScene->removeChildByName(mjhelper_name_collectionaudiobookitem); // CollectionAudiobookItem layout
    /*runningScene->removeChildByName(name_screen_enter_email_popup);
    runningScene->removeChildByName(name_screen_apaccount_update_phone);
    runningScene->removeChildByName(name_screen_apaccount_update_password);
    runningScene->removeChildByName(name_screen_apaccount_update_email);
    runningScene->removeChildByName(name_screen_apaccount_confirm_otp);
    runningScene->removeChildByName(name_screen_apaccount_add_password);*/
    runningScene->removeChildByName(name_screen_apaccount_confirm_password);
   // runningScene->removeChildByName(name_screen_enter_email_popup);
}

void MSHome::ScanSubFolderImageCache()
{
    std::vector <std::string> subFolders;
    std::string imageCacheFolder = "imgcache/";
    std::vector < std::string > fileList = FileUtils::getInstance()->listFiles(FileUtils::getInstance()->getWritablePath() + imageCacheFolder);
    for (std::string item : fileList) {
        if (FileUtils::getInstance()->isDirectoryExist(item) && !(item.find("./") != std::string::npos))
            subFolders.push_back(item);
    }
    if (subFolders.size() <= 0)
        return;
    std::ostringstream vts;
    std::copy(subFolders.begin(), subFolders.end(),
        std::ostream_iterator<std::string>(vts, ", "));

    MJDEFAULT->setStringForKey(key_image_cache_subfolders_converted, vts.str());
}

void MSHome::addLayoutChooseProfile(bool createNewProfileShowed)
{
    auto choose_profile = APChooseProfileView::createView();
    choose_profile->setName(mshome_name_choose_profile_view);
    this->removeChildByName(mshome_name_choose_profile_view);
    this->addChild(choose_profile, INT_MAX-1);
    
    if (createNewProfileShowed)
    {
        choose_profile->onNewProfile(choose_profile);
    }
}

void MSHome::goToPromotionLayout()
{
    CRASH_AUTO_LOG;

    m_sidebar->onPromotionsForce();
}

void MSHome::goToPopularStories()
{
    CRASH_AUTO_LOG;

    this->goToStoriesTab();
    m_tabStories->goToPopularStories();
}

void MSHome::goToStoriesTab(bool showGrades)
{
    CRASH_AUTO_LOG;

    m_sidebar->onStoriesForce();
    
    if (showGrades)
    {
        if(auto gradeBox = cocos2d::utils::findChild<MSGradeBox *>(m_tabStories, "story_gradebox"))
        {
            gradeBox->showGrades();
        }
    }
}

void MSHome::goToStoriesTabWithCategory(int catId)
{
    ms::EventLogger::getInstance().logEvent("goToStoriesTabWithCategory", {
        {"catId",cocos2d::Value(catId)},
    });

    this->goToStoriesTab();
    m_tabStories->goToCategory(catId);
}

void MSHome::goToLessonsTab()
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onLessonsForce();
}

void MSHome::goToAudiobooksTab(bool showGrades)
{
    ms::EventLogger::getInstance().logEvent("goToAudiobooksTab", {
        {"showGrades",cocos2d::Value(showGrades)},
    });
    
    m_sidebar->onAudiobooksForce();
    
    if (showGrades)
    {
        if(auto gradeBox = cocos2d::utils::findChild<MSGradeBox *>(m_tabAudiobooks, "audio_gradebox"))
        {
            gradeBox->showGrades();
        }
    }
}

void MSHome::goToQRCodeTab()
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onQRForce();
}

void MSHome::goToMaterialTab(mj::model::MaterialInfo::Type type, int grade_id, const std::string& selected_ids)
{
    ms::EventLogger::getInstance().logEvent("goToMaterialTab", {
        {"type",cocos2d::Value(type == mj::model::MaterialInfo::Type::NONE?"none": (type ==mj::model::MaterialInfo::Type::STORY?"story":"phonic"))},
        {"grade_id",cocos2d::Value(grade_id)},
        {"selected_ids",cocos2d::Value(selected_ids)},
    });
    
    m_sidebar->onMaterialForce();
    
    if (m_tabMaterial)
    {
        if (type == mj::model::MaterialInfo::Type::STORY)
        {
            m_tabMaterial->onSelectStoriesWithItems(selected_ids, grade_id);
        }
        else if (type == mj::model::MaterialInfo::Type::PHONICS)
        {
            m_tabMaterial->onSelectPhonicsWithItems(selected_ids, grade_id);
        }
    }
}

void MSHome::goToSubscribeLayout()
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onSubscriptionsForce();
}

void MSHome::goToSettingsLayout()
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onSettingsForce();
}

void MSHome::goToAccountProfileDetailLayout()
{
    CRASH_AUTO_LOG;
    
    auto profile_detail = APAccountProfileDetail::createPage();
    profile_detail->setParentDelegate(this);
    profile_detail->adjustWidth(this->getContentSize().width);
    this->addChild(profile_detail, INT_MAX - 1);
}

void MSHome::goToAccountLayout(bool showCalculator)
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onAccountForce(showCalculator);
}

void MSHome::goToParentAccountProfilePage()
{
    CRASH_AUTO_LOG;
    
    m_sidebar->onSettingsForce();
    
    auto settingsLayout = (StoryParentPage_Landscape *)this->getChildByName(mshome_name_storyparentpage_landscape);
    if (settingsLayout)
    {
        settingsLayout->goToAccountProfilePage();
        this->removeChildByName(mshome_name_tabsubscribe);
    }
}

void MSHome::goToLicensePage()
{
    m_isLicenseShow = true;
    
    onSelectedTab(ms::HomeTab::SETTINGS, ms::MovingDirection::NONE);
}

void MSHome::goToParent()
{
    m_sidebar->onParentForce();
}

#pragma mark - Zzz
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onExit()
{
    ui::Layout::onExit();
    
    m_haveExited = true;
}

void MSHome::onShowFreeLessonIcon()
{
    if (auto freeLessonButtonBackground = utils::findChild<ui::Button*>(this, "free_button"))
    {
        if (F2P_MANAGER->isSkipUser() || APPURCHASE_MANAGER->isFreeUser() || APPURCHASE_MANAGER->isUserTrial())
        {
            freeLessonButtonBackground->setVisible(true);
            freeLessonButtonBackground->setZOrder(6);

            if (auto freeLessonBackgroundText = utils::findChild<ui::ImageView*>(freeLessonButtonBackground, "bg_free_lesson_text"))
            {
                _freeLessonContentText = freeLessonBackgroundText;
                _freeLessonContentText->setZOrder(1);
                _freeLessonContentText->runAction(Sequence::create(FadeIn::create(1), DelayTime::create(3), FadeOut::create(0.5), NULL));
            }

            freeLessonButtonBackground->addClickEventListener([=](Ref* sender) {
                if (_clickFreeLessonButton)
                {
                    auto scene = MSFreeContentPage::createScene();
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
                }
                });

            freeLessonButtonBackground->addTouchEventListener([=](Ref* sender, ui::Widget::TouchEventType event) {
                switch (event) {
                case ui::Widget::TouchEventType::BEGAN:
                {
                    if (_freeLessonContentText)
                    {
                        _freeLessonContentText->setVisible(false);
                    }
                    break;
                }
                case ui::Widget::TouchEventType::MOVED:
                {
                    onMoveFreeButton(freeLessonButtonBackground);
                    break;
                }
                case ui::Widget::TouchEventType::ENDED:
                {

                }
                case ui::Widget::TouchEventType::CANCELED:
                {
                    if (freeLessonButtonBackground->getTouchBeganPosition().getDistance(freeLessonButtonBackground->getTouchEndPosition()) != 0) {
                        _clickFreeLessonButton = false;
                        auto visiable = Director::getInstance()->getVisibleSize();
                        auto buttonSize = freeLessonButtonBackground->getContentSize();
                        cocos2d::Vec2 sidebarContentSize = Vec2(0, 0);
                        if (auto sidebarContent = utils::findChild<Layout*>(m_sidebar, "content"))
                        {
                            sidebarContentSize = sidebarContent->getContentSize();
                        }

                        if ((freeLessonButtonBackground->getPosition().x - sidebarContentSize.x) <= (visiable.width - sidebarContentSize.x) / 2)
                        {
                            freeLessonButtonBackground->runAction(MoveTo::create(0.5, Vec2(sidebarContentSize.x + buttonSize.width / 2, freeLessonButtonBackground->getPosition().y)));
                        }
                        else {
                            freeLessonButtonBackground->runAction(MoveTo::create(0.5, Vec2(visiable.width - buttonSize.width / 2, freeLessonButtonBackground->getPosition().y)));
                        }
                    }
                    else
                    {
                        _clickFreeLessonButton = true;
                    }
                    break;
                }
                default:
                    break;
                }
                });
        }
        else {
            freeLessonButtonBackground->setVisible(false);
        }
    }
   
}

void MSHome::onMoveFreeButton(cocos2d::ui::Button* btn)
{
    auto touchMovePosition = btn->getTouchMovePosition();
    auto visiable = Director::getInstance()->getVisibleSize();
    auto buttonSize = btn->getContentSize();
    cocos2d::Vec2 panelOfTabStories = Vec2(0, 0);
    if (cocos2d::ui::Layout* panel_2 = utils::findChild<Layout*>(m_tabStories, "Panel_2"))
    {
        panelOfTabStories = panel_2->getContentSize();
    }

    cocos2d::Vec2 sidebarContentSize = Vec2(150, 150);
    if (auto sidebarContent = utils::findChild<Layout*>(m_sidebar, "content"))
    {
        sidebarContentSize = sidebarContent->getContentSize();
    }

    if (touchMovePosition.x <= (buttonSize.width / 2 + sidebarContentSize.x) && (touchMovePosition.y > visiable.height - buttonSize.height / 2 - panelOfTabStories.y))
    {
        btn->setPosition(Vec2(buttonSize.width / 2 + sidebarContentSize.x, visiable.height - buttonSize.height / 2 - panelOfTabStories.y));
    } 
    else if (touchMovePosition.x <= (buttonSize.width / 2 + sidebarContentSize.x) && (touchMovePosition.y < buttonSize.height / 2))
    {
        btn->setPosition(Vec2(buttonSize.width / 2 + sidebarContentSize.x, buttonSize.height / 2));
    }
    else if ((touchMovePosition.x > visiable.width - buttonSize.width / 2) && (touchMovePosition.y > visiable.height - buttonSize.height / 2 - panelOfTabStories.y))
    {
        btn->setPosition(Vec2(visiable.width - buttonSize.width / 2, visiable.height - buttonSize.height / 2 - panelOfTabStories.y));
    }
    else if ((touchMovePosition.x > visiable.width - buttonSize.width / 2) && (touchMovePosition.y < buttonSize.height / 2))
    {
        btn->setPosition(Vec2(visiable.width - buttonSize.width / 2, buttonSize.width / 2));
    }
    else if (touchMovePosition.x <= (buttonSize.width / 2 + sidebarContentSize.x))
    {
        btn->setPosition(Vec2(buttonSize.width / 2 + sidebarContentSize.x, btn->getTouchMovePosition().y));
    } 
    else if (touchMovePosition.x > visiable.width - buttonSize.width / 2) {
        btn->setPosition(Vec2(visiable.width - buttonSize.width / 2, btn->getTouchMovePosition().y));
    }
    else if (touchMovePosition.y < buttonSize.height / 2 )
    {
        btn->setPosition(Vec2(btn->getTouchMovePosition().x, buttonSize.height / 2));
    }
    else if (touchMovePosition.y > visiable.height - buttonSize.height / 2 - panelOfTabStories.y)
    {
        btn->setPosition(Vec2(btn->getTouchMovePosition().x, visiable.height - buttonSize.height / 2 - panelOfTabStories.y));
    }
    else {
        btn->setPosition(btn->getTouchMovePosition());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::deleteSidebarBlack()
{
    /*if (auto clipperBlack = utils::findChild(Director::getInstance()->getRunningScene(), "clipper_black"))
    {
        clipperBlack->removeFromParent();
    }*/
}

void MSHome::onSelectedTab(ms::HomeTab i_tab, ms::MovingDirection i_direction)
{
    mj::helper::SetFpsByNumberFrames(40);
    this->scheduleOnce([](float dt)
        {

            mj::helper::purgeTextureData();

        }, 1.5, "purge_cached");

    ms::EventLogger::getInstance().logEvent("onSelectedTab", {
        {"tab",cocos2d::Value((int)i_tab)},
    });

    if (i_tab == m_currentTab && i_tab != ms::SETTINGS && i_tab != ms::LICENSE && i_tab != ms::PROMOTION && i_tab != ms::SUBSCRIPTIONS && i_tab != ms::ACCOUNT)
    {
        return;
    }

    if(i_tab == ms::HomeTab::LESSONS && (m_currentTab == ms::HomeTab::LESSONS_PHONIC|| m_currentTab == ms::HomeTab::LESSONS_COMPREHENSION))
    {
        if(auto content = utils::findChild(this, "tab_content"))
        {
           // deleteSidebarBlack();
            if(m_tabLessonSelect== nullptr)
            {
                m_tabLessonSelect = MSTabLessonSelect::createTab();
                m_tabLessonSelect->setContentSize(content->getContentSize());
                m_tabLessonSelect->setVisible(false);
                m_tabLessonSelect->setDelegate(this);
                content->addChild(m_tabLessonSelect);
            }

            content->removeChildByName("content");
            m_tabLessonSelect->setVisible(true);
            m_tabLessonSelect->setPosition(Vec2::ZERO);
        }
      //  deleteSidebarBlack();
        m_currentTab = i_tab;

        return;
    }

    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
    }
    else {
        if (i_tab != ms::SETTINGS && i_tab != ms::LICENSE && i_tab != ms::PROMOTION && i_tab != ms::SUBSCRIPTIONS && i_tab != ms::ACCOUNT)
        {
            m_currentTab = i_tab;
        }
    }

    switch (i_tab)
    {
        case ms::HomeTab::LESSONS:
        {
            mj::helper::SetFpsByNumberFrames(20);
            MJDEFAULT->setIntegerForKey(key_current_hometab, 1);

            const bool k_isPhonicEnabel = MJDEFAULT->getBoolForKey(key_feature_phonics,true);

            if(k_isPhonicEnabel && !CONFIG_MANAGER->isVietnameseStories()) // comprehension and phonic
            {
                if(m_tabLessonSelect== nullptr)
                {
                    auto content = utils::findChild(this, "tab_content");
                    m_tabLessonSelect = MSTabLessonSelect::createTab();
                    m_tabLessonSelect->setContentSize(content->getContentSize());
                    m_tabLessonSelect->setVisible(false);
                    m_tabLessonSelect->setDelegate(this);
                    content->addChild(m_tabLessonSelect);
                }

                ms::EventLogger::getInstance().logEvent("selected_lesson_tab", {
                    {"type",cocos2d::Value("phonic")},
                });
                if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
                {
                    if (m_prevTab != m_tabLessonSelect)
                        hideAllTabs();
                }
                else {
                    hideAllTabs();
                }
                transitionBetweenTab(m_prevTab, m_tabLessonSelect, i_direction);
            }
            else //only comprehension lesson
            {
              //  deleteSidebarBlack();
                ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
                MJDEFAULT->setBoolForKey(key_is_phonic_lesson, false);

                if(m_comprehensionLesson == nullptr)
                {
                    auto content = utils::findChild(this, "tab_content");
                    m_comprehensionLesson = MSTabLessons::createTab(ms::GradeManager::Scope::LESSONS);//gradeBox
                    m_comprehensionLesson->setContentSize(content->getContentSize());
                    m_comprehensionLesson->setVisible(false);
                    //tab_lessons2->setName("content");
                    content->addChild(m_comprehensionLesson);
                }

                ms::EventLogger::getInstance().logEvent("selected_lesson_tab", {
                    {"type",cocos2d::Value("comprehension")},
                });
                if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
                {
                    if (m_prevTab != m_tabLessonSelect)
                        hideAllTabs();
                }
                else {
                    hideAllTabs();
                }
                transitionBetweenTab(m_prevTab, m_comprehensionLesson, i_direction);
            }

            break;
        }
        case ms::HomeTab::STORIES:
        {
            mj::helper::SetFpsByNumberFrames(10);
            ms::EventLogger::getInstance().logEvent("selected_stories_tab", {
            });

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
            {
                ms::CacheManager::getInstance().setListInfoAutoPlay();
                return;
            }
//#endif
            //deleteSidebarBlack();
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                if (m_prevTab != m_tabStories)
                    hideAllTabs();
            }
            else {
                hideAllTabs();
            }
            transitionBetweenTab(m_prevTab, m_tabStories, i_direction);
            MJDEFAULT->setIntegerForKey(key_current_hometab, 0);
            break;
        }
        case ms::HomeTab::MATERIAL:
        {   mj::helper::SetFpsByNumberFrames(20);
            ms::EventLogger::getInstance().logEvent("selected_material_tab", {
            });

            //if (m_tabMaterial != nullptr)
            //{
            //    m_tabMaterial->removeFromParent();
            //    m_tabMaterial = nullptr;
            //}

            auto content = utils::findChild(this, "tab_content");
            if (!m_tabMaterial)
            {
                m_tabMaterial = MSTabMaterial::createTab();
                m_tabMaterial->setContentSize(content->getContentSize());
                m_tabMaterial->setVisible(false);
                m_tabMaterial->setMSHome(this);
                m_tabMaterial->setPosition(Vec2::ZERO);
                content->addChild(m_tabMaterial);
                content->removeChildByName("content");
            }
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                if (m_prevTab != m_tabMaterial)
                    hideAllTabs();
            }
            else {
                hideAllTabs();
            }
            transitionBetweenTab(m_prevTab, m_tabMaterial, i_direction);
            break;
        }
        case ms::SUBSCRIPTIONS:
        {
            mj::helper::SetFpsByNumberFrames(15);
            try
            {
                ms::EventLogger::getInstance().logEvent("selected_subscription_tab", {
                    });

                CleverTapManager::GetInstance()->pushEvent("click_shop", {
                    {"type", cocos2d::Value(MJDEFAULT->getBoolForKey(key_click_shop_type_popup, false) ? "popup" : "organic")}
                    });
            }
            catch (const std::exception&)
            {

            }

            MS_LOGGER.logEvent("click_shop",{ });

            if (MJDEFAULT->getStringForKey(key_source_subscribe) == "")
            {
                MJDEFAULT->setStringForKey(key_source_subscribe, "organic");
            }

            auto subscribeTab = MSNewTabSubscribe::createTab();
            subscribeTab->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
            subscribeTab->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);


            subscribeTab->setName(mshome_name_tabsubscribe);
            this->removeChildByName(mshome_name_tabsubscribe);
            this->addChild(subscribeTab, kMSHomeZOrder_parentSetting);
            subscribeTab->runActionSequenAnimation();
            subscribeTab->setOpacity(0);
            subscribeTab->runAction(FadeIn::create(1));

            MJDEFAULT->setBoolForKey(key_click_shop_type_popup, false);

            break;
        }
        case ms::SETTINGS:
        {
            mj::helper::SetFpsByNumberFrames(10);
            ms::EventLogger::getInstance().logEvent("selected_settings_tab", {
            });

            auto parentSettingsTab = StoryParentPage_Landscape::createPage();
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                parentSettingsTab->setDelegate(this);
            }
            else {
            }
            parentSettingsTab->setName(mshome_name_storyparentpage_landscape);

            if (m_isLicenseShow)
            {
                parentSettingsTab->goToLicensePage();
                m_isLicenseShow = false;
            }
            this->removeChildByName(mshome_name_storyparentpage_landscape);
            this->addChild(parentSettingsTab, kMSHomeZOrder_parentSetting);
            parentSettingsTab->setOpacity(0);
            parentSettingsTab->runAction(FadeIn::create(0.25));
            break;
        }
        case ms::ACCOUNT:
        {
            mj::helper::SetFpsByNumberFrames(15);
            ms::EventLogger::getInstance().logEvent("selected_account_tab", {
            });

            auto accountTab = APAccountUserInfo::createView();
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                accountTab->setDelegate(this);
            }
            else {
            }
            accountTab->setName(mshome_name_account_userinfo);
            this->removeChildByName(mshome_name_account_userinfo);
            this->addChild(accountTab, kMSHomeZOrder_parentSetting);
            accountTab->setOpacity(0);
            accountTab->runAction(FadeIn::create(0.25));
            break;
        }
        case ms::LICENSE:
        {
            mj::helper::SetFpsByNumberFrames(15);
            ms::EventLogger::getInstance().logEvent("selected_license_tab", {
            });

            auto tabParent = StoryParentPage_Landscape::createPage();
            tabParent->goToLicensePage();
            tabParent->setName(mshome_name_storyparentpage_landscape);
            this->removeChildByName(mshome_name_storyparentpage_landscape);
            this->addChild(tabParent, kMSHomeZOrder_parentSetting);
            tabParent->setOpacity(0);
            tabParent->runAction(FadeIn::create(0.25));
            break;
        }
        case ms::AUDIOBOOKS :
        {
            mj::helper::SetFpsByNumberFrames(10);
            ms::EventLogger::getInstance().logEvent("selected_audiobook_tab", { });

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
            {
                ms::CacheManager::getInstance().setListInfoAutoPlayAudiobook();
                return;
            }
//#endif
           // deleteSidebarBlack();
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                if (m_prevTab != m_tabAudiobooks)
                    hideAllTabs();
            }
            else {
                hideAllTabs();
            }

            transitionBetweenTab(m_prevTab, m_tabAudiobooks, i_direction);
            MJDEFAULT->setIntegerForKey(key_current_hometab, 3);
            break;
        }
        case ms::PROMOTION:
        {
            mj::helper::SetFpsByNumberFrames(15);
            ms::EventLogger::getInstance().logEvent("selected_promotion_tab", { });

            auto drawer = MSAppDrawer::createDrawer();
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                drawer->setDelegate(this);
            }
            else {
            }
            drawer->setName(mshome_name_appdrawer);
            Director::getInstance()->getRunningScene()->removeChildByName(mshome_name_appdrawer);
            Director::getInstance()->getRunningScene()->addChild(drawer);
            break;
        }
        case ms::FREE:
        {
            mj::helper::SetFpsByNumberFrames(20);
            ms::EventLogger::getInstance().logEvent("selected_free_tab", { });

            if(m_tabFree == nullptr)
            {
                //deleteSidebarBlack();
                auto content = utils::findChild(this, "tab_content");
                m_tabFree = MSTabFree::createTab();
                m_tabFree->setContentSize(content->getContentSize());
                //tab_free->setName("content");
                m_tabFree->setTag(10);
                content->addChild(m_tabFree);
            }

            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                if (m_prevTab != m_tabFree)
                    hideAllTabs();
            }
            else {
                hideAllTabs();
            }
            transitionBetweenTab(m_prevTab, m_tabFree, i_direction);
          //  deleteSidebarBlack();

            break;
        }
        case ms::FAVOURITE:
        {
            mj::helper::SetFpsByNumberFrames(20);
            ms::EventLogger::getInstance().logEvent("selected_favorite_tab", { });

            //deleteSidebarBlack();
            ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);//same create function.
            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
            {
                if (m_prevTab != m_tabMaterial->getMSTabFavourite())
                    hideAllTabs();
            }
            else {
                hideAllTabs();
            }
            transitionBetweenTab(m_prevTab, m_tabMaterial->getMSTabFavourite(), i_direction);
            break;
        }

//        case ms::AI_SPEAKING:
//        {
//            if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
//            {
//                ms::EventLogger::getInstance().logEvent("selected_ai_speaking_tab", {
//                    });
//
//                //deleteSidebarBlack();
//                ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AISPEAKING);
//                if (m_prevTab != _tabAISpeaking)
//                    hideAllTabs();
//                transitionBetweenTab(m_prevTab, _tabAISpeaking, i_direction);
//                _tabAISpeaking->onReloadAISpeakingPage();
//                _tabAISpeaking->setVisible(true);
//                MJDEFAULT->setIntegerForKey(key_current_hometab, 0);
//            break;
//            }
//        }
    }

    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        m_currentTab = i_tab;
    }
   
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::transitionBetweenTab(ui::Layout * i_prevTab, ui::Layout * i_currTab, ms::MovingDirection i_direction)
{
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
    {
        if (i_prevTab == i_currTab)
            return;
    }
    if(m_prevTab == (ui::Layout*)m_comprehensionLesson)
    {
        m_comprehensionLesson->removeLessonReviewLayout();
    }

    if(m_prevTab == (ui::Layout*)m_tabAudiobooks)
    {
        m_tabAudiobooks->clearUnusedContents();
    }

    if(m_prevTab == (ui::Layout*)m_tabStories)
    {
        m_tabStories->clearUnusedContents();
    }
//    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
//    {
//        if (m_prevTab == (ui::Layout*)_tabAISpeaking)
//        {
//            _tabAISpeaking->clearUnusedContents();
//        }
//    }
    const float k_positionY   = i_direction * Director::getInstance()->getVisibleSize().height;

    if (i_prevTab)
    {
        i_prevTab->setVisible(true);
        i_prevTab->runAction(FadeOut::create(0.25f));
        i_prevTab->runAction(Sequence::create(EaseSineOut::create(MoveBy::create(0.25, Vec2(0, 0 - k_positionY))),cocos2d::Hide::create(),nullptr));
    }

    i_currTab->setVisible(true);
    i_currTab->setPosition(Point(0, k_positionY));
    i_currTab->runAction(MoveTo::create(0.25, Point::ZERO));
    i_currTab->setOpacity(0);
    i_currTab->runAction(FadeIn::create(0.25));

    m_prevTab = i_currTab;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onSelectedLanguage(bool i_isShow, std::string i_langCode)
{
    ms::EventLogger::getInstance().logEvent("onSelectedLanguage", {

        {"is_show",cocos2d::Value(i_isShow)},
        {"lang_code",cocos2d::Value(i_langCode)},
    });


    auto content = utils::findChild(this, "tab_content");

    if (i_isShow)
    {
        m_sidebar->setLocalZOrder(2);

        auto layout = ui::Layout::create();
        layout->setContentSize(content->getContentSize());
        layout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B::BLACK);
        layout->setTouchEnabled(true);
        layout->setOpacity(200);
        layout->setName("overlay");
        layout->addClickEventListener([=](Ref *) {

            m_sidebar->hideLanguages();
            layout->removeFromParent();

        });

        content->addChild(layout);

        return;
    }

    m_sidebar->setLocalZOrder(0);

    if (content->getChildByName("overlay"))
    {
        content->removeChildByName("overlay");
    }

    auto isLanguageChanged = StoryLanguageManager::setCurrentLanguage(i_langCode);

    if (isLanguageChanged)
    {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_language_changed);
    }
}




void MSHome::onPushEventForWinMac()
{

    auto listEvent = PROFILE_MANAGER.getInstance().getDataPushEvent();

    int timeDelay = 1;

    for (auto eventPush : listEvent)
    {
        std::string NameKey = "delay_pushEvent" + std::to_string(timeDelay);

       this->scheduleOnce([=](float dt) {
           CleverTapManager::GetInstance()->CleverTapApi(eventPush.second, nullptr, eventPush.first);
       }, timeDelay + 1.f, NameKey);

       timeDelay++;
    }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onboarding()
{
    auto onboard = CSLoader::getInstance()->createNode("csb/new_ui/intro/Onboard2.csb");

    auto bg = utils::findChild<ui::ImageView *>(onboard, "background");
    bg->setScale(0.73);

    if(auto onBoardIU = utils::findChild<ui::Widget*>(onboard, "arrow_v"))
    {
        auto stencil = ui::Scale9Sprite::create("account/grey9path.png");
        stencil->setScale9Enabled(true);
        stencil->setContentSize({250,50});
        stencil->setPosition(m_tabStories->getGradePosition());

        ClippingNode *clipper = ClippingNode::create();
        clipper->setStencil(stencil);
        clipper->setAlphaThreshold(0.8f);
        clipper->setInverted(true);
        this->addChild(clipper, kMSHomeZOrder_sideBar);

        auto layerColor = LayerColor::create(Color4B::BLACK);
        layerColor->setIgnoreAnchorPointForPosition(false);
        layerColor->setOpacity(0.75f*255.0f);
        layerColor->setAnchorPoint(Vec2(0.5f,0.5f));
        layerColor->setContentSize(Director::getInstance()->getVisibleSize());
        layerColor->setPosition(Director::getInstance()->getVisibleOrigin()+Director::getInstance()->getVisibleSize()/2);
        clipper->addChild(layerColor);

        auto lockButton = Button::create();
        lockButton->setScale9Enabled(true);
        lockButton->setIgnoreAnchorPointForPosition(false);
        lockButton->setAnchorPoint(Vec2(0.5f,0.5f));
        lockButton->setPosition(layerColor->getContentSize()/2);
        lockButton->setContentSize(Director::getInstance()->getVisibleSize());
        layerColor->addChild(lockButton,-1);

        auto guide = onBoardIU->clone();
        guide->setScale(0.8f);
        guide->setName("update_popup");
        this->addChild(guide, kMSHomeZOrder_sideBar);

        auto pos = m_tabStories->getGradePosition();
        guide->setPosition(pos+Vec2(0,-m_tabStories->getGradeSize().height*0.5f));


        if(auto closeButton = utils::findChild<ui::Button*>(guide,"button_next"))
        {
            closeButton->setCascadeColorEnabled(false);

            if (FileUtils::getInstance()->isFileExist(FRAME_GRADEBOX_BG))
            {
                auto m_extendCloseButton = ui::Button::create(FRAME_GRADEBOX_BG);
                m_extendCloseButton->setContentSize(closeButton->getContentSize());
                m_extendCloseButton->setPosition(Point(closeButton->getContentSize().width / 2, closeButton->getContentSize().height / 2));
                m_extendCloseButton->setScale(5);
                m_extendCloseButton->setScaleX(20);
                m_extendCloseButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                m_extendCloseButton->setColor(Color3B::RED);
                m_extendCloseButton->setEnabled(true);
                m_extendCloseButton->setZOrder(closeButton->getZOrder() + 1);
                m_extendCloseButton->setOpacity(0);
                closeButton->addChild(m_extendCloseButton);
                closeButton->setTouchEnabled(false);
                m_extendCloseButton->addTouchEventListener([=](Ref*, Widget::TouchEventType) {
                    btnClickedEffect(closeButton);
                    });
                m_extendCloseButton->addClickEventListener([=](Ref*)
                    {
                        MJDEFAULT->setBoolForKey(key_onboarding_part_2, true);

                        mm::RealtimeDatabase::getInstance().updateUserSettings();

                        clipper->removeFromParent();
                        guide->removeFromParent();

                        if (auto scene = Director::getInstance()->getRunningScene())
                        {
                            auto check_node = scene->getChildByName("update_popup");

                            if (check_node)
                            {
                                check_node->removeFromParent();
                            }
                        }

                    });
            }
            closeButton->addClickEventListener([=](Ref*)
                                               {
                MJDEFAULT->setBoolForKey(key_onboarding_part_2, true);

                mm::RealtimeDatabase::getInstance().updateUserSettings();

                clipper->removeFromParent();
                guide->removeFromParent();

                if(auto scene = Director::getInstance()->getRunningScene())
                {
                    auto check_node = scene->getChildByName("update_popup");

                    if (check_node)
                    {
                        check_node->removeFromParent();
                    }
                }

            });
        }

        if(auto labelNext = utils::findChild<ui::Text *>(guide, "label_next"))
        {
            labelNext->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");

            labelNext->setString(LANGUAGE_MANAGER->getDisplayTextByKey("onboard.finish"));
        }

        if(auto title = utils::findChild<ui::Text *>(guide, "title"))
        {
            title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");

            title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("new_onboard_title"));
        }

        if(auto optout = utils::findChild(guide, "optout"))
        {
            optout->removeFromParent();
        }

    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onFinishOnboard()
{
    //    if (CONFIG_MANAGER->isAppVNStories() && CONFIG_MANAGER->isCountryVN()) {
    //        auto view = DatePicker::showPicker();
    //        this->addChild(view, 10);
    //        view->setAnchorPoint(Point::ANCHOR_MIDDLE);
    //        view->setPosition(Director::getInstance()->getVisibleSize()/2);
    //    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onSelectedMaterial(mj::model::MaterialInfo::Type type, const std::string& selected_ids)
{
   
    switch (type) {
        case mj::model::MaterialInfo::Type::STORY: m_currentTab = ms::HomeTab::MATERIAL_STORIES; break;
        case mj::model::MaterialInfo::Type::PHONICS: m_currentTab = ms::HomeTab::MATERIAL_PHONICS; break;
        default: break;
    }

    auto collection_view = MJWorksheet::createView(type);
    collection_view->setContentSize(this->getContentSize());
    ui::Helper::doLayout(collection_view);
    collection_view->setOpacity(0);
    collection_view->runAction(FadeIn::create(0.25f));
    this->addChild(collection_view, kMSHomeZOrder_contentFull);
}

void MSHome::goToSignInForF2PMerge()
{
    if (!F2P_MANAGER->isCanMergeProfile())
        return;
    CCLOG("Sign in");
    auto loginScreen = APLoginView::createScene(APLoginView::SCREEN_TYPE::LOGIN);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, loginScreen));
}
void MSHome::btnClickedEffect(ui::Button* btn)
{
    auto m_effectClickCloseButton = Sequence::create(ScaleTo::create(0.15f, 1.2), ScaleTo::create(0.15f, 1), NULL);
    btn->runAction(m_effectClickCloseButton);
}

void MSHome::onSelectedLesson(ms::LessonTab tab, int force_cat_id, int force_les_id)
{
    mj::helper::SetFpsByNumberFrames(8);
    MS_LOGGER.logEvent("click_lesson", {
        {"lesson_type",cocos2d::Value(tab == ms::LessonTab::STORIES_LESSON ? "STORIES" : "PHONICS")}
    });
    
    switch(tab)
    {
        case ms::LessonTab::STORIES_LESSON:
        {
            MJDEFAULT->setBoolForKey(key_is_phonic_lesson, false);
            m_currentTab = ms::HomeTab::LESSONS_COMPREHENSION;
            auto tab_lessons = MSTabLessons::createTab(ms::GradeManager::Scope::LESSONS);
            tab_lessons->setContentSize(Director::getInstance()->getVisibleSize());
            tab_lessons->setName("content");
            this->addChild(tab_lessons, kMSHomeZOrder_contentFull);
            
            if (force_cat_id > 0 && force_les_id > 0) {
                tab_lessons->loadCustomGradeWithID(force_cat_id);
                tab_lessons->openLessonForce(force_cat_id, force_les_id);
            }
            break;
        }
        case ms::LessonTab::PHONICS_LESSON:
        {
            m_currentTab = ms::HomeTab::LESSONS_PHONIC;
            MJDEFAULT->setBoolForKey(key_is_phonic_lesson, true);
            auto tab_lessons = MSTabLessons::createTab(ms::GradeManager::Scope::LESSONS_PHONIC);
            tab_lessons->setContentSize(Director::getInstance()->getVisibleSize());
            tab_lessons->setName("content");
            this->addChild(tab_lessons, kMSHomeZOrder_contentFull);
            MJDEFAULT->setIntegerForKey(key_current_hometab, 2);
            
            if (force_cat_id > 0 && force_les_id > 0) {
                tab_lessons->openLessonForce(force_cat_id, force_les_id);
            }
            break;
        }
        default:
            break;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::hideAllTabs()
{
    if(auto content = utils::findChild(this, "tab_content"))
    {
        content->removeChildByName("content");
    }
    
    if(m_tabFavourite)
    {
        m_tabFavourite->setVisible(false);
    }
    
    if(m_tabFree)
    {
        m_tabFree->setVisible(false);
    }
    
    if(m_tabAudiobooks)
    {
        m_tabAudiobooks->setVisible(false);
    }
    
    if(m_tabStories)
    {
        m_tabStories->setVisible(false);
    }
    
    if(m_comprehensionLesson != nullptr)
    {
        m_comprehensionLesson->setVisible(false);
    }
    
    if(m_tabLessonSelect!=nullptr)
    {
        m_tabLessonSelect->setVisible(false);
    }
    
    if(m_tabMaterial != nullptr)
    {
        m_tabMaterial->setVisible(false);
    }
//    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true))
//    {
//        if (_tabAISpeaking != nullptr)
//        {
//            _tabAISpeaking->setVisible(false);
//        }
//    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::intCheatMenu()
{
    auto test_button = ui::Button::create("mjstory/new_ui/button_lesson_learn.png");
    test_button->setScale9Enabled(true);
    test_button->setCapInsets(Rect(15, 15, 100, 5));
    this->addChild(test_button, INT_MAX - 1);
    test_button->setPosition(Point(Director::getInstance()->getVisibleSize().width - 150, 50));
    test_button->setContentSize(Size(150, 80));
    test_button->addClickEventListener(CC_CALLBACK_1(MSHome::onTest, this));
    
    auto id =  std::to_string(ap::Database::getInstance().getUserInfo()._id);
    test_button->setTitleFontSize(12);
    test_button->setTitleText(id +"|"+ std::to_string(MJDEFAULT->getIntegerForKey(key_current_profile_id)));
    
    test_button->setVisible(false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSHome::onTest(cocos2d::Ref * sender)
{
  //  auto debugUI = DebugUI::createView();/
  //  this->addChild(debugUI,1000);
}

void MSHome::processQRCode(std::string url)
{
    //    auto pending_qr = MJDEFAULT->getStringForKey(key_qrcode_received, "");
    auto pending_qr = url.empty()? ms::analytics::Firebase::QR_DEEP_LINK:url;

    CCLOG("Handle deep link: %s - %s", pending_qr.c_str(), !m_haveExited ? "status ok":"status failed");
    
    //HelperManager::GetInstance()->writeLogToFile(pending_qr.empty()?"pending_qr is empty":pending_qr);
    
    if (!pending_qr.empty() && !m_haveExited)
    {
        ms::EventLogger::getInstance().logEvent("MSHomeprocessQRCode", {
            {"pending_qr",cocos2d::Value(url)},
        });
        
        //HelperManager::GetInstance()->writeLogToFile("Handle deep link...");
        
        CCLOG("Handle deep link...");
        std::map<std::string, std::string> url_parse = mj::helper::url_parse(pending_qr);
        
        if ( (url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_MS) != std::string::npos) || (url_parse[URL_DOMAIN].find(DEF_DOMAIN_CORRECT_VM) != std::string::npos) )
        {
            const std::string& part = url_parse[URL_ROOT_PART];
            
            // MARK: URL PART is "app"
            if (part.compare(DEF_DEEPLINK_PART_APP) == 0)
            {
                const std::map<std::string, std::string>& query_parse = mj::helper::url_query_parse(url_parse[URL_QUERY]);
                const std::string& taskValue = query_parse.find(DEF_DEEPLINK_KEYNAME_TASK)->second;
                
                // TODO: -- task: newprofile
                if (taskValue.compare("newprofile") == 0)
                {
                    this->clearLayouts();
                    this->addLayoutChooseProfile(true);
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "newprofile";
                }
                // TODO: -- task: chooseprofile
                else if (taskValue.compare("chooseprofile") == 0)
                {
                    this->clearLayouts();
                    this->addLayoutChooseProfile(false);
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: stories
                else if (taskValue.compare("stories") == 0)
                {
                    bool FLAG = true;;
                    
                    // Màn hình Truyện phổ biến
                    const std::string& grade = query_parse.find("grade")->second; //query_parse["grade"];
                    const std::string& collect = query_parse.find("collect")->second; //query_parse["collect"];
                    
                    if (collect.compare("popular") == 0 && grade.compare("default") == 0)
                    {
                        FLAG = false;
                        this->clearLayouts();
                        this->goToPopularStories();
                        
                        ms::analytics::Firebase::QR_DEEP_LINK = "";
                    }
                    
                    // Màn hình Chọn cấp độ
                    const std::string& display = query_parse.find("display")->second; //query_parse["display"];
                    
                    if (FLAG && display.compare("gradebox") == 0)
                    {
                        FLAG = false;
                        this->clearLayouts();
                        this->goToStoriesTab(true);
                        
                        ms::analytics::Firebase::QR_DEEP_LINK = "";
                    }
                    
                    // Màn hình Chủ đề truyện
                    const std::string& catid = query_parse.find("catid")->second; //query_parse["catid"];
                    
                    if (FLAG && catid.length())
                    {
                        auto catid_numb = std::stoi(catid);
                        auto categories = StoryDataModel::getInstance()->getCategories(StoryLanguageManager::getCurrentLangId());
                        
                        for (auto category : categories)
                        {
                            if (catid_numb == category.category_id)
                            {
                                FLAG = false;
                                this->clearLayouts();
                                this->goToStoriesTabWithCategory(catid_numb);
                                
                                ms::analytics::Firebase::QR_DEEP_LINK = "";
                            }
                        }
                    }
                    
                    if (FLAG)
                    {
                        this->clearLayouts();
                        this->goToStoriesTab();
                        
                        ms::analytics::Firebase::QR_DEEP_LINK = "";
                    }
                }
                // TODO: -- task: lessons
                else if (taskValue.compare("lessons") == 0)
                {
                    this->clearLayouts();
                    this->goToLessonsTab();
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: audiobooks
                else if (taskValue.compare("audiobooks") == 0)
                {
                    // hiển thị Chọn cấp độ
                    const std::string& display = query_parse.find("display")->second; //query_parse["display"];
                    bool showGrades = false;
                    
                    if (display.compare("gradebox") == 0)
                    {
                        showGrades = true;
                    }
                    
                    this->clearLayouts();
                    this->goToAudiobooksTab(showGrades);
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: qrcode
                else if (taskValue.compare("qrcode") == 0)
                {
                    this->clearLayouts();
                    this->goToQRCodeTab();
                    this->goToMaterialTab();
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: subscribe
                else if (taskValue.compare("subscribe") == 0)
                {
                    this->clearLayouts();
                    this->goToSubscribeLayout();
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: settings
                else if (taskValue.compare("settings") == 0)
                {
                    this->clearLayouts();
                    this->goToSettingsLayout();
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: moreapps
                else if (taskValue.compare("moreapps") == 0)
                {
                    this->clearLayouts();
                    this->goToPromotionLayout();
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: account
                else if (taskValue.compare("account") == 0)
                {
                    this->clearLayouts();
                    this->goToAccountLayout(true);
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: worksheet
                else if (taskValue.compare("worksheet") == 0)
                {
                    this->clearLayouts();
                    
                    // type
                    const std::string& str_type = query_parse.find("type")->second;
                    mj::model::MaterialInfo::Type type = mj::model::MaterialInfo::Type::NONE;
                    if (str_type.compare("stories") == 0)
                    {
                        type = mj::model::MaterialInfo::Type::STORY;
                    }
                    else if (str_type.compare("phonics") == 0)
                    {
                        type = mj::model::MaterialInfo::Type::PHONICS;
                    }
                    
                    // grade
                    const std::string& str_grade = query_parse.find("grade")->second;
                    int grade = std::stoi(mj::helper::isDigitString(str_grade) ? str_grade : "0");
                    
                    // selected ids
                    const std::string& str_selected_ids = query_parse.find("ids")->second;
                    
                    this->goToMaterialTab(type, grade, str_selected_ids.empty() ? "" : str_selected_ids);
                    
                    ms::analytics::Firebase::QR_DEEP_LINK = "";
                }
                // TODO: -- task: ...
                else
                {
                    // nothing here
                }
            }
            // MARK: URL PART is "qrstory"
            else if (part.compare(DEF_DEEPLINK_PART_QRSTORY) == 0)
            {
                std::map<std::string, std::string> query_parse = mj::helper::url_query_parse(url_parse[URL_QUERY]);
                std::string id_truyen_rieng = query_parse.find("id")->second;
                auto story_info = StoryDataModel::getInstance()->getStoryById(std::stoi(id_truyen_rieng));

                if (story_info.unique_id != 0)
                {
                    bool checkStoryIsTutoring = StoryDataModel::getInstance()->CheckStoryIsTutoring(story_info.unique_id);
                    bool user_is_tutoring = MJDEFAULT->getBoolForKey("key_check_user_is_tutoring", false);

                    if (!user_is_tutoring && checkStoryIsTutoring != 0)
                    {
                        Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                            StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.message.poup.incorrect.account"), LANGUAGE_MANAGER->getDisplayTextByKey("subscription.yes"), "Close");

                        });
                        return;
                    }

                    this->clearLayouts();
                    this->goToStoriesTab();
                    auto story_item = MSCollectionStoryItem::createItem(story_info);
                    mj::helper::playTransitionStoryCover(story_item, nullptr, -1);
                }
                else
                {
                    CCLOG("hieunt: ERROR - không có truyện!");
                }
                
                ms::analytics::Firebase::QR_DEEP_LINK = "";
            }
            
            // MARK: URL PART is "qraubook"
            else if (part.compare(DEF_DEEPLINK_PART_QRAUBOOK) == 0)
            {
                std::map<std::string, std::string> query_parse = mj::helper::url_query_parse(url_parse[URL_QUERY]);
                std::string str_audiobook_id = query_parse.find("id")->second;
                
                if (mj::helper::isDigitString(str_audiobook_id))
                {
                    int audiobook_id = std::stoi(str_audiobook_id);
                    mj::model::AudiobookInfo audiobook_info = StoryDataModel::getInstance()->getAudiobook(audiobook_id);
                    
                    if (audiobook_info.book_id > 0)
                    {
                        this->clearLayouts();
                        this->goToAudiobooksTab();
                        auto audiobook_item = MSCollectionAudiobookItem::createThumb(audiobook_info);
                        mj::helper::playTransitionAudiobookCover(audiobook_item, nullptr, 1);
                    }
                    else
                    {
                        CCLOG("hieunt: ERROR - không có audiobook!");
                    }
                }
                
                ms::analytics::Firebase::QR_DEEP_LINK = "";
            }
            // MARK: URL PART is "qrbook"
            else if (part.compare(DEF_DEEPLINK_PART_QRBOOK) == 0)
            {
                // dưới đây là các đoạn code cũ
                auto story_id = mj::helper::parseDynamicLinkQR(pending_qr);
                auto view = MSStoryAudioPlayer::createView(story_id);
                Director::getInstance()->getRunningScene()->removeChildByName("qr_story_player");
                Director::getInstance()->getRunningScene()->addChild(view, INT_MAX - 1);
                ms::analytics::Firebase::QR_DEEP_LINK = "";
            }
            else if ((part.compare(DEF_DEEPLINK_PART_QRPHONICS) == 0)||(part.compare(DEF_DEEPLINK_PART_QRREADING_COMPREHENSION) ==0))
            {
                auto query = url_parse[URL_QUERY];

                MMQRScan::handleQRDeeplinkLesson(query,part.compare(DEF_DEEPLINK_PART_QRPHONICS) == 0);
            }
            else
            {
                // nothing here
            }
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
   cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity","sClearData");
#endif

}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

void MSHome::changeIconAppForIos()
{
    //0 ngay, 1 thang, 2 nam
    std::vector <int> valueDateTime = mj::helper::getDateTimeIOS();
    int date = 0, month = 0, year = 0;
    
    if(valueDateTime.size() == 3)
    {
        date = valueDateTime[0];
        month = valueDateTime[1];
        year = valueDateTime[2];
    }
    else{
        return;
    }
    
    if(date >= 13 && month == 7)
    {
        MJDEFAULT->setIntegerForKey("key_country_change_icon_app", 1);
        
        if(CONFIG_MANAGER->isCountryVN())
        {
            mj::helper::changeIconAppForIos("ChangeIconVN");
            return;
        }
        
        if(CONFIG_MANAGER->isCountryThailand())
        {
            mj::helper::changeIconAppForIos("ChangeIconTH");
            return;
        }
        
        mj::helper::changeIconAppForIos("ChangeIconEN");
        return;
    }
    
}

void MSHome::revertIconAPPForIos()
{
    //0 ngay, 1 thang, 2 nam
    std::vector <int> valueDateTime = mj::helper::getDateTimeIOS();
    int date = 0, month = 0, year = 0;
    
    if(valueDateTime.size() == 3)
    {
        date = valueDateTime[0];
        month = valueDateTime[1];
        year = valueDateTime[2];
    }
    else{
        return;
    }
    
    if( month >= 8)
    {
        MJDEFAULT->setIntegerForKey("key_country_change_icon_app", -1);
        mj::helper::changeIconAppForIos("RevertIcon");
    }
}

#endif



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_processQRCode(JNIEnv* env, jobject thiz, jobject jUrl)
    {
        jstring str = (jstring)jUrl;
        std::string token;
        if (!str)
        {
            token = std::string();
        }
        jboolean isCopy;
        const char* chars = env->GetStringUTFChars(str, &isCopy);
        if (chars) {
            token = chars;
            env->ReleaseStringUTFChars(str, chars);
        }

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([token] {

            ms::analytics::DynamicLinksListener::OnDynamicLinkReceivedAndroid(token);

        });
    }
}

#endif

