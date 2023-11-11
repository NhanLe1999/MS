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

class APRegisterPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:

	enum class REGISTER_TYPE
	{
		EMAIL,
		PHONE
	};
	
	CREATE_FUNC(APRegisterPopup);

	static APRegisterPopup* createView(REGISTER_TYPE i_type , std::string i_mailOrPhoneNumberText);

private:

	void onFinish(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(REGISTER_TYPE i_type, std::string i_mailOrPhoneNumberText);

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	std::string		m_mailOrPhoneText;

	REGISTER_TYPE	m_registerType;

	std::vector< ui::TextField*> m_passwordTextfields;

};


CREATE_READER(APRegisterPopup);

