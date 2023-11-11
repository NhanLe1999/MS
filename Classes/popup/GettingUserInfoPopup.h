//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "PopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class GettingUserInfoPopup :public PopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	CREATE_FUNC(GettingUserInfoPopup);

	static GettingUserInfoPopup* createView(bool i_isskipButtonEnable,bool i_isRewardPopupShow);

private:

	void onEnter() override;

	void onExit() override;
    
	void onFinishButtonClicked(cocos2d::Ref * sender);

	void onSkipButtonClicked(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(bool i_isskipButtonEnable,bool i_isRewardPopupShow );

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

    void hideVirtualKeyboardsOfTextFields();
    
    void pushCleverTapEvent(std::string i_clickedType);

private:

	bool m_isskipButtonEnable = false;

	bool m_isRewardPopupShow = false;
};

CREATE_READER(GettingUserInfoPopup);

