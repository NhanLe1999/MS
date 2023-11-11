#pragma once

#include "APPopupBase.h"

class APChangeDefaultPasswordScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

	enum class START_FROM
	{
		CONNECTION_SCREEN,
		LOGIN_METHOS_SCREEN,
		UPDATE_PHONE_NUMBER_SCREEN_FOR_FACEBOOK,
		UPDATE_PHONE_NUMBER_SCREEN,
		LOGIN_SCREEN,
	};

	CREATE_FUNC(APChangeDefaultPasswordScreen);

	static APChangeDefaultPasswordScreen * createView(ap::api::Login::LoginDataInfo info, START_FROM i_from);

	CC_SYNTHESIZE(std::string, m_phoneNumber, PhoneNumber);

	CC_SYNTHESIZE(std::string, m_countryCode, CountryCode);

private:

	bool didLoadFromCSB(ap::api::Login::LoginDataInfo info, START_FROM i_from);

	void changeDisplayLanguage() override final;

	void onExit() override;

	void onConnectionClicked(cocos2d::Ref * sender);

	void onShowPasswordButtonClicked(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void updatePasswordAPICall(std::string i_password);

	void changePasswordV3APICall(std::string i_password);

    void loginChangePassword();
    
    void clevertapTracking(std::string i_evName, bool isSuccessfull);
    
private://variable

	std::vector< ui::TextField*> m_passwordTextfields;

	ap::api::Login::LoginDataInfo m_info;

	START_FROM		m_from;

	cocos2d::ui::Button* m_connectionButton = nullptr;
};

CREATE_READER(APChangeDefaultPasswordScreen);
