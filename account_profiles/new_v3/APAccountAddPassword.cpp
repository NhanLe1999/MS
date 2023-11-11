//
//  APAccountAddPassword.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#include "APAccountAddPassword.h"
#include "StoryLanguageManager.h"
#include "APAccountConfirmOTP.h"
#include "APAccountUpdateEmail.h"
#include "APAccountUpdatePhone.h"
#include "APUpdatePasswordAPI.h"
#include "APAccountUserInfo.h"
#include "APDatabase.h"
#include "StoryConfigManager.h"
#include "MSHome.h"
#include "CleverTapManager.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountAddPassword);

std::once_flag v3_ap_account_add_password_reader_flag;

#pragma mark - Initialize
APAccountAddPassword * APAccountAddPassword::createView(DEF_APACCOUNT_FLOW flow)
{
    std::call_once(v3_ap_account_add_password_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountAddPassword);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountAddPassword.csb");
    auto p = static_cast<APAccountAddPassword *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB(flow))
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountAddPassword::onExit()
{
    this->hideVirtualKeyboard(m_passwordTextfields);
    
    APPopupBase::onExit();
}

bool APAccountAddPassword::didLoadFromCSB(DEF_APACCOUNT_FLOW flow)
{
    this->setName(name_screen_apaccount_add_password);
    
	HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

    MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, "");
    
    m_flow = flow;
    
    m_textfield_password = cocos2d::utils::findChild<ui::TextField *>(this, "password_textField");
    m_textfield_re_password = cocos2d::utils::findChild<ui::TextField *>(this, "re_password_textField");
    m_button_save = cocos2d::utils::findChild<ui::Button *>(this, "btn_save");
    
    // button save
    m_button_save->setEnabled(false);
    m_button_save->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_save->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.save"));
    m_button_save->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // title & desc
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setFontName(DEF_FONT_FILE_PATH);
    title->setString(DEF_GET_TEXT("key.accountv3.screen.addpassword.title"));
    
    auto desc = utils::findChild<ui::Text *>(this, "desc");
    desc->setFontName(DEF_FONT_FILE_PATH);
    desc->setString(DEF_GET_TEXT("key.accountv3.screen.addpassword.desc"));
    
    // textfields
    m_passwordTextfields = std::vector<ui::TextField *> {
        m_textfield_password,
        m_textfield_re_password,
    };
    
    for (auto textfield : m_passwordTextfields)
    {
        textfield->setFontName(DEF_FONT_FILE_PATH);
        textfield->setCursorChar('|');
        textfield->setCursorEnabled(true);
        textfield->setPlaceHolderColor(Color4B(0, 0, 0, 64));
        textfield->setTextColor(Color4B::BLACK);
        textfield->setMaxLengthEnabled(true);
        textfield->setMaxLength(PASSWORD_MAX_LENGTH);
        
        textfield->addEventListener([this, textfield](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
        {
            switch (type)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                case TextField::EventType::ATTACH_WITH_IME:
                {
                    textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0));
                    this->moveUpForm();
                }
                    break;
                    
                case TextField::EventType::DETACH_WITH_IME:
                {
                    textfield->setPlaceHolderColor(Color4B(0, 0, 0, 64));
                    this->removeSpace(textfield);
                    this->moveDownForm();
                }
                    break;
#endif
                case TextField::EventType::INSERT_TEXT:
                case TextField::EventType::DELETE_BACKWARD:
                {
                    this->trimSpace(textfield);
                }
                    break;
                    
                default:
                    break;
            }
        });
    }
    
    m_textfield_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.new.password"));
    m_textfield_re_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.renew.password"));
    
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
        
        m_button_save->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->scheduleUpdate();
    this->effectShowing();
    
    return true;
}

void APAccountAddPassword::update(float dt)
{
    (m_isWaitingForResponse == false &&
     m_textfield_password->getString().length() >= 1 &&
     m_textfield_re_password->getString().length() >= 1) ? m_button_save->setEnabled(true) : m_button_save->setEnabled(false);
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountAddPassword::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountAddPassword, onBack),
        CLICK_MAP(APAccountAddPassword, onSave),
        CLICK_MAP(APAccountAddPassword, onShowPasswordButtonClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountAddPassword::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    
    if (!button)
    {
        CCLOG("Error|APAccountAddPassword::onShowPasswordButtonClicked");
        return;
    }
    
    ui::TextField * textFiled = nullptr;
    auto buttonName = button->getName();
    
    if (buttonName.compare("show_password_btn") == 0)
    {
        textFiled = cocos2d::utils::findChild<ui::TextField *>(this, "password_textField");
    }
    else if (buttonName.compare("show_re_password_btn") == 0)
    {
        textFiled = cocos2d::utils::findChild<ui::TextField *>(this, "re_password_textField");
    }
    
    if (textFiled)
    {
        bool isShowPassword = textFiled->isPasswordEnabled();
        
        if(auto eye = utils::findChild<ImageView*>(button,"eye"))
        {
            eye->loadTexture(isShowPassword? "new_v2/eye.png": "new_v2/eye_enable.png");
        }
        
        textFiled->setPasswordEnabled(!isShowPassword);
    }
}

void APAccountAddPassword::onBack(cocos2d::Ref * sender)
{
    // TODO: CleverTap-pushEvent: "add_password"
    try
    {
        CleverTapManager::GetInstance()->pushEvent("add_password",
        {
            {"is_successfull", cocos2d::Value(m_button_save_counter ? "no" : "none")},
            {"type", cocos2d::Value("add")},
            {"source_update", cocos2d::Value(m_flow == DEF_APACCOUNT_FLOW::ENTER_INFO ? "pop_up" : "organic")},
        });
    }
    catch (const std::exception&)
    {
        
    }
    
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

void APAccountAddPassword::onSave(cocos2d::Ref * sender)
{
    m_button_save_counter++;
    
    auto password = m_textfield_password->getString();
    auto re_password = m_textfield_re_password->getString();
    
    this->hideVirtualKeyboard(m_passwordTextfields);
    
    if (password.empty())
    {
        CCLOG("ERROR: password is empty!");
    }
    else if (re_password.empty())
    {
        CCLOG("ERROR: re-password is empty!");
    }
    else if (strlen(password.c_str()) < PASSWORD_MIN_LENGTH)
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("ap_waring_password"));
    }
    else if (password.compare(re_password) != 0)
    {
        APPopupBase::showErrorAlert(DEF_GET_TEXT("ap_match_password"));
    }
    else
    {
        m_textfield_password->setEnabled(false);
        m_textfield_re_password->setEnabled(false);
        m_button_save->setEnabled(false);
        
        m_isWaitingForResponse = true;
        
        auto user_info = ap::Database::getInstance().getUserInfo();
        
        if (m_flow != DEF_APACCOUNT_FLOW::NONE)
        {
            // lưu tạm password
            MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, password);
            
            // chuyển màn
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
            
            // delay and remove this screen
            this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([this] {
                this->removeFromParent();
            }), NULL));
        }
        else
        {
            // TODO: call API: UpdatePassword
            ap::api::UpdatePassword::call("", password, CC_CALLBACK_2(APAccountAddPassword::onUpdatePasswordAPICalled, this));
        }
    }
}

// TODO: on API: UpdatePassword
void APAccountAddPassword::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        // TODO: Save value for key "need_to_fill_password"
        MJDEFAULT->setBoolForKey("need_to_fill_password", false);
        MJDEFAULT->setStringForKey(DEF_TEMP_PASSWORD_FOR_ENTER_INFO_THREAD, "");
        
        if (m_flow == DEF_APACCOUNT_FLOW::ENTER_INFO)
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                auto temp_email = MJDEFAULT->getStringForKey(DEF_TEMP_EMAIL_FOR_ENTER_INFO_THREAD);
                auto screen_confirmEmail = APAccountConfirmOTP::createView(temp_email);
                screen_confirmEmail->callAPISendOTP();
                Director::getInstance()->getRunningScene()->addChild(screen_confirmEmail, INT_MAX-1);
            });
        }
        else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_EMAIL)
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                auto screen_updateEmail = APAccountUpdateEmail::createView();
                screen_updateEmail->setAccountBackendLayer(m_account_backend);
                Director::getInstance()->getRunningScene()->addChild(screen_updateEmail, INT_MAX-1);
            });
        }
        else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_PHONE)
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                auto screen_updatePhone = APAccountUpdatePhone::createView();
                screen_updatePhone->setAccountBackendLayer(m_account_backend);
                Director::getInstance()->getRunningScene()->addChild(screen_updatePhone, INT_MAX-1);
            });
        }
        else
        {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
            {
                if (m_account_backend)
                {
                    m_account_backend->resetAccountInfoScreen();
                }
            });
        }
        
        // TODO: CleverTap-pushEvent: "add_password"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("add_password",
            {
                {"is_successfull", cocos2d::Value("yes")},
                {"type", cocos2d::Value("add")},
                {"source_update", cocos2d::Value(m_flow == DEF_APACCOUNT_FLOW::ENTER_INFO ? "pop_up" : "organic")},
            });
        }
        catch (const std::exception&)
        {
            
        }
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            this->runAction(Sequence::create(DelayTime::create(0.25f), CallFunc::create([this] {
                this->removeFromParent();
            }), NULL));
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            m_textfield_password->setEnabled(true);
            m_textfield_re_password->setEnabled(true);
            m_button_save->setEnabled(true);
            
            m_isWaitingForResponse = false;
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}
