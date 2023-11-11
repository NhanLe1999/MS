//
//  MJPayPopup_COD.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#include "MJPayPopup_COD.h"
#include "MJPayPopup.h"
#include "MJRequest.h"
#include "StoryAlert.h"
#include "MJPay.h"
#include "MJPlatformConfig.h"
#include "StoryConfigManager.h"
#include "NativeBridge.h"
#include "HSBaseScene.h"

#include "MSEventLogger.h"

INITIALIZE_READER_INSTANCE(MJPayPopup_CODReader);

MJPayPopup_COD * MJPayPopup_COD::createPopup(const mj::ListPackages & packages, const std::string &product_id, const int & discount) {
    auto p = static_cast<MJPayPopup_COD *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/pay/MJPayPopup_COD.csb"));
    if (p) {
        p->didLoadFromCSB(packages, product_id, discount);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJPayPopup_COD::onEnter() {
    MJPaySubPopup::onEnter();
    _event_counter = 0;

    utils::findChild<ui::TextField *>(this, "textfield_name")->setCursorEnabled(true);
    utils::findChild<ui::TextField *>(this, "textfield_phone")->setCursorEnabled(true);
    utils::findChild<ui::TextField *>(this, "textfield_mail")->setCursorEnabled(true);
    utils::findChild<ui::TextField *>(this, "textfield_address")->setCursorEnabled(true);


    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        mj::PlatformConfig::getInstance().rotateKeyboard(true);
    }
}

void MJPayPopup_COD::onExit() {
    mj::PlatformConfig::getInstance().rotateKeyboard(false);
    MJPaySubPopup::onExit();
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPayPopup_COD::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJPayPopup_COD, onNext),
        CLICK_MAP(MJPayPopup_COD, onBack),
        CLICK_MAP(MJPayPopup_COD, onSwitch)
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}

cocos2d::ui::Widget::ccWidgetEventCallback MJPayPopup_COD::onLocateEventCallback(const std::string &callback_name) {
    if (callback_name == "onTextFieldEvent") {
        return CC_CALLBACK_2(MJPayPopup_COD::onTextFieldEvent, this);
    }

    return nullptr;
}

void MJPayPopup_COD::onNext(cocos2d::Ref *sender) {
    if (!NativeBridge::networkAvaiable()) {
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode("csb/StoryNetworkDown.csb"));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }

    this->enumerateChildren("//.*", [](Node * child) {
        if (dynamic_cast<ui::TextField *>(child)) {
            reinterpret_cast<ui::TextField *>(child)->setDetachWithIME(true);
        }
        return false;
    });

    auto name = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_name"))->getString();
    auto mail = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_mail"))->getString();
    auto addr = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_address"))->getString();
    auto phone = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_phone"))->getString();

    if (name.empty() || (mail.empty() || phone.empty())) {
        StoryAlert::showAlert("Vui lòng điền đầy đủ thông tin", "OK", "", false, 24);
        return;
    }

    if (!mj::helper::isValidEmail(mail)) {
        StoryAlert::showAlert("Vui lòng điền đúng địa chỉ email", "OK", "", false, 24);
        return;
    }

    if (!mj::helper::isValidPhoneNumber(phone)) {
        StoryAlert::showAlert("Vui lòng điền đúng số điện thoại", "OK", "", false, 24);
        return;
    }

    mj::Request::Data request_data = {
        {"coupon", mj::Pay::getInstance().getCoupon()},
        {"product_id", _product_id},
        {"phone", phone},
        {"email", mail},
        {"address", addr},
        {"name", name},
        {"pay_method", "1"}
    };
    CCLOG("_product_id = %s", _product_id.c_str());
    _request = new mj::Request(mj::Request::ActionType::REGISTER_COD, request_data, mj::Request::RequestType::POST);
    _request->setDelegate(this);
    _request->send();
    
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFillInfo, {
        {ms::analytics::kPropertyPackage, cocos2d::Value(_product_id)},
        {ms::analytics::kPropertyCheckoutOption, cocos2d::Value("cod")}
    });
    
    MS_LOGGER.logEventCODInfo();
}

void MJPayPopup_COD::onRequestFailed(int tag, int error_code, std::string error_string, rapidjson::Value data) {
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([error_code] {
        StoryAlert::showAlert(StringUtils::format("Đã có lỗi xảy ra.\nVui lòng thử lại sau.\n(%d)", error_code), "OK", "");
    });
}

void MJPayPopup_COD::onRequestSuccess(int tag, rapidjson::Value data) {
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
        StoryAlert::showAlert("Bạn đã đăng ký thành công.\n Chúng tôi sẽ liên hệ với bạn trong thời gian sớm nhất.", "OK", "");
        this->onSwitch(nullptr);
    });
}

void MJPayPopup_COD::onTextFieldEvent(Ref * sender, int event) {
    auto textfield = static_cast<ui::TextField *>(sender);
    auto textfield_event = static_cast<ui::TextField::EventType>(event);
    auto popup = utils::findChild(this, "popup");

    auto popup_position = Vec2(this->getContentSize().width / 2, 615);
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        popup_position = Vec2(this->getContentSize().width / 3, this->getContentSize().height / 2);
    }

    switch (textfield_event) {
        case TextField::EventType::ATTACH_WITH_IME:
            _event_counter++;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            popup->setPosition(popup_position);
#endif
            textfield->setPlaceHolder("");
            textfield->getParent()->setColor(Color3B(173, 216, 230));
            break;

        case TextField::EventType::DETACH_WITH_IME:
            _event_counter--;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            if (_event_counter == 0) {
                utils::findChild(this, "popup")->setPosition(this->getContentSize() / 2);
            }
#endif

            textfield->getParent()->setColor(Color3B::WHITE);
            if (textfield->getString().empty()) {
                auto tf_name = textfield->getName();
                if (tf_name == "textfield_name")
                    textfield->setPlaceHolder("Họ và tên");
                if (tf_name == "textfield_phone")
                    textfield->setPlaceHolder("Số điện thoại");
                if (tf_name == "textfield_mail")
                    textfield->setPlaceHolder("Email");
                if (tf_name == "textfield_address")
                    textfield->setPlaceHolder("Địa chỉ giao giờ hành chính");
            }
            break;

        case TextField::EventType::INSERT_TEXT:break;
        case TextField::EventType::DELETE_BACKWARD:break;
    }
}
