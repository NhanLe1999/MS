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

class APOTPPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol \
{
public:
	
	enum class OTP_TYPE
	{
		EMAIL,
		PHONE
	};

	CREATE_FUNC(APOTPPopup);

	static APOTPPopup* createView(OTP_TYPE i_type, std::string i_mailOrPhoneText ,std::string i_countryCode, APPopupBase::Flow i_flow = APPopupBase::Flow::NONE);

private:

    void onContinute(cocos2d::Ref * sender);

	void onResend(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(OTP_TYPE i_type, std::string i_mailOrPhoneText,std::string i_countryCode, APPopupBase::Flow i_flow = APPopupBase::Flow::NONE);

	void changeDisplayLanguage() override final;

	void onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type);

	void insertOTP();

	void updateCusor();

	void resendOTP();
    
    void showWarning(const std::string& warning);
    
    void hideWarning();

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	OTP_TYPE m_OTPType = OTP_TYPE::EMAIL;

	std::vector<Text*> m_textNumbers;

	float	m_resendingCodeTimer = 0.0f;

	std::string m_mailOrPhoneText   = "";
    
    std::string m_countryCode       = "";


};

CREATE_READER(APOTPPopup);

