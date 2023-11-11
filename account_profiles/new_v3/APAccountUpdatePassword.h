//
//  APAccountUpdatePassword.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#ifndef APAccountUpdatePassword_h
#define APAccountUpdatePassword_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APAccountDefinesV3.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountUpdatePassword : public APAccountBaseV3
{
public:
    enum class State
    {
        NONE,
        UPDATE_SUCCESS,
        UPDATE_FAIL,
    };
    
    CREATE_FUNC(APAccountUpdatePassword);

    static APAccountUpdatePassword * createView(DEF_APACCOUNT_FLOW flow = DEF_APACCOUNT_FLOW::NONE);
    
private:
    ui::TextField *                 m_textfield_old_password;   // assign var
    ui::TextField *                 m_textfield_new_password;   // assign var
    ui::TextField *                 m_textfield_renew_password; // assign var
    ui::Button *                    m_button_update;            // assign var
    
    std::vector< ui::TextField *>   m_passwordTextfields;
    bool                            m_isWaitingForResponse = false; // đang chờ phản hồi của API
    APAccountUpdatePassword::State  m_state;
    int                             m_button_update_counter = 0; // số lần bấm nút update
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onExit() override;
    void update(float dt) override;
    
    bool didLoadFromCSB(DEF_APACCOUNT_FLOW flow = DEF_APACCOUNT_FLOW::NONE);
    
    void removeFromParentWithEffect();
    
    void onUpdate(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);
    void onShowPasswordButtonClicked(cocos2d::Ref * sender);
    
    void onUpdatePasswordAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APAccountUpdatePassword);

#endif /* APAccountUpdatePassword_h */
