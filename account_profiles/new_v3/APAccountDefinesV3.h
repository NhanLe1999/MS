//
//  APAccountDefinesV3.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/28/20.
//

#ifndef APAccountDefinesV3_h
#define APAccountDefinesV3_h

#include "../../account_profiles/new/APPopupBase.h"

#define DEF_APACCOUNT_FLOW                      APPopupBase::Flow

#define DEF_DEBUG_V3_ALWAYS_SHOW_ENTER_EMAIL    false                                        // luôn luôn hiện popup EnterEmail, nhớ set FALSE khi commit

#define DEF_TEMP_EMAIL_FOR_ENTER_INFO_THREAD    "def_temp_email_for_enter_info_thread"      // email lưu tạm cho luồng Lấy thông tin người dùng để nhận ebook
#define DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD "def_temp_password_for_enter_info_thread"   // mật khẩu lưu tạm khi user tạo mật khẩu mà không có cả email và phone

#define DEF_BUTTON_COLOR_DISABLED               Color3B(161, 161, 161) //APAccountBaseV3::lightenColor(Color3B(41, 176, 223), 128)
#define DEF_BUTTON_COLOR_DISABLED_VM            Color3B(115, 174, 195) //APAccountBaseV3::lightenColor(Color3B(41, 176, 223), 96)

#define DEF_FONT_FILE_PATH                      (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf")
#define DEF_GET_TEXT(__KEY__)                   LANGUAGE_MANAGER->getDisplayTextByKey(__KEY__)

#define DEF_ENTER_INFO_MAX_COUNT                3                                           // số lần tối đa bấm close popup EnterEmailPopup
#define DEF_OTP_LENGTH                          4                                           // số ký tự của mã OTP
#define DEF_PHONE_MIN_LENGTH                    6                                           // số ký tự tối thiểu của phone, chưa tính đầu số
#define DEF_PHONE_MAX_LENGTH                    14                                          // số ký tự tối đa của phone, chưa tính đầu số
#define DEF_NAME_MAX_LENGTH                     100                                         // số ký tự tối đa của TÊN
#define DEF_EMAIL_MAX_LENGTH                    160                                         // số ký tự tối đa của EMAIL, tiêu chuẩn của thế giới là 320
#define DEF_TIME_LIMIT_FOR_RESEND_OTP           60.05f                                      // thời gian chờ để Gửi lại mã OTP

#define name_screen_enter_email_popup           "screen_enter_email_popup"                  //
#define name_screen_apaccount_update_phone      "screen_apaccount_update_phone"             //
#define name_screen_apaccount_update_password   "screen_apaccount_update_password"          //
#define name_screen_apaccount_update_email      "screen_apaccount_update_email"             //
#define name_screen_apaccount_confirm_otp       "screen_apaccount_confirm_otp"              //
#define name_screen_apaccount_add_password      "screen_apaccount_add_password"             //
#define name_screen_apaccount_confirm_password  "screen_apaccount_confirm_password"         //
//#define name_screen_apaccount_user_info         "screen_apaccount_user_info"

class APAccountUserInfo;

class APAccountBaseV3 : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    static void checkShowingEnterEmailPopup(Node * parent);
    
    static Color3B lightenColor(Color3B color, GLubyte factor){
        float p = ((float)factor) / 255.0f;
        Color3B wColor = Color3B(255, 255, 255);
        
        return Color3B((1.0f - p)*wColor.r + p*color.r, (1.0f - p)*wColor.g + p*color.g, (1.0f - p)*wColor.b + p*color.b);
    }
    
    static Color3B darkenColor(Color3B color, GLubyte factor){
        float p = ((float)factor) / 255.0f;
        Color3B wColor = Color3B(0, 0, 0);
        
        return Color3B((1.0f - p)*wColor.r + p*color.r, (1.0f - p)*wColor.g + p*color.g, (1.0f - p)*wColor.b + p*color.b);
    }
    
    APAccountUserInfo * getAccountBackend() {return m_account_backend;};
    
    void setAccountBackendLayer(APAccountUserInfo *layer) {m_account_backend = layer;};
    
protected:
    APAccountUserInfo *     m_account_backend = nullptr;    // assign var
    
    bool                    m_isFormMovedUp = false;
    float                   m_moveUpDistance = 200.f;       // muốn dịch bao nhiêu thì set lại trong hàm didLoadFromCSB(...)
    
    virtual void changeDisplayLanguage() override {};
    
    virtual void moveUpForm();
    virtual void moveDownForm();
    virtual void effectShowing();
};

#endif /* APAccountDefinesV3_h */
