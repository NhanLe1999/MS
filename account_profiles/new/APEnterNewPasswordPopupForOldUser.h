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
#include "APEnterNewPasswordPopup.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class APEnterNewPasswordPopupForOldUser : public APEnterNewPasswordPopup{
public:

	CREATE_FUNC(APEnterNewPasswordPopupForOldUser);

	static APEnterNewPasswordPopupForOldUser* createView(RECOVERY_TYPE i_type , std::string i_mailOrPhoneNumberText);

private:

	void changeDisplayLanguage() override final;

	void onFinish(cocos2d::Ref * sender) ;
	
	bool didLoadFromCSB(RECOVERY_TYPE i_type, std::string i_mailOrPhoneNumberText);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:
	
	std::string		m_mailOrPhoneNumberText = "";

	RECOVERY_TYPE	m_type ;
};

CREATE_READER(APEnterNewPasswordPopupForOldUser);

