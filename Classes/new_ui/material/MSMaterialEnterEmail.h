//
//  MSMaterialEnterEmail.h
//  MJStory
//
//  Created by Hieu Nguyen on 11/17/20.
//

#ifndef MSMaterialEnterEmail_h
#define MSMaterialEnterEmail_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "APPopupBase.h"
#include "StoryDataModel.h"

USING_NS_CC;

class MSMaterialEnterEmailDelegate
{
public:
    virtual mj::model::MaterialInfo::Type fromMaterialScreen() {return mj::model::MaterialInfo::Type::NONE;};
    virtual void onMaterialEmailUpdated() {};                   // hành động khi cập nhật email học liệu mới
    virtual void onMaterialEmailSent() {};                      // hành động khi đã gửi email học liệu xong
    virtual std::string getStringMaterialIds() {return "";};    // lấy về danh sách id học liệu để gửi
    virtual void pushEventNotation() {}; //push event notation
};

class MSMaterialEnterEmail : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    enum class Type
    {
        ENTER_EMAIL,            // đăng ký email nhận học liệu
        SEND_MATERIAL,          // gửi học liệu
        NOTICE,                 // thông báo đơn thuần
        POPUP_NOTE,             // popup chú thích: cung cấp thông tin về phần học liệu
        POPUP_FREE_LIMITED,     // popup thông báo người dùng FREE đã download học liệu hết giới hạn cho phép
    };
    
    enum class EmailType
    {
        NONE,           // email nhận học liệu đã được đăng ký
        NEW,            // email đăng ký mới
        UPDATE,         // email cập nhật mới
    };
    
    CREATE_FUNC(MSMaterialEnterEmail);
    static MSMaterialEnterEmail * createPopup(MSMaterialEnterEmail::Type type, MSMaterialEnterEmailDelegate * delegate = nullptr);
    static std::string getCurrentMaterialEmail();
    
    void showPopup();
    void hidePopup();
    void showNotice(const std::string& mess);
    void showPopupNote(ui::Widget * contentNode = nullptr);
    void showPopupFreeLimited(const std::string& mess);
    
private:
    MSMaterialEnterEmailDelegate *  m_delegate;             // assign var
    ui::ImageView *                 m_email_background;
    ui::TextField *                 m_email_textfield;
    ui::ImageView *                 m_warning_icon;
    ui::Text *                      m_warning_text;
    ui::Text *                      m_desc;
    ui::Button *                    m_button_ok;
    ui::Button *                    m_button_edit;
    ui::Button *                    m_button_cancel;
    
    MSMaterialEnterEmail::Type      m_type;                 //
    MSMaterialEnterEmail::EmailType m_emailType;            // kiểu email khi nhấn nút Gửi học liệu về email
    std::string                     m_email = "";           //
    std::string                     m_email_duplicated = "";// email trong trường hợp: Gửi học liệu - không có hoặc sửa email mà bị trùng với email đã được đăng ký khác
    bool                            m_isRunning = false;    // đang chạy hiệu ứng show/hide popup
    bool                            m_isSuccess = false;    // Đã đăng ký email thành công hoặc Đã gửi học liệu về email thành công
    bool                            m_hasEmail = false;     //
    std::string                     m_failReason = "none";      //lý do người dùng không nhập đc email
    
    void changeDisplayLanguage() override {};
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    bool didLoadFromCSB(MSMaterialEnterEmail::Type type, MSMaterialEnterEmailDelegate * delegate);
    void onExit() override;
    void onCancel(cocos2d::Ref *ref) override;
    
    void onOK(cocos2d::Ref *ref);
    void onEditEmail(cocos2d::Ref *ref);
    
    void handleWithUpdateEmailMaterial(cocos2d::Ref *ref);
    void handleWithSendEmailMaterial(cocos2d::Ref *ref);
    
    void onUpdateEmailMaterialAPICalled(bool is_success, std::string msg, int code);
    void onSendEmailMaterialAPICalled(bool is_success, std::string msg, int code);
    
    void sendEmailMaterial(const std::string& email_address);
    
    void showSuccessMessage(const std::string& text_top, const std::string& text_email = "", const std::string& text_bottom = "");
    
    void showWarning(const std::string& text);
    void hideWarning();
    
    void pushCleverTapEvent_emailSetting(bool is_success);
    void pushCleverTapEvent_confirmEmail(const std::string& status, const std::string& type, bool is_success, bool close_popup, const std::string& failReason="");

    std::string mapMessageFromAPIToEventName(const std::string& msg, int code);

    ScrollView* scrollView = nullptr;

    TextField* userNamePhoneEtcTextfield = nullptr;
};

CREATE_READER(MSMaterialEnterEmail);

#endif /* MSMaterialEnterEmail_h */
