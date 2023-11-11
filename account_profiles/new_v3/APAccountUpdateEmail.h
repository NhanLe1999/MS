//
//  APAccountUpdateEmail.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#ifndef APAccountUpdateEmail_h
#define APAccountUpdateEmail_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APAccountDefinesV3.h"

#define APACCOUNTUPDATEEMAIL_TITLE_BUTTON_CUR_REMAIN_TIME   DEF_GET_TEXT("key.accountv3.button.title.resendcode") +                     \
                                                            (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "(" : " (")   \
                                                            + std::to_string((int)m_timeCounter) + "s)"                                 \

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountUserInfo;

class APAccountUpdateEmail : public APAccountBaseV3
{
public:
    enum class State
    {
        NONE,
        EMAIL_UPDATE_SUCCESS,
        EMAIL_UPDATE_FAIL,
    };
    
    CREATE_FUNC(APAccountUpdateEmail);

    static APAccountUpdateEmail * createView();
    
    void callAPISendOTP();
    
private:
    ui::TextField *                 m_textfield_email;          // assign var
    ui::TextField *                 m_textfield_confirmcode;    // assign var
    ui::Button *                    m_button_sendcode;          // assign var
    ui::Button *                    m_button_update;            // assign var
    
    std::vector< ui::TextField *>   m_textfields;
    APAccountUpdateEmail::State     m_state;
    std::string                     m_email;                        // email sẽ được submit
    std::string                     m_email_old;                    // email hiện tại
    float                           m_timeCounter;                  // bộ đếm ngược thời gian
    bool                            m_isTimerRunning = false;       // bộ đếm thời gian (để enable nút Re-send Code) đang chạy hay không
    bool                            m_isCodeSent = false;           // code đã được gửi đến người dùng
    int                             m_button_sendcode_counter = 0;  // số lần bấm nút Send Code
    
    bool didLoadFromCSB() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void update(float dt) override;
    void onExit() override;
    
    void turnOnResendTimer();
    void turnOffResendTimer();
    
    void removeFromParentWithEffect();
    
    void onUpdate(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);
    void onSendCode(cocos2d::Ref * sender);
    
    void onSendOptAPICalled(bool is_success, std::string msg);
    void onCommomVerifyOTPAPICalled(bool is_success, std::string msg, int code);
    void onUpdatePasswordAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APAccountUpdateEmail);

#endif /* APAccountUpdateEmail_h */
