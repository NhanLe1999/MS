#pragma once

#include "APPopupBase.h"

struct LoginMethodsInfo
{
	std::string userName = "";

	std::string phoneNumber = "";

	std::string email = "";

	std::string fbName = "";

};

class APLoginMethodsScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	enum class ScreenType
	{
		CONTINUE_TO_CHANGE_PASSWORD,
		CONTINUE_TO_SHOW_PROFILE
	};

	enum class MethodsDisplayType
	{
		DISPLAY_2_METHOD,
		DISPLAY_2_METHOD_INCLUDING_FACEBOOK,
		DISPLAY_3_METHOD,
		DISPLAY_3_METHOD_INCLUDING_FACEBOOK,
		DISPLAY_4_METHOD,
		NONE
	};

	CREATE_FUNC(APLoginMethodsScreen);

	static APLoginMethodsScreen * createView(ap::api::Login::LoginDataInfo i_info, ScreenType i_type);

	CC_SYNTHESIZE(bool, m_needToCallLinkAccount, NeedToCallLinkAccount);

private:

	bool didLoadFromCSB(ap::api::Login::LoginDataInfo i_info, ScreenType i_type);

	void changeDisplayLanguage() override final;

	void onExit() override;

	void onNextButtonClicked(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void linkAccountAPICall();

    void clevertapTracking(bool isAccountLinked);
    
private:

	ScreenType			m_screenType;

	MethodsDisplayType	m_methodsDiplayType;

	LoginMethodsInfo	m_loginMethodsInfo;

	ap::api::Login::LoginDataInfo m_info;
};

CREATE_READER(APLoginMethodsScreen);
