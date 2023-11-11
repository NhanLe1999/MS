//
//  MJListDeviceView_Cell.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJListDeviceView_Cell.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "LTDate.h"
INITIALIZE_READER(MJListDeviceView_Cell);

USING_NS_CC;
#define CSB_NAME "csb/licensekey/MJListDeviceView_Cell.csb"
MJListDeviceView_Cell * MJListDeviceView_Cell::createCell(license::LicenseDeviceInfo device_info, bool show_btremove) {
    auto p = static_cast<MJListDeviceView_Cell *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(device_info, show_btremove);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJListDeviceView_Cell::didLoadFromCSB(license::LicenseDeviceInfo device_info, bool show_btremove) {
    this->device_info = device_info;
    this->show_btremove = show_btremove;
    this->loadFrames();
}

void MJListDeviceView_Cell::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJListDeviceView_Cell::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    ui::Helper::doLayout(this);
    
    auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    this->getTextByName("deviceid_lb")->setString(StringUtils::toString(device_info.device_id));
    this->getTextByName("devicename_lb")->setString(device_info.device_name);
    
    if (device_info.device_status) {
        this->getTextByName("status_lb")->setString(is_vn?std::string("Đang sử dụng"):std::string("Active"));
        this->getTextByName("deactivedate_lb")->setString("");
        this->getTextByName("activedate_lb")->setString(is_vn?LTDate(device_info.active_date).toPrettyString():LTDate(device_info.active_date).toPrettyStringGB());
    }else {
        auto color_text = Color4B(206, 52, 0, 255);
        this->getTextByName("deviceid_lb")->setTextColor(color_text);
        this->getTextByName("devicename_lb")->setTextColor(color_text);
        this->getTextByName("activedate_lb")->setTextColor(color_text);
        this->getTextByName("deactivedate_lb")->setTextColor(color_text);
        this->getTextByName("status_lb")->setTextColor(color_text);
        this->getTextByName("status_lb")->setString(is_vn?std::string("Đã hủy"):std::string("Removed"));
        utils::findChild(this, "btremove")->setVisible(false);
        
        if (device_info.deactive_date > 0) {
            this->getTextByName("deactivedate_lb")->setString(is_vn?LTDate(device_info.deactive_date).toPrettyString():LTDate(device_info.deactive_date).toPrettyStringGB());
        } else {
            this->getTextByName("deactivedate_lb")->setString(is_vn ? "Không\nxác định" : "N/A");
        }

        if (device_info.active_date > 0) {
            this->getTextByName("activedate_lb")->setString(is_vn?LTDate(device_info.active_date).toPrettyString():LTDate(device_info.active_date).toPrettyStringGB());
        }
    }
    
    if (!is_vn) {
        this->getTextByName("btremove_lb")->setString("Remove");
    }
    
    if (!show_btremove){
        utils::findChild(this, "btremove")->setVisible(false);
    }
}

#pragma callback
void MJListDeviceView_Cell::onRemove(cocos2d::Ref *sender){
    CCLOG("cell remove clicked");
    if (delegate) {
        delegate->onDeActiveDeviceCell(device_info);
    }
}

ui::Widget::ccWidgetClickCallback MJListDeviceView_Cell::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRemove") {
        return CC_CALLBACK_1(MJListDeviceView_Cell::onRemove, this);
    }
    
    return nullptr;
}

void MJListDeviceView_Cell::setDelegate(MJListDeviceView_CellDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJListDeviceView_Cell::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
