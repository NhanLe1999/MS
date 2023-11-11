#include "APChangePasswordPopup.h"
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
#include "APPasswordAPI.h"
USING_NS_CC;
INITIALIZE_READER(APChangePasswordPopup);

std::once_flag ap_change_password_popup_flag;

APChangePasswordPopup * APChangePasswordPopup::createView(CHANGE_TYPE i_type,std::string i_mailOrPhoneNumberText)
{
	std::call_once(ap_change_password_popup_flag, [] 
	{
		REGISTER_CSB_READER(APChangePasswordPopup);
	});

	auto p = static_cast<APChangePasswordPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APChangePasswordPopup.csb"));

	if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneNumberText))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APChangePasswordPopup::didLoadFromCSB(CHANGE_TYPE i_type, std::string i_mailOrPhoneNumberText)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_type = i_type;

	m_mailOrPhoneText = i_mailOrPhoneNumberText;

	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	
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

	enableAButtonBasedOnTextfieldsLength(cocos2d::utils::findChild<ui::Button*>(this, "finish_button"), m_passwordTextfields, cocos2d::utils::findChild<ui::Widget*>(this, "warning_label"), PASSWORD_MIN_LENGTH);

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

	return true;
}

void APChangePasswordPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_change_password_title"));
	}

	if (auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield"))
	{
		passwordTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_password"));
	}

	if (auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield"))
	{
		passwordTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_reenter_password"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "finish_button_title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_finish_button_title"));
	}

	if (auto waringLabel = cocos2d::utils::findChild<ui::Text*>(this, "warning_label"))
	{
		waringLabel->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_password"));
	}
	
}

void APChangePasswordPopup::onFinish(Ref * sender)
{
	hideVirtualKeyboard(m_passwordTextfields);

	auto finishButton = cocos2d::utils::findChild<ui::Button*>(this, "finish_button");

	finishButton->setTouchEnabled(false);

	auto reenteringPasswordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield");

	auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield");

	if (passwordTextfield->getString().empty() || reenteringPasswordTextfield->getString().empty())
	{
		finishButton->setTouchEnabled(true);

		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password"));

		return;
	}

	if (!passwordTextfield->getString().empty() && !reenteringPasswordTextfield->getString().empty() && passwordTextfield->getString() != reenteringPasswordTextfield->getString())
	{
		finishButton->setTouchEnabled(true);

		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_match_password"));

		return;
	}

	showWaittingView();

	const std::string k_emailText = (m_type == CHANGE_TYPE::MAIL ? m_mailOrPhoneText : "");

	const std::string k_phoneText = (m_type == CHANGE_TYPE::MAIL ? "" : m_mailOrPhoneText);

	ap::api::APPasswordAPI::changePasswordCall(k_emailText, k_phoneText, passwordTextfield->getString(), [=](bool i_success, std::string i_msg, const rapidjson::Value &i_data)
	{
		removeWaittingView();

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, finishButton, k_phoneText, k_emailText, passwordTextfield]
		{
			finishButton->setTouchEnabled(true);

			if (i_success) 
			{
				MJDEFAULT->setBoolForKey("account_unverified", false);

				ap::api::Login::call((k_phoneText == "") ? (ap::api::Login::GOOGLE) : (ap::api::Login::PHONE), "", k_phoneText, k_emailText, "", "", "", passwordTextfield->getString(), [=](int error_code, bool is_success, std::string msg) {
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg] 
					{
						if (is_success) 
						{
							cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
						}
						else 
						{
							showFailedPopup(msg);
						}
					});
				});
				//cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			}
			else 
			{
				showFailedPopup(i_msg);
			}
		});

	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APChangePasswordPopup::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APChangePasswordPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APChangePasswordPopup, onFinish),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}


