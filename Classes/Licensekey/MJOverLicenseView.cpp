//
//  MJOverLicenseView.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJOverLicenseView.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "LicenseInfoModel.h"
INITIALIZE_READER(MJOverLicenseView);

USING_NS_CC;
std::once_flag MJOverLicenseView_reader;
#define CSB_NAME "csb/licensekey/MJOverLicenseView.csb"
MJOverLicenseView * MJOverLicenseView::createView(std::string message) {
    std::call_once(MJOverLicenseView_reader, [] {
        REGISTER_CSB_READER(MJOverLicenseView);
    });
    auto p = static_cast<MJOverLicenseView *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(message);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJOverLicenseView::didLoadFromCSB(std::string message) {
    this->message = message;
    this->loadFrames();
}

void MJOverLicenseView::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

void MJOverLicenseView::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    utils::findChild(this, "bg")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
    
    this->getTextByName("msg")->setString(message);
    if (!LICENSE_INFO_MODEL->isDisplayLanguageVN()) {
        this->getTextByName("title")->setString("Unsuccessful Activation");
        utils::findChild<Button *>(this, "btok")->setTitleText("Manage devices");
        
        auto freeuse = this->getTextByName("freeuse");
        freeuse->setString("Continue with the free version");
        auto subscribe = this->getTextByName("subscribe");
        subscribe->setString("Buy new license key");
        
        utils::findChild(freeuse, "gachngang")->setContentSize(cocos2d::Size(freeuse->getContentSize().width, 2));
        utils::findChild(freeuse, "gachngang")->setPositionX(freeuse->getContentSize().width/2);
        utils::findChild(subscribe, "gachngang")->setContentSize(cocos2d::Size(subscribe->getContentSize().width, 2));
        utils::findChild(subscribe, "gachngang")->setPositionX(subscribe->getContentSize().width/2);
    }
}

void MJOverLicenseView::onShow(){
    if (is_showed) return;
    is_showed = true;
    utils::findChild(this, "bg")->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJOverLicenseView::onHide(){
    if (!is_showed) return;
    is_showed = false;
    utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}

#pragma callback
void MJOverLicenseView::onClose(cocos2d::Ref *sender){
    this->onHide();
}

#include "MJDefinitions.h"
#include "MJDefault.h"
void MJOverLicenseView::onSubscribe(cocos2d::Ref *sender){
    MJDEFAULT->setBoolForKey(key_show_parent_lock, false);
    LICENSE_INFO_MODEL->onSubscribe();
    MJDEFAULT->setBoolForKey(key_show_parent_lock, true);
    this->onHide();
    if (delegate) {
        delegate->onOverLicenseViewSubscribe();
    }
}
void MJOverLicenseView::onFreeUse(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onOverLicenseViewFreeUse();
    }
    this->onHide();
}

void MJOverLicenseView::onViewListDevice(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onOverLicenseViewListDevice();
    }
}

ui::Widget::ccWidgetClickCallback MJOverLicenseView::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJOverLicenseView::onClose, this);
    }
    
    if (callback_name == "onSubscribe") {
        return CC_CALLBACK_1(MJOverLicenseView::onSubscribe, this);
    }
    
    if (callback_name == "onFreeUse") {
        return CC_CALLBACK_1(MJOverLicenseView::onFreeUse, this);
    }
    
    if (callback_name == "onViewListDevice") {
        return CC_CALLBACK_1(MJOverLicenseView::onViewListDevice, this);
    }
    
    return nullptr;
}

void MJOverLicenseView::setDelegate(MJOverLicenseViewDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJOverLicenseView::getTextByName(std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}
