//
//  PayInit.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/2/19.
//

#include "PayInit.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "MJPlatformConfig.h"
#include "StoryAlert.h"

USING_NS_CC;

INITIALIZE_READER(PayInit);

std::once_flag ms_pay_init_reader_flag;

PayInit * PayInit::createView(std::function<void()> pay_callback) {
    std::call_once(ms_pay_init_reader_flag, [] {
        REGISTER_CSB_READER(PayInit);
    });
    
    auto node = reinterpret_cast<PayInit *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/PayInit.csb"));
    if (node) {
        node->didLoadFromCSB(pay_callback);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void PayInit::didLoadFromCSB(std::function<void()> pay_callback) {
    this->setTouchEnabled(false);
    _callback = pay_callback;
    
    auto str = LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    str = mj::helper::replace_string(str, "{os}", "iOS");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    str = mj::helper::replace_string(str, "{os}", "Android");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    str = mj::helper::replace_string(str, "{os}", "macOS");
#endif
    
    utils::findChild<ui::Text *>(this, "message")->setString(str);
}

void PayInit::onEnter() {
    ui::Layout::onEnter();
}

void PayInit::onExit() {
    ui::Layout::onExit();
}

void PayInit::onClose(cocos2d::Ref *sender) {
    this->removeFromParent();
}

void PayInit::onView(cocos2d::Ref *sender) {
    //Application::getInstance()->openURL("http://truyentiengviet.vmonkey.vn/?coupon=VMONKEY&utm_source=App&utm_medium=App-BangGia&Utm_campaign=VMonkey");

	Application::getInstance()->openURL(MJDEFAULT->getStringForKey("pay_use_url"));
}

void PayInit::onBuy(cocos2d::Ref *sender) {
    if (_callback) {
        _callback();
    }
}

void PayInit::onCall(cocos2d::Ref *sender) {
    if (MJ_PLATFORM_CONFIG.phoneSupport("1900636052")) {
    } else {
        auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
        auto str = LANGUAGE_MANAGER->getTextByKey(lang_id, "support.action.error");
        StoryAlert::showAlert(str, "OK", "");
    }
}

ui::Widget::ccWidgetClickCallback PayInit::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(PayInit, onClose),
        CLICK_MAP(PayInit, onBuy),
        CLICK_MAP(PayInit, onView),
        CLICK_MAP(PayInit, onCall)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

