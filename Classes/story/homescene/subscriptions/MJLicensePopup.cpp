//
//  MJLicensePopup.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/1/17.
//
//

#include "MJLicensePopup.h"
#include "StoryAlert.h"

INITIALIZE_READER_INSTANCE(MJLicensePopupReader);

MJLicensePopup * MJLicensePopup::createPopup() {
    auto csb_name = mj::helper::getCSBName("csb/pay/MJLicense.csb");
    auto p = static_cast<MJLicensePopup *>(cocos2d::CSLoader::createNodeWithVisibleSize(csb_name));

    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJLicensePopup::didLoadFromCSB() {
    this->setAnchorPoint(cocos2d::Point::ANCHOR_BOTTOM_LEFT);
    this->setPosition(cocos2d::Point::ZERO);
}

cocos2d::ui::Widget::ccWidgetClickCallback MJLicensePopup::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJLicensePopup, onNext),
        CLICK_MAP(MJLicensePopup, onBack),
    };
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }

    return nullptr;
}

void MJLicensePopup::onNext(cocos2d::Ref *sender) {
    auto license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_license"))->getString();
    mj::Request::Data request_data = {
        {"licence", license}
    };
    _request = new mj::Request(mj::Request::ActionType::ACTIVE_LICENSE_V2, request_data, mj::Request::RequestType::POST);
    _request->setDelegate(this);
    _request->send();

    static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_license"))->didNotSelectSelf();
}

void MJLicensePopup::onBack(cocos2d::Ref *sender) {
    
    this->removeFromParent();
}

void MJLicensePopup::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
    auto license = static_cast<cocos2d::ui::TextField *>(cocos2d::ui::Helper::seekWidgetByName(this, "textfield_license"))->getString();
    if (message == "") {
        StoryAlert::showAlert(StringUtils::format("Đã có lỗi xảy ra.\nVui lòng thử lại sau.\n[%s]", license.c_str()), "OK", "");
    } else {
        StoryAlert::showAlert(message+ std::string("\n[") + license + std::string("]"), "OK", "");
    }
}

void MJLicensePopup::onRequestSuccess(int tag, rapidjson::Value data) {

}
