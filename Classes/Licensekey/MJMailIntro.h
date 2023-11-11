//
//  MJMailIntro.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/28/18.
//

#ifndef MJMailIntro_hpp
#define MJMailIntro_hpp
#include "cocostudiomacro2.h"
using namespace cocos2d;
class MJMailIntro : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ui::EditBoxDelegate {
public:
    CREATE_FUNC(MJMailIntro);
    static MJMailIntro * createView(bool is_popup = false);
    static MJMailIntro * createGiftView();
    void onShow();
    inline void setCallback(std::function<void()> callback) { _callback = callback; }
private:
    void didLoadFromCSB(bool is_popup);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    void onClose(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    void onSend(cocos2d::Ref * sender);
    void onSendVN(cocos2d::Ref * sender);
    void onConfirmGift(cocos2d::Ref * sender);
    
    void onAccountKitFinished(const std::string & phone);
    void onAccountKitFailed();
            
    std::string _entered_license;
        
    int edu_alert_tag = 0;
    void showAlertActivedLicense(int time_expire);
    void sendLicense(std::string license);
    std::string license_key;
    bool is_popup = false;
    cocos2d::ui::ImageView *icon_warning, *box1, *box2;
    cocos2d::ui::Text *warning_label;
    
//    ui::EditBox* textBox = nullptr;
    void editBoxEditingDidBegin(ui::EditBox *editBox) override;
    void editBoxEditingDidEnd(ui::EditBox *editBox) override;
    void editBoxTextChanged(ui::EditBox *editBox, const std::string& text) override;
    void editBoxReturn(ui::EditBox *editBox) override;
    ui::TextField * textField_mail = nullptr;
    ui::TextField * textField_phone = nullptr;
    ui::TextField * textField_name = nullptr;
    void onTextfieldChanged(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);
    bool is_vn;
    
    std::function<void()> _callback = nullptr;
    
    std::once_flag account_kit_listener_flag;
};

CREATE_READER(MJMailIntro);

#endif /* MJMailIntro_hpp */
