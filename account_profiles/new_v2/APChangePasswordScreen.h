//
//  APEnterPhoneNumberPopup.hpp
//  MJStory
//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "../../account_profiles/new/APPopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APChangePasswordScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol 
{
public:
    
    enum class TYPE
    {
        EMAIL,
        PHONE
    };
	
    enum class START_FROM
    {
        RESET_PASS_SCREEN,
        LOGIN_SCREEN,
        ENTER_INFO,     // luồng lấy thông tin người dùng
        UPDATE_PHONE,    // luồng Thêm/Cập nhật số điện thoại
        UPDATE_EMAIL,   // luồng Thêm/Cập nhật email
    };
    
	CREATE_FUNC(APChangePasswordScreen);

	static APChangePasswordScreen* createView(const START_FROM i_from,  const TYPE i_type, const std::string i_mailOrPhone,const std::string i_tokenToChangePassword,const std::string i_countryCode);

private:
    
    ~APChangePasswordScreen();

	void onSaveButtonClicked(cocos2d::Ref * sender);
	
	void onSkipButton(cocos2d::Ref * sender);

	void onShowPasswordButtonClicked(cocos2d::Ref * sender);

	bool didLoadFromCSB(const START_FROM i_from, const TYPE i_type, const std::string i_mailOrPhone,const std::string i_tokenToChangePassword,const std::string i_countryCode);

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private:

	std::string m_tokenToChangePassword;
    
    std::string m_countryCode;

    std::vector< ui::TextField*> m_passwordTextfields;

    TYPE            m_type;
    
    START_FROM      m_from;
    
    std::string m_phoneOrEmailInput = "";
    
    bool m_passwordChangedSuccess = false;
};

CREATE_READER(APChangePasswordScreen);

