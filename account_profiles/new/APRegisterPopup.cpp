#include "APRegisterPopup.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "APPopup.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "MJPlatformConfig.h"
#include "APLoginAPI.h"

USING_NS_CC;
INITIALIZE_READER(APRegisterPopup);

std::once_flag ap_register_popup_flag;

APRegisterPopup * APRegisterPopup::createView(REGISTER_TYPE i_type,std::string i_mailOrPhoneNumberText)
{
	std::call_once(ap_register_popup_flag, [] {
		REGISTER_CSB_READER(APRegisterPopup);
		});

	auto p = static_cast<APRegisterPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APRegisterPopup.csb"));

	if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneNumberText))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APRegisterPopup::didLoadFromCSB(REGISTER_TYPE i_type, std::string i_mailOrPhoneNumberText)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_mailOrPhoneText = i_mailOrPhoneNumberText;

	m_registerType = i_type;

	showingAnimation(utils::findChild(this, "popup_bg"), [=]()
	{


	});

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "input_textfield"))
	{
		inputTextfield->setEnabled(false);
		inputTextfield->setString(i_mailOrPhoneNumberText);
	}

	m_passwordTextfields =
	{
		 cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield"),
		 cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield"),
	};

	enableAButtonBasedOnTextfieldsLength(cocos2d::utils::findChild<ui::Button*>(this, "register_button"), m_passwordTextfields, cocos2d::utils::findChild<ui::Widget*>(this, "warning_label"), PASSWORD_MIN_LENGTH);

	for (auto inputTextfield : m_passwordTextfields)
	{
		if (inputTextfield)
		{
			inputTextfield->setPlaceHolderColor(Color4B(155, 155, 155, 255));
			inputTextfield->setCursorChar('|');
			inputTextfield->setCursorEnabled(true);
			inputTextfield->setMaxLengthEnabled(true);
			inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
		}
	}

	m_onCancelCallBack = [=]()
	{
		
	};

	return true;
}

void APRegisterPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_title"));
	}

	if (auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield"))
	{
		passwordTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_password"));
	}

	if (auto reenteringPasswordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield"))
	{
		reenteringPasswordTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_reenter_password"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "register_button_title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"));
	}

	if (auto waringLabel = cocos2d::utils::findChild<ui::Text*>(this, "warning_label"))
	{
		waringLabel->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_password"));
	}
}

void APRegisterPopup::onFinish(Ref * sender)
{
	hideVirtualKeyboard(m_passwordTextfields);

	auto registerButton = cocos2d::utils::findChild<ui::Button*>(this, "register_button");

	registerButton->setTouchEnabled(false);

	auto reenteringPasswordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield");

	auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield");

	if (passwordTextfield->getString().empty() || reenteringPasswordTextfield->getString().empty())
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password"));

		registerButton->setTouchEnabled(true);

		return;
	}

	if (!passwordTextfield->getString().empty() && !reenteringPasswordTextfield->getString().empty() && passwordTextfield->getString() != reenteringPasswordTextfield->getString())
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_match_password"));

		registerButton->setTouchEnabled(true);

		return;
	}

	showWaittingView();

	const std::string k_emailText = (m_registerType == REGISTER_TYPE::EMAIL ? m_mailOrPhoneText : "");

	const std::string k_phoneText = (m_registerType == REGISTER_TYPE::EMAIL ? "" : m_mailOrPhoneText);

	ap::api::Login::call(m_registerType == REGISTER_TYPE::EMAIL ? ap::api::Login::GOOGLE: ap::api::Login::PHONE, "", k_phoneText, k_emailText, "", "", "", passwordTextfield->getString(), [=](int error_code, bool is_success2, std::string msg2)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success2, msg2, error_code, registerButton] {

			registerButton->setTouchEnabled(true);

			removeWaittingView();

			if (is_success2)
			{
				MJDEFAULT->setBoolForKey("account_unverified", false);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			}
			else
			{
				this->removeChildByName("waiting_view");
				//button->setTouchEnabled(true);
				showFailedPopup(msg2);
			}
		});

	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APRegisterPopup::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APRegisterPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APRegisterPopup, onFinish),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}


