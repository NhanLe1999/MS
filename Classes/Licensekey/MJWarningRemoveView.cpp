//
//  MJWarningRemoveView.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJWarningRemoveView.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "MJAPIRemoveLicense.h"
#include "MJWaitingHUD.h"
INITIALIZE_READER(MJWarningRemoveView);

USING_NS_CC;
std::once_flag MJWarningRemoveView_reader;
#define CSB_NAME "csb/licensekey/MJWarningRemoveView.csb"
MJWarningRemoveView * MJWarningRemoveView::createView(license::LicenseDeviceInfo device_info, std::string license_key, int current_device_id) {
    std::call_once(MJWarningRemoveView_reader, [] {
        REGISTER_CSB_READER(MJWarningRemoveView);
    });
    auto p = static_cast<MJWarningRemoveView *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(device_info, license_key, current_device_id);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJWarningRemoveView::didLoadFromCSB(license::LicenseDeviceInfo device_info, std::string license_key, int current_device_id) {
    this->device_info = device_info;
    this->license_key = license_key;
    this->current_device_id = current_device_id;
    this->loadFrames();
}

void MJWarningRemoveView::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

void MJWarningRemoveView::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    auto current_device_lb = this->getTextByName("current_device_lb");

    if (device_info.device_id == current_device_id){
        current_device_lb->setVisible(true);
    }
    
    auto msg0 = this->getTextByName("msg0");
    auto msg1 = this->getTextByName("msg1");
    auto msg2 = this->getTextByName("msg2");
    auto msg3 = this->getTextByName("msg3");
    auto list_view = utils::findChild(this, "listview");
    if (is_vn) {
        
    }else{
        utils::findChild<Text *>(this, "title")->setString("WARNING!");
        utils::findChild<Button *>(this, "btclose")->setTitleText("Cancel");
        utils::findChild<Button *>(this, "btok")->setTitleText("Remove");
        msg0->setVisible(false);
        msg1->setString("Are you sure to remove ");
        msg3->setString("");
        current_device_lb->setString("(This is your current device)");
        
//        list_view->setPositionY(list_view->getPositionY()+20);
//        current_device_lb->setPositionY(current_device_lb->getPositionY()+20);
    }
    msg2->setString(StringUtils::format("%s - ID%d", device_info.device_name.c_str(), device_info.device_id));
    
    list_view->setContentSize(cocos2d::Size(msg1->getContentSize().width + msg2->getContentSize().width + msg3->getContentSize().width, list_view->getContentSize().height));
    
    
}

void MJWarningRemoveView::onShow(){
    if (is_showed) return;
    is_showed = true;
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJWarningRemoveView::onHide(){
    if (!is_showed) return;
    is_showed = false;
    utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}

#pragma callback
void MJWarningRemoveView::onClose(cocos2d::Ref *sender){
    this->onHide();
}

void MJWarningRemoveView::onOk(cocos2d::Ref *sender){
    if (NativeBridge::networkAvaiable()) {
        MJWaitingHUD::show(this);
        mj::MJAPIRemoveLicense::call(StringUtils::toString(current_device_id), license_key, StringUtils::toString(device_info.device_id), [=](bool is_succes, std::string msg){
            MJWaitingHUD::hide();
            if (is_succes) {
                if (delegate) {
                    delegate->onRemoveDeviceLicenseSuccess();
                }
                this->onHide();
            }else{
                std::string message;
                if (msg != ""){
                    message = msg;
                }else {
                    static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
                    static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
                    message = LICENSE_INFO_MODEL->isDisplayLanguageVN()?msg_errorConnect_vn:msg_errorConnect;
                }
                MJLicenseAlert::showAlert(0, message, "", LICENSE_INFO_MODEL->isDisplayLanguageVN()?"Đóng":"Close");
            }
        });
    }else {
        static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
        static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
        MJLicenseAlert::showAlert(0, LICENSE_INFO_MODEL->isDisplayLanguageVN()?msg_errorConnect_vn:msg_errorConnect, "", LICENSE_INFO_MODEL->isDisplayLanguageVN()?"Đóng":"Close");
    }
}

ui::Widget::ccWidgetClickCallback MJWarningRemoveView::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJWarningRemoveView::onClose, this);
    }
    
    if (callback_name == "onOk") {
        return CC_CALLBACK_1(MJWarningRemoveView::onOk, this);
    }
    
    return nullptr;
}

void MJWarningRemoveView::setDelegate(MJWarningRemoveViewDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJWarningRemoveView::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
