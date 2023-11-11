//
//  MJPayPopup.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPayPopup.h"
#include "MJPayPopup_OnePay.h"
#include "MJPayPopup_COD.h"
#include "MJPayPopup_BankTransfer.h"
#include "MJPayPopup_Store.h"
#include "MJRequest.h"
#include "MJHelper.h"
#include "HSLib.h"

#include "MSEventLogger.h"

INITIALIZE_READER_INSTANCE(MJPayPopupReader);

MJPayPopup * MJPayPopup::createPopUp(const mj::ListPackages & packages, const std::string & product_id, const int & discount) {
    auto p = static_cast<MJPayPopup *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/pay/MJPayPopup.csb"));

    if (p) {
        p->_packages = packages;
        p->_product_id = product_id;
        p->_discount = discount;
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJPayPopup::didLoadFromCSB() {
    this->setPosition(cocos2d::Point::ZERO);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_BOTTOM_LEFT);
    auto rotation_angle = CONFIG_MANAGER->isHomeScreenLandscape() ? 0 : -90;
    this->getChildByName("popup")->setRotation(rotation_angle);

//    if (_product_id.find("6month") != std::string::npos) {
//        cocos2d::utils::findChild(this, "lb_device")->setVisible(true);
//        cocos2d::utils::findChild(this, "Button_2_2")->setVisible(false);
//    }
    
	cocos2d::utils::findChild(this, "lb_device")->setVisible(false); // hide text 2 device
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    cocos2d::utils::findChild(this, "Button_2_2")->setVisible(false);
#endif
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayPopup::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJPayPopup, onOnePay),
        CLICK_MAP(MJPayPopup, onCOD),
        CLICK_MAP(MJPayPopup, onBankTransfer),
        CLICK_MAP(MJPayPopup, onStore),
        CLICK_MAP(MJPayPopup, onClose)
    };
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (callback_name == "onClose") {
        
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onClose(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    }
#endif
    
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}

void MJPayPopup::onBankTransfer(cocos2d::Ref *sender) {
    MS_LOGGER.logEventSelectMethod(_product_id, "bank_transfer");
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutChooseMethod, {
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("bank_transfer")},
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)}
    });
    
    auto bank = MJPaySubPopup::createPopup(mj::PayMethod::MJPAY_BANK_TRANSFER, _packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(bank, INT_MAX - 1);
    this->removeFromParent();
}

void MJPayPopup::onStore(cocos2d::Ref *sender) {
    MS_LOGGER.logEventSelectMethod(_product_id, "store");
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutChooseMethod, {
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("store")},
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)}
    });
    auto store = MJPaySubPopup::createPopup(mj::PayMethod::MJPAY_STORE, _packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(store, INT_MAX - 1);
    this->removeFromParent();
}

void MJPayPopup::onCOD(cocos2d::Ref *sender) {
    MS_LOGGER.logEventSelectMethod(_product_id, "cod");
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutChooseMethod, {
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("cod")},
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)}
    });
    auto cod = MJPaySubPopup::createPopup(mj::PayMethod::MJPAY_COD, _packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(cod, INT_MAX - 1);
    this->removeFromParent();
}

void MJPayPopup::onOnePay(cocos2d::Ref *sender) {
    MS_LOGGER.logEventSelectMethod(_product_id, "onepay");
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventCheckoutChooseMethod, {
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("onepay")},
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)}
    });
    auto onepay = MJPaySubPopup::createPopup(mj::PayMethod::MJPAY_ONEPAY, _packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(onepay, INT_MAX - 1);
    this->removeFromParent();
}

void MJPayPopup::onClose(cocos2d::Ref *sender) {
    this->removeFromParent();

    cocos2d::EventCustom e(k_event_pay_popup_close);
    mj::PayMethod * data = new mj::PayMethod;
    * data = mj::PayMethod::MJPAY_NONE;
    e.setUserData(data);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);

    delete data;
}
