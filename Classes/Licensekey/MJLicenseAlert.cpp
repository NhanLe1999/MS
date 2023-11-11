//
//  MJ_MJLicenseAlert.h
//  Created by HoiSung on 02/02/18.
//
//

#include "MJLicenseAlert.h"
#include "GlobalFunc.h"
INITIALIZE_READER(MJLicenseAlert);
std::once_flag MJLicenseAlert_reader;
#define CSB_NAME "csb/licensekey/MJLicenseAlert.csb"
MJLicenseAlert * MJLicenseAlert::showAlert(int tag, string message, string ok_label, string cancel_label, MJLicenseAlertDelegate *delegate, bool showExit,bool xanhdo) {
    std::call_once(MJLicenseAlert_reader, [] {
        REGISTER_CSB_READER(MJLicenseAlert);
    });
    auto p = static_cast<MJLicenseAlert *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(tag, message, ok_label, cancel_label, delegate, showExit, xanhdo);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJLicenseAlert::didLoadFromCSB(int tag, string message, string ok_label, string cancel_label, MJLicenseAlertDelegate *delegate, bool showExit,bool xanhdo){
    auto sceneSize = cocos2d::Director::getInstance()->getWinSize();
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setPosition(sceneSize/2);
    this->setTouchEnabled(true);
    alert_tag = tag;
    this->delegate = delegate;
    cocos2d::Director::getInstance()->getRunningScene()->addChild(this, INT_MAX - 1);
    
    utils::findChild<cocos2d::ui::Text *>(this, "message")->setString(message);
    utils::findChild<cocos2d::ui::Text *>(this, "lbcancel")->setString(cancel_label);
    utils::findChild<cocos2d::ui::Text *>(this, "lbok")->setString(ok_label);
    auto bg = this->getChildByName("bg");
    bg->setScale(0);
    bg->runAction(Sequence::create(EaseBackOut::create(ScaleTo::create(0.25f, 1.0f)), CallFunc::create([=]{
        is_showed = true;
    }), NULL));
    this->getChildByName("color_layout")->runAction(FadeTo::create(0.25f, 0.9f*255));
    
    if (ok_label == "") {
        utils::findChild(this, "btok")->setVisible(false);
        utils::findChild(this, "btcancel")->setPositionX(bg->getContentSize().width/2);
    }
    
    if (cancel_label == "") {
        utils::findChild(this, "btcancel")->setVisible(false);
        utils::findChild(this, "btok")->setPositionX(bg->getContentSize().width/2);
    }
    
}

void MJLicenseAlert::fixSizeText(Label *label, int font_size, float max_height){
    font_size -= 2;
    label->setTTFConfig(cocos2d::TTFConfig("fonts/monkeystories_medium.ttf", font_size));
    if(label->getContentSize().height > max_height) {
        this->fixSizeText(label, font_size, max_height);
    }
}

void MJLicenseAlert::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJLicenseAlert::onOkClicked(cocos2d::Ref *sender){
    if (!is_showed) return;
    this->onHide(true);
}
void MJLicenseAlert::onCancelClicked(cocos2d::Ref *sender){
    if (!is_showed) return;
    this->onHide(false);
}

void MJLicenseAlert::onHide(bool is_ok){
    is_showed = false;
    cocos2d::utils::findChild(this, "color_layout")->runAction(FadeOut::create(0.25f));
    cocos2d::utils::findChild(this, "bg")->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.25f, 0)), CallFunc::create([=]{
        if (delegate) {
            delegate->onMJAlertClicked(is_ok, alert_tag);
        }
        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.05f), CallFunc::create([=]{
        }), NULL));
        this->removeFromParent();
    }), NULL));
    
}

ui::Widget::ccWidgetClickCallback MJLicenseAlert::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onOkClicked") {
        return CC_CALLBACK_1(MJLicenseAlert::onOkClicked, this);
    }
    
    if (callback_name == "onCancelClicked") {
        return CC_CALLBACK_1(MJLicenseAlert::onCancelClicked, this);
    }
    
    return nullptr;
}
