//
//  APAccountUpdatePhone.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/25/20.
//

#ifndef APAccountUpdatePhone_h
#define APAccountUpdatePhone_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APPhoneCodePopup.h"
#include "APAccountDefinesV3.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountUserInfo;

class APAccountUpdatePhone : public APAccountBaseV3, public APPhoneCodePopupDelegate
{
public:
    enum class State
    {
        NONE,
        PHONE_UPDATE_SUCCESS,
        PHONE_UPDATE_FAIL,
    };
    
    CREATE_FUNC(APAccountUpdatePhone);

    static APAccountUpdatePhone * createView();
    
private:
    ui::Text *                  m_text_numbercode;              // assign var
    ui::Text *                  m_text_countrycode;             // assign var
    ui::Button *                m_button_country;               // assign var
    ui::Button *                m_button_update;                // assign var
    ui::TextField *             m_textfield_phone;              // assign var
    
    std::string                 m_phone;                        // phone number sẽ được submit
    std::string                 m_phone_old;                    // phone number hiện tại
    APAccountUpdatePhone::State m_state;
    
    bool didLoadFromCSB() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onPhoneCodeItemSelected(PhoneCodeInfo i_info) override;
    void onExit() override;
    
    void removeFromParentWithEffect();
    
    void onUpdate(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);
    void onCountryCodeClicked(cocos2d::Ref * sender);
    
    void onUpdateUserInfoAPICalled(bool is_success, std::string msg);
    void onUpdatePasswordAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APAccountUpdatePhone);

#endif /* APAccountUpdatePhone_h */
