//
//  MJPaySubPopup.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPaySubPopup.h"
#include "MJPayPopup.h"
#include "MJPayPopup_OnePay.h"
#include "MJPayPopup_COD.h"
#include "MJPayPopup_BankTransfer.h"
#include "MJPayPopup_Store.h"
#include "MJPayPopup_BankInfo.h"
#include "MJHelper.h"
#include "HSLib.h"
#include "MJPlatformConfig.h"

MJPaySubPopup * MJPaySubPopup::createPopup(mj::PayMethod method, mj::ListPackages packages, std::string product_id, int discount) {
    MJPaySubPopup * sub_popup = nullptr;
    switch (method) {
        case mj::PayMethod::MJPAY_ONEPAY:
            sub_popup = MJPayPopup_OnePay::createPopup(packages, product_id, discount);
            break;

        case mj::PayMethod::MJPAY_COD:
            sub_popup = MJPayPopup_COD::createPopup(packages, product_id, discount);
            break;

        case mj::PayMethod::MJPAY_BANK_TRANSFER:
            sub_popup = MJPayPopup_BankTransfer::createPopup(packages, product_id, discount);
            break;

        case mj::PayMethod::MJPAY_STORE:
            sub_popup = MJPayPopup_Store::createPopup(packages, product_id, discount);
            break;
        case mj::PayMethod::MJPAY_BANK_INFO:
            sub_popup = MJPayPopup_BankInfo::createPopup(packages, product_id, discount);
            break;

        default:
            break;
    }

    if (sub_popup) {
        sub_popup->_pay_method = method;
    }
    return sub_popup;
}

void MJPaySubPopup::didLoadFromCSB(mj::ListPackages packages, std::string product_id, int discount) {
    this->_packages = packages;
    this->_product_id = product_id;
    this->_discount = discount;
    this->setAnchorPoint(cocos2d::Point::ANCHOR_BOTTOM_LEFT);
    this->setPosition(cocos2d::Point::ZERO);

    auto rotation_angle = CONFIG_MANAGER->isHomeScreenLandscape() ? 0 : -90;
    this->getChildByName("popup")->setRotation(rotation_angle);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //back key listener for android
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
#endif
    
    
    reloadPriceTag();
}

void MJPaySubPopup::onBack(cocos2d::Ref *sender) {
    auto pay = MJPayPopup::createPopUp(_packages, _product_id, _discount);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(pay, INT_MAX - 1);
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    this->removeFromParent();
}

void MJPaySubPopup::onSwitch(cocos2d::Ref *sender) {
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    cocos2d::EventCustom e(k_event_pay_popup_close);
    mj::PayMethod * data = &_pay_method;
    e.setUserData(data);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    this->removeFromParent();
//
//    int weight;
//    for (auto p : _packages) {
//        if (p.product_id == _product_id) {
//            weight = p.weight;
//            break;
//        }
//    }
//    weight++; if (weight > 3) weight = 1;
//    for (auto p : _packages) {
//        if (p.weight == weight) {
//            _product_id = p.product_id;
//            reloadPriceTag();
//            break;
//        }
//    }
}

void MJPaySubPopup::reloadPriceTag() {
    for (auto p : _packages) {
        if (p.product_id == _product_id) {
            auto price = p.getActualPrice(_discount);
            auto text_pricetag = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "text_pricetag"));
            auto name_display = std::string("");
            if (p.product_id.find("6month") != std::string::npos) {
                name_display = "6 tháng";
            }
            if (p.product_id.find("1year") != std::string::npos) {
                name_display = "1 năm";
            }
            if (p.product_id.find("lifetime") != std::string::npos) {
                name_display = "Trọn đời";
            }

            text_pricetag->setString(cocos2d::StringUtils::format("%s: %s VND", name_display.c_str(), mj::helper::prettyInteger((int) price).c_str()));
        }
    }
}
