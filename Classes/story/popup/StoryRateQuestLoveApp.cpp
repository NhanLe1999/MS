//
//  StoryRateQuestLoveApp.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryRateQuestLoveApp.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryAlert.h"
#include "StoryRatePopUp.h"
#include "MJAPIRateApp.h"
INITIALIZE_READER_INSTANCE(StoryRateQuestLoveAppReader);

USING_NS_CC;
using namespace ui;

StoryRateQuestLoveApp * StoryRateQuestLoveApp::createRateQuestLoveApp() {
    REGISTER_READER(StoryRateQuestLoveAppReader);
    StoryRateQuestLoveApp *p = static_cast<StoryRateQuestLoveApp *>(CSLoader::createNodeWithVisibleSize("csb/StoryRateQuestLoveApp.csb"));
    if (p) {
        p->didLoadFromCSB();
    }
    return p;
}

void StoryRateQuestLoveApp::didLoadFromCSB() {
    std::string classes_name = "storypagevc";
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_message")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_loveapp_content"));
    
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_yes")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_btok"));
    
    cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "label_no")->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "alert_btcancel"));
    
}
#pragma mark - onEnter
void StoryRateQuestLoveApp::onEnter() {
    Layout::onEnter();
}

void StoryRateQuestLoveApp::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryRateQuestLoveApp::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

void StoryRateQuestLoveApp::setDelegate(StoryRateQuestLoveAppDelegate *_delegate){
    delegate = _delegate;
}

void StoryRateQuestLoveApp::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    auto csb_name = std::string("csb/StoryRateQuestLoveApp.csb");
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

#include "MJHelper.h"
void StoryRateQuestLoveApp::showNetwordDown(){
    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setNetworkDownContent(false, true);
    alert->show();
}

#pragma mark - BUTTON
void StoryRateQuestLoveApp::onOk(cocos2d::Ref *sender){
    if (isRunning) return;
    onClose(nullptr);
    this->scheduleOnce([=] (float){
        if (delegate){
            delegate->onRateQuestLoveAppOk();
        }
    }, 0.25f, "delay_show_ok");
}

void StoryRateQuestLoveApp::onCancel(cocos2d::Ref *sender){
    if (isRunning) return;
    onClose(nullptr);
    this->scheduleOnce([=] (float){
        if (delegate){
            delegate->onRateQuestLoveAppCancel();
        }
    }, 0.25f, "delay_show_cancel");
}

void StoryRateQuestLoveApp::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryRateQuestLoveApp::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryRateQuestLoveApp, onOk),
        CLICK_MAP(StoryRateQuestLoveApp, onCancel),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


