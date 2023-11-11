//
//  APPhoneLogin.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APPhoneLogin.h"
//#include "StoryAlert.h"
//#include "StoryLanguageManager.h"
#include "GameData.h"
#include "APDatabase.h"
#include "APProfileAgeView.h"
#include "GlobalFunc.h"
#include "MSEventLogger.h"
#include "APPhoneOTP.h"
#include "APAuth.h"
#include "APLoginAPI.h"
#include "APSendOptAPI.h"
#include "MSIntroLicense.h"
#include "APWaitView.h"
#include "MSEventLogger.h"
#include "MJDefault.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "StoryParentLicense_Landscape.h"
USING_NS_CC;

INITIALIZE_READER(APPhoneLogin);

std::once_flag ap_phone_login_reader_flag;

APPhoneLogin * APPhoneLogin::createView(const bool & is_confirm_otp) {
    std::call_once(ap_phone_login_reader_flag, [] {
        REGISTER_CSB_READER(APPhoneLogin);
    });
    
    auto node = reinterpret_cast<APPhoneLogin *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountPhoneLogin.csb"));
    if (node) {
        node->didLoadFromCSB(is_confirm_otp);
		
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "MJHelper.h"
void APPhoneLogin::didLoadFromCSB(const bool & is_confirm_otp){
	_is_confirm_otp = is_confirm_otp;
	btn_send = utils::findChild<ui::Button *>(this, "btn_next");
    auto textfield = utils::findChild<ui::TextField *>(this, "textfield");
    textfield->setCursorEnabled(true);
    textfield->setCursorChar('|');
   /* textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
    textfield->setTextVerticalAlignment(TextVAlignment::CENTER);*/
    textfield->setMaxLengthEnabled(true);
    textfield->setMaxLength(12);
    textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	txt_phone = textfield;
	txt_phone->addEventListener(CC_CALLBACK_2(APPhoneLogin::onTextfieldChanged, this));
	if (is_confirm_otp) {
		txt_phone->setString(ap::Database::getInstance().getUserInfo()._phone);
	}
}


void APPhoneLogin::onEnter() {

    ui::Layout::onEnter();
	//ms::EventLogger::getInstance().logEvent("view_profile_name", {});
	// an exit
	auto close = utils::findChild<ui::ImageView*>(this, "close");
	if (_is_confirm_otp) utils::findChild<Text*>(this, "title")->setString("Tài khoản của bạn chưa được xác thực, vui lòng nhập SĐT để xác thực thông tin");
	changeDisplayLanguage((LICENSE_INFO_MODEL->isDisplayLanguageVN()));
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountPhoneLogin.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
	this->schedule(schedule_selector(APPhoneLogin::update), 0.2f);
	if (!_is_confirm_otp) {
		ms::EventLogger::getInstance().logEvent("popup_after_click_login_show", {
			{"show_type", cocos2d::Value("phone")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
			});
	}
}

void APPhoneLogin::onExit() {
	//ms::EventLogger::getInstance().logEvent("profile_create_exit", {});
    ui::Layout::onExit();
}

void APPhoneLogin::changeDisplayLanguage(bool isVN){
	if (!isVN) {
		if (!_is_confirm_otp) {
			utils::findChild<Text*>(this, "title")->setString("Please enter your phone number to verify");
		}
		else {
			utils::findChild<Text*>(this, "title")->setString("Your account is not verified yet, please enter your phone number to verify");
		}
		utils::findChild<Text*>(this, "text_next")->setString("Send");
	}
    
}

void APPhoneLogin::onNext(Ref* sender) {
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	//ms::EventLogger::getInstance().logEvent("new_profile_created", {});
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);
    bool is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();

	// Validate phone
	auto text_phone = utils::findChild<ui::TextField*>(this, "textfield")->getString();
	auto is_valid_phone = (!text_phone.empty()) && (mj::helper::isValidPhoneNumber(text_phone));
	auto msg = "";
	if (text_phone.length() == 0 || (text_phone.length() > 12)) {
		if (!is_vn) {
			msg = "Please enter a valid phone number";
		}
		else {
			if (!text_phone.length() == 0)
				msg = "Sai định dạng số điện thoại";
				//warning_phone->getChildByName<ui::Text*>("warning_label")->setString(StringUtils::format("Sai định dạng số điện thoại"));
			else
				msg = "Vui lòng nhập số điện thoại";
				//warning_phone->getChildByName<ui::Text*>("warning_label")->setString(StringUtils::format("Vui lòng nhập số điện thoại"));
		}
		utils::findChild<ImageView*>(this, "icon_warning")->setVisible(true);
		utils::findChild<Text*>(this, "warning_label")->setString(msg);
		//MMAlert::show({ "",msg ,"","ok" });
		button->setTouchEnabled(true);
		txt_phone->setString("");
		//storyalert::showalert(is_vn ? "vui lòng nhập đúng định dạng cho số điện thoại" : "please enter a valid phone number", "ok", is_vn ? "đóng" : "close");
		//is_valid_ok = false;
		return;
	}
	else {
		//warning_phone->setVisible(false);
	}
	auto wait_view = APWaitView::createView(std::string(""), false, true);
	wait_view->setName("waiting_view_phone");
	Director::getInstance()->getRunningScene()->addChild(wait_view);
	utils::findChild<ImageView*>(this, "icon_warning")->setVisible(false);
	if (!_is_confirm_otp) {
		ms::EventLogger::getInstance().logEvent("popup_after_click_login_continue", {
				{"continue_type", cocos2d::Value("phone")},
				{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
			});
		ap::api::Login::call(ap::api::Login::LoginType::PHONE, text_phone, text_phone, "", "", "","", [=](int error_code, bool is_success, std::string msg) {
			button->setTouchEnabled(true);
			if (is_success) {
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
					Director::getInstance()->getRunningScene()->removeChildByName("waiting_view_phone");
					MJDEFAULT->setBoolForKey("account_unverified", true);
					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
					});
			}
			else {
				if (error_code == 302) {
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,text_phone] {
						MJDEFAULT->setBoolForKey("account_unverified", false);
						Director::getInstance()->getRunningScene()->removeChildByName("waiting_view_phone");
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("phone")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});
						ap::Auth::authenticateWithPhoneNumber(text_phone);
						cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_completed_login_browser");
						});
				}
				else {
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						Director::getInstance()->getRunningScene()->removeChildByName("waiting_view_phone");
						});
					showErrorAlert(msg);
					CCLOG("error: %s", msg.c_str());
				}
			}
			});
	}
	else {
		ap::api::SendOpt::call(text_phone, [=](bool is_success, std::string msg) {
			button->setTouchEnabled(true);
			if (is_success) {
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,text_phone] {
					Director::getInstance()->getRunningScene()->removeChildByName("waiting_view_phone");
					auto otp = APPhoneOTP::createView(text_phone);
					this->addChild(otp);
				});
			}
			else {
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
					Director::getInstance()->getRunningScene()->removeChildByName("waiting_view_phone");
				});
				showErrorAlert(msg);
				CCLOG("error: %s", msg.c_str());
			}
		});
	}
	
	
	
}
void APPhoneLogin::showErrorAlert(std::string msg) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,msg] {
		 if (msg == "") {
                   StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                          
              }
              else {
                   StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4)? "Đóng" : "Close");
              }
	});
}


void APPhoneLogin::onClose(cocos2d::Ref * sender)
{
//	utils::findChild<ui::TextField*>(this, "textfield")->setDetachWithIME(true);
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
    ime->closeIME();
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountPhoneLogin.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([=] {
		this->removeFromParent();
	});
	if (!_is_confirm_otp) {
		ms::EventLogger::getInstance().logEvent("popup_after_click_login_cancel", {
			{"cancel_type", cocos2d::Value("phone")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
			});
		ms::EventLogger::getInstance().logEvent("click_exit_phone_login", {});
		cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
			cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
		});
	}
}
void APPhoneLogin::onVerifyOTPSuccess() {
	if (is_onboard) {
		auto parent = (MSIntroLicense*)this->getParent();
		parent->is_unverified_account = true;
		parent->onSend(NULL);
	}
	else {
		auto parent = (StoryParentLicense_Landscape*)this->getParent();
		parent->onOK(NULL);
	}
	this->removeFromParent();
}
void APPhoneLogin::update(float delta) {
	if (txt_phone->getString().size() == 0) {
		btn_send->setTouchEnabled(false);
		btn_send->setEnabled(false);
	}
	else {
		btn_send->setTouchEnabled(true);
		btn_send->setEnabled(true);
	}
	
}
void APPhoneLogin::onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
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
		auto utf8 = StringUtils::StringUTF8(txt_phone->getString());
		if (utf8.length() >= 1) {
			auto new_char = utf8.getString().at(utf8.length() - 1)._char;
			if (new_char == " " || (new_char != "+" && !mj::helper::isValidNumber(new_char))) {
				utf8.deleteChar(utf8.length() - 1);
				txt_phone->setString(utf8.getAsCharSequence());
			}
		}
	}
	break;
	default:
		break;
	}

}

ui::Widget::ccWidgetClickCallback APPhoneLogin::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APPhoneLogin, onNext),
		CLICK_MAP(APPhoneLogin, onClose)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

