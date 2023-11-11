//
//  MJFullLicenseView.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJFullLicenseView.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "MJHelper.h"
INITIALIZE_READER(MJFullLicenseView);

USING_NS_CC;
std::once_flag MJFullLicenseView_reader;
#define CSB_NAME "csb/MJFullLicenseView.csb"
MJFullLicenseView * MJFullLicenseView::createView(license::Package lic) {
    std::call_once(MJFullLicenseView_reader, [] {
        REGISTER_CSB_READER(MJFullLicenseView);
    });
    auto p = static_cast<MJFullLicenseView *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(lic);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJFullLicenseView::didLoadFromCSB(license::Package lic) {
    this->_license = lic;
    this->loadFrames();
}

void MJFullLicenseView::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

void MJFullLicenseView::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    auto msg = this->getTextByName("msg");
    auto msg_str = msg->getString();
    msg_str = mj::helper::replace_string(msg_str, "ABCDF", _license.license);
    
    auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    int num_index = 12;
    if (!is_vn) {
        num_index = 12;
    }
    msg->setString(msg_str);
    for (int i = 0; i < StringUtils::getCharacterCountInUTF8String(msg_str); i++) {
        auto letter = msg->getLetter(i);
        if (letter) {
            letter->setColor(Color3B(1, 1, 1));
        }
    }
    
    for (int i = num_index; i <= (num_index + _license.license.length()); i++) {
        auto letter = msg->getLetter(i);
        if (letter) {
            letter->setColor(Color3B(255, 1, 1));
        }
    }
}

void MJFullLicenseView::onShow(){
    if (is_showed) return;
    is_showed = true;
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJFullLicenseView::onHide(){
    if (!is_showed) return;
    is_showed = false;
    utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}

#pragma callback
void MJFullLicenseView::onClose(cocos2d::Ref *sender){
    this->onHide();
}

#include "MJDefinitions.h"
#include "MJDefault.h"
void MJFullLicenseView::onSubscribe(cocos2d::Ref *sender){
    MJDEFAULT->setBoolForKey(key_show_parent_lock, false);
    LICENSE_INFO_MODEL->onSubscribe();
    MJDEFAULT->setBoolForKey(key_show_parent_lock, true);
    this->onHide();
    if (delegate) {
        delegate->onFullLicenseonSubscribe();
    }
}
void MJFullLicenseView::onFreeUse(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onFullLicenseViewFreeUse();
    }
    this->onHide();
}

void MJFullLicenseView::onRemoveDevice(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onFullLicenseonRemoveDevice(_license);
    }
}


ui::Widget::ccWidgetClickCallback MJFullLicenseView::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJFullLicenseView::onClose, this);
    }
    
    if (callback_name == "onSubscribe") {
        return CC_CALLBACK_1(MJFullLicenseView::onSubscribe, this);
    }
    
    if (callback_name == "onFreeUse") {
        return CC_CALLBACK_1(MJFullLicenseView::onFreeUse, this);
    }
    
    if (callback_name == "onRemoveDevice") {
        return CC_CALLBACK_1(MJFullLicenseView::onRemoveDevice, this);
    }
    
    return nullptr;
}

void MJFullLicenseView::setDelegate(MJFullLicenseViewDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJFullLicenseView::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
