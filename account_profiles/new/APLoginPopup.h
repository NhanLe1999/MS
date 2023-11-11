//
//  APEnterPhoneNumberPopup.hpp
//  MJStory
//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APPopupBase.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class APLoginPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	
	enum class LOGIN_TYPE
	{
		EMAIL,
		PHONE
	};

	CREATE_FUNC(APLoginPopup);

	static APLoginPopup* createView(LOGIN_TYPE i_type, std::string i_mailOrPhoneText);

private:

    void onLogin(cocos2d::Ref * sender);

	void onForgetPasswordClicked(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(LOGIN_TYPE i_type, std::string i_mailOrPhoneText) ;

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	std::string m_mailOrPhoneText;

	LOGIN_TYPE	m_loginType;
};

CREATE_READER(APLoginPopup);

