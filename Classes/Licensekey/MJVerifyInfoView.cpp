//
//  MJVerifyInfoView.cpp
//  Created by HaVanHoi on 21/06/18.
//
//
#include "MJVerifyInfoView.h"
#include "GameData.h"
#include "NativeBridge.h"
#include "MJDefault.h"
#include "MJAPIVerifyInfoLicense.h"
#include "MJWaitingHUD.h"
#include "MJHelper.h"
INITIALIZE_READER(MJVerifyInfoView);

USING_NS_CC;
std::once_flag MJVerifyInfoView_reader;
#define CSB_NAME "csb/licensekey/MJVerifyInfoView.csb"
#define CSB_NAME_EN "csb/licensekey/MJVerifyInfoView_EN.csb"
MJVerifyInfoView * MJVerifyInfoView::createView(license::Package license) {
    std::call_once(MJVerifyInfoView_reader, [] {
        REGISTER_CSB_READER(MJVerifyInfoView);
    });
    auto p = static_cast<MJVerifyInfoView *>(cocos2d::CSLoader::createNode(LICENSE_INFO_MODEL->isDisplayLanguageVN()?CSB_NAME:CSB_NAME_EN));
    if (p) {
        p->didLoadFromCSB(license);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJVerifyInfoView::didLoadFromCSB(license::Package p) {
    this->_license = p;
    this->loadFrames();
}

void MJVerifyInfoView::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->onShow();
}

void MJVerifyInfoView::loadFrames() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    this->setContentSize(screen_size);
    ui::Helper::doLayout(this);
    utils::findChild(this, "bg_phone")->setScale(0);
    utils::findChild(this, "bg_mail")->setScale(0);
    utils::findChild(this, "color_layout")->setOpacity(0);
//    auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    if (_license.phone != std::string("")) {
        info_verify = _license.phone;
        bg_name = "bg_phone";
        verify_type = PHONE;
        
    }else if (_license.mail != std::string("")){
        info_verify = _license.mail;
        bg_name = "bg_mail";
        verify_type = PHONE;
        
    }else {
        verify_type = NULL_INFO;
    }
    
    if (verify_type != NULL_INFO) {
        auto bg = utils::findChild(this, bg_name);
        auto content1_lb = this->getTextByName(bg, "content1_lb");
        auto content2_lb = this->getTextByName(bg, "content2_lb");
        content2_lb->setString(info_verify);
        auto box_img = utils::findChild(bg, "box_img");
        placeholder_lb = this->getTextByName(bg, "placeholder_lb");
        placeholder_lb->setLocalZOrder(100);
        auto listview = utils::findChild(bg, "listview");
        listview->setContentSize(cocos2d::Size(content1_lb->getContentSize().width + content2_lb->getContentSize().width, listview->getContentSize().height));
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        box_img->setVisible(false);
        textBox = ui::EditBox::create(box_img->getContentSize(), ui::Scale9Sprite::create("license/verifyinfoview/verifyinfoview_textbox.png"));
        textBox->setFontName("fonts/Lato-Bold.ttf");
        textBox->setFontColor(Color3B::BLACK);
        textBox->setPlaceholderFontColor(Color3B{103, 103, 103});
        textBox->setMaxLength(100);
        textBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
        textBox->setText("");
        bg->addChild(textBox, 2);
        textBox->setPosition(box_img->getPosition());
        textBox->setTag(200);
        textBox->setInputFlag(ui::EditBox::InputFlag::SENSITIVE);
        textBox->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
        textBox->setDelegate(this);
#else
        
        auto placeholder = std::string("");
        textField = TextField::create(placeholder,"fonts/Lato-Bold.ttf",30);
        textField->setPlaceHolder(placeholder_lb->getString());
        placeholder_lb->setVisible(false);
        textField->ignoreContentAdaptWithSize(false);
        textField->setContentSize(box_img->getContentSize());
        textField->setTextColor(Color4B::BLACK);
        textField->setTextHorizontalAlignment(TextHAlignment::CENTER);
        textField->setTextVerticalAlignment(TextVAlignment::CENTER);
        textField->setPosition(box_img->getPosition());
        textField->setMaxLengthEnabled(true);
        textField->setMaxLength(100);
        
        //    textField->addEventListener(CC_CALLBACK_2(LoginScene::textFieldEvent, this));
        textField->setCursorEnabled(true);
        bg->addChild(textField, 2);
#endif
    }
    
}

void MJVerifyInfoView::onShow(){
    if (verify_type == NULL_INFO) {
        if (delegate) {
            delegate->onVerifySuccess();
        }
        this->onHide();
        return;
    }
    utils::findChild(this, bg_name)->runAction(EaseBackOut::create(ScaleTo::create(0.5f, 1.0f)));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0.9f*255));
}
void MJVerifyInfoView::onHide(){
    utils::findChild(this, bg_name)->runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(0.5f, 0)), CallFunc::create([=]{
        this->removeFromParent();
    }), NULL));
    utils::findChild(this, "color_layout")->runAction(FadeTo::create(0.5f, 0));
}

#pragma callback
void MJVerifyInfoView::onClose(cocos2d::Ref *sender){
    this->onHide();
}
void MJVerifyInfoView::onSend(cocos2d::Ref *sender){
    if (NativeBridge::networkAvaiable()) {
        std::string info_user;
#if (CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        info_user = textBox->getText();
#else
        info_user = textField->getString();
#endif
        if(info_user.length() == 0 || (!mj::helper::isValidEmail(info_user) && !mj::helper::isValidPhoneNumber(info_user))) {
            auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
            MJLicenseAlert::showAlert(0, is_vn?"Vui lòng nhập đúng định dạng cho email hoặc số điện thoại.":"Please enter a valid email address or phone number.", "", is_vn?"Đóng":"Close", nullptr);
            return;
        }
        MJWaitingHUD::show(this);
        mj::MJAPIVerifyInfoLicense::call(info_user, _license.license, [=](bool is_succes, std::string msg){
            if (is_succes) {
                if (delegate) {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                        MJWaitingHUD::hide();
                        delegate->onVerifySuccess();
                        this->removeFromParent();
                    });
//                    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
//
//                    }), NULL));
                }
            }else{
                if (msg == "") {
                    static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
                    static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
                    msg = LICENSE_INFO_MODEL->isDisplayLanguageVN()?msg_errorConnect_vn:msg_errorConnect;
                }
                //
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg] {
                    MJWaitingHUD::hide();
                    MJLicenseAlert::showAlert(0, msg, "", LICENSE_INFO_MODEL->isDisplayLanguageVN()?"Đóng":"Close");
                });
//                this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
//
////                    delegate->onVerifySuccess();
////                    this->removeFromParent();
//                }), NULL));
            }
        });
    }else {
        static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
        static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
        MJLicenseAlert::showAlert(0, LICENSE_INFO_MODEL->isDisplayLanguageVN()?msg_errorConnect_vn:msg_errorConnect, "", LICENSE_INFO_MODEL->isDisplayLanguageVN()?"Đóng":"Close", this);
    }
    
}

void MJVerifyInfoView::onMJAlertClicked(bool is_ok, int tag) {
    
}

ui::Widget::ccWidgetClickCallback MJVerifyInfoView::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClose") {
        return CC_CALLBACK_1(MJVerifyInfoView::onClose, this);
    }
    
    if (callback_name == "onSend") {
        return CC_CALLBACK_1(MJVerifyInfoView::onSend, this);
    }
    
    return nullptr;
}

void MJVerifyInfoView::setDelegate(MJVerifyInfoViewDelegate *delegate){
    this->delegate = delegate;
}

cocos2d::ui::Text *MJVerifyInfoView::getTextByName(cocos2d::Node *parent, std::string text_name, int font_size){
    auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(parent, text_name);
    if (font_size > 0) {
        text->setFontSize(font_size);
    }
    return text;
}

void MJVerifyInfoView::onTextfieldChanged(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type){
    switch (type)
    {
        case TextField::EventType::INSERT_TEXT:
        {
            CCLOG("INSERT_TEXT - %s", textField->getString().c_str());
            auto utf8 = StringUtils::StringUTF8(textField->getString());
            if (utf8.length() >= 1) {
                if (utf8.getString().at(utf8.length() - 1)._char == " ") {
                    utf8.deleteChar(utf8.length() - 1);
                    textField->setString(utf8.getAsCharSequence());
                }
            }
//            textField->setString(NativeBridge::string_towupper(textField->getString()));
        }
            break;
        default:
            break;
    }
}

void MJVerifyInfoView::editBoxEditingDidBegin(ui::EditBox *editBox){
    
}
void MJVerifyInfoView::editBoxEditingDidEnd(ui::EditBox *editBox) {
    
}
void MJVerifyInfoView::editBoxTextChanged(ui::EditBox *editBox, const std::string& text) {
    CCLOG("text change = %s", text.c_str());
    auto utf8 = StringUtils::StringUTF8(text);
    if (utf8.length() >= 1) {
        if (utf8.getString().at(utf8.length() - 1)._char == " ") {
            utf8.deleteChar(utf8.length() - 1);
            editBox->setText(utf8.getAsCharSequence().c_str());
        }
    }
    if (editBox->getText() == std::string("")) {
        placeholder_lb->setVisible(true);
    }else placeholder_lb->setVisible(false);
}
void MJVerifyInfoView::editBoxReturn(ui::EditBox *editBox) {
    
}
