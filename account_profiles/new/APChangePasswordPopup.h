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

class APChangePasswordPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:

	enum class CHANGE_TYPE
	{
		MAIL,
		PHONE
	};
	
	CREATE_FUNC(APChangePasswordPopup);

	static APChangePasswordPopup* createView(CHANGE_TYPE i_type , std::string i_mailOrPhoneNumberText);

private:

	void onFinish(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(CHANGE_TYPE i_type, std::string i_mailOrPhoneNumberText);

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private://private variables

	CHANGE_TYPE m_type;

	std::string m_mailOrPhoneText = "";

	std::vector< ui::TextField*> m_passwordTextfields;
};

CREATE_READER(APChangePasswordPopup);

