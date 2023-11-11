//
//  APAccountConfirmPassword.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#include "APAccountConfirmPassword.h"
#include "StoryLanguageManager.h"
#include "APAccountConfirmOTP.h"
#include "APAccountUpdateEmail.h"
#include "APAccountUpdatePhone.h"
#include "APUpdatePasswordAPI.h"
#include "APResetPasswordScreen.h"
#include "APDatabase.h"
#include "StoryAlert.h"
#include "MSHome.h"
#include "CleverTapManager.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountConfirmPassword);

std::once_flag v3_ap_account_confirm_password_reader_flag;

#pragma mark - Initialize
APAccountConfirmPassword * APAccountConfirmPassword::createView(DEF_APACCOUNT_FLOW flow)
{
    std::call_once(v3_ap_account_confirm_password_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountConfirmPassword);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountConfirmPassword.csb");
    auto p = static_cast<APAccountConfirmPassword *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB(flow))
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountConfirmPassword::onExit()
{
    this->hideVirtualKeyboard(m_textfield_password);
    
    APPopupBase::onExit();
}

bool APAccountConfirmPassword::didLoadFromCSB(DEF_APACCOUNT_FLOW flow)
{
	HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

    this->setName(name_screen_apaccount_confirm_password);
    
    m_flow = flow;
    
    // button confirm
    m_button_confirm = utils::findChild<ui::Button *>(this, "btn_confirm");
    m_button_confirm->setEnabled(false);
    m_button_confirm->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_confirm->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.confirm"));
    m_button_confirm->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // button forgot
    auto password_bg = utils::findChild<ui::ImageView *>(this, "password_bg");
    m_button_forgot = utils::findChild<ui::Button *>(this, "btn_forgot");
    m_button_forgot->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_forgot->setTitleText(DEF_GET_TEXT("ap_forgot_pass_title") + "?");
    m_button_forgot->setPosition(Point(password_bg->getPosition().x +
                                       password_bg->getContentSize().width/2 +
                                       (m_button_forgot->getContentSize().width - m_button_forgot->getTitleLabel()->getContentSize().width)/2,
                                       m_button_forgot->getPosition().y));
    
    // title & desc
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setFontName(DEF_FONT_FILE_PATH);
    title->setString(DEF_GET_TEXT("key.accountv3.screen.confirmpassword.title"));
    
    auto desc = utils::findChild<ui::Text *>(this, "desc");
    desc->setFontName(DEF_FONT_FILE_PATH);
    desc->setString(DEF_GET_TEXT("key.accountv3.screen.confirmpassword.desc"));
    
    // textfield
    m_textfield_password = utils::findChild<ui::TextField *>(this, "password_textfield");
    m_textfield_password->setFontName(DEF_FONT_FILE_PATH);
    m_textfield_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.renew.password"));
    m_textfield_password->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_textfield_password->setTextColor(Color4B::BLACK);
    m_textfield_password->setCursorChar('|');
    m_textfield_password->setCursorEnabled(true);
    m_textfield_password->setMaxLengthEnabled(true);
    m_textfield_password->setMaxLength(PASSWORD_MAX_LENGTH);
    
    m_textfield_password->addEventListener([this](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
    {
        switch (type)
        {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
            {
                m_textfield_password->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                this->moveUpForm();
            }
                break;
                
            case TextField::EventType::DETACH_WITH_IME:
            {
                m_textfield_password->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                this->removeSpace(m_textfield_password);
                this->moveDownForm();
            }
                break;
#endif
            case TextField::EventType::INSERT_TEXT:
            {
                this->trimSpace(m_textfield_password);
                this->enableWidgetNodeBasedOnTextLength(m_textfield_password->getString(), PASSWORD_MIN_LENGTH, m_button_confirm);
				        break;
            }
            case TextField::EventType::DELETE_BACKWARD:
            {
                this->trimSpace(m_textfield_password);
                this->enableWidgetNodeBasedOnTextLength(m_textfield_password->getString(), PASSWORD_MIN_LENGTH, m_button_confirm);
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
        
        m_button_confirm->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->effectShowing();
    
    return true;
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountConfirmPassword::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountConfirmPassword, onBack),
        CLICK_MAP(APAccountConfirmPassword, onForgot),
        CLICK_MAP(APAccountConfirmPassword, onConfirm),
        CLICK_MAP(APAccountConfirmPassword, onShowPasswordButtonClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountConfirmPassword::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    
    if (!button)
    {
        CCLOG("Error|APAccountAddPassword::onShowPasswordButtonClicked");
        return;
    }
    
    bool isShowPassword = m_textfield_password->isPasswordEnabled();
    
    if(auto eye = utils::findChild<ImageView*>(button,"eye"))
    {
        eye->loadTexture(isShowPassword? "new_v2/eye.png": "new_v2/eye_enable.png");
    }
    
    m_textfield_password->setPasswordEnabled(!isShowPassword);
}

void APAccountConfirmPassword::onBack(cocos2d::Ref * sender)
{
    if (m_flow == DEF_APACCOUNT_FLOW::ENTER_INFO)
    {
        auto runningScene = Director::getInstance()->getRunningScene();
        
        if (runningScene->getName().compare(ms_scene_name_home) == 0)
        {
            ((MSHome *)runningScene->getChildByName(MSHOME_NAME))->goToAccountLayout();
        }
    }
    else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL || m_flow == DEF_APACCOUNT_FLOW::UPDATE_PHONE)
    {
        // TODO: CleverTap-pushEvent: "update_user_info"
        try
        {
            auto user_info = ap::Database::getInstance().getUserInfo();
            
            CleverTapManager::GetInstance()->pushEvent("update_user_info",
            {
                {"click_type", cocos2d::Value(m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL ? "email" : "phone")},
                {"update_type", cocos2d::Value((m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL && user_info._mail.length()) ||
                                               (m_flow == DEF_APACCOUNT_FLOW::UPDATE_PHONE && user_info._phone.length()) ? "update" : "add")},
                {"email_verified", cocos2d::Value("none")},
                {"update_info_successfull", cocos2d::Value("no")},
                {"source_update", cocos2d::Value("organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
    
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void APAccountConfirmPassword::onForgot(cocos2d::Ref * sender)
{
    auto user_info = ap::Database::getInstance().getUserInfo();
    auto phone = user_info._phone;
    auto email = user_info._mail;
    
    if (phone.length() || email.length())
    {
        avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));
        
        this->hideVirtualKeyboard(m_textfield_password);
        
        auto resetType = APResetPasswordScreen::RESET_TYPE::PHONE;
        
        if (phone.length() == 0)
        {
            resetType = APResetPasswordScreen::RESET_TYPE::EMAIL;
        }
        else if (email.length() == 0)
        {
            resetType = APResetPasswordScreen::RESET_TYPE::PHONE;
        }
        else
        {
            resetType = (m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL) ? APResetPasswordScreen::RESET_TYPE::EMAIL : APResetPasswordScreen::RESET_TYPE::PHONE;
        }
        
        auto screen = APResetPasswordScreen::createView(resetType, m_flow);
        
        screen->setTextfieldsWithFixedConfig(phone, email);
        
        animationMoveToLeft(screen);
        
        Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX-1);
    }
    else
    {
        CCLOG("hieunt: something wrong!");
    }
}

void APAccountConfirmPassword::onConfirm(cocos2d::Ref * sender)
{
    if (m_textfield_password->getString().length() < PASSWORD_MIN_LENGTH)
    {
        this->hideVirtualKeyboard(m_textfield_password);
    // thông báo mật khẩu phải từ 8 ký tự trở lên
        APPopupBase::showErrorAlert(DEF_GET_TEXT("ap_waring_password"));
        return;
    }
    if (!m_textfield_password->getString().empty())
    {
        this->hideVirtualKeyboard(m_textfield_password);
        
        m_button_confirm->setEnabled(false);
        m_textfield_password->setEnabled(false);
        
        // TODO: call API: UpdatePassword
        ap::api::UpdatePassword::call(m_textfield_password->getString(), "", CC_CALLBACK_2(APAccountConfirmPassword::onUpdatePasswordAPICalled, this));
    }
    else
    {
        CCLOG("ERROR: password is empty!");
    }
}

#pragma mark - API: UpdatePassword
void APAccountConfirmPassword::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            if (m_flow == DEF_APACCOUNT_FLOW::ENTER_INFO)
            {
                auto temp_email = MJDEFAULT->getStringForKey(DEF_TEMP_EMAIL_FOR_ENTER_INFO_THREAD);
                auto screen_confirmEmail = APAccountConfirmOTP::createView(temp_email);
                screen_confirmEmail->callAPISendOTP();
                Director::getInstance()->getRunningScene()->addChild(screen_confirmEmail, INT_MAX-1);
            }
            else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL)
            {
                auto screen_updateEmail = APAccountUpdateEmail::createView();
                screen_updateEmail->setAccountBackendLayer(m_account_backend);
                Director::getInstance()->getRunningScene()->addChild(screen_updateEmail, INT_MAX-1);
            }
            else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_PHONE)
            {
                auto screen_updatePhone = APAccountUpdatePhone::createView();
                screen_updatePhone->setAccountBackendLayer(m_account_backend);
                Director::getInstance()->getRunningScene()->addChild(screen_updatePhone, INT_MAX-1);
            }
            else
            {
                // nothing here
            }
            
            this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([this]() {
                this->removeFromParent();
            }), NULL));
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            m_button_confirm->setEnabled(true);
            m_textfield_password->setEnabled(true);
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}
