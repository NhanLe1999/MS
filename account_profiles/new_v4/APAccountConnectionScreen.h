#pragma once

#include	"APPopupBase.h"
#include	"APPhoneCodePopup.h"

class APAccountConnectionScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

    //for clevertap tracking.
    enum class START_FROM
    {
        ConnectWithCurrentAccount,//old
        HaveUsedMonkeyApps,//new
        UpdatePhoneNumberScreen //new
    };
    
	CREATE_FUNC(APAccountConnectionScreen);

	static APAccountConnectionScreen * createView(ap::api::Login::LoginDataInfo info);

	void fillPhoneNumber(std::string i_phoneNumber,std::string i_country,std::string i_code);

	void changeTextTitle();

	CC_SYNTHESIZE(bool, m_isFromNotHaveUsedApps, IsFromNotHaveUsedApps);

    CC_SYNTHESIZE(START_FROM, m_startFrom, StartFrom);//for clevertap tracking.

private:
	
	APAccountConnectionScreen();

	bool didLoadFromCSB(ap::api::Login::LoginDataInfo info);

	void changeDisplayLanguage() override final;

	void onExit() override;

	void onConnectionClicked(cocos2d::Ref * sender);

	void onFacebookButtonClicked(cocos2d::Ref * sender);
	
	void onGmailButtonClicked(cocos2d::Ref * sender);

	void onSkipButton(cocos2d::Ref * sender);

	void onShowPasswordButtonClicked(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void hideVirtualKeyboardOnScreen();

    void clevertapTracking(std::string connectionType,std::string connectSuccessfull,std::string fbUpdate);
    
private:

	ap::api::Login::LoginDataInfo m_info;
};

CREATE_READER(APAccountConnectionScreen);
