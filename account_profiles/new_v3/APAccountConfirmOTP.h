//
//  APAccountConfirmOTP.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/23/20.
//

#ifndef APAccountConfirmOTP_h
#define APAccountConfirmOTP_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APAccountDefinesV3.h"

#define APACCOUNTCONFIRMOTP_TITLE_BUTTON_CUR_REMAIN_TIME    DEF_GET_TEXT("key.accountv3.button.title.resendcode") + " (" + std::to_string((int)m_timeCounter) + "s)"

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountConfirmOTP : public APAccountBaseV3
{
public:
    CREATE_FUNC(APAccountConfirmOTP);

    static APAccountConfirmOTP * createView(const std::string& email);
    
    void callAPISendOTP();
    
    void update(float dt) override;
    
private:
    ui::TextField * m_textfield_code;           // assign var
    ui::Button *    m_button_sendcode;          // assign var
    ui::Button *    m_button_confirm;           // assign var
    
    std::string     m_email;
    float           m_timeCounter;              // bộ đếm ngược thời gian
    bool            m_isTimerRunning = false;   // bộ đếm thời gian (để enable nút Re-send Code) đang chạy hay không
    int             m_button_sendcode_counter = 0; // số lần bấm nút Send Code
    
    bool didLoadFromCSB() override {return true;};
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onExit() override;
    
    bool didLoadFromCSB(const std::string& email);
    
    void removeFromParentWithEffect();
    
    void onBack(cocos2d::Ref * sender);
    void onSendCode(cocos2d::Ref * sender);
    void onConfirmCode(cocos2d::Ref * sender);
    
    void onSendOptAPICalled(bool is_success, std::string msg);
    void onCommomVerifyOTPAPICalled(bool is_success, std::string msg, int code);
    void onUpdatePasswordAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APAccountConfirmOTP);

#endif /* APAccountConfirmOTP_h */
