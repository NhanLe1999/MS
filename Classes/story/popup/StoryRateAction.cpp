//
//  SEStoryRateAction.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#include "StoryRateAction.h"
#include "StoryLanguageManager.h"
#include "StoryFreeManager.h"
#include "MJDefault.h"
#include "StoryConfigManager.h"
#include "StoryFeedback.h"
#include "StoryRatePopUp.h"
#include "NativeBridge.h"
#include "MJAPIRateApp.h"

#define max_num_to_show_free 5
#define max_time_to_show_free 1
#define max_num_to_show_active 30
#define max_time_to_show_active 10

#define key_user_number_action_completed "key.default.number.action.completed"
#define key_user_last_time_show_rate "key.default.last.time.show.rate"

using namespace mjrate;

bool StoryRateAction::shouldShowRate() {
    return false;
    
    if (MJDEFAULT->getBoolForKey(key_rate_app) || MJDEFAULT->getBoolForKey(key_rate_app_won)) {
        return false;
    }
    
    
    auto number_action = MJDEFAULT->getIntegerForKey(key_user_number_action_completed, 0);
    CCLOG("Numbers action completed: %d", number_action);
    if (CONFIG_MANAGER->isAppActive() && (!FREE_STORY_MANAGER->isFirst2Days())) {
        //người dùng đã mua
        if (number_action < 3) {
            return false;
        }
        
        auto last_time_show_rate = MJDEFAULT->getIntegerForKey(key_user_last_time_show_rate);
        auto time_current = int(utils::getTimeInMilliseconds() / 1000);
        if (time_current - last_time_show_rate > 24 * 60 * 60 * 5) {
            //kể tử lần hiện rate cuối cùng đến nay đã quá 5 ngày, chắc chắn hiện
            return true;
        }
        
        if (number_action == 3) {
            return true;
        }
        
        if ((number_action - 3) % 15 == 0) {
            return true;
        }
        
        return false;
    }
    
    //người dùng chưa mua
    if (FREE_STORY_MANAGER->isFirst2Days()) {
        //trong 2 ngày đầu tiên
        if (number_action < 3) {
            return false;
        }
        
        if (number_action == 3) {
            return true;
        }
        
        if ((number_action - 3) % 5 == 0) {
            return true;
        }
        
        return false;
    }
    
    // trong các ngày tiếp theo, cứ xong 1 action là hiện (vì 1 ngày chỉ có thể hoàn thành được 1 action
    return true;
}

void StoryRateAction::updateActionCompleted() {
    auto number_action = MJDEFAULT->getIntegerForKey(key_user_number_action_completed, 0);
    MJDEFAULT->setIntegerForKey(key_user_number_action_completed, number_action + 1);
}

StoryRateAction* StoryRateAction::createPage(int type)
{
    StoryRateAction* obj = new StoryRateAction();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadData(type);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void StoryRateAction::didLoadData(int type) {
    action_type = type;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    action_type == FEEDBACK_ACTION;
#else
    auto rate_action_value = MJDEFAULT->getIntegerForKey(key_rate_action);
    if (rate_action_value == 0) {
        action_type = ALL_ACTION;
    }else {
        action_type = RATE_ACTION;
    }
    
#endif
    
//    if (MJDEFAULT->getBoolForKey(key_rate_app) || MJDEFAULT->getBoolForKey(key_rate_app_won)) return;
//    if (FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) return;
//    auto comapare_show = max_num_to_show_free - 1;
//    auto max_show = max_num_to_show_free;
//    auto max_time = max_time_to_show_free;
//    if (CONFIG_MANAGER->isAppActive() && (!FREE_STORY_MANAGER->isFirst2Days())) {
//        comapare_show = 0;
//        max_show = max_num_to_show_active;
//        max_time = max_time_to_show_active;
//    }
//    auto count_to_show = MJDEFAULT->getIntegerForKey(key_count_to_show, comapare_show);
//    MJDEFAULT->setIntegerForKey(key_count_to_show, count_to_show++);
//    bool check_show = false;
//    if (count_to_show >= max_show) {
//        check_show = true;
//    }else {
//        auto time_to_show = MJDEFAULT->getIntegerForKey(key_time_to_show);
//        auto time_current = MJDEFAULT->getIntegerForKey(key_time_curent);
//        time_current = int(utils::getTimeInMilliseconds() / 1000);
//        if (time_current - time_to_show > (max_time*24*3600)) {
//            check_show = true;
//        }
//    }
//
//    if (!check_show) return;
    
    if (action_type == ALL_ACTION) {
        showQuesLoveAppPopup();
    }else if (action_type == RATE_ACTION) {
        showRatePopup();
    }
    
    MJDEFAULT->setIntegerForKey(key_user_last_time_show_rate, (int)(utils::getTimeInMilliseconds() / 1000));
}

void StoryRateAction::onEnter(){
    cocos2d::ui::Layout::onEnter();
    auto showrateconfirm = EventListenerCustom::create("mjstory.showrateconfirm", [=](EventCustom * event_custom) {
        if (CONFIG_MANAGER->isAppActive() && (!FREE_STORY_MANAGER->isFirst2Days())) {
            this->showThanks();
        }else {
            this->showRateConfirm();
        }
        
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(showrateconfirm, this);
}

void StoryRateAction::showThanks(){
    MJDEFAULT->setBoolForKey(key_rate_app, true);
    MJDEFAULT->setBoolForKey(key_rate_app_won, true);
    auto rate_bonus = StoryRatePopUp::createPage(StoryRatePopUp::RATE_THANKS);
    this->addChild(rate_bonus, 19999999);
    rate_bonus->onShowHidePopUp(true);
}

void StoryRateAction::showRatePopup(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    return;
#endif
    auto rate_type = StoryRatePopUp::RATE_FREE;
    if (CONFIG_MANAGER->isAppActive() && (!FREE_STORY_MANAGER->isFirst2Days())) {
        rate_type = StoryRatePopUp::RATE_ACTIVE;
    }
    auto rate_popup = StoryRatePopUp::createPage(rate_type);
    this->addChild(rate_popup, 19999999);
    rate_popup->onShowHidePopUp(true);
}

void StoryRateAction::showFeedbackPopup(){
    auto feedback = StoryFeedback::createPage();
    this->addChild(feedback, 99999998);
}

void StoryRateAction::showRateConfirm(){
    auto rate_confirm = StoryRateConfirm::createRateConfirm();
    this->addChild(rate_confirm, 19999999);
    rate_confirm->setDelegate(this);
    rate_confirm->onShowHidePopUp(true);
}
void StoryRateAction::showQuesLoveAppPopup(){
//    if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_app_firstdate), MJDEFAULT->getIntegerForKey(key_time_curent))) {
        auto love_app = StoryRateQuestLoveApp::createRateQuestLoveApp();
        this->addChild(love_app, 19999999);
        love_app->setDelegate(this);
        love_app->onShowHidePopUp(true);
//    }
}

#include "MSEventLogger.h"
void StoryRateAction::showRateBonus(){
    ms::EventLogger::getInstance().logEvent("ms2_rate_app", {});
    mj::RateAppAPI::call();
    //display popup bonus rated.
    auto rate_bonus = StoryRatePopUp::createPage(StoryRatePopUp::RATE_BONUS);
    this->addChild(rate_bonus, 19999999);
    rate_bonus->onShowHidePopUp(true);
    
    MJDEFAULT->setBoolForKey(key_rate_app_won, true);
//    MJDEFAULT->setIntegerForKey(key_numstories_rate_bonus, 4);
    
    if (!CONFIG_MANAGER->isAppActive()) {
        FREE_STORY_MANAGER->setRateTimeExpire(time_bonus_rate);
    }
    if (FREE_STORY_MANAGER->isFirst2Days()) {
        FREE_STORY_MANAGER->setRateTimeExpire(time_bonus_rate);
    }
}

void StoryRateAction::onRateConfirmOk(){
    showRateBonus();
}

void StoryRateAction::onRateQuestLoveAppOk(){
    MJDEFAULT->setIntegerForKey(key_count_to_show, 0);
    MJDEFAULT->setIntegerForKey(key_time_to_show, MJDEFAULT->getIntegerForKey(key_time_curent));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    showFeedbackPopup();
#else
    showRatePopup();
#endif
}
void StoryRateAction::onRateQuestLoveAppCancel(){
    MJDEFAULT->setIntegerForKey(key_count_to_show, 0);
    MJDEFAULT->setIntegerForKey(key_time_to_show, MJDEFAULT->getIntegerForKey(key_time_curent));
    showFeedbackPopup();
}
