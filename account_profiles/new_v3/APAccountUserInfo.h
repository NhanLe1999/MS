//
//  APAccountUserInfo.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#ifndef APAccountUserInfo_h
#define APAccountUserInfo_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APAccountDefinesV3.h"
#include "StoryAlert.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APAccountUserInfo : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate
{
public:
    CREATE_FUNC(APAccountUserInfo);

    static APAccountUserInfo * createView();
    
    void callAPILoadUpdateAndResetScreen(); // gọi api LoadUpdate sau đó hiển thị lại toàn bộ thông tin account
    void resetAccountInfoScreen();          // hiển thị lại toàn bộ thông tin account
    void resetProfileInfo();                // hiển thị lại thông tin profiles
    
private:
    ui::Button *        m_button_update_name;   // assign var
    ui::TextField *     m_textfield_name;       // assign var
    
    std::string         m_name;                         // new name if you want to submit for updating
    bool                m_didLoadFromCSB = false;       // kết thúc hàm didLoadFromCSB(...) thì phải set true
    bool                m_bothEmailPhoneEmpty = false;  // nếu cả email và phone đều không có
    
    void changeDisplayLanguage() override {};
    bool didLoadFromCSB() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    void onExit() override;
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertClose(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void loadUserInfo();            // tham khảo hàm StoryParentAccountProfile_Landscape::loadUserInfo()
    void loadLicenseInfo();         // tham khảo hàm StoryParentAccountProfile_Landscape::loadLicenseInfo()
    void loadProfileInfo();         // nội dung hàm này lấy từ StoryParentAccountProfile_Landscape::loadProfileInfo()
    
    void onSignOut(cocos2d::Ref * sender); // nội dung hàm này lấy từ StoryParentAccountProfile_Landscape::onClickSignOut(cocos2d::Ref * sender)
    void onClose(cocos2d::Ref * sender);
    void onUpdateNameClicked(cocos2d::Ref * sender);
    void onUpdateEmailClicked(cocos2d::Ref * sender);
    void onAddPhoneClicked(cocos2d::Ref * sender);
    void onUpdatePasswordClicked(cocos2d::Ref * sender);
    void onViewProfilesClicked(cocos2d::Ref * sender);
    
    void onUpdateUserInfoAPICalled(bool is_success, std::string msg);
    void onLoadUpdateAPICalled(bool is_success, std::string msg);
    
    bool isShowUpgradePopup();
};

CREATE_READER(APAccountUserInfo);

#endif /* APAccountUserInfo_h */
