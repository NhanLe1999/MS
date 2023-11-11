//
//  Created by NguyenHoangThienPhuoc.
//

#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "../../account_profiles/new/APPopupBase.h"
#include "APPhoneCodePopup.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APResetPasswordScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public APPhoneCodePopupDelegate
{
public:
	
	enum class RESET_TYPE
	{
		EMAIL,
		PHONE
	};

	CREATE_FUNC(APResetPasswordScreen);

	static APResetPasswordScreen* createView(RESET_TYPE i_type, APPopupBase::Flow i_flow = APPopupBase::Flow::NONE);
    
    // Author:  Hieu Nguyen
    // Desc:    Nếu muốn fix cứng email/phone và không cho sửa thì gọi hàm này
    //
    void setTextfieldsWithFixedConfig(const std::string& phone, const std::string& email, int button_continue_counter = 0);

private:
    
    bool            m_fixedConfig = false;
    std::string     m_fixedPhone;
    std::string     m_fixedEmail;
    int             m_button_continue_counter = 0; // số lần bấm nút continue
    
    void onCancel(cocos2d::Ref *sender) override;

	void onContineButtonClicked(cocos2d::Ref * sender);

	void onPhoneCodeButtonClicked(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(RESET_TYPE i_type, APPopupBase::Flow i_flow = APPopupBase::Flow::NONE);

	void changeDisplayLanguage() override final;

    void onPhoneCodeItemSelected(PhoneCodeInfo i_info) override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    
    void hideVirtualKeyBoard();
    
private:

	RESET_TYPE m_resetType;
};

CREATE_READER(APResetPasswordScreen);

