//
//  StorySubscribePopup.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StorySubscribePopup.h"
#include "ui/UIHelper.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "StoryFreeManager.h"
#include "MJHelper.h"
#include "CleverTapManager.h"
#include "StoryConfigManager.h"
#include "MJDefault.h"
#include "MSEventLogger.h"
#include "F2PManager.h"
#include "MsSubscriptionLandscape.h"

INITIALIZE_READER_INSTANCE(StorySubscribePopupReader);

USING_NS_CC;
using namespace ui;

StorySubscribePopup * StorySubscribePopup::createSubscribePopup(int _id, int type)
{
    REGISTER_READER(StorySubscribePopupReader);
    
    auto csb_name = mj::helper::getCSBName("csb/StorySubscribePopup.csb");
    
    StorySubscribePopup *p = static_cast<StorySubscribePopup *>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (p)
    {
        p->didLoadFromCSB(_id, type);
    }
    
    return p;
}

void StorySubscribePopup::didLoadFromCSB(int _id, int type)
{
    CRASH_AUTO_LOG;
    
    action_id = _id;
    
    auto msg = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_message");
    
    auto type_display = 0;
    
    if (FREE_STORY_MANAGER->checkFreeTimeAvailable())
    {
        type_display = 1;
    }
    else
    {
        auto percent = FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed();
        
        switch (type)
        {
            case SUB_STORY:
            {
                m_startFrom = "story";
                
                if (FREE_STORY_MANAGER->isStoryOnListFree(_id))
                {
                    type_display = 1;
                }
                else
                {
                    if (percent < 10.0f)
                    {
                        type_display = 1;
                    }
                }
                break;
            }
            case SUB_AUDIO:
            {
                m_startFrom = "audiobook";

                if (FREE_STORY_MANAGER->isAudiobookOnListFree(_id))
                {
                    type_display = 1;
                }
                else
                {
                    if (percent < 10.0f)
                    {
                        type_display = 1;
                    }
                }
                break;
            }
            case SUB_LESSON:
            {
                m_startFrom = "lesson";

                type_display = 1;
                break;
            }
            case SUB_AI_SPEAKING_LESSON:
            {
                m_startFrom = "lesson";

                type_display = 0;
                break;
            }
            default:
                break;
        }
        
    }
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "label_free"),
            utils::findChild<ui::Text *>(this, "label_yes"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/monkeystories_medium.ttf");
        }
    }
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "label_message"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        }
    }
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "label_title"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/monkeystories_ultra.ttf");
        }
    }
    
    if (type_display == 1)
    {
        msg->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.content"));
        if (CONFIG_MANAGER->isVietnameseStories()) {
            msg->setPositionY(310);
        } else {
            msg->setPositionY(273);
        }
        cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_yes")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.btok"));
        
        if (auto btcancel = cocos2d::utils::findChild(this, "btcancel")) {
            if (type == SUB_LESSON) {
                btcancel->setVisible(false);
            } else {
                btcancel->setVisible(true);
            }
        }
        
        // sửa lại giao diện
        msg->setPositionY(225);
        
        if (auto btn_free = cocos2d::utils::findChild(this, "btfree_2")) {
            btn_free->setVisible(true);
        }
        
        auto icon = ui::ImageView::create("mjstory/material/mat_icon_warning_big.png");
        icon->setScale(0.75);
        icon->setPosition(msg->getPosition() + Point(0, 130));
        msg->getParent()->addChild(icon);
    }
    else
    {
        msg->setPositionY(315);
        msg->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.content.free"));
        cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_yes")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.btsub"));
        
        // sửa lại giao diện
//        cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_free")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.btfree"));
//        cocos2d::utils::findChild(this, "btfree")->setVisible(true);
        
        msg->setPositionY(225);
        
        if (auto btn_free = cocos2d::utils::findChild(this, "btfree_2")) {
            btn_free->setVisible(true);
        }
        
        auto icon = ui::ImageView::create("mjstory/material/mat_icon_warning_big.png");
        icon->setScale(0.75);
        icon->setPosition(msg->getPosition() + Point(0, 110));
        msg->getParent()->addChild(icon);
    }
}
#pragma mark - onEnter
void StorySubscribePopup::onEnter() {
    Layout::onEnter();
}

void StorySubscribePopup::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
    CRASH_AUTO_LOG;
}
#pragma mark - onExit
void StorySubscribePopup::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
    CRASH_AUTO_LOG;
}

void StorySubscribePopup::onShowHidePopUp(bool isShow){
    CRASH_AUTO_LOG;
    if (isRunning) return;
    isRunning = true;
    auto csb_name = std::string("csb/StorySubscribePopup.csb");
    auto runani = CSLoader::createTimeline(csb_name);
    runani->setTag(1011);
    this->runAction(runani);
    if (isShow) {
        runani->play("show", false);
    }else runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        isRunning = false;
        if (!isShow) {
            this->removeFromParent();
        }
    });
}

void StorySubscribePopup::setDelegate(StorySubscribePopupDelegate *_delegate){
    delegate = _delegate;
}

#pragma mark - BUTTON
void StorySubscribePopup::onOk(cocos2d::Ref *sender)
{
    CRASH_AUTO_LOG;
    if (isRunning) return;
    MJDEFAULT->setBoolForKey(key_click_shop_type_popup, true);
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
    this->scheduleOnce([=] (float)
    {
        pushCleverTapEvent("ok_button");
        if (delegate)
        {
            delegate->onSubscribeOk();
        }
    }, 0.2f, "delay_sub_ok");
    onShowHidePopUp(false);
    
    if (MJDEFAULT->getBoolForKey("new_subscirbe_popup_comfim"))
        {
            MJDEFAULT->setBoolForKey("new_subscirbe_popup_comfim", false);
            auto subscribeTab = MSNewTabSubscribe::createTab();
            subscribeTab->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
            subscribeTab->setPosition(Director::getInstance()->getRunningScene()->getContentSize() / 2);
            Director::getInstance()->getRunningScene()->addChild(subscribeTab, INT_MAX - 1);
            MJDEFAULT->setBoolForKey(key_click_shop_type_popup, false);
        }
}

void StorySubscribePopup::onCancel(cocos2d::Ref *sender)
{
    CRASH_AUTO_LOG;
    if (isRunning) return;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
    this->scheduleOnce([=] (float)
    {
        if (delegate)
        {
            pushCleverTapEvent("cancel_button");

            delegate->onSubscribeCancel();
        }
    }, 0.35f, "delay_sub_cancel");
    
    onShowHidePopUp(false);
}

void StorySubscribePopup::onFree(cocos2d::Ref *sender)
{
    CRASH_AUTO_LOG;
    if (isRunning) return;
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_search_page_close);
    this->scheduleOnce([=] (float)
    {
        if (delegate)
        {
            pushCleverTapEvent("free_button");

            delegate->onSubscribeFree();
        }
    }, 0.35f, "delay_sub_free");
    
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StorySubscribePopup::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StorySubscribePopup, onOk),
        CLICK_MAP(StorySubscribePopup, onCancel),
        CLICK_MAP(StorySubscribePopup, onFree),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void StorySubscribePopup::pushCleverTapEvent(std::string i_clickedType)
{
    CleverTapManager::GetInstance()->pushEvent("click_popup",
    {
        {"screen_name",cocos2d::Value(m_startFrom)},
        {"click_type",cocos2d::Value(i_clickedType)},
        {"pop_up_name",cocos2d::Value("subscribe_popup")},
    });
}

void StorySubscribePopupDelegate::onSubscribeOk() {
    CRASH_AUTO_LOG;
    CCLOG("dispatch go to subscribe event.");
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_subscribe_page);
}

void StorySubscribePopupDelegate::onSubscribeFree() {
    CRASH_AUTO_LOG;
    CCLOG("dispatch go to free event.");
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_go_to_free_page);
}

