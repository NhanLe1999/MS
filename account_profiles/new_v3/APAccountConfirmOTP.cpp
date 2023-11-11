//
//  APAccountConfirmOTP.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/23/20.
//

#include "APAccountConfirmOTP.h"
#include "StoryLanguageManager.h"
#include "APVerifyOptForEmailAPI.h"
#include "APSendOptAPI.h"
#include "APLoadUpdateAPI.h"
#include "APUpdatePasswordAPI.h"
#include "APDatabase.h"
#include "MSHome.h"
#include "APAccountUserInfo.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountConfirmOTP);

std::once_flag v3_ap_account_confirm_otp_reader_flag;

#pragma mark - Initialize
APAccountConfirmOTP * APAccountConfirmOTP::createView(const std::string& email)
{
    std::call_once(v3_ap_account_confirm_otp_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountConfirmOTP);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountConfirmOTP.csb");
    auto p = static_cast<APAccountConfirmOTP *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB(email))
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountConfirmOTP::onExit()
{
    this->hideVirtualKeyboard(m_textfield_code);
    
    APPopupBase::onExit();
}

bool APAccountConfirmOTP::didLoadFromCSB(const std::string& email)
{
    this->setName(name_screen_apaccount_confirm_otp);
    
    m_email = email;
    m_textfield_code = cocos2d::utils::findChild<ui::TextField *>(this, "confirmcode_textfield");
    m_button_sendcode = cocos2d::utils::findChild<ui::Button *>(this, "btn_sendcode");
    m_button_confirm = cocos2d::utils::findChild<ui::Button *>(this, "btn_confirm");
    
    // button confirm
    m_button_confirm->setEnabled(false);
    m_button_confirm->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_confirm->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.confirm"));
    m_button_confirm->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // button send_code
    m_button_sendcode->setEnabled(false);
    m_button_sendcode->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_sendcode->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.resendcode"));
    m_button_sendcode->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // textfield confirm_code
    m_textfield_code->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_code->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.confirmotp.activationcode"));
    m_textfield_code->setCursorChar('|');
    m_textfield_code->setCursorEnabled(true);
    m_textfield_code->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_code->setTextColor(Color4B::BLACK);
    m_textfield_code->setMaxLengthEnabled(true);
    m_textfield_code->setMaxLength(DEF_OTP_LENGTH);
    
    m_textfield_code->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_code->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                this->moveUpForm();
            }
                break;
                
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_code->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_code);
                this->moveDownForm();
            }
                break;
#endif
            case TextField::EventType::INSERT_TEXT:
            case TextField::EventType::DELETE_BACKWARD:
            {
//                this->trimSpace(m_textfield_code);
                validPhoneInput(m_textfield_code);
                this->enableWidgetNodeBasedOnTextLength(m_textfield_code->getString(), OTP_MIN_LENGTH, m_button_confirm);
            }
                break;
                
            default:
                break;
        }
    });
    
    // title & desc
    auto text_title = cocos2d::utils::findChild<ui::Text *>(this, "title");
    text_title->setFontName(DEF_FONT_FILE_PATH);
    text_title->setString(DEF_GET_TEXT("key.accountv3.screen.confirmotp.title"));
    
    auto text_desc = cocos2d::utils::findChild<ui::Text *>(this, "desc");
    text_desc->setFontName(DEF_FONT_FILE_PATH);
    text_desc->setString(__String::createWithFormat(DEF_GET_TEXT("key.accountv3.screen.confirmotp.desc").c_str(), m_email.c_str())->getCString());
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
    {
        text_desc->setFontSize(29.f);
    }
    else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
    {
        m_button_sendcode->setTitleFontSize(29.f);
    }
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        text_title->setTextColor(Color4B::WHITE);
        text_desc->setTextColor(Color4B::WHITE);
        
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
        m_button_confirm->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->effectShowing();
    
    return true;
}

#pragma mark - Zzz
void APAccountConfirmOTP::callAPISendOTP()
{
    if (m_email.length())
    {
        // TODO: call API: SendOpt
        ap::api::SendOpt::call(true, " ", m_email, CC_CALLBACK_2(APAccountConfirmOTP::onSendOptAPICalled, this));
        
        m_isTimerRunning = true;
        m_timeCounter = DEF_TIME_LIMIT_FOR_RESEND_OTP;
        
        this->scheduleUpdate();
        
        if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
        {
            m_button_sendcode->setTitleFontSize(26.f);
        }
        else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
        {
            m_button_sendcode->setTitleFontSize(26.f);
        }
        
        m_button_sendcode->setEnabled(false);
        m_button_sendcode->setTitleText(APACCOUNTCONFIRMOTP_TITLE_BUTTON_CUR_REMAIN_TIME);
    }
}

void APAccountConfirmOTP::onSendOptAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            this->enableWidgetNodeBasedOnTextLength(m_textfield_code->getString(), OTP_MIN_LENGTH, m_button_confirm);
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}

void APAccountConfirmOTP::update(float dt)
{
    if (m_isTimerRunning)
    {
        m_timeCounter -= dt;
        
        if (m_timeCounter < 0)
        {
            m_isTimerRunning = false;
            
            this->unscheduleUpdate();
            m_button_sendcode->setEnabled(true);
            m_button_sendcode->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.resendcode"));
            
            if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_EN_CODE)
            {
                m_button_sendcode->setTitleFontSize(34.f);
            }
            else if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
            {
                m_button_sendcode->setTitleFontSize(34.f);
            }
        }
        else
        {
            m_button_sendcode->setTitleText(APACCOUNTCONFIRMOTP_TITLE_BUTTON_CUR_REMAIN_TIME);
        }
    }
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountConfirmOTP::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountConfirmOTP, onBack),
        CLICK_MAP(APAccountConfirmOTP, onSendCode),
        CLICK_MAP(APAccountConfirmOTP, onConfirmCode),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountConfirmOTP::removeFromParentWithEffect()
{
    auto runningScene = Director::getInstance()->getRunningScene();
    
    if (runningScene->getName().compare(ms_scene_name_home) == 0)
    {
        ((MSHome *)runningScene->getChildByName(MSHOME_NAME))->goToAccountLayout();
    }
    
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void APAccountConfirmOTP::onBack(cocos2d::Ref * sender)
{
    // TODO: CleverTap-pushEvent: "update_user_info"
    try
    {
        CleverTapManager::GetInstance()->pushEvent("update_user_info",
        {
            {"click_type", cocos2d::Value("email")},
            {"update_type", cocos2d::Value("add")},
            {"email_verified", cocos2d::Value(m_button_sendcode_counter ? "no" : "none")},
            {"update_info_successfull", cocos2d::Value("no")},
            {"source_update", cocos2d::Value("pop_up")},
        });
    }
    catch (const std::exception&)
    {
        
    }
    
    this->removeFromParentWithEffect();
}

void APAccountConfirmOTP::onSendCode(cocos2d::Ref * sender)
{
    m_button_sendcode_counter++;
    
    this->hideVirtualKeyboard(m_textfield_code);
    this->callAPISendOTP();
}

void APAccountConfirmOTP::onConfirmCode(cocos2d::Ref * sender)
{
    this->hideVirtualKeyboard(m_textfield_code);
    
    auto code = m_textfield_code->getString();
    
    if (!code.empty())
    {
        m_button_confirm->setEnabled(false);
        
        // TODO: call API: CommomVerifyOTP
        ap::api::CommomVerifyOTP::call(code, m_email, CC_CALLBACK_3(APAccountConfirmOTP::onCommomVerifyOTPAPICalled, this));
    }
    else
    {
        CCLOG("ERROR: confirmation code is empty!");
    }
}

void APAccountConfirmOTP::onCommomVerifyOTPAPICalled(bool is_success, std::string msg, int code)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            auto user_info = ap::Database::getInstance().getUserInfo();
            
            // TODO: Save email to database
            ap::Database::getInstance().saveUserInfo(std::to_string(user_info._id), user_info._name, m_email, user_info._phone, user_info._address, user_info._userName);
            
            // TODO: CleverTap-pushEvent: "update_user_info"
            try
            {
                CleverTapManager::GetInstance()->pushEvent("update_user_info",
                {
                    {"click_type", cocos2d::Value("email")},
                    {"update_type", cocos2d::Value("add")},
                    {"email_verified", cocos2d::Value("yes")},
                    {"update_info_successfull", cocos2d::Value("yes")},
                    {"source_update", cocos2d::Value("pop_up")},
                });
            }
            catch (const std::exception&)
            {
                
            }
            
            if (MJDEFAULT->getBoolForKey("need_to_fill_password"))
            {
                auto password = MJDEFAULT->getStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD);
                
                if (password.empty()) {
                    CCLOG("screen APAccountConfirmOTP: WTF???");
                    this->removeFromParentWithEffect();
                    return;
                }
                
                auto runningScene = Director::getInstance()->getRunningScene();
                
                if (runningScene->getName().compare(ms_scene_name_home) == 0)
                {
                    ((MSHome *)runningScene->getChildByName(MSHOME_NAME))->goToAccountLayout();
                }
                
                // TODO: call API: UpdatePassword
                ap::api::UpdatePassword::call("", password, CC_CALLBACK_2(APAccountConfirmOTP::onUpdatePasswordAPICalled, this));
                
                this->runAction(FadeOut::create(0.25f));
            }
            else
            {
                this->removeFromParentWithEffect();
            }
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            this->enableWidgetNodeBasedOnTextLength(m_textfield_code->getString(), OTP_MIN_LENGTH, m_button_confirm);
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}

void APAccountConfirmOTP::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([is_success, msg, this]
    {
        if (is_success)
        {
            // TODO: Save value for key "need_to_fill_password"
            MJDEFAULT->setBoolForKey("need_to_fill_password", false);
            MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, "");
            
            auto runningScene = Director::getInstance()->getRunningScene();
            
            if (runningScene->getName().compare(ms_scene_name_home) == 0)
            {
                ((MSHome *)runningScene->getChildByName(MSHOME_NAME))->goToAccountLayout();
            }
        }
        else
        {
            APPopupBase::showErrorAlert(msg);
        }
        
        this->removeFromParent();
    });
}
