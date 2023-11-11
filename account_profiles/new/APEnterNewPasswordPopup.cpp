#include "APEnterNewPasswordPopup.h"
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

USING_NS_CC;
INITIALIZE_READER(APEnterNewPasswordPopup);

std::once_flag ap_enter_new_password_popup_flag;

APEnterNewPasswordPopup * APEnterNewPasswordPopup::createView(RECOVERY_TYPE i_type,std::string i_mailOrPhoneNumberText)
{
	std::call_once(ap_enter_new_password_popup_flag, [] 
	{
		REGISTER_CSB_READER(APEnterNewPasswordPopup);
	});

	auto p = static_cast<APEnterNewPasswordPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APEnterNewPasswordPopup.csb"));

	if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneNumberText))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APEnterNewPasswordPopup::didLoadFromCSB(RECOVERY_TYPE i_type, std::string i_mailOrPhoneNumberText)
{
	m_type = i_type;

	m_mailOrPhoneNumberText = i_mailOrPhoneNumberText;

	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	});

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

void APEnterNewPasswordPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_new_password_title"));
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

void APEnterNewPasswordPopup::onFinish(Ref * sender)
{
	auto finishButton = cocos2d::utils::findChild<ui::Button*>(this, "finish_button");

	finishButton->setTouchEnabled(false);

	hideVirtualKeyboard(m_passwordTextfields);

	auto reenteringPasswordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "reentering_password_textfield");

	auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield");

	if (passwordTextfield->getString().empty() || reenteringPasswordTextfield->getString().empty() )
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

	const std::string k_emailText = (m_type == RECOVERY_TYPE::MAIL ? m_mailOrPhoneNumberText : "");

	const std::string k_phoneText = (m_type == RECOVERY_TYPE::MAIL ? "": m_mailOrPhoneNumberText);

	ap::api::APPasswordAPI::changePasswordCall(k_emailText, k_phoneText, passwordTextfield->getString(), [=](bool i_success, std::string i_msg, const rapidjson::Value &i_data)
	{
		removeWaittingView();

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
		}
		else
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, finishButton] {

				finishButton->setTouchEnabled(true);

			});

			showFailedPopup(i_msg);
		}
	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APEnterNewPasswordPopup::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APEnterNewPasswordPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APEnterNewPasswordPopup, onFinish),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}


