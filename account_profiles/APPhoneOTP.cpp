//
//  APPhoneOTP.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APPhoneOTP.h"
//#include "StoryAlert.h"
//#include "StoryLanguageManager.h"
#include "GameData.h"
#include "APDatabase.h"
#include "APProfileAgeView.h"
#include "GlobalFunc.h"
#include "MSEventLogger.h"
#include "APSendOptAPI.h"
#include "APVerifyOptAPI.h"
#include "APPhoneLogin.h"
#include "APActiveLicenseAPI.h"
#include "APWaitView.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#define TIME_COUNTDOWN 60
USING_NS_CC;

INITIALIZE_READER(APPhoneOTP);

std::once_flag ap_phone_otp_reader_flag;

APPhoneOTP * APPhoneOTP::createView(const std::string & phone_number) {
    std::call_once(ap_phone_otp_reader_flag, [] {
        REGISTER_CSB_READER(APPhoneOTP);
    });
    
    auto node = reinterpret_cast<APPhoneOTP *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountPhoneOTP.csb"));
    if (node) {
        node->didLoadFromCSB(phone_number);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "MJHelper.h"
void APPhoneOTP::didLoadFromCSB(const std::string & phone_number) {
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
	textfield->addEventListener(CC_CALLBACK_2(APPhoneOTP::onTextfieldChanged, this));
	textfield->setOpacity(0);
	_phone_number = phone_number;
	auto lb_phone_number = utils::findChild<ui::Text *>(this, "phone_number");
	lb_phone_number->setString(_phone_number);
	
	index_cusor = 0;
	count_time_cusor = 0;
	text_numbers.clear();
	for (int i = 1; i <= 4; i++) {
		auto s = "Image_" + std::to_string(i);
		auto image = utils::findChild<ui::ImageView *>(this, s);
		text_numbers.push_back(image->getChildByName<Text*>("Text"));
	}
	_timer = TIME_COUNTDOWN;
	textResend = utils::findChild<ui::Text*>(this, "text_resend");
	std::string s;
	if (is_vn) {
		s = "Gửi lại (" + std::to_string((int)_timer) + ")";
	}
	else {
		s = "Resend (" + std::to_string((int)_timer) + ")";
	}
	textResend->setString(s);

}


void APPhoneOTP::onEnter() {
    ui::Layout::onEnter();
	//ms::EventLogger::getInstance().logEvent("view_profile_name", {});
	// an exit
	auto close = utils::findChild<ui::ImageView*>(this, "close");
	is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	changeDisplayLanguage((LICENSE_INFO_MODEL->isDisplayLanguageVN()));
	auto csb_name = /*mj::helper::getCSBName*/("csb/account/AccountPhoneOTP.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
	this->schedule(schedule_selector(APPhoneOTP::countDown),0.2f);
	this->schedule(schedule_selector(APPhoneOTP::updateCusor));
}

void APPhoneOTP::onExit() {
	//ms::EventLogger::getInstance().logEvent("profile_create_exit", {});
    ui::Layout::onExit();
}

void APPhoneOTP::changeDisplayLanguage(bool isVN){
	if (!isVN) {
		utils::findChild<Text*>(this, "title")->setString("Please enter the verification code sent to your phone");
		utils::findChild<Text*>(this, "text_next")->setString("Send");
		utils::findChild<Text*>(this, "text_resend")->setString("Resend");
	}
    
}

void APPhoneOTP::onNext(Ref* sender) {

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
	Director::getInstance()->getRunningScene()->addChild(wait_view);
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);

	bool is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	ap::api::VerifyOpt::call(otp, [=](bool is_success, std::string msg, std::string condition) {
		if (is_success) {
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg, condition] {
				Director::getInstance()->getRunningScene()->removeChildByName("waiting_view");
				CCLOG("%s", msg.c_str());
				auto parent = (APPhoneLogin*)this->getParent();
				parent->onVerifyOTPSuccess();
				//this->removeFromParent();
			});
		}
		else
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
				Director::getInstance()->getRunningScene()->removeChildByName("waiting_view");
				});
			button->setTouchEnabled(true);
			showErrorAlert(msg);
		}
	});

}


void APPhoneOTP::onClose(cocos2d::Ref * sender)
{
//	utils::findChild<ui::TextField*>(this, "textfield")->setDetachWithIME(true);
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
    ime->closeIME();
	this->removeFromParent();
}
void APPhoneOTP::insertOTP() {
	auto text = textfield->getString();
	for (int i = 0; i < text.size(); i++) {
		std::string s(1, text[i]);
		text_numbers[i]->setString(s);
	}
	for (int i = text.size(); i < 4; i++) {
		text_numbers[i]->setString("");
	}
}
void APPhoneOTP::countDown(float delta) {
	if (textfield->getString().size() < 4) {
		btn_send->setTouchEnabled(false);
		btn_send->setEnabled(false);
	}
	else {
		btn_send->setTouchEnabled(true);
		btn_send->setEnabled(true);
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
void APPhoneOTP::updateCusor(float dt) {
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
void APPhoneOTP::onResend(cocos2d::Ref * sender)
{
	//	utils::findChild<ui::TextField*>(this, "textfield")->setDetachWithIME(true);
	Button* btn = (Button*)sender;
	btn->setEnabled(false);
	btn->setTouchEnabled(false);
	_timer = TIME_COUNTDOWN;
	ap::api::SendOpt::call(_phone_number, [=](bool is_success, std::string msg) {
		btn->setTouchEnabled(true);
		if (is_success) {
			
		}
		else {
			showErrorAlert(msg);
			CCLOG("error: %s", msg.c_str());
		}
		});
}
void APPhoneOTP::showErrorAlert(std::string msg) {
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

void APPhoneOTP::onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type)
{

	auto box = utils::findChild(this, "bg_ngang");
	switch (type)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//|| (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
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

ui::Widget::ccWidgetClickCallback APPhoneOTP::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APPhoneOTP, onNext),
		CLICK_MAP(APPhoneOTP, onClose),
		CLICK_MAP(APPhoneOTP, onResend)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

