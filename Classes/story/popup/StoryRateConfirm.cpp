//
//  StoryRateConfirm.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryRateConfirm.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryAlert.h"

INITIALIZE_READER_INSTANCE(StoryRateConfirmReader);

USING_NS_CC;
using namespace ui;

StoryRateConfirm * StoryRateConfirm::createRateConfirm() {
    REGISTER_READER(StoryRateConfirmReader);
    StoryRateConfirm *p = static_cast<StoryRateConfirm *>(CSLoader::createNodeWithVisibleSize("csb/StoryRateConfirm.csb"));
    if (p) {
        p->didLoadFromCSB();
    }
    return p;
}

void StoryRateConfirm::didLoadFromCSB() {
    std::string classes_name = "storypagevc";
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_message")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_content"));
    
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_yes")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_ok"));
    
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_no")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_cancel"));
    
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_guide")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_rate_guiderate"));
    
}
#pragma mark - onEnter
void StoryRateConfirm::onEnter() {
    Layout::onEnter();
    
}

void StoryRateConfirm::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryRateConfirm::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();

}

void StoryRateConfirm::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    auto csb_name = std::string("csb/StoryRateConfirm.csb");
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

void StoryRateConfirm::setDelegate(StoryRateConfirmDelegate *_delegate){
    delegate = _delegate;
}

#include "MJHelper.h"
void StoryRateConfirm::showNetwordDown(){
    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setNetworkDownContent(false, true);
    alert->show();
}

#pragma mark - BUTTON
void StoryRateConfirm::onOk(cocos2d::Ref *sender){
    if (isRunning) return;
    MJDEFAULT->setBoolForKey(key_rate_app_won, true);
//    MJDEFAULT->setIntegerForKey(key_numstories_rate_bonus, 4);
    this->scheduleOnce([=] (float){
        if (delegate){
            delegate->onRateConfirmOk();
        }
    }, 0.35f, "delay_confirm_ok");
    onClose(nullptr);
}

void StoryRateConfirm::onCancel(cocos2d::Ref *sender){
    if (isRunning) return;
    onClose(nullptr);
    NativeBridge::rateAppMJStories();
}

void StoryRateConfirm::onGuide(cocos2d::Ref *sender){
    if (isRunning) return;
    this->scheduleOnce([=] (float){
        string guide_url = "http://www.earlystart.co/rate-ios.html";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        guide_url = "http://www.earlystart.co/rate-android.html";
#endif
        
        if(MJDEFAULT->getIntegerForKey(key_lang_display) == 4) {
            guide_url = "http://www.daybehoc.com/rate-ios.html";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            guide_url = "http://www.daybehoc.com/rate-android.html";
#endif
        }
        Application::getInstance()->openURL(guide_url);
}, 0.35f, "delay_guide_rate                        ");

    onClose(nullptr);
}

void StoryRateConfirm::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryRateConfirm::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryRateConfirm, onOk),
        CLICK_MAP(StoryRateConfirm, onCancel),
        CLICK_MAP(StoryRateConfirm, onGuide),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


