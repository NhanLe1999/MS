#pragma once

#include "APPopupBase.h"



class APHaveUsedMonkeyApps : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

    CREATE_FUNC(APHaveUsedMonkeyApps);

	static APHaveUsedMonkeyApps* createView(ap::api::Login::LoginDataInfo i_info); // AP_CREATE_VIEW -> define in CPP file.

private:
	
	bool didLoadFromCSB(ap::api::Login::LoginDataInfo i_info);

	void changeDisplayLanguage() override final;

	void onExit() override;
    
    void onBack(cocos2d::Ref * sender);

	void onUseClicked(cocos2d::Ref * sender);

    void onNotUseClicked(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	ap::api::Login::LoginDataInfo m_info;
};

CREATE_READER(APHaveUsedMonkeyApps);



