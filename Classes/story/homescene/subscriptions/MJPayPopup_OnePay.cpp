//
//  MJPayPopup_OnePay.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPayPopup_OnePay.h"
#include "MJPay.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "NativeBridge.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"
#include "MSIntroGrade.h"
#include "HSBaseScene.h"
#include "MJDefinitions.h"

INITIALIZE_READER_INSTANCE(MJPayPopup_OnePayReader);

MJPayPopup_OnePay *MJPayPopup_OnePay::createPopup(const mj::ListPackages &packages, const std::string &product_id, const int &discount) {
    auto p = static_cast<MJPayPopup_OnePay *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/pay/MJPayPopup_OnePay.csb"));
    if (p) {
        p->didLoadFromCSB(packages, product_id, discount);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJPayPopup_OnePay::didLoadFromCSB(mj::ListPackages packages, std::string product_id, int discount) {
    MJPaySubPopup::didLoadFromCSB(packages, product_id, discount);

    auto text = utils::findChild<ui::Text *>(this, "text_info");
    auto str = text->getString();
    str = mj::helper::replace_string(str, "http://monkeystories.net", MJDEFAULT->getStringForKey("pay_use_url", "http://monkeystories.net"));
    text->setString(str);

    auto event_listener = cocos2d::EventListenerCustom::create("com.earlystart.stories.onepay.success", [=](cocos2d::EventCustom *e) {
//        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
        auto str = LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.license.success");
        StoryAlert::showAlert(str, "OK", "", false, 24, this);
        
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishCheckout, {
            {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)},
            {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("onepay")}
        });
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);

    auto event_listener_fail = cocos2d::EventListenerCustom::create("com.earlystart.stories.onepay.fail", [=](cocos2d::EventCustom *e) {
//        [[UIApplication sharedApplication] setStatusBarOrientation:UIInterfaceOrientationPortrait];
        auto str = LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "pay.purchase.failed");
        StoryAlert::showAlert(str, "OK", "");
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener_fail, this);

    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        auto lb = utils::findChild(this, "label_immediate");
        lb->setPosition(lb->getPosition() + Vec2(-3, -1.5));
    }
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayPopup_OnePay::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
            CLICK_MAP(MJPayPopup_OnePay, onNext),
            CLICK_MAP(MJPayPopup_OnePay, onBack),
            CLICK_MAP(MJPayPopup_OnePay, onSwitch)
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}

void MJPayPopup_OnePay::onNext(cocos2d::Ref *sender) {
    if (!NativeBridge::networkAvaiable()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }
    
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutStep2, {
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)},
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("onepay")}
    });
    
    mj::Pay::getInstance().payWithOnePay(_product_id);
//    mj::PayService::getInstance().payWithOnePay(_product_id, mj::Pay::getInstance().getCoupon());
}

void MJPayPopup_OnePay::onAlertOK(cocos2d::Ref *sender) {
    MJDEFAULT->setBoolForKey(key_first_api_called, true);
    Director::getInstance()->replaceScene(MSIntroGrade::createScene());
}
