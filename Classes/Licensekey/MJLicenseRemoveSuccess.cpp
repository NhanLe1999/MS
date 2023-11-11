//
//  MJLicenseRemoveSuccess.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJLicenseRemoveSuccess.h"
#include "GameData.h"
#include "NativeBridge.h"
INITIALIZE_READER(MJLicenseRemoveSuccess);

USING_NS_CC;
std::once_flag MJLicenseRemoveSuccess_reader;
#define CSB_NAME "csb/licensekey/MJLicenseRemoveSuccess.csb"
MJLicenseRemoveSuccess * MJLicenseRemoveSuccess::createView(license::LicenseDeviceInfo device_info, int success_type) {
    std::call_once(MJLicenseRemoveSuccess_reader, [] {
        REGISTER_CSB_READER(MJLicenseRemoveSuccess);
    });
    auto p = static_cast<MJLicenseRemoveSuccess *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(device_info, success_type);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJLicenseRemoveSuccess::didLoadFromCSB(license::LicenseDeviceInfo device_info, int success_type) {
    this->success_type = success_type;
    this->device_info = device_info;
    this->loadFrames();
    
}

void MJLicenseRemoveSuccess::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

void MJLicenseRemoveSuccess::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    auto lbok = this->getTextByName("lbok");
    auto title = this->getTextByName("title");
    auto msg1 = this->getTextByName("msg1");
    auto msg2 = this->getTextByName("msg2");
    auto msg3 = this->getTextByName("msg3");
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    if (success_type == REMOVE_CURRENT){
        title->setString(is_vn?"HỦY quyền sử dụng thiết bị hiện tại thành công":"Successful removal!");
        title->setPosition(title->getParent()->getContentSize() / 2);
        lbok->setString(is_vn?"Khởi động lại ứng dụng":"Restart the app");
        
//        msg1->setString(is_vn?"Phiên bản hiện tại: bản dùng thử miễn phí":"You are now using the free version.");
        msg1->setString("");
        msg2->setString("");
        msg3->setString("");
    }else if (success_type == REMOVE_OTHER){
        title->setString(is_vn?"HỦY quyền sử dụng thành công":"Successful removal!");
        lbok->setString("Ok");
        
        msg1->setString(is_vn?"Thiết bị ":"");
        msg2->setString(StringUtils::format("%s - ID%d", device_info.device_name.c_str(), device_info.device_id));
        msg3->setString(is_vn?" đã được HỦY quyền sử dụng":" was successfully removed.");
    }else {
        title->setString(is_vn?"Chúc mừng bạn đã kích hoạt thành công":"Successful activation");
        lbok->setString(is_vn?"Khởi động lại ứng dụng":"Restart the app");
        
        msg1->setString(is_vn?"Thiết bị ":"");
        msg2->setString(StringUtils::format("%s - ID%d", device_info.device_name.c_str(), device_info.device_id));
        msg3->setString(is_vn?" đã được HỦY quyền sử dụng":" was successfully removed.");
        
        auto active_lb = this->getTextByName("active_lb");
        active_lb->setVisible(true);
        active_lb->setString(is_vn?"Thiết bị bạn đang sử dụng đã kích hoạt quyền sử dụng thành công":"Your device was successfully activated.");
        listview->setPositionX(listview->getPositionX()+5);
    }
    
    listview->setContentSize(cocos2d::Size(msg1->getContentSize().width + msg2->getContentSize().width + msg3->getContentSize().width, listview->getContentSize().height));
    listview->setScrollBarEnabled(false);
    
}

void MJLicenseRemoveSuccess::onShow(){
    if (is_showed) return;
    is_showed = true;
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJLicenseRemoveSuccess::onHide(){
    if (!is_showed) return;
    is_showed = false;
    utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}

#pragma callback
void MJLicenseRemoveSuccess::onOk(cocos2d::Ref *sender){
    if (success_type == REMOVE_OTHER){
        utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.25f, 0)), DelayTime::create(0.1f), CallFunc::create([=]{
            if (delegate) {
                delegate->onAlertRemoveSuccessConfirm();
            }
        }), NULL));
        utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
    }else if (success_type == REMOVE_CURRENT){
        LICENSE_INFO_MODEL->deleteActiveApp();
        LICENSE_INFO_MODEL->restartApp();
    }else {
        //khởi động lại app.
        LICENSE_INFO_MODEL->restartApp();
    }
}

ui::Widget::ccWidgetClickCallback MJLicenseRemoveSuccess::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onOk") {
        return CC_CALLBACK_1(MJLicenseRemoveSuccess::onOk, this);
    }
    return nullptr;
}

void MJLicenseRemoveSuccess::setDelegate(MJLicenseRemoveSuccessDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJLicenseRemoveSuccess::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
