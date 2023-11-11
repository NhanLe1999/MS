//
//  APAccountUpdatePassword.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/17/20.
//

#include "APAccountUpdatePassword.h"
#include "StoryLanguageManager.h"
#include "APUpdatePasswordAPI.h"
#include "APAccountUpdateEmail.h"
#include "APAccountUpdatePhone.h"
#include "APAccountConfirmOTP.h"
#include "MJHelper.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APAccountUpdatePassword);

std::once_flag v3_ap_account_update_password_reader_flag;

#pragma mark - Initialize
APAccountUpdatePassword * APAccountUpdatePassword::createView(DEF_APACCOUNT_FLOW flow)
{
    std::call_once(v3_ap_account_update_password_reader_flag, []
    {
        REGISTER_CSB_READER(APAccountUpdatePassword);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v3/APAccountUpdatePassword.csb");
    auto p = static_cast<APAccountUpdatePassword *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB(flow))
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

void APAccountUpdatePassword::onExit()
{
    this->hideVirtualKeyboard(m_passwordTextfields);
    
    APPopupBase::onExit();
}

bool APAccountUpdatePassword::didLoadFromCSB(DEF_APACCOUNT_FLOW flow)
{
    this->setName(name_screen_apaccount_update_password);
    
    m_flow = flow;
    
    m_textfield_old_password = utils::findChild<ui::TextField *>(this, "old_password_textField");
    m_textfield_new_password = utils::findChild<ui::TextField *>(this, "password_textField");
    m_textfield_renew_password = utils::findChild<ui::TextField *>(this, "re_password_textField");
    m_button_update = utils::findChild<ui::Button *>(this, "btn_update");
    
    // button update
    m_button_update->setEnabled(false);
    m_button_update->setTitleFontName(DEF_FONT_FILE_PATH);
    m_button_update->setTitleText(DEF_GET_TEXT("key.accountv3.button.title.save"));
    m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED);
    
    // title
    auto title = utils::findChild<ui::Text *>(this, "title");
    title->setFontName(DEF_FONT_FILE_PATH);
    title->setString(DEF_GET_TEXT("key.accountv3.screen.updatepassword.title"));
    
    // textfields
    m_passwordTextfields = std::vector<ui::TextField *> {
        m_textfield_old_password,
        m_textfield_new_password,
        m_textfield_renew_password,
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
                case TextField::EventType::INSERT_TEXT:{
                    this->trimSpace(textfield);
                }

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
    
    m_textfield_old_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.old.password"));
    m_textfield_new_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.new.password"));
    m_textfield_renew_password->setPlaceHolder(DEF_GET_TEXT("key.accountv3.screen.updatepassword.renew.password"));
    
    // MARK: -- for VMonkey --
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        title->setTextColor(Color4B::WHITE);
        
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
        
        m_button_update->getRendererDisabled()->setColor(DEF_BUTTON_COLOR_DISABLED_VM);
    }
    
    this->scheduleUpdate();
    this->effectShowing();
    
    return true;
}

void APAccountUpdatePassword::update(float dt)
{
    (m_isWaitingForResponse == false &&
     m_textfield_old_password->getString().length() >= PASSWORD_MIN_LENGTH &&
     m_textfield_new_password->getString().length() >= PASSWORD_MIN_LENGTH &&
     m_textfield_renew_password->getString().length() >= PASSWORD_MIN_LENGTH) ? m_button_update->setEnabled(true) : m_button_update->setEnabled(false);
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountUpdatePassword::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(APAccountUpdatePassword, onUpdate),
        CLICK_MAP(APAccountUpdatePassword, onBack),
        CLICK_MAP(APAccountUpdatePassword, onShowPasswordButtonClicked),
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void APAccountUpdatePassword::onUpdate(cocos2d::Ref * sender)
{
    m_button_update_counter++;
    
    this->hideVirtualKeyboard(m_passwordTextfields);
    
    auto old_password = m_textfield_old_password->getString();
    auto new_password = m_textfield_new_password->getString();
    auto renew_password = m_textfield_renew_password->getString();
    
    if (std::strlen(old_password.c_str()) == 0)
    {
        CCLOG("ERROR: old password is empty!");
    }
    else if (std::strlen(new_password.c_str()) == 0)
    {
        CCLOG("ERROR: new password is empty!");
    }
    else if (std::strlen(renew_password.c_str()) == 0)
    {
        CCLOG("ERROR: re-new password is empty!");
    }
    else if (new_password.length() < PASSWORD_MIN_LENGTH)
    {
        // thông báo mật khẩu phải từ 8 ký tự trở lên
        APPopupBase::showErrorAlert(DEF_GET_TEXT("ap_waring_password"));
    }
    else if (new_password.compare(renew_password) != 0)
    {
        // thông báo mật khẩu không khớp
        APPopupBase::showErrorAlert(DEF_GET_TEXT("ap_match_password"));
    }
    else
    {
        m_textfield_old_password->setEnabled(false);
        m_textfield_new_password->setEnabled(false);
        m_textfield_renew_password->setEnabled(false);
        m_button_update->setEnabled(false);
        
        m_isWaitingForResponse = true;
        
        // TODO: call API: UpdatePassword
        ap::api::UpdatePassword::call(old_password, new_password, CC_CALLBACK_2(APAccountUpdatePassword::onUpdatePasswordAPICalled, this));
    }
}

void APAccountUpdatePassword::removeFromParentWithEffect()
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void APAccountUpdatePassword::onBack(cocos2d::Ref * sender)
{
    // TODO: CleverTap-pushEvent: "add_password"
    try
    {
        CleverTapManager::GetInstance()->pushEvent("add_password",
        {
            {"is_successfull", cocos2d::Value(m_button_update_counter ? "no" : "none")},
            {"type", cocos2d::Value("update")},
            {"source_update", cocos2d::Value("organic")},
        });
    }
    catch (const std::exception&)
    {
        
    }
    
    this->removeFromParentWithEffect();
}

void APAccountUpdatePassword::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    
    if (!button)
    {
        CCLOG("Error|APAccountAddPassword::onShowPasswordButtonClicked");
        return;
    }
    
    ui::TextField * textFiled = nullptr;
    auto buttonName = button->getName();
    
    if (buttonName.compare("show_old_password_btn") == 0)
    {
        textFiled = m_textfield_old_password;
    }
    else if (buttonName.compare("show_password_btn") == 0)
    {
        textFiled = m_textfield_new_password;
    }
    else if (buttonName.compare("show_re_password_btn") == 0)
    {
        textFiled = m_textfield_renew_password;
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

#pragma mark - API: UpdatePassword
void APAccountUpdatePassword::onUpdatePasswordAPICalled(bool is_success, std::string msg)
{
    if (is_success)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
        {
            // thông báo update thành công
            m_state = APAccountUpdatePassword::State::UPDATE_SUCCESS;
            
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
            else if (m_flow == DEF_APACCOUNT_FLOW::UPDATE_PHONE) {
                auto screen_updateEmail = APAccountUpdatePhone::createView();
                screen_updateEmail->setAccountBackendLayer(m_account_backend);
                Director::getInstance()->getRunningScene()->addChild(screen_updateEmail, INT_MAX-1);
            }
            
            // TODO: CleverTap-pushEvent: "add_password"
            try
            {
                CleverTapManager::GetInstance()->pushEvent("add_password",
                {
                    {"is_successfull", cocos2d::Value("yes")},
                    {"type", cocos2d::Value("update")},
                    {"source_update", cocos2d::Value("organic")},
                });
            }
            catch (const std::exception&)
            {
                
            }
            
            this->removeFromParentWithEffect();
        });
    }
    else
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([msg, this]
        {
            m_textfield_old_password->setEnabled(true);
            m_textfield_new_password->setEnabled(true);
            m_textfield_renew_password->setEnabled(true);
            m_button_update->setEnabled(true);
            
            m_isWaitingForResponse = false;
            
            APPopupBase::showErrorAlert(msg);
        });
    }
}
