#include "APMessagePopup.h"
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
INITIALIZE_READER(APMessagePopup);

std::once_flag ap_message_popup_flag;

APMessagePopup * APMessagePopup::createView(MESSAGE_TYPE i_type, std::string i_msg)
{
	std::call_once(ap_message_popup_flag, [] {
		REGISTER_CSB_READER(APMessagePopup);
		});

	auto p = static_cast<APMessagePopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APMessagePopup.csb"));

	if (p && p->didLoadFromCSB(i_type, i_msg))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APMessagePopup::didLoadFromCSB(MESSAGE_TYPE i_type, std::string i_msg)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	if (i_type == APMessagePopup::MESSAGE_TYPE::SUCCESS)
	{
		showingAnimation(utils::findChild(this, "popup_bg"), [=]() {

		});
	}

	if (auto success = cocos2d::utils::findChild<ui::ImageView*>(this, "success"))
	{
		success->setVisible(i_type == APMessagePopup::MESSAGE_TYPE::SUCCESS);
	}

	if (auto unsuccess = cocos2d::utils::findChild<ui::ImageView*>(this, "unsuccess"))
	{
		unsuccess->setVisible(i_type == APMessagePopup::MESSAGE_TYPE::UNSUCCESS);

		runWarningActionOnBackgroundNode();
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(i_msg);
	}

	return true;
}

void APMessagePopup::changeDisplayLanguage()
{

}

void APMessagePopup::onFinish(Ref * sender)
{

}

cocos2d::ui::Widget::ccWidgetClickCallback APMessagePopup::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APMessagePopup, onCancel),//locate at parent (APPopupBase)
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }

	return nullptr;
}


