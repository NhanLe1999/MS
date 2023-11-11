//
//  APEnterEmailPopup.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/18/20.
//

#ifndef APEnterEmailPopup_h
#define APEnterEmailPopup_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "StoryAlert.h"
#include "APAccountDefinesV3.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APEnterEmailPopupDelegate {
public:
    virtual void onEnterEmailPopupOk();
    virtual void onEnterEmailPopupCancel() {};
};

class APEnterEmailPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate
{
public:
    CREATE_FUNC(APEnterEmailPopup);

    static APEnterEmailPopup * createPopup();
    
    void setDelegate(APEnterEmailPopupDelegate * delegate);
    void showPopup();
    void hidePopup();
    
private:
    APEnterEmailPopupDelegate * m_delegate;         // assign var
    ui::TextField *             m_textfield_email;  // assign var
    ui::Button *                m_button_ok;        // assign var
    ui::Text *                  m_text_warning;     // assign var
    
    bool                        m_isRunning = false;
    
    void changeDisplayLanguage() override {};
    bool didLoadFromCSB() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onExit() override;
    void onCancel(cocos2d::Ref * sender) override;
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void onOk(cocos2d::Ref * sender);
    
    void setEnabledButtonOk(bool enabled);
    void showWarning(const std::string& warningText);
    void hideWarning();
    
    void onUpdateEmailAPICalled(bool is_success, std::string msg);
};

CREATE_READER(APEnterEmailPopup);

#endif /* APEnterEmailPopup_h */
