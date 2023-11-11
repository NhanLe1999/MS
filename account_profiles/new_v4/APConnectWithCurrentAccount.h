#pragma once

#include "APPopupBase.h"

class APConnectWithCurrentAccount : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

	CREATE_FUNC(APConnectWithCurrentAccount);

	static APConnectWithCurrentAccount * createView(ap::api::Login::LoginDataInfo info);

private:

	bool didLoadFromCSB(ap::api::Login::LoginDataInfo info);

	void changeDisplayLanguage() override final;

	void onExit() override;

	void onConnectionClicked(cocos2d::Ref * sender);

	void onSkipButton(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

    void clevertapTracking(std::string connectSuccessfull);

private:
	
	ap::api::Login::LoginDataInfo m_info;

    bool m_isClevertapTrackingPushed = false;
};

CREATE_READER(APConnectWithCurrentAccount);
