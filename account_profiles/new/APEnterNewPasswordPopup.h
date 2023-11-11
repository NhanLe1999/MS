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

class APEnterNewPasswordPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:

	enum class RECOVERY_TYPE
	{
		MAIL,
		PHONE
	};
	
	CREATE_FUNC(APEnterNewPasswordPopup);

	static APEnterNewPasswordPopup* createView(RECOVERY_TYPE i_type , std::string i_mailOrPhoneNumberText);

protected:

	virtual void changeDisplayLanguage() override;

	virtual bool didLoadFromCSB(RECOVERY_TYPE i_type, std::string i_mailOrPhoneNumberText);

	virtual cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	void onFinish(cocos2d::Ref * sender);

protected:
	
	std::string		m_mailOrPhoneNumberText = "";

	RECOVERY_TYPE	m_type ;

	std::vector< ui::TextField*> m_passwordTextfields;

};

CREATE_READER(APEnterNewPasswordPopup);

