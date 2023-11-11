#pragma once

#include "APPopupBase.h"
#include "APPhoneCodePopup.h"
#include "StoryAlert.h"

class APUpdatePhoneNumberScreen : public APPopupBase , public APPhoneCodePopupDelegate, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate
{
public:
	enum class START_FROM
	{
		CONNECTION_SCREEN,
		CONNECTION_SCREEN_WITH_FACEBOOK,
		HAVE_USED_APPS
	};
    CREATE_FUNC(APUpdatePhoneNumberScreen);

    static APUpdatePhoneNumberScreen * createView(ap::api::Login::LoginDataInfo info, START_FROM i_from);
    
private:

	bool didLoadFromCSB(ap::api::Login::LoginDataInfo info, START_FROM i_from);

    void changeDisplayLanguage() override final;

	void onExit() override;

	void onUpdateButtonClicked(cocos2d::Ref * sender);

	void onPhoneCodeButtonClicked(cocos2d::Ref * sender);

	void onPhoneCodeItemSelected(PhoneCodeInfo i_info) override final;

	void onAlertOK(cocos2d::Ref * sender) override;

	void onAlertCancel(cocos2d::Ref * sender) override;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName);

	void hideVirtualKeyboardOnScreen();

	void linkAccountAPICall(ap::api::Login::LoginDataInfo i_info, std::function<void()> i_callback = nullptr);

    void pushCleverTapEvent(std::string i_connectionStatus);
    
private:
	
	ap::api::Login::LoginDataInfo m_info;

	START_FROM m_from;

};

CREATE_READER(APUpdatePhoneNumberScreen);
