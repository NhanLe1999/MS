//
//  MJListDeviceView.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJListDeviceView.h"
#include "GameData.h"
#include "MJDefault.h"
#include "NativeBridge.h"
#include "MSEventLogger.h"
#include "LTDate.h"
#include "MJWaitingHUD.h"
#include "MJAPILicense.h"
#include "MJHelper.h"
INITIALIZE_READER(MJListDeviceView);

USING_NS_CC;
using namespace ui;
std::once_flag MJListDeviceView_reader;
#define CSB_NAME "csb/licensekey/MJListDeviceView.csb"
#define CSB_NAME_EN "csb/licensekey/MJListDeviceView_EN.csb"
MJListDeviceView * MJListDeviceView::createView(std::vector<license::LicenseDeviceInfo> list_device_info, int deviceid_current, license::Package license, std::string total_active, bool is_active_license, bool show_btremove) {
    std::call_once(MJListDeviceView_reader, [] {
        REGISTER_CSB_READER(MJListDeviceView);
        REGISTER_CSB_READER(MJListDeviceView_Cell);
    });
    auto p = static_cast<MJListDeviceView *>(cocos2d::CSLoader::createNodeWithVisibleSize(LICENSE_INFO_MODEL->isDisplayLanguageVN()?CSB_NAME:CSB_NAME_EN));
    if (p) {
        p->didLoadFromCSB(list_device_info, deviceid_current, license, total_active, is_active_license, show_btremove);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJListDeviceView::didLoadFromCSB(std::vector<license::LicenseDeviceInfo> list_device_info, int deviceid_current, license::Package license, std::string total_active, bool is_active_license, bool show_btremove) {
    this->list_device_info = list_device_info;
    this->deviceid_current = deviceid_current;
    this->_license = license;
    this->is_active_license = is_active_license;
    this->show_btremove = show_btremove;
    this->total_active = total_active;
    this->loadFrames();
}

#include "StoryConfigManager.h"
void MJListDeviceView::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    
    if (CONFIG_MANAGER->isVietnameseStories()) {
        auto logo =  utils::findChild<ImageView*>(this, "logo");
        logo->loadTexture("mjstory/monkey_logo_vi_new.png");
        logo->ignoreContentAdaptWithSize(true);
        logo->setScale(0.8);
    }
    
    auto listview_navi = utils::findChild<ListView *>(this, "listview_navi");
    auto listview_device = utils::findChild<ListView *>(this, "listview_device");
    listview_navi->setScrollBarEnabled(false);
    this->getTextByName(this, "license_lb")->setString(_license.license);
    auto is_have_current_device = false;
    for (int i = 0; i < (int) list_device_info.size(); i++) {
        CCLOG("check_device_id = %d", list_device_info[i].device_id);
        if (list_device_info[i].device_id == deviceid_current) {
            current_device_info = list_device_info[i];
            this->setDisplayCurrentDevice(current_device_info);
            list_device_info.erase(list_device_info.begin() + i);
            is_have_current_device = true;
            break;
        }
    }
    
    if (!is_have_current_device) {
        utils::findChild(this, "bg")->removeChildByName("note_img");
        listview_navi->removeChild(utils::findChild(listview_navi, "cell_currentdevice"));
        listview_device->setContentSize(Size(listview_device->getContentSize().width, listview_device->getContentSize().height + 76));
        utils::findChild(this, "total_active_lb")->setPosition(Vec2(40,50));
    }else {
        
    }
    
    for (int i = 0; i < (int) list_device_info.size(); i++) {
        auto cell = MJListDeviceView_Cell::createCell(list_device_info[i], show_btremove);
        cell->setDelegate(this);
        listview_device->addChild(cell);
    }
    
    info_user = LICENSE_INFO_MODEL->phone_info;
    if (info_user == "") {
        info_user = LICENSE_INFO_MODEL->mail_info;
    }
    auto total_active_lb = utils::findChild<ui::Text *>(this, "total_active_lb");
    if (total_active != ""){
        auto list_split = mj::helper::split(total_active, '/');
        if ((int)list_split.size() > 1){
            if (LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
                total_active_lb->setString(StringUtils::format("Bạn đã kích hoạt %s trên tổng %s thiết bị", list_split[0].c_str(), list_split[1].c_str()));
            }else total_active_lb->setString(StringUtils::format("You have activated %s out of %s devices", list_split[0].c_str(), list_split[1].c_str()));
        }else total_active_lb->setString("");
    }else total_active_lb->setString("");
}

void MJListDeviceView::setDisplayCurrentDevice(license::LicenseDeviceInfo device_info){
    auto cell_current = utils::findChild(this, "cell_currentdevice");
    this->getTextByName(cell_current, "deviceid_lb")->setString(StringUtils::toString(device_info.device_id));
    this->getTextByName(cell_current, "devicename_lb")->setString(device_info.device_name);
    this->getTextByName(cell_current, "deactivedate_lb")->setString("");
    this->getTextByName(cell_current, "activedate_lb")->setString(LICENSE_INFO_MODEL->isDisplayLanguageVN()?LTDate(device_info.active_date).toPrettyString():LTDate(device_info.active_date).toPrettyStringGB());
    this->getTextByName(cell_current, "status_lb")->setString(LICENSE_INFO_MODEL->isDisplayLanguageVN()?std::string("Đang sử dụng"):std::string("Active"));
    
    std::string note_str = "Thiết bị bạn đang sử dụng";
    if (!LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
        note_str = "Your current device";
    }
    this->getTextByName(this, "note_lb")->setString(StringUtils::format("%s - %s", device_info.device_name.c_str(), note_str.c_str()));
}

void MJListDeviceView::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJListDeviceView::onShow(){
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.85f*255));
}
void MJListDeviceView::onHide(){
    utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}


void MJListDeviceView::onDeActiveDeviceCell(license::LicenseDeviceInfo device_info){
    choose_device_info = device_info;
    this->showVerifyInfoView();
}

void MJListDeviceView::showVerifyInfoView(){
    if (_license.phone.empty() && _license.mail.empty()) {
        onVerifySuccess();
        return;
    }
    auto verify_view = MJVerifyInfoView::createView(_license);
    verify_view->setDelegate(this);
    this->addChild(verify_view, INT_MAX - 1);
    
}

void MJListDeviceView::showWarningRemoveView(){
    auto warningremoveview = MJWarningRemoveView::createView(choose_device_info, _license.license, deviceid_current);
    warningremoveview->setDelegate(this);
    this->addChild(warningremoveview, INT_MAX - 1);
}

void MJListDeviceView::onRemoveDeviceLicenseSuccess(){
    this->showAlertLicenseRemoveSuccess();
}

void MJListDeviceView::showAlertLicenseRemoveSuccess(){
    auto success_type = MJLicenseRemoveSuccess::REMOVE_OTHER;
    if (is_active_license) {
        success_type = MJLicenseRemoveSuccess::ACTIVE_LICENSE;
        //hs_active_licese
        this->activeLicenseWhenRemoveOtherDeviceSuccess();
    } else if (choose_device_info.device_id == deviceid_current) {
        success_type = MJLicenseRemoveSuccess::REMOVE_CURRENT;
    }
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
    auto success_alert_view = MJLicenseRemoveSuccess::createView(choose_device_info, success_type);
    this->addChild(success_alert_view, INT_MAX-1);
    success_alert_view->setDelegate(this);
    });
}

void MJListDeviceView::activeLicenseWhenRemoveOtherDeviceSuccess(){
    mj::MJAPILicense::call(_license.license, false, [=](bool is_succes, std::string msg, rapidjson::Value response_data, int error_code) {
        if (is_succes) {
            this->updateDeviceInfoLicense(response_data.GetObject(), true);
        }else {
            this->activeLicenseWhenRemoveOtherDeviceSuccess();
        }
    });
}


void MJListDeviceView::updateDeviceInfoLicense(rapidjson::Value response_data, bool is_active_success){
    auto data = response_data.GetObject();
    std::string phone_info, mail_info, total_active;
    if (data.HasMember("phone") && data["phone"].IsString()) {
        phone_info = data["phone"].GetString();
    }
    
    if (data.HasMember("email") && data["email"].IsString()) {
        mail_info = data["email"].GetString();
    }
    if (data.HasMember("total_active") && data["total_active"].IsString()) {
        total_active = data["total_active"].GetString();
    }
    
    LICENSE_INFO_MODEL->setInfoVerify(phone_info, mail_info);
    
    if (data.HasMember("device_in_licence") && data["device_in_licence"].IsObject()) {
        auto device_in_licence = data["device_in_licence"].GetObject();
        list_device_info.clear();
        if (device_in_licence.HasMember("list_active") && device_in_licence["list_active"].IsArray()) {
            auto list_active = device_in_licence["list_active"].GetArray();
            for (int i = 0; i < (int) list_active.Size(); i++) {
                auto device_data = list_active[i].GetObject();
                license::LicenseDeviceInfo device_info;
                if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                    device_info.device_id = atoi(device_data["device_id"].GetString());
                }
                if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                    device_info.active_date = atoi(device_data["active_date"].GetString());
                }
                if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                    device_info.device_name = device_data["device_info"].GetString();
                }
                device_info.device_status = 1;
                list_device_info.push_back(device_info);
            }
        }
        
        if (device_in_licence.HasMember("list_remove") && device_in_licence["list_remove"].IsArray()) {
            auto list_remove = device_in_licence["list_remove"].GetArray();
            for (int i = 0; i < (int) list_remove.Size(); i++) {
                auto device_data = list_remove[i].GetObject();
                license::LicenseDeviceInfo device_info;
                if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                    device_info.device_id = atoi(device_data["device_id"].GetString());
                }
                
                if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                    device_info.active_date = atoi(device_data["active_date"].GetString());
                }
                
                if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                    device_info.device_name = device_data["device_info"].GetString();
                }
                
                if (device_data.HasMember("time_remove") && device_data["time_remove"].IsString()) {
                    device_info.deactive_date = atoi(device_data["time_remove"].GetString());
                }
                device_info.device_status = 0;
                list_device_info.push_back(device_info);
            }
        }
        
        if (is_active_success){
            LICENSE_INFO_MODEL->updateLicenseDeviceInfo(list_device_info);
            MJDEFAULT->setStringForKey("phone_info", phone_info);
            MJDEFAULT->setStringForKey("mail_info", mail_info);
            MJDEFAULT->setStringForKey("total_active", total_active);
        }
    }
}

#include "StoryDataModel.h"
void MJListDeviceView::onAlertRemoveSuccessConfirm() {
    for (int i = 0; i < (int) list_device_info.size(); i++) {
        if (list_device_info[i].device_id == choose_device_info.device_id) {
            list_device_info[i].device_status = 0;
            break;
        }
    }
    
    if (!is_active_license) {
        list_device_info.push_back(current_device_info);
        LICENSE_INFO_MODEL->updateLicenseDeviceInfo(list_device_info);
    }
    
    list_device_info = StoryDataModel::getInstance()->getLicenseDeviceInfoOfLicense(_license.license);
    auto list_new_reload = MJListDeviceView::createView(list_device_info, deviceid_current, _license, total_active, is_active_license, show_btremove);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(list_new_reload, INT_MAX);
    this->removeFromParent();
    
}

void MJListDeviceView::onRemove(cocos2d::Ref *sender){
    this->onDeActiveDeviceCell(current_device_info);
}

void MJListDeviceView::onMJAlertClicked(bool is_ok, int tag){
    if (is_ok) {
        
    }else {
        
    }
}

void MJListDeviceView::onVerifySuccess() {
    CCLOG("onVerifySuccess device %s", choose_device_info.device_name.c_str());
    this->showWarningRemoveView();
}

#pragma callback
void MJListDeviceView::onClose(cocos2d::Ref *sender){
    this->onHide();
}

ui::Widget::ccWidgetClickCallback MJListDeviceView::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJListDeviceView::onClose, this);
    }
    
    if (callback_name == "onRemove") {
        return CC_CALLBACK_1(MJListDeviceView::onRemove, this);
    }
    
    return nullptr;
}

void MJListDeviceView::setDelegate(MJListDeviceViewDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJListDeviceView::getTextByName(cocos2d::Node *parent, std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(parent, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
