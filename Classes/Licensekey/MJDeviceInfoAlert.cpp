//
//  MJDeviceInfoAlert.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJDeviceInfoAlert.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "LicenseInfoModel.h"
#include "MJDefault.h"
#include "MJListDeviceView.h"
INITIALIZE_READER(MJDeviceInfoAlert);

USING_NS_CC;
std::once_flag MJDeviceInfoAlert_reader;
#define CSB_NAME "csb/licensekey/MJDeviceInfoAlert.csb"
MJDeviceInfoAlert * MJDeviceInfoAlert::createView(cocos2d::ValueMap noti_map) {
    std::call_once(MJDeviceInfoAlert_reader, [] {
        REGISTER_CSB_READER(MJDeviceInfoAlert);
    });
    auto p = static_cast<MJDeviceInfoAlert *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(noti_map);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJDeviceInfoAlert::didLoadFromCSB(cocos2d::ValueMap noti_map) {
    _info.notification_id = noti_map["id"].asInt();
    _info.message = noti_map["content"].asString();
    _info.url = noti_map["url"].asString();
    _info.time_created = noti_map["dateinsert"].asInt();
    _info.title = noti_map["title"].asString();
    this->loadFrames();
}

void MJDeviceInfoAlert::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

#include "StoryDataModel.h"
void MJDeviceInfoAlert::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    
    this->getTextByName("title")->setString(_info.title);
    this->getTextByName("msg")->setString(_info.message);
    
    auto packages = StoryDataModel::getInstance()->getLicensePackages();
    
    if (packages.size() == 1) {
        if (!LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
            utils::findChild<Button *>(this, "btok")->setTitleText("Manage devices");
        }
    } else {
        if (!LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
            utils::findChild<Button *>(this, "btok")->setTitleText("Manage licenses");
        } else {
            utils::findChild<Button *>(this, "btok")->setTitleText("Quản lý gói học");
        }
    }
    
    
}

void MJDeviceInfoAlert::onShow(){
    if (is_showed) return;
    is_showed = true;
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJDeviceInfoAlert::onHide(){
    if (!is_showed) return;
    is_showed = false;
    
    utils::findChild(this, "color_layout")->runAction(FadeOut::create(0.25));
    auto bg = utils::findChild(this, "bg");
    bg->runAction(EaseCubicActionOut::create(MoveTo::create(1, Vec2(Director::getInstance()->getWinSize().width - 80, 76))));
    bg->runAction(ScaleTo::create(1, 0.05));
    bg->runAction(Sequence::create(DelayTime::create(0.25), FadeOut::create(0.5), CallFunc::create([] {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mj.animate.notialert");
    }), NULL));
    this->runAction(Sequence::create(DelayTime::create(1), RemoveSelf::create(), NULL));
}

#pragma callback
void MJDeviceInfoAlert::onClose(cocos2d::Ref *sender){
    MJDEFAULT->setIntegerForKey("key_popup_id_to_show", 0);
    auto key = StringUtils::format("%s%d", "key_popup_showed", _info.notification_id);
    MJDEFAULT->setBoolForKey(key, true);
    this->onHide();
}

#include "MJListLicense.h"
#include "StoryDataModel.h"
void MJDeviceInfoAlert::onViewListDevice(cocos2d::Ref *sender){
    MJDEFAULT->setIntegerForKey("key_popup_id_to_show", 0);
    auto key = StringUtils::format("%s%d", "key_popup_showed", _info.notification_id);
    MJDEFAULT->setBoolForKey(key, true);
    
    auto packages = StoryDataModel::getInstance()->getLicensePackages();
    
    if (packages.size() == 1) {
        auto list_device = StoryDataModel::getInstance()->getLicenseDeviceInfo();
        auto current_device = LICENSE_INFO_MODEL->getDeviceID();
//        auto license_key = MJDEFAULT->getStringForKey("license_key");
        auto license = LICENSE_INFO_MODEL->getCurrentLicense();
        LICENSE_INFO_MODEL->setInfoVerify(license.phone, license.mail);
        auto total_active = StringUtils::format("%d/%d", license.current_active, license.total_active);
        auto list_device_view = MJListDeviceView::createView(list_device, current_device, license, total_active);
        cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
        list_device_view->onShow();
    } else {
        auto list_license = MJListLicense::createView(packages);
        cocos2d::Director::getInstance()->getRunningScene()->addChild(list_license, INT_MAX - 1);
    }
    
    this->removeFromParent();
    
}

ui::Widget::ccWidgetClickCallback MJDeviceInfoAlert::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJDeviceInfoAlert::onClose, this);
    }
    
    if (callback_name == "onViewListDevice") {
        return CC_CALLBACK_1(MJDeviceInfoAlert::onViewListDevice, this);
    }
    
    return nullptr;
}

void MJDeviceInfoAlert::setDelegate(MJDeviceInfoAlertDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJDeviceInfoAlert::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
