//
//  APEmailOTP.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APEmailOTP.h"
//#include "StoryAlert.h"
//#include "StoryLanguageManager.h"
#include "GameData.h"
#include "APDatabase.h"
#include "APProfileAgeView.h"
#include "GlobalFunc.h"
#include "MSEventLogger.h"
#include "APVerifyOptForEmailAPI.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "APActiveLicenseAPI.h"
#include "APWaitView.h"
#include "APLoginAPI.h"
#include "LicenseInfoModel.h"
#include "new/APLoginPopup.h"
#include "new/APRegisterPopup.h"
#include "APAuth.h"

#define TIME_COUNTDOWN 60
USING_NS_CC;

INITIALIZE_READER(APEmailOTP);

std::once_flag ap_email_otp_reader_flag;

APEmailOTP * APEmailOTP::createView(const std::string & i_inputText, const OTP_TYPE i_type,const int i_reponseCode)
{
    std::call_once(ap_email_otp_reader_flag, [] {
        REGISTER_CSB_READER(APEmailOTP);
    });
    
    auto node = reinterpret_cast<APEmailOTP *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountEmailOTP.csb"));
    if (node) {
        node->didLoadFromCSB(i_inputText, i_type, i_reponseCode);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "MJHelper.h"
void APEmailOTP::didLoadFromCSB(const std::string & inputText, const OTP_TYPE i_type, const int i_reponseCode)
{
	m_responeCode	= i_reponseCode;
	m_OTPType		= i_type;

	is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	btn_resend = utils::findChild<ui::Button *>(this, "btn_resend");
	btn_send = utils::findChild<ui::Button *>(this, "btn_next");
    textfield = utils::findChild<ui::TextField *>(this, "textfield");
	
    //textfield->setCursorEnabled(true);
    //textfield->setCursorChar('|');
	textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
    textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
    textfield->setMaxLengthEnabled(true);
    textfield->setMaxLength(4);
    //textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	textfield->addEventListener(CC_CALLBACK_2(APEmailOTP::onTextfieldChanged, this));
	textfield->setOpacity(0);
	m_inputText = inputText;

	auto lb_email = utils::findChild<ui::Text *>(this, "mail");
	lb_email->setString(m_inputText);
	
	index_cusor = 0;
	count_time_cusor = 0;
	text_numbers.clear();
	for (int i = 1; i <= 4; i++) {
		auto s = "Image_" + std::to_string(i);
		auto image = utils::findChild<ui::ImageView *>(this, s);
		text_numbers.push_back(image->getChildByName<Text*>("Text"));
	}
	_timer = TIME_COUNTDOWN;

	textResend = utils::findChild<ui::Text *>(this, "text_resend");

	changeDisplayLanguage(true);
}


void APEmailOTP::onEnter() {

    ui::Layout::onEnter();
	//ms::EventLogger::getInstance().logEvent("view_profile_name", {});
	// an exit
	auto close = utils::findChild<ui::ImageView*>(this, "close");
	is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	changeDisplayLanguage(is_vn);
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountEmailOTP.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
	this->schedule(schedule_selector(APEmailOTP::countDown),0.2f);
	this->schedule(schedule_selector(APEmailOTP::updateCusor));
	ms::EventLogger::getInstance().logEvent("open_verify_otp_3nd", {
			{"open_verify_otp", cocos2d::Value("email")},
		});
}

void APEmailOTP::onExit() {
	//ms::EventLogger::getInstance().logEvent("profile_create_exit", {});
    ui::Layout::onExit();
}

void APEmailOTP::changeDisplayLanguage(bool isVN)
{
	utils::findChild<Text*>(this, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(StringUtils::format("ap_%s_verification_code", m_OTPType == OTP_TYPE::EMAIL ? "email" : "phone")));
	utils::findChild<Text*>(this, "text_next")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_title"));
	utils::findChild<Text*>(this, "text_resend")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_resend_code_button_title2"));
	utils::findChild<Text*>(this, "warning_label")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_otp"));
}

void APEmailOTP::onNext(Ref* sender) 
{
	//ms::EventLogger::getInstance().logEvent("new_profile_created", {});
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	auto otp = textfield->getString();
	if (otp.size() != 4) {
		std::string msg;
		if (!is_vn) {
			msg = "Please enter a valid OTP";
		}
		else {
			msg = "Nhập sai định dạng";
		}
		utils::findChild<ImageView*>(this, "icon_warning")->setVisible(true);
		utils::findChild<Text*>(this, "warning_label")->setString(msg);
		return;
	}
	auto wait_view = APWaitView::createView(std::string(""), false, true);
	wait_view->setName("waiting_view");
	this->addChild(wait_view);
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);

	bool is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();

	ap::api::CommomVerifyOTP::call(otp, (ap::api::CommomVerifyOTP::VERIFY_TYPE) m_OTPType, m_inputText,"", [=](bool is_success, std::string msg,int code)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,is_success,msg,code,button]
		{
			if (is_success) 
			{
				ms::EventLogger::getInstance().logEvent("success_verify_otp_3nd", {
				{"verify_otp", cocos2d::Value("email")},
				});

				if (code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_ENTERING_PASSWORD_POPUP) //307
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});

						auto popup = APLoginPopup::createView((APLoginPopup::LOGIN_TYPE) m_OTPType, m_inputText);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else if (code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP) //306
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});

						auto popup = APRegisterPopup::createView((APRegisterPopup::REGISTER_TYPE) m_OTPType, m_inputText);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else 
				{
					CCLOG("somthing wrong!");
				}
			}
			else
			{
				ms::EventLogger::getInstance().logEvent("fail_verify_otp_3nd", {
								{"verify_otp", cocos2d::Value("email")},
					});
				this->removeChildByName("waiting_view");
				button->setTouchEnabled(true);
				showErrorAlert(msg);
			}

		});
	});
}


void APEmailOTP::onClose(cocos2d::Ref * sender)
{
//	utils::findChild<ui::TextField*>(this, "textfield")->setDetachWithIME(true);
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
    ime->closeIME();
	this->removeFromParent();
    ms::EventLogger::getInstance().logEvent("click_exit_email_otp_login", {});
	cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");

}
void APEmailOTP::insertOTP() {
	auto text = textfield->getString();
	for (int i = 0; i < text.size(); i++) {
		std::string s(1, text[i]);
		text_numbers[i]->setString(s);
	}
	for (int i = text.size(); i < 4; i++) {
		text_numbers[i]->setString("");
	}
}
void APEmailOTP::countDown(float delta) {
	if (textfield->getString().size() < 4) {
		btn_send->setTouchEnabled(false);
		btn_send->setEnabled(false);
	}
	else {
		if (!btn_send->isEnabled()) {
			UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
			ime->closeIME();
			btn_send->setTouchEnabled(true);
			btn_send->setEnabled(true);
		}
	}
	if (_timer > 0) {
		_timer -= delta;
		std::string s;
		if (is_vn) {
			s = "Gửi lại (" + std::to_string((int)_timer) + ")";
		}
		else {
			s = "Resend (" + std::to_string((int)_timer) + ")";
		}
		textResend->setString(s);
		
	}
	else {
	
		textResend->setString(is_vn?"Gửi lại":"Resend");
		btn_resend->setTouchEnabled(true);
		btn_resend->setEnabled(true);
	}
}
void APEmailOTP::updateCusor(float dt) {
	auto size = textfield->getString().size();
	index_cusor = size;
	for (int i = index_cusor+1; i < 3; i++) {
		text_numbers[i]->setString("");
	}
	count_time_cusor += dt;
	if (count_time_cusor >= 0.3f){
		count_time_cusor = 0;
		check += 1;
		if (index_cusor < 4 && index_cusor >= 0) {
			if (check % 2) {
				text_numbers[index_cusor]->setString("|");
			}
			else {
				text_numbers[index_cusor]->setString("");
			}
		}
	}
}

void APEmailOTP::callAPI()
{

}

void APEmailOTP::onResend(cocos2d::Ref * sender)
{
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	Button* btn = (Button*)sender;
	btn->setEnabled(false);
	btn->setTouchEnabled(false);
	_timer = TIME_COUNTDOWN;

	const std::string k_email = (m_OTPType == OTP_TYPE::EMAIL ? m_inputText : "");

	const std::string k_phoneNumber = (m_OTPType == OTP_TYPE::EMAIL ? "" : m_inputText);

	ap::api::Login::call(m_OTPType == OTP_TYPE::EMAIL ? ap::api::Login::LoginType::GOOGLE: ap::api::Login::LoginType::PHONE, "", k_phoneNumber, k_email, "", "","", [=](int error_code, bool is_success, std::string msg)
	{
		btn->setTouchEnabled(true);

		if (is_success) 
		{

		}
		else 
		{
			if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_OTP_POPUP) //302
			{
				// verify qua otp
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, error_code] {
					MJDEFAULT->setBoolForKey("account_unverified", false);
					this->removeChildByName("waiting_view_email");
					utils::findChild(this, "bg_ngang")->setOpacity(0);

					auto email_otp = APEmailOTP::createView(m_inputText, m_OTPType, m_responeCode);
					this->getParent()->addChild(email_otp,POPUP_ZORDER);

					this->removeFromParent();

				});
			}
			else if (m_OTPType == OTP_TYPE::EMAIL && error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_GOOGLE_LOGIN_POPUP) //303
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
			else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_ENTERING_PASSWORD_POPUP) //307
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
					ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
						{"open_3nd_type", cocos2d::Value("email")},
						{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
						});

					auto popup = APLoginPopup::createView((APLoginPopup::LOGIN_TYPE) m_OTPType, m_inputText);
					this->getParent()->addChild(popup);
					this->removeFromParent();
				});
			}
			else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP) //306
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
					ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
						{"open_3nd_type", cocos2d::Value("email")},
						{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
						});

					auto popup = APRegisterPopup::createView((APRegisterPopup::REGISTER_TYPE)m_OTPType, m_inputText);
					this->getParent()->addChild(popup);
					this->removeFromParent();
				});
			}
			else 
			{
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] 
				{
					this->removeChildByName("waiting_view_email");
				});
				showErrorAlert(msg);
				CCLOG("error: %s", msg.c_str());
			}
		}
	});

}
void APEmailOTP::showErrorAlert(std::string msg) {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,msg] {
		textfield->setString("");
		for (auto t : text_numbers) {
			t->setString("");
		}
		if (msg == "") {
             StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                    
        }
        else {
             StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4)? "Đóng" : "Close");
        }
	});
}

void APEmailOTP::onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
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
		auto utf8 = StringUtils::StringUTF8(textfield->getString());
		if (utf8.length() >= 1) {
			auto new_char = utf8.getString().at(utf8.length() - 1)._char;
			if (new_char == " " || (new_char != "+" && !mj::helper::isValidNumber(new_char))) {
				utf8.deleteChar(utf8.length() - 1);
				textfield->setString(utf8.getAsCharSequence());
			}
			
		}
		this->insertOTP();
	}
	case TextField::EventType::DELETE_BACKWARD:
	{
		this->insertOTP();
	}
	break;
	default:
		break;
	}

}

ui::Widget::ccWidgetClickCallback APEmailOTP::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APEmailOTP, onNext),
		CLICK_MAP(APEmailOTP, onClose),
		CLICK_MAP(APEmailOTP, onResend)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

