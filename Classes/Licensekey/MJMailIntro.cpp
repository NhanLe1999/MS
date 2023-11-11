//
//  MJMailIntro.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/28/18.
//

#include "MJMailIntro.h"
#include "MJDefault.h"
#include "NativeBridge.h"
#include "MJHelper.h"
#include "MJLicenseAlert.h"
#include "LicenseInfoModel.h"
#include "MJWaitingHUD.h"
#include "MJAPISendMail.h"
#include "MJAPIVerifyViral.h"
#include "MJAPIRefresh.h"

USING_NS_CC;
INITIALIZE_READER(MJMailIntro);
using namespace cocos2d;
using namespace ui;
std::once_flag MJMailIntro_reader_flag;

MJMailIntro * MJMailIntro::createView(bool is_popup) {
    std::call_once(MJMailIntro_reader_flag, [] {
        REGISTER_CSB_READER(MJMailIntro);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/licensekey/MJMailIntro.csb");
    auto view = reinterpret_cast<MJMailIntro *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (view) {
        view->didLoadFromCSB(is_popup);
    } else {
        CC_SAFE_DELETE(view);
    }
    
    return view;
}

MJMailIntro * MJMailIntro::createGiftView() {
    std::call_once(MJMailIntro_reader_flag, [] {
        REGISTER_CSB_READER(MJMailIntro);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/licensekey/InAppFinish_Gift.csb");
    auto view = reinterpret_cast<MJMailIntro *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (view) {
        view->didLoadFromCSB(true);
    } else {
        CC_SAFE_DELETE(view);
    }
    
    return view;
}

#include "StoryConfigManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "firebase/remote_config.h"
#endif

#define kRC_installPromotionPopup "install_promotion_popup"
#define kRC_installPromotionPopupAction "install_promotion_popup_action"
#define kRC_installPromotionPopup_Formatted "install_promotion_popup_formatted"

void MJMailIntro::didLoadFromCSB(bool is_popup) {
    this->is_popup = is_popup;
    utils::findChild<cocos2d::ui::ImageView *>(this, "logo")->setVisible(false);
    utils::findChild(this, "skip_label")->setVisible(true);
    
    if (CONFIG_MANAGER->isAppVNStories()) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/*        auto title = firebase::remote_config::GetString(kRC_installPromotionPopup);
        title = mj::helper::replace_string(title, "<br>", "\n");
        auto action = firebase::remote_config::GetString(kRC_installPromotionPopupAction);
        utils::findChild<ui::Text *>(this, "box_title")->setString(title);
        utils::findChild<ui::Text *>(this, "label_send")->setString(action);*/
#else

#endif
        std::string title = "Mời bạn nhập thông tin để nhận được<br>mã giảm giá lên đến 50%";
        title = mj::helper::replace_string(title, "<br>", "\n");
        auto action = "Nhận ưu đãi";
        auto box_title = utils::findChild<ui::Text *>(this, "box_title");
		box_title->setString(title);
        
        std::string rich_string = "<font color='#000000' size='25' face='fonts/Montserrat-SemiBold.ttf'>Mời bạn nhập thông tin dưới đây<br/>để nhận được <font color='#512079' size='25' face='fonts/Montserrat-Bold.ttf'>MÃ GIẢM GIÁ LÊN ĐẾN 50%</font><br/>và <font color='#512079' size='25' face='fonts/Montserrat-Bold.ttf'>BỘ 5 QUYỂN TRUYỆN TRANH</font> trị giá 150.000đ (**)</font>";
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
/*        std::string rich_string_remote = firebase::remote_config::GetString(kRC_installPromotionPopup_Formatted);
        if (!rich_string_remote.empty()) {
            rich_string = rich_string_remote;
        }*/
#else
#endif

        auto rich = ui::RichText::createWithXML(rich_string);
        rich->setContentSize(box_title->getContentSize());
        rich->setPosition(box_title->getPosition());
        rich->setAnchorPoint(box_title->getAnchorPoint());
        rich->ignoreContentAdaptWithSize(false);
        rich->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
        rich->setWrapMode(ui::RichText::WRAP_PER_WORD);
        rich->setVisible(box_title->isVisible());
        
        box_title->getParent()->addChild(rich);
        box_title->setVisible(false);
        
        
		//utils::findChild<ui::Text *>(this, "label_send")->setString(action);
    }
    
    if (is_popup) {
        this->getChildByName("bg")->setVisible(false);
        utils::findChild(this, "skip_label")->setVisible(false);
        auto button_close = utils::findChild(this, "button_close");
        if (button_close) {
            button_close->setVisible(true);
        }
    }
    
    is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    auto bg = this->getChildByName("bg");
    auto width = cocos2d::Director::getInstance()->getWinSize().width;
    if (bg->getBoundingBox().size.width < width) {
        bg->setScale(width*1.0f/bg->getBoundingBox().size.width);
    }
    
    icon_warning = utils::findChild<ImageView *>(this, "icon_warning");
    warning_label = utils::findChild<Text *>(this, "warning_label");
    
    textField_name = utils::findChild<ui::TextField *>(this, "textfield_name");
    if(textField_name){
        textField_name->setString(MJDEFAULT->getStringForKey("install.name", ""));
        textField_name->setTextHorizontalAlignment(TextHAlignment::CENTER);
        textField_name->setTextVerticalAlignment(TextVAlignment::CENTER);
        textField_name->setCursorEnabled(true);
        textField_name->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44 ));
        textField_name->addEventListener(CC_CALLBACK_2(MJMailIntro::onTextfieldChanged, this));
    }
    
//    textField_mail = utils::findChild<ui::TextField *>(this, "textfield_name");
//    textField_mail->setTextHorizontalAlignment(TextHAlignment::CENTER);
//    textField_mail->setTextVerticalAlignment(TextVAlignment::CENTER);
//    textField_mail->setCursorEnabled(true);
//    textField_mail->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44));
    
    textField_phone = utils::findChild<ui::TextField *>(this, "textfield_phone");
    auto prefill_phone = MJDEFAULT->getStringForKey("install.phone", "");
    if (prefill_phone == "0") {
        prefill_phone = "";
    }
    textField_phone->setString(prefill_phone);
    textField_phone->setTextHorizontalAlignment(TextHAlignment::CENTER);
    textField_phone->setTextVerticalAlignment(TextVAlignment::CENTER);
    textField_phone->setCursorEnabled(true);
    textField_phone->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44 ));
    textField_phone->addEventListener(CC_CALLBACK_2(MJMailIntro::onTextfieldChanged, this));

    
    textField_mail = utils::findChild<ui::TextField *>(this, "textfield_mail");
    textField_mail->setString(MJDEFAULT->getStringForKey("install.mail", ""));
    textField_mail->setTextHorizontalAlignment(TextHAlignment::CENTER);
    textField_mail->setTextVerticalAlignment(TextVAlignment::CENTER);
    textField_mail->setCursorEnabled(true);
    textField_mail->setPlaceHolderColor(Color4B(Color3B::BLACK, 0x44 ));
    textField_mail->addEventListener(CC_CALLBACK_2(MJMailIntro::onTextfieldChanged, this));

//#endif
}

void MJMailIntro::onTextfieldChanged(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type){
    auto box = utils::findChild(this, "box");

    switch (type)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        case TextField::EventType::ATTACH_WITH_IME:
            box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
            break;
        case TextField::EventType::DETACH_WITH_IME:
            box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
            break;
#endif
        case TextField::EventType::INSERT_TEXT:
        {
            CCLOG("INSERT_TEXT - %s", textField_mail->getString().c_str());
            auto utf8 = StringUtils::StringUTF8(textField_mail->getString());
            if (utf8.length() >= 1) {
                if (utf8.getString().at(utf8.length() - 1)._char == " ") {
                    utf8.deleteChar(utf8.length() - 1);
                    textField_mail->setString(utf8.getAsCharSequence());
                }
            }
            
            utf8 = StringUtils::StringUTF8(textField_phone->getString());
            if (utf8.length() >= 1) {
                auto new_char = utf8.getString().at(utf8.length() - 1)._char;
                
                if (new_char == " " || (new_char != "+" && !mj::helper::isValidNumber(new_char))) {
                    utf8.deleteChar(utf8.length() - 1);
                    textField_phone->setString(utf8.getAsCharSequence());
                }
            }
        }
            break;
        default:
            break;
    }
}

void MJMailIntro::editBoxEditingDidBegin(ui::EditBox *editBox){
    
}
void MJMailIntro::editBoxEditingDidEnd(ui::EditBox *editBox) {
    
}
void MJMailIntro::editBoxTextChanged(ui::EditBox *editBox, const std::string& text) {
    CCLOG("text change = %s", text.c_str());
    auto utf8 = StringUtils::StringUTF8(text);
    if (utf8.length() >= 1) {
        if (utf8.getString().at(utf8.length() - 1)._char == " ") {
            utf8.deleteChar(utf8.length() - 1);
            editBox->setText(utf8.getAsCharSequence().c_str());
        }
    }
}
void MJMailIntro::editBoxReturn(ui::EditBox *editBox) {
    
}

void MJMailIntro::onShow(){
    auto screen_size = cocos2d::Director::getInstance()->getWinSize();
    auto bg = cocos2d::utils::findChild(this, "bg");
    auto content = cocos2d::utils::findChild(this, "content");
    content->setPositionY(screen_size.height);
    content->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(0, -screen_size.height)), NULL));
    bg->setOpacity(0);
    bg->runAction(FadeIn::create(0.5f));
}

#include "StoryConfigManager.h"
#include "MJDefinitions.h"
#include "MSHome.h"
#include "StoryWelcomePopUp.h"
#include "StoryLanguageManager.h"
#include "StoryFreeManager.h"
#include "MSEventLogger.h"
#include "MJAPI_RegisterInstall.h"
#include "MJMailIntro.h"
#include "MSInstallPromotion.h"

void MJMailIntro::onClose(cocos2d::Ref * sender) {
    onSkip(nullptr);
}

void MJMailIntro::onSkip(cocos2d::Ref * sender) {
    if (sender) {
        ms::EventLogger::getInstance().logEvent("ms_skip_enter_information", {});
    }
    std::string text_phone, text_name, text_mail;
    text_phone = textField_phone->getString();
    if (textField_name) {
        text_name = textField_name->getString();
    } else {
        text_name = "";
    }
    text_mail = textField_mail->getString();
    
    if (!text_phone.empty() && text_phone.front() != '0') {
        text_phone = "0" + text_phone;
    }
    
    MJDEFAULT->setStringForKey("install.name", text_name);
    MJDEFAULT->setStringForKey("install.phone", text_phone);
    MJDEFAULT->setStringForKey("install.mail", text_mail);
    MJDEFAULT->setBoolForKey("install.verified", false);
    
    if (!text_name.empty() || !text_phone.empty() || !text_mail.empty())
        mj::api::RegisterInstall::call(text_name, text_phone, text_mail, 0, false, [](bool success, std::string voucher) {
        });
    
    MS_LOGGER.logEventOnboardInfoSubmit();

//    if (textField) textField->setVisible(false);
//    if (textBox) textBox->setVisible(false);
//    this->runAction(Sequence::create(FadeOut::create(0.5f), CallFunc::create([=]{
//        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("switch_scene");
//    }), NULL));
    
    this->runAction(Sequence::create(FadeOut::create(0.25), Hide::create(), nullptr));
    
    if (is_popup) {
        if (_callback) {
            _callback();
        }
        return;
    }
    
    if (FREE_STORY_MANAGER->isFirst2Days()) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
            auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Intro_Welcome.csb");
            auto free_text = CSLoader::createNode(csb_name);
            utils::findChild<ui::Text *>(free_text, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("free_welcome"));
            this->getParent()->addChild(free_text);
            free_text->setOpacity(0);
            //TODO: căn lại free welcome
            free_text->setAnchorPoint(Point::ANCHOR_MIDDLE);
            free_text->setPosition(Director::getInstance()->getVisibleSize()/2);
            free_text->runAction(Sequence::create(FadeIn::create(0.25), DelayTime::create(3.5), CallFunc::create([] {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                    MJDEFAULT->setIntegerForKey(key_call_welcome_popup, CONFIG_MANAGER->isAppActive() ? 3 : 0);
                    MJDEFAULT->setBoolForKey(key_onboarding_license, true);
                    
                    auto home = MSHome::createScene();
                    if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false)) {
                        if (MSInstallPromotion::shouldShowPopup()) {
                            auto popup = MSInstallPromotion::createView(true);
                            home->addChild(popup);
                        } else {
                            auto popup = StoryWelcomePopUp::createPage();
                            home->addChild(popup);
                        }
                    }
                    
                    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
                });
            }), NULL));
        });
    } else {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
            MJDEFAULT->setIntegerForKey(key_call_welcome_popup, CONFIG_MANAGER->isAppActive() ? 3 : 0);
            MJDEFAULT->setBoolForKey(key_onboarding_license, true);
            
            auto home = MSHome::createScene();
            if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false)) {
                if (MSInstallPromotion::shouldShowPopup()) {
                    auto popup = MSInstallPromotion::createView(true);
                    home->addChild(popup);
                } else {
                    auto popup = StoryWelcomePopUp::createPage();
                    home->addChild(popup);
                }
            }
            
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
        });
    }
}


void MJMailIntro::onSend(Ref * sender) {
    std::string text_phone, text_mail;
    text_phone = textField_phone->getString();
    text_mail = textField_mail->getString();
    
    auto is_valid_mail = (!text_mail.empty()) && (mj::helper::isValidEmail(text_mail));
    auto is_valid_phone = (!text_phone.empty()) && (mj::helper::isValidPhoneNumber(text_phone));
    
    if (is_valid_mail || is_valid_phone) {
        if (NativeBridge::networkAvaiable()) {
//            mj::MJAPISendMail::call(text_mail, text_phone);
//            this->onSkip(nullptr);
            MJWaitingHUD::show(this);
            MS_LOGGER.logEventOnboardInfoSubmit();
            mj::MJAPISendMail::call(text_mail, text_phone, [=](bool is_succes, std::string msg){
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
                    MJWaitingHUD::hide();
                });
                if (is_succes) {
                    if (textField_mail) textField_mail->setVisible(false);
                    if (textField_phone) textField_phone->setVisible(false);
                    
                    MJDEFAULT->setStringForKey(key_user_mail, text_mail);
                    MJDEFAULT->setStringForKey(key_user_phone, text_phone);
                    
                    //goi them api verify user
                    mj::MJAPIVerifyViral::call(text_phone, [=](bool is_success, std::string msg){
                        if(is_success){
                            mj::RefreshAPI::call([=](bool success){
                                MJDEFAULT->setBoolForKey("key.is.refer", true);
                                this->onSkip(nullptr);
                            });
                        }else{
                            this->onSkip(nullptr);
                        }
                    });
                    
                   
                }else{
                    std::string message;
                    if (msg != ""){
                        message = msg;
                    }else {
                        static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
                        static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
                        message = is_vn?msg_errorConnect_vn:msg_errorConnect;
                    }
                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, message] {
                        MJLicenseAlert::showAlert(0, message, "", is_vn?"Đóng":"Close");
                    });
                }
            });
        }else {
            static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
            static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                MJLicenseAlert::showAlert(0, is_vn?msg_errorConnect_vn:msg_errorConnect, "", is_vn?"Đóng":"Close");
            });
        }
        
        return;
    }
    
    if(text_mail.length() == 0 || (!mj::helper::isValidEmail(text_mail))) {;
        MJLicenseAlert::showAlert(0, is_vn?"Vui lòng nhập đúng định dạng cho email":"Please enter a valid email address", "", is_vn?"Đóng":"Close", nullptr);
        return;
    }
    
    if (text_phone.length() == 0 || (!mj::helper::isValidPhoneNumber(text_phone))) {
        MJLicenseAlert::showAlert(0, is_vn?"Vui lòng nhập đúng định dạng cho số điện thoại":"Please enter a valid phone number", "", is_vn?"Đóng":"Close", nullptr);
        return;
    }
}

#include "StoryAlert.h"
void MJMailIntro::onSendVN(Ref* sender){
    std::string text_phone, text_name, text_mail;
    text_phone = textField_phone->getString();
    text_name = textField_name->getString();
    text_mail = textField_mail->getString();
    
    while (!text_name.empty() && text_name.front() == ' ') {
        text_name.erase(text_name.begin());
    }
    
    auto is_valid_name = (!text_name.empty());
    auto is_valid_mail = mj::helper::isValidEmail(text_mail) || text_mail.empty();
    auto is_valid_phone = (!text_phone.empty()) && (mj::helper::isValidPhoneNumber(text_phone));
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    if (is_valid_name && is_valid_phone) {
        auto box = utils::findChild(this, "box");
        box->runAction(Sequence::create(FadeOut::create(0.25), Hide::create(), nullptr));
        //accountkit::startVerificationWithPhoneNumber(text_phone);
        ms::EventLogger::getInstance().logEvent("vmonkey_start_verified_phone_number", {});
        std::call_once(account_kit_listener_flag, [=] {
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.accountkit.finished", [=](EventCustom * e) {
                auto phone = reinterpret_cast<std::string *>(e->getUserData());
                this->onAccountKitFinished(* phone);
            }), this);
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.accountkit.cancelled", [=](EventCustom * e) {
                this->onAccountKitFailed();
            }), this);
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.accountkit.failed", [=](EventCustom * e) {
                this->onAccountKitFailed();
            }), this);
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.accountkit.start.application", [=](EventCustom * e) {
                this->onSkip(nullptr);
            }), this);
        });

        return;
    }
#else
    if(is_valid_phone){
        auto mail = mj::helper::isValidEmail(text_mail)? text_mail : StringUtils::format("%s@mail.com", text_name.c_str());
            if (NativeBridge::networkAvaiable()) {
                MS_LOGGER.logEventOnboardInfoSubmit();
                mj::MJAPISendMail::call(mail, text_phone, [=](bool is_succes, std::string msg){
                    if (is_succes) {
                        MJDEFAULT->setStringForKey(key_user_phone, text_phone);
                        this->onSkip(nullptr);
                    }else {
                        static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
                        static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
                        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                            StoryAlert::showAlert( is_vn?msg_errorConnect_vn:msg_errorConnect, "OK", is_vn?"Đóng":"Close");
                        });
                    }
                });
            }else {
                static string msg_errorConnect = "You are not connected to the Internet, please check your Internet connection.";
                static string msg_errorConnect_vn = "Lỗi không kết nối được với máy chủ. Vui lòng kiểm tra đường truyền mạng của bạn.\nXin cảm ơn!";
                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                    StoryAlert::showAlert( is_vn?msg_errorConnect_vn:msg_errorConnect, "OK", is_vn?"Đóng":"Close");
                });
            }
    }
#endif
    if(text_name.length() == 0 ) {;
        StoryAlert::showAlert( is_vn?"Vui lòng nhập tên của bạn":"Please enter your name", "OK", is_vn?"Đóng":"Close");
        return;
    }

    if (text_phone.length() == 0 || (!mj::helper::isValidPhoneNumber(text_phone))) {
        StoryAlert::showAlert( is_vn?"Vui lòng nhập đúng định dạng cho số điện thoại":"Please enter a valid phone number", "OK", is_vn?"Đóng":"Close" );
        return;
    }
    
    if (!is_valid_mail) {
        StoryAlert::showAlert( is_vn?"Vui lòng nhập đúng định dạng cho email":"Please enter a valid email", "OK", is_vn?"Đóng":"Close");
        return;
    }
    
   
}

//#include "../accountkit/MSAccountKitFinishView.h"
void MJMailIntro::onAccountKitFinished(const std::string & phone) {
    auto reformated = mj::helper::replace_string(phone, "+84", "0");
    while (reformated.front() == '0') {
        reformated.erase(reformated.begin());
    }
    reformated = "0" + reformated;
    
    ms::EventLogger::getInstance().logEvent("vmonkey_finish_verified_phone_number", {});
    std::string text_phone, text_name, text_mail;
    text_phone = reformated;
    textField_phone->setString(reformated);
    text_name = textField_name->getString();
    text_mail = textField_mail->getString();
    
    if (text_phone.front() != '0') {
        text_phone = "0" + text_phone;
    }
    
    MJDEFAULT->setStringForKey("install.name", text_name);
    MJDEFAULT->setStringForKey("install.phone", text_phone);
    MJDEFAULT->setStringForKey("install.mail", text_mail);
    MJDEFAULT->setBoolForKey("install.verified", true);

//    auto finish_view = MSAccountKitFinishView::createView();
//    this->addChild(finish_view, 10000);
//    mj::api::RegisterInstall::call(text_name, text_phone, text_mail, 0, true, [finish_view](bool success, std::string voucher) {
//        if (success) {
//            MJDEFAULT->setStringForKey("key.install.voucher", voucher);
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([finish_view, voucher] {
//                finish_view->setVoucher(voucher);
//            });
//        } else {
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([finish_view, voucher] {
//                finish_view->onFailed(voucher);
//            });
//        }
//    });
}

void MJMailIntro::onAccountKitFailed() {
    ms::EventLogger::getInstance().logEvent("vmonkey_fail_verified_phone_number", {});
    auto box = utils::findChild(this, "box");
    box->runAction(Sequence::create(Show::create(), FadeIn::create(0.25), nullptr));
}

void MJMailIntro::sendLicense(std::string license){
}

void MJMailIntro::onConfirmGift(cocos2d::Ref *sender) {
    std::string text_phone, text_name, text_mail;
    text_phone = textField_phone->getString();
    if (textField_name) {
        text_name = textField_name->getString();
    } else {
        text_name = "";
    }
    text_mail = textField_mail->getString();
    
    if (text_phone.front() != '0') {
        text_phone = "0" + text_phone;
    }
    
    MJDEFAULT->setStringForKey("install.name", text_name);
    MJDEFAULT->setStringForKey("install.phone", text_phone);
    MJDEFAULT->setStringForKey("install.mail", text_mail);
    MJDEFAULT->setBoolForKey("install.verified", false);
    
    auto is_valid_name = (!text_name.empty());
    auto is_valid_mail = mj::helper::isValidEmail(text_mail) || text_mail.empty();
    auto is_valid_phone = (!text_phone.empty()) && (mj::helper::isValidPhoneNumber(text_phone));
    
    if (is_valid_name && is_valid_phone) {
        mj::api::RegisterInstall::confirmGift(text_name, text_phone, text_mail, [](bool success, std::string voucher) {
        });
        
        this->runAction(Sequence::create(FadeOut::create(0.25), RemoveSelf::create(), nullptr));
        
        if (is_popup) {
            if (_callback) {
                _callback();
            }
            return;
        }
    } else {
        if(!is_valid_name) {;
            StoryAlert::showAlert( is_vn?"Vui lòng nhập tên của bạn":"Please enter your name", "OK", is_vn?"Đóng":"Close");
            return;
        }
        
        if (!is_valid_phone) {
            StoryAlert::showAlert( is_vn?"Vui lòng nhập đúng định dạng cho số điện thoại":"Please enter a valid phone number", "OK", is_vn?"Đóng":"Close" );
            return;
        }
        
        if (!is_valid_mail) {
            StoryAlert::showAlert( is_vn?"Vui lòng nhập đúng định dạng cho email":"Please enter a valid email", "OK", is_vn?"Đóng":"Close");
            return;
        }
    }
}

ui::Widget::ccWidgetClickCallback MJMailIntro::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJMailIntro, onClose),
        CLICK_MAP(MJMailIntro, onSkip),
        CLICK_MAP(MJMailIntro, onSend),
        CLICK_MAP(MJMailIntro, onSendVN),
        CLICK_MAP(MJMailIntro, onConfirmGift)
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}
