//
//  APEmailLogin.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APEmailLogin.h"
//#include "StoryAlert.h"
//#include "StoryLanguageManager.h"
#include "GameData.h"
#include "APDatabase.h"
#include "GlobalFunc.h"
#include "MSEventLogger.h"
#include "APAuth.h"
#include "APLoginAPI.h"
#include "MSIntroLicense.h"
#include "APWaitView.h"
#include "MSEventLogger.h"
#include "MJDefault.h"
#include "APEmailOTP.h"
#include "MJHelper.h"
#include "api/APLoginAPI.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "new/APRegisterPopup.h"
#include "new/APLoginPopup.h"

USING_NS_CC;

INITIALIZE_READER(APEmailLogin);

std::once_flag ap_email_login_reader_flag;

APEmailLogin * APEmailLogin::createView() {
    std::call_once(ap_email_login_reader_flag, [] {
        REGISTER_CSB_READER(APEmailLogin);
    });
    
    auto node = reinterpret_cast<APEmailLogin *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountEmailLogin.csb"));
    if (node) {
        node->didLoadFromCSB();
		
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "MJHelper.h"
void APEmailLogin::didLoadFromCSB(){
	btn_send = utils::findChild<ui::Button *>(this, "btn_next");
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setCursorEnabled(true);
    textfield->setCursorChar('|');
    textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	txt_phone = textfield;
	txt_phone->addEventListener(CC_CALLBACK_2(APEmailLogin::onTextfieldChanged, this));
}


void APEmailLogin::onEnter() {

    ui::Layout::onEnter();
	// an exit
	auto close = utils::findChild<ui::ImageView*>(this, "close");
	changeDisplayLanguage(LICENSE_INFO_MODEL->isDisplayLanguageVN());
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountEmailLogin.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
	this->schedule(schedule_selector(APEmailLogin::update), 0.2f);
	auto btn_google = utils::findChild<ui::Button*>(this, "btn_google");

#if !(defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0))
	if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
		btn_google->setVisible(false);
	}
#endif
	ms::EventLogger::getInstance().logEvent("popup_after_click_login_show", {
		{"show_type", cocos2d::Value("email")},
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});
}

void APEmailLogin::onExit() {
	//ms::EventLogger::getInstance().logEvent("profile_create_exit", {});
    ui::Layout::onExit();
}

void APEmailLogin::changeDisplayLanguage(bool isVN){
	if (!isVN) {
		utils::findChild<Text*>(this, "text_google")->setString("Login with Gmail!");
		utils::findChild<Text*>(this, "title")->setString("Please enter your email address that is \n regularly used to receive support and \n latest priority policy from Monkey Stories.");
		utils::findChild<Text*>(this, "text_next")->setString("Send");
	}
    
}

void APEmailLogin::onNext(Ref* sender) {
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);
	bool is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	auto warning_email = utils::findChild<ui::ImageView*>(this, "icon_warning");
	// Validate phone
	auto text_email = utils::findChild<ui::TextField*>(this, "textfield")->getString();
	auto is_valid_mail = mj::helper::isValidLoginEmail(text_email);
	if (!is_valid_mail) {
		warning_email->setVisible(true);
		
		//bg_ngang
		utils::findChild(this, "bg_ngang")->runAction(Repeat::create(Sequence::createWithTwoActions(MoveBy::create(0.1f, Vec2(+25, 0)), MoveBy::create(0.1f, Vec2(-25, 0))),2));
		
		cocos2d::Device::vibrate(1.0f);

		if (!is_vn) {
			warning_email->getChildByName<ui::Text*>("warning_label")->setString("please enter a valid email");
		}
		else {
			if (text_email != "") {
				warning_email->getChildByName<ui::Text*>("warning_label")->setString(StringUtils::format("Sai định dạng email"));
			}
			else {
				warning_email->getChildByName<ui::Text*>("warning_label")->setString(StringUtils::format("Vui lòng nhập email"));
			}
		}
		button->setTouchEnabled(true);
		return;
	}
	else {
		warning_email->setVisible(false);
	}
	button->setTouchEnabled(true);
	auto wait_view = APWaitView::createView(std::string(""), false, true);
	wait_view->setName("waiting_view_email");
	this->addChild(wait_view);
	ms::EventLogger::getInstance().logEvent("popup_after_click_login_continue", {
			{"continue_type", cocos2d::Value("email")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});

	ap::api::Login::call(ap::api::Login::LoginType::GOOGLE, "", "", text_email, "", "","","", [=](int error_code, bool is_success, std::string msg) {
		button->setTouchEnabled(true);

		if (is_success) 
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
				this->removeChildByName("waiting_view_email");
				MJDEFAULT->setBoolForKey("account_unverified", false);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			});
		}
		else 
		{
			if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_OTP_POPUP) 
			{
					// verify qua otp
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, text_email, error_code] {
					MJDEFAULT->setBoolForKey("account_unverified", false);
					this->removeChildByName("waiting_view_email");
					//utils::findChild(this, "bg_ngang")->setOpacity(0);

					auto email_otp = APEmailOTP::createView(text_email, APEmailOTP::OTP_TYPE::EMAIL, (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP);
					this->addChild(email_otp);
                    
				});
			}
			else 
			{
				if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_GOOGLE_LOGIN_POPUP) //303
				{
					// verify qua google
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});
						ap::Auth::authenticateWithGoogle("");
						this->removeFromParent();
						});
				}
				else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP) //306
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, text_email] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});
						
						auto popup = APRegisterPopup::createView(APRegisterPopup::REGISTER_TYPE::EMAIL, text_email);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_ENTERING_PASSWORD_POPUP)  //307
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, text_email] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});

						auto popup = APLoginPopup::createView(APLoginPopup::LOGIN_TYPE::EMAIL, text_email);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else //error handel
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] 
					{
						this->removeChildByName("waiting_view_email");
					});
					showErrorAlert(msg);
					CCLOG("error: %s", msg.c_str());
				}
			}
		}
	});
}
void APEmailLogin::showErrorAlert(std::string msg) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,msg] {
       if (msg == "") {
            StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                   
       }
       else {
            StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4)? "Đóng" : "Close");
       }
	});
}


void APEmailLogin::onClose(cocos2d::Ref * sender)
{
//	utils::findChild<ui::TextField*>(this, "textfield")->setDetachWithIME(true);
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
    ime->closeIME();
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountEmailLogin.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([=] {
		this->removeFromParent();
	});
	ms::EventLogger::getInstance().logEvent("popup_after_click_login_cancel", {
			{"cancel_type", cocos2d::Value("email")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});
	ms::EventLogger::getInstance().logEvent("click_exit_email_login", {});
	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
	});
}

void APEmailLogin::update(float delta) {
	if (txt_phone->getString().size() == 0) {
		btn_send->setTouchEnabled(false);
		btn_send->setEnabled(false);
	}
	else {
		btn_send->setTouchEnabled(true);
		btn_send->setEnabled(true);
	}
	
}
void APEmailLogin::onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
{

	auto box = utils::findChild(this, "bg_ngang");
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
		auto str_email = txt_phone->getString();
		GlobalFunc::trim(str_email, " ");
		txt_phone->setString(str_email);
		auto utf8 = StringUtils::StringUTF8(txt_phone->getString());
		if (utf8.length() >= 20) {
			if (utf8.length() >= 24) txt_phone->setFontSize(20);
			else txt_phone->setFontSize(25);
		}
		else {
			txt_phone->setFontSize(29);
		}
	}
	break;
	default:
		break;
	}

}
void APEmailLogin::onGoogle(cocos2d::Ref* sender) {
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	auto button = (Button*)sender;
	button->setTouchEnabled(false);
	ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
		{"open_3nd_type", cocos2d::Value("email")},
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});
	ap::Auth::authenticateWithGoogle("");
	this->removeFromParent();
}
ui::Widget::ccWidgetClickCallback APEmailLogin::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APEmailLogin, onNext),
		CLICK_MAP(APEmailLogin, onClose),
		CLICK_MAP(APEmailLogin, onGoogle)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

