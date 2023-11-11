//
//  APAccountUpdateEmail.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#include "APAccountUpdateEmail.h"
#include "StoryLanguageManager.h"
#include "MJHelper.h"
#include "APVerifyOptForEmailAPI.h"
#include "APSendOptAPI.h"
#include "APDatabase.h"
#include "APLoadUpdateAPI.h"
#include "MSHome.h"
#include "APAccountUserInfo.h"
#include "APUpdatePasswordAPI.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountUpdateEmail);

std::once_flag v3_ap_account_update_email_reader_flag;

#pragma mark - Initialize
APAccountUpdateEmail * APAccountUpdateEmail::createView()
{
    std::call_once(v3_ap_account_update_email_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountUpdateEmail);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountUpdateEmail.csb");
    auto p = static_cast<APAccountUpdateEmail *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB())
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountUpdateEmail::onExit()
{
    this->hideVirtualKeyboard(m_textfields);
    
    APPopupBase::onExit();
}

bool APAccountUpdateEmail::didLoadFromCSB()
{
    this->setName(name_screen_apaccount_update_email);
    
    m_state = APAccountUpdateEmail::State::NONE;
    
    auto user_info = ap::Database::getInstance().getUserInfo();
    m_email_old = user_info._mail;
    
    m_textfield_email = utils::findChild<ui::TextField *>(this, "email_textfield");
    m_textfield_confirmcode = utils::findChild<ui::TextField *>(this, "confirmcode_textfield");
    m_button_sendcode = utils::findChild<ui::Button *>(this, "btn_sendcode");
    m_button_update = utils::findChild<ui::Button *>(this, "btn_update");
    
    // button update
    m_button_update->setEnabled(false);
    m_button_update->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_update->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.save"));
    m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // button send_code
    m_button_sendcode->setEnabled(false);
    m_button_sendcode->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_sendcode->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.sendcode"));
    m_button_sendcode->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
    {
        m_button_sendcode->setTitleFontSize(22.f);
    }
    else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
    {
        m_button_sendcode->setTitleFontSize(27.f);
    }
    
    // title & desc
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setFontName(DEF_FONT_FILE_PATH);
    title->setString(DEF_GET_TEXT("key.accountv3.screen.updateemail.title"));
    
    auto desc = utils::findChild<ui::Text *>(this, "desc");
    desc->setFontName(DEF_FONT_FILE_PATH);
    desc->setString(DEF_GET_TEXT("key.accountv3.screen.updateemail.desc") + "\n" + DEF_GET_TEXT("key.accountv3.text.have.new.way.to.signin"));
    
    // textfields
    m_textfields = std::vector< ui::TextField *> {
        m_textfield_email,
        m_textfield_confirmcode,
    };
    
    // textfield email
    m_textfield_email->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_email->setPlaceHolder(DEF_GET_TEXT("key.enter.email"));
    m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_email->setTextColor(Color4B::BLACK);
    m_textfield_email->setCursorChar('|');
    m_textfield_email->setCursorEnabled(true);
    m_textfield_email->setMaxLengthEnabled(true);
    m_textfield_email->setMaxLength(DEF_EMAIL_MAX_LENGTH);
    
    m_textfield_email->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                this->moveUpForm();
            }
                break;
                
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_email->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_email);
                this->moveDownForm();
            }
                break;
#endif
            case TextField::EventType::INSERT_TEXT: {
                m_textfield_email->setString(mj::helper::str_trim(m_textfield_email->getString()));
                if (m_textfield_email->getString().empty())
                {
                    m_button_sendcode->setEnabled(false);
                }
                else if (!m_isTimerRunning)
                {
                    m_button_sendcode->setEnabled(true);
                }
            }
            case TextField::EventType::DELETE_BACKWARD:
            {
                m_textfield_email->setString(mj::helper::str_trim(m_textfield_email->getString()));
                
                if (m_textfield_email->getString().empty())
                {
                    m_button_sendcode->setEnabled(false);
                }
                else if (!m_isTimerRunning)
                {
                    m_button_sendcode->setEnabled(true);
                }
            }
                break;
                
            default:
                break;
        }
    });
    
    // textfield confirm code
    m_textfield_confirmcode->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_confirmcode->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.confirmotp.activationcode"));
    m_textfield_confirmcode->setCursorChar('|');
    m_textfield_confirmcode->setCursorEnabled(true);
    m_textfield_confirmcode->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_confirmcode->setTextColor(Color4B::BLACK);
    m_textfield_confirmcode->setMaxLengthEnabled(true);
    m_textfield_confirmcode->setMaxLength(DEF_OTP_LENGTH);
    m_textfield_confirmcode->setEnabled(false);
    
    m_textfield_confirmcode->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_confirmcode->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                this->moveUpForm();
				break;
            }
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_confirmcode->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_confirmcode);
                this->moveDownForm();
				break;
            }
            case TextField::EventType::INSERT_TEXT:
            case TextField::EventType::DELETE_BACKWARD:
            {
                this->trimSpace(m_textfield_confirmcode);
                
                if (m_isCodeSent && m_textfield_confirmcode->getString().length() >= DEF_OTP_LENGTH)
                {
                    m_button_update->setEnabled(true);
                }
                else
                {
                    m_button_update->setEnabled(false);
                }
				break;
            }
            default:
                break;
        }
    });
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        title->setTextColor(Color4B::WHITE);
        desc->setTextColor(Color4B::WHITE);
        
        if (auto background = utils::findChild<ui::Layout *>(this, "background"))
        {
            background->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
        }
        
        if (auto button_back = cocos2d::utils::findChild<ui::Button *>(this, "btn_back"))
        {
            button_back->loadTextureNormal("account/new_v3_vn/button_back_left.png");
            button_back->loadTexturePressed("account/new_v3_vn/button_back_left.png");
            button_back->resetNormalRender();
            button_back->resetPressedRender();
            button_back->resetDisabledRender();
            button_back->getRendererNormal()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererClicked()->setPosition(button_back->getContentSize()/2);
            button_back->getRendererDisabled()->setPosition(button_back->getContentSize()/2);
        }
        
        m_button_sendcode->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
        m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->effectShowing();
    
    return true;
}

void APAccountUpdateEmail::update(float dt)
{
    // bộ đếm thời gian
    if (m_isTimerRunning)
    {
        m_timeCounter -= dt;
        
        if (m_timeCounter < 0)
        {
            this->turnOffResendTimer();
        }
        else
        {
            m_button_sendcode->setTitleText(APACCOUNTUPDATEEMAIL_TITLE_BUTTON_CUR_REMAIN_TIME);
        }
    }
}

void APAccountUpdateEmail::turnOnResendTimer()
{
    m_isCodeSent = false;
    m_isTimerRunning = true;
    m_timeCounter = DEF_TIME_LIMIT_FOR_RESEND_OTP;
    
    m_button_sendcode->setEnabled(false);
    m_button_sendcode->setTitleText(APACCOUNTUPDATEEMAIL_TITLE_BUTTON_CUR_REMAIN_TIME);
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
    {
        m_button_sendcode->setTitleFontSize(22.f);
    }
    else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
    {
        m_button_sendcode->setTitleFontSize(24.f);
    }
}

void APAccountUpdateEmail::turnOffResendTimer()
{
    m_isTimerRunning = false;
    
    if (!m_textfield_email->getString().empty())
    {
        m_button_sendcode->setEnabled(true);
    }
    
    m_button_sendcode->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.resendcode"));
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
    {
        m_button_sendcode->setTitleFontSize(22.f);
    }
    else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
    {
        m_button_sendcode->setTitleFontSize(27.f);
    }
}

void APAccountUpdateEmail::callAPISendOTP()
{
    if (mj::helper::isValidEmail(m_email))
    {
        // TODO: call API: SendOpt
        ap::api::SendOpt::call(true, "", m_email, CC_CALLBACK_2(APAccountUpdateEmail::onSendOptAPICalled, this));
        
        this->turnOnResendTimer();
        
        m_textfield_confirmcode->setEnabled(true);
    }
}

void APAccountUpdateEmail::onSendOptAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            m_isCodeSent = true;
            
            // thông báo đã gửi mã OTP về địa chỉ email
            APPopupBase::showErrorAlert(__String::createWithFormat(DEF_GET_TEXT("key.accountv3.screen.updateemail.popup.sendotp").c_str(), m_email.c_str())->getCString());
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            this->turnOffResendTimer();
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountUpdateEmail::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountUpdateEmail, onBack),
        CLICK_MAP(APAccountUpdateEmail, onUpdate),
        CLICK_MAP(APAccountUpdateEmail, onSendCode),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountUpdateEmail::onUpdate(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfields);
    
    auto code = m_textfield_confirmcode->getString();
    
    if (!code.empty() && m_email.compare(m_textfield_email->getString()) == 0)
    {
        m_button_update->setEnabled(false);
        
        // TODO: call API: CommomVerifyOTP
        ap::api::CommomVerifyOTP::call(code, m_email, CC_CALLBACK_3(APAccountUpdateEmail::onCommomVerifyOTPAPICalled, this));
    }
    else
    {
        CCLOG("ERROR: somthing wrongs!");
    }
}

void APAccountUpdateEmail::onCommomVerifyOTPAPICalled(bool is_success, std::string msg, int code)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            m_state = APAccountUpdateEmail::State::EMAIL_UPDATE_SUCCESS;
            
            // TODO: Save email to database
            auto user_info = ap::Database::getInstance().getUserInfo();
            ap::Database::getInstance().saveUserInfo(std::to_string(user_info._id), user_info._name, m_email, user_info._phone, user_info._address, user_info._userName);
            
            // TODO: CleverTap-pushEvent: "update_user_info"
            try
            {
                CleverTapManager::GetInstance()->pushEvent("update_user_info",
                {
                    {"click_type", cocos2d::Value("email")},
                    {"update_type", cocos2d::Value(user_info._mail.length() ? "update" : "add")},
                    {"email_verified", cocos2d::Value("yes")},
                    {"update_info_successfull", cocos2d::Value("yes")},
                    {"source_update", cocos2d::Value("organic")},
                });
            }
            catch (const std::exception&)
            {
                
            }
            
            if (m_account_backend)
            {
                m_account_backend->resetAccountInfoScreen();
            }
            
            if (MJDEFAULT->getBoolForKey("need_to_fill_password"))
            {
                auto password = MJDEFAULT->getStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD);
                if (password.empty()) {
                    CCLOG("screen APAccountUpdatePhone: WTF???");
                    this->removeFromParentWithEffect();
                    return;
                }
                
                // TODO: call API: UpdatePassword
                ap::api::UpdatePassword::call("", password, CC_CALLBACK_2(APAccountUpdateEmail::onUpdatePasswordAPICalled, this));
                
                this->runAction(FadeOut::create(0.25f));
            }
            else
            {
                // Cập nhật thành công thì cho tắt màn hình luôn
                this->removeFromParentWithEffect();
            }
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            if (m_textfield_confirmcode->getString().length() >= DEF_OTP_LENGTH)
            {
                m_button_update->setEnabled(true);
            }
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}

void APAccountUpdateEmail::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([is_success, msg, this]
    {
        if (is_success)
        {
            // TODO: Save value for key "need_to_fill_password"
            MJDEFAULT->setBoolForKey("need_to_fill_password", false);
            MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, "");
            
            if (m_account_backend)
            {
                m_account_backend->resetAccountInfoScreen();
            }
        }
        else
        {
            APPopupBase::showErrorAlert(msg);
        }
        
        this->removeFromParent();
    });
}

void APAccountUpdateEmail::removeFromParentWithEffect()
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void APAccountUpdateEmail::onBack(cocos2d::Ref * sender)
{
    if (m_state != APAccountUpdateEmail::State::EMAIL_UPDATE_SUCCESS)
    {
        // TODO: CleverTap-pushEvent: "update_user_info"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("update_user_info",
            {
                {"click_type", cocos2d::Value("email")},
                {"update_type", cocos2d::Value(ap::Database::getInstance().getUserInfo()._mail.length() ? "update" : "add")},
                {"email_verified", cocos2d::Value(m_button_sendcode_counter ? "no" : "none")},
                {"update_info_successfull", cocos2d::Value("no")},
                {"source_update", cocos2d::Value("organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
    
    this->removeFromParentWithEffect();
}

void APAccountUpdateEmail::onSendCode(cocos2d::Ref * sender)
{
    m_button_sendcode_counter++;
    
    this->hideVirtualKeyboard(m_textfields);
    
    if (mj::helper::isValidEmail(m_textfield_email->getString()))
    {
        if (m_email_old.compare(m_textfield_email->getString()) == 0)
        {
            APPopupBase::showErrorAlert(DEF_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
        }
        else
        {
            m_state = APAccountUpdateEmail::State::NONE;
            
            m_email = m_textfield_email->getString();
            
            this->callAPISendOTP();
        }
    }
    else
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("key.accountv3.popup.enteremail.invalid.email"));
    }
}
