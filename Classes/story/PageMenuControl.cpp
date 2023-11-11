//
//  PageMenuControl.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "PageMenuControl.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "TruyenTranhPage.h"
#include "MJ_AnimationCache.h"
#include "AppDelegate.h"
#include "StoryFreeManager.h"
#include "StoryPageVC.h"
#include "MJHelper.h"
#include "MJFirebaseListener.h"
#include "MSScreenRecorder.h"
#include "MJCalculator.h"
#include "APProfileManager.h"
#include "MMFirebaseDatabase.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MJPlatformConfig.h"
#include "MSCacheManager.h"

INITIALIZE_READER_INSTANCE(PageMenuControlReader);

USING_NS_CC;
using namespace ui;

void PageMenuControl::didLoadFromCSB()
{
    btfavorite = utils::findChild<ui::Button *>(this, "btfavorite");
    if (XHSLib->isFavoritesStory(XHSLib->_id)) {
        btfavorite->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_favourite.png");
    } else {
        btfavorite->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_favourite_empty.png");
    }
    class_name = "popup";
    btplay = static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btplay"));
    text_play = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_play"));
    text_play->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_pause"));
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_startover")),
            static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_favorite")),
            static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_share")),
            static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_close")),
            static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_play")),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf");
        }
    }
    
    static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_startover"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_startover"));
    static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_favorite"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_favorite"));
    static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_share"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_share"));
    static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_close"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_close"));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

	//remove the share button on PageMenuControl if target platform is windows.
	if (auto btshare = utils::findChild<ui::Button *>(this, "btshare"))
	{
		btshare->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_close.png");
		static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_share"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_close"));

		if (auto btclose = utils::findChild<ui::Button *>(this, "btclose"))
		{
			btshare->setTag(btclose->getTag());
			btclose->setVisible(false);

			if (auto parentOfCloseButton = btclose->getParent())
			{
				parentOfCloseButton->setVisible(false);
				parentOfCloseButton->removeFromParentAndCleanup(true);
			}
		}
		
		if (auto leftMenu = utils::findChild<ui::ImageView *>(this, "left"))
		{
			leftMenu->setPositionX(leftMenu->getParent()->getContentSize().width / 2);
		}
	}

#endif // 

}
#pragma mark - onEnter
void PageMenuControl::onEnter() {
    Layout::onEnter();
    didLoadFromCSB();
}

void PageMenuControl::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void PageMenuControl::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();

}

#pragma mark - BUTTON
void PageMenuControl::onClickButton(cocos2d::Ref *sender){
    
    CRASH_AUTO_LOG;
    
    AUDIO_ENGINE->playEffect("sounds/tap-mellow.mp3");
    if (!isClickEnable) return;
    auto tag = static_cast<Button *>(sender)->getTag();
    switch (tag) {
        case 201://play
        {
            if(auto recommentPage = utils::findChild(Director::getInstance()->getRunningScene(),"recomment_page")) {
                return;
            }
            
            if (XHSLib->pageIndex == 0 || XHSLib->pageIndex > XHSLib->pageNumAll) return;
            isPlay = !isPlay;
            if (isPlay) {
                mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "playstory_click", "true");
                btplay->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_pause.png");
                text_play->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_pause"));
            }else {
                mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "pause_click", "true");
                btplay->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_play.png");
                text_play->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_play"));
            }
            if (_delegate) {
                _delegate->onClickBtPlayPause(isPlay);
            }
        }
            break;
        case 202://start over
        {
            mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "startover_click", "true");
            
            auto parent = (StoryPageVC *) this->getParent();
            auto storyInfo = parent->getInfoStory();
            
            XHSLib->story_id = storyInfo.storyid;   // reset lại, đè việc set giá trị trong didLoadFromCSB() của StoryInfoCover
            XHSLib->lang_id = storyInfo.languageid; // reset lại, đè việc set giá trị trong didLoadFromCSB() của StoryInfoCover
            XHSLib->level_current = storyInfo.readinglevel; // reset lại, đè việc set giá trị trong onEnter() của StoryInfoCover
            XHSLib->_id = storyInfo._id;                    // reset lại, đè việc set giá trị trong onEnter() của StoryInfoCover
            
            XHSLib->isEndingPageClick = false;
            XHSLib->pageIndex = 1;
            
            auto home_scene = static_cast<StoryPageVC *>(CSLoader::createNode("csb/StoryPageVC.csb"));
            home_scene->setInfoStory(storyInfo, parent->is_activity_play, parent->is_sample_read);
            parent = nullptr;
            DIRECTOR->replaceScene(TransitionCrossFade::create(0.35, home_scene));
        }
            break;
        case 203://favorite
        {
            mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "favorite_click", "true");
            bool isFavorited = false;
            
            if (!XHSLib->isFavoritesStory(XHSLib->_id))
            {
                btfavorite->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_favourite.png");
                XHSLib->addFavoritesStory(XHSLib->_id);
                PROFILE_MANAGER.updateLoveStories(MJDEFAULT->getIntegerForKey(key_current_profile_id), XHSLib->_id);
                mm::RealtimeDatabase::getInstance().updateLoveStories(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id));
                isFavorited =true;
            }
            else
            {
                btfavorite->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_favourite_empty.png");
                XHSLib->removeFavoritesStory(XHSLib->_id);
                isFavorited =false;
                PROFILE_MANAGER.deleteLoveStories(MJDEFAULT->getIntegerForKey(key_current_profile_id), XHSLib->_id);
                mm::RealtimeDatabase::getInstance().deleteLoveStories(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id),XHSLib->_id);

            }

            _delegate ? _delegate->onClickBtFavorite(isFavorited) : (void) nullptr;

        }
            break;
        case 204://share
            MJCalculator::showCalculator(Director::getInstance()->getRunningScene(), [this]() {
                mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "share_click", "true");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                    mj::helper::shareStory();
                //            if (ms::recorder::isRecording()) {
                //                ms::recorder::stopRecord();
                //            } else {
                //                ms::recorder::startRecord();
                //            }
#endif
            });
            
            
            break;
        case 205://close
        {
            if (XHSLib->pageIndex < XHSLib->pageNumAll) {
                mm::RealtimeDatabase::getInstance().pushDataFirestore(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), XHSLib->_id);
                auto parent = (StoryPageVC *) this->getParent();
                parent->sendEventAnalyticsWhenCloseStory();
                parent = nullptr;
            }
      
            XHSLib->isEndingPageClick = false;
            XHSLib->fontNameStory = "";
            mj::AnimationCache::endSession();
            _delegate ? _delegate->onClickBtClose() : (void) nullptr;
            DIRECTOR->popScene([](Scene * scene) {
                scene->scheduleOnce([=](float dt) {
                    if (MJDEFAULT->getBoolForKey("key_is_auto_play_stroy", false))
                    {
                        ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlay();
                    }
                    
                    }, 1.0f, "delay_continue_story");
                return TransitionSlideInL::create(MJDEFAULT->getBoolForKey(key_is_phonic_lesson, false) ? 0.25 : 0.5, scene);
            });
            
            if (ms::recorder::isRecording()) {
                ms::recorder::stopRecord();
            }
        }
            break;
        default:
            break;
    }
}

void PageMenuControl::resetStateMenu(){
    isPlay = true;
    btplay->getChildByName<ui::ImageView *>("img")->loadTexture("mjstory/new_ui/button_story_menu_pause.png");
    text_play->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(class_name, "menu_pause"));
}

void PageMenuControl::setDelegate(PageMenuControlDelegate *delegate){
    _delegate = delegate;
}

void PageMenuControl::setClickEnable(bool is_click_enable){
    isClickEnable = is_click_enable;
}

void PageMenuControl::onOverlay(Ref * sender) {
    auto parent = dynamic_cast<StoryPageVC *>(this->getParent());
    if (parent) {
        parent->setShowHideMenuControl(false);
    }
}

void PageMenuControl::onShareFacebookCallback(bool succed) {
    _isSharing = false;
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback PageMenuControl::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(PageMenuControl, onClickButton),
        CLICK_MAP(PageMenuControl, onOverlay)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


