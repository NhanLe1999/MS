//
//  StoryWelcomePopUp.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryWelcomePopUp.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "MJDefinitions.h"
#include "MJFirebaseListener.h"
#include "MJHelper.h"
#include "CleverTapManager.h"
#include "MSEventLogger.h"

INITIALIZE_READER_INSTANCE(StoryWelcomePopUpReader);

USING_NS_CC;
using namespace ui;

StoryWelcomePopUp * StoryWelcomePopUp::createPage()
{
    auto csb_name = mj::helper::getCSBName("csb/StoryWelcomePopUp.csb");
    
    auto p = static_cast<StoryWelcomePopUp *>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (p)
    {
        p->didLoadFromCSB();
    }
    
    return p;
}

void StoryWelcomePopUp::didLoadFromCSB()
{
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_app_become_purchased, [this](EventCustom *)
    {
        this->onShowHidePopUp(false);
    }), this);
    setCallbackShowPop();
    this->onShowHidePopUp(true);
}

#pragma mark - onEnter
void StoryWelcomePopUp::onEnter()
{
    Layout::onEnter();

    _bg = this->getChildByName<ImageView*>(_isPortrait?"bg_doc":"bg_ngang");

    _bg->setVisible(true);
    title = _bg->getChildByName<Text *>("title");
    content = _bg->getChildByName<Text *>("content");
    titlesub = static_cast<Text *>(ui::Helper::seekWidgetByName(_bg, "titlesub"));
    titlefree = static_cast<Text *>(ui::Helper::seekWidgetByName(_bg, "titlefree"));

	LANGUAGE_MANAGER->updateLanguageWithID(MJDEFAULT->getIntegerForKey(key_lang_display));
    
    {
          std::vector<cocos2d::ui::Text*> textSameFonts =
          {
              utils::findChild<ui::Text *>(this, "content"),
              utils::findChild<ui::Text *>(this, "titlesub"),
              utils::findChild<ui::Text *>(this, "titlefree"),
          };
          
          for(auto text : textSameFonts)
          {
              if(!text)
              {
                  continue;
              }
              
              text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
          }
      }
    
    //title
    title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    title->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "welcome_title"));
    content->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name,CONFIG_MANAGER->isAppVNStories()? "welcome_content_vmonkey": "welcome_content"));
    
    titlesub->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "welcome_btsub"));
    titlefree->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "welcome_btfree"));
    
}

void StoryWelcomePopUp::onEnterTransitionDidFinish()
{
    Layout::onEnterTransitionDidFinish();
}

#pragma mark - onExit
void StoryWelcomePopUp::onExitTransitionDidStart()
{
    MS_LOGGER.logEvent("StoryWelcomePopUp_OnExit", {});
    _callbackShowPop = nullptr;
    Layout::onExitTransitionDidStart();
}

void StoryWelcomePopUp::onShowHidePopUp(bool isShow)
{
    if (_callbackShowPop)
    {
        _callbackShowPop(isShow);
    }
}

#pragma mark - BUTTON
void StoryWelcomePopUp::onSubscribe(cocos2d::Ref *sender)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
    {
        if (isRunning) return;
        
        pushCleverTapEvent("subcribe_button");
        index_choose = 1;
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_welcomepopup_subscribe, "", "");
        MJDEFAULT->setIntegerForKey(key_call_welcome_popup, 1);
        auto type = new int;
        * type = 1;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_welcome_popup_close, type);
        delete type;
        onShowHidePopUp(false);
        CCLOG("on_subscribe");
    });
}

void StoryWelcomePopUp::onFree(cocos2d::Ref *sender)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
    {
        if (isRunning) return;
        
        pushCleverTapEvent("free_button");
        index_choose = 2;
        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_welcomepopup_free, "", "");
        MJDEFAULT->setIntegerForKey(key_call_welcome_popup, 2);
        MJDEFAULT->setIntegerForKey(key_show_viral, 2);
        auto type = new int;
        * type = 2;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_welcome_popup_close, type);
        delete type;
        onShowHidePopUp(false);
        CCLOG("on_free");
    });
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryWelcomePopUp::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryWelcomePopUp, onSubscribe),
        CLICK_MAP(StoryWelcomePopUp, onFree),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void StoryWelcomePopUp::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value("home_screen")},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("welcome_popup")},
    });
}

void StoryWelcomePopUp::setCallbackShowPop()
{
    _callbackShowPop = [=](bool isShow) {
        if (isRunning) return;
        isRunning = true;
        auto csb_name = mj::helper::getCSBName("csb/StoryWelcomePopUp.csb");
        auto runani = CSLoader::createTimeline(StringUtils::format("%s", csb_name.c_str()));
        runani->setTag(1011);
        this->runAction(runani);
        if (isShow) {
            runani->play("show", false);
        }
        else runani->play("hide", false);
        runani->setLastFrameCallFunc([=] {
            isRunning = false;
            if (!isShow) {
                this->removeFromParent();
                //            Scene *scene;
                //            scene = static_cast<Scene *>(CSLoader::createNode("csb/StoryHome.csb"));
                //            DIRECTOR->replaceScene(TransitionCrossFade::create(0.35, scene));
            }
            });
    };
}
