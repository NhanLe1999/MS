//
//  MJVerifyInfoView.h
//  Created by HaVanHoi on 21/06/18.
//
//
#ifndef MJVerifyInfoView_hpp
#define MJVerifyInfoView_hpp

#include "cocostudiomacro2.h"
#include "MJLicenseAlert.h"
#include "LicenseInfoModel.h"
#include "MJLicensePackage.h"

using namespace cocos2d;
using namespace ui;

class MJVerifyInfoViewDelegate {
public:
    virtual void onVerifySuccess() {};
};

class MJVerifyInfoView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ui::EditBoxDelegate, public MJLicenseAlertDelegate  {
public:
    CREATE_FUNC(MJVerifyInfoView);
    static MJVerifyInfoView * createView(license::Package p);
    void setDelegate(MJVerifyInfoViewDelegate *delegate);
private:
    void didLoadFromCSB(license::Package p);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    cocos2d::Size screen_size;
    MJVerifyInfoViewDelegate *delegate = nullptr;
    cocos2d::ui::Text *getTextByName(cocos2d::Node *parent, std::string text_name, int font_size = 0);
    void onClose(cocos2d::Ref *sender);
    void onSend(cocos2d::Ref *sender);
    void onShow();
    void onHide();
    
    enum VERIFY_INFO_TYPE {
        PHONE,
        EMAIL,
        NULL_INFO
    };
    
    int verify_type = 0;
    std::string info_verify, bg_name;
    
    ui::EditBox* textBox;
    void editBoxEditingDidBegin(ui::EditBox *editBox) override;
    void editBoxEditingDidEnd(ui::EditBox *editBox) override;
    void editBoxTextChanged(ui::EditBox *editBox, const std::string& text) override;
    void editBoxReturn(ui::EditBox *editBox) override;
    ui::TextField* textField;
    void onTextfieldChanged(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);
    cocos2d::ui::Text *placeholder_lb;
    license::Package _license;
    void onMJAlertClicked(bool is_ok, int tag) override;
};

CREATE_READER(MJVerifyInfoView);

#endif
