#include "APEnterNewPasswordPopupForOldUser.h"
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
INITIALIZE_READER(APEnterNewPasswordPopupForOldUser);

std::once_flag ap_enter_new_password_popup_for_old_user_flag;

APEnterNewPasswordPopupForOldUser * APEnterNewPasswordPopupForOldUser::createView(RECOVERY_TYPE i_type,std::string i_mailOrPhoneNumberText)
{
	std::call_once(ap_enter_new_password_popup_for_old_user_flag, []
	{
		REGISTER_CSB_READER(APEnterNewPasswordPopupForOldUser);
	});

	auto p = static_cast<APEnterNewPasswordPopupForOldUser*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APEnterNewPasswordPopupForOldUser.csb"));

	if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneNumberText))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APEnterNewPasswordPopupForOldUser::didLoadFromCSB(RECOVERY_TYPE i_type, std::string i_mailOrPhoneNumberText)
{
	m_type = i_type;

	m_mailOrPhoneNumberText = i_mailOrPhoneNumberText;

	if (!APEnterNewPasswordPopup::didLoadFromCSB(i_type, i_mailOrPhoneNumberText))
	{
		return false;
	}

	return true;
}

void APEnterNewPasswordPopupForOldUser::changeDisplayLanguage()
{
	APEnterNewPasswordPopup::changeDisplayLanguage();

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_new_password_for_old_user_title"));
	}
}

void APEnterNewPasswordPopupForOldUser::onFinish(Ref * sender)
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
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {

				MJDEFAULT->setBoolForKey("need_to_fill_password", false);
				showSuccessPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_password_changed_successfully"));
				removeFromParentWithAnmation();
			});
		}
		else
		{
			showFailedPopup(i_msg);

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, finishButton] {

				finishButton->setTouchEnabled(true);

			});

		}
	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APEnterNewPasswordPopupForOldUser::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APEnterNewPasswordPopupForOldUser, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APEnterNewPasswordPopupForOldUser, onFinish),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}


