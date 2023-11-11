//
//  APAccountConfirmPassword.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#ifndef APAccountConfirmPassword_h
#define APAccountConfirmPassword_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APAccountDefinesV3.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountConfirmPassword : public APAccountBaseV3
{
public:
    CREATE_FUNC(APAccountConfirmPassword);

    static APAccountConfirmPassword * createView(DEF_APACCOUNT_FLOW flow = DEF_APACCOUNT_FLOW::NONE);
    
private:
    ui::Button *        m_button_confirm;               // assign var
    ui::Button *        m_button_forgot;                // assign var
    ui::TextField *     m_textfield_password;           // assign var
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onExit() override;
    
    bool didLoadFromCSB(DEF_APACCOUNT_FLOW flow);
    
    void onShowPasswordButtonClicked(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);
    void onForgot(cocos2d::Ref * sender);
    void onConfirm(cocos2d::Ref * sender);
    
    void onUpdatePasswordAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APAccountConfirmPassword);

#endif /* APAccountConfirmPassword_h */
