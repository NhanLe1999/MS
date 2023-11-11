#include "APChangePasswordScreen.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "APPopup.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "MJPlatformConfig.h"
#include "APLoginAPI.h"
#include "APPasswordAPI.h"
#include "APSendOTPAPI.h"
#include "APLoginView.h"
#include "CleverTapManager.h"

#include "APAccountConfirmOTP.h"
#include "APAccountUpdateEmail.h"
#include "APAccountUpdatePhone.h"
#include "APAccountConfirmPassword.h"

USING_NS_CC;
INITIALIZE_READER(APChangePasswordScreen);

std::once_flag v2_ap_change_pasword_screen_reader_flag;

APChangePasswordScreen::~APChangePasswordScreen()
{
    if ( (m_passwordChangedSuccess == false) && (m_from == START_FROM::ENTER_INFO || m_from == START_FROM::UPDATE_EMAIL || m_from == START_FROM::UPDATE_PHONE) )
    {
        // TODO: CleverTap-pushEvent: "forgot_password"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("forgot_password",
            {
                {"update_new_password", cocos2d::Value("destroy")},
                {"verify_otp_type", cocos2d::Value(m_type == TYPE::EMAIL ? "email" : "sms")},
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
}

APChangePasswordScreen * APChangePasswordScreen::createView(const START_FROM i_from, const TYPE i_type,const std::string i_mailOrPhone,const std::string i_tokenToChangePassword,const std::string i_countryCode)
{
	std::call_once(v2_ap_change_pasword_screen_reader_flag, []
	{
		REGISTER_CSB_READER(APChangePasswordScreen);
	});

	auto p = static_cast<APChangePasswordScreen*>(CSLoader::createNodeWithVisibleSize(CONFIG_MANAGER->isVietnameseStories()?"csb/account/new_v2/APChangePasswordScreen_VN.csb":"csb/account/new_v2/APChangePasswordScreen.csb"));
    
	if (p && p->didLoadFromCSB(i_from, i_type,i_mailOrPhone,i_tokenToChangePassword,i_countryCode))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}


bool APChangePasswordScreen::didLoadFromCSB(const START_FROM i_from, const TYPE i_type,const std::string i_mailOrPhone,const std::string i_tokenToChangePassword,const std::string i_countryCode)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}
 
    m_countryCode = i_countryCode;
    
    m_type = i_type;
    
    m_from = i_from;
    
    m_phoneOrEmailInput = i_mailOrPhone;
    
    m_tokenToChangePassword = i_tokenToChangePassword;
    
    m_passwordTextfields =
    {
         cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"),
         cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword"),
    };
    
    if(auto skipButton = cocos2d::utils::findChild(this, "skip_button"))
    {
        skipButton->setVisible(m_from == START_FROM::LOGIN_SCREEN);
    }
    
    for (auto inputTextfield : m_passwordTextfields)
    {
        inputTextfield->setCursorChar('|');
        inputTextfield->setCursorEnabled(true);
        inputTextfield->setMaxLengthEnabled(true);
        inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        inputTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
        inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
        inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
                                         {
            switch (type)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                case TextField::EventType::ATTACH_WITH_IME:
                    this->runAction(MoveBy::create(0.25, Vec2(0, 200)));
                    break;
                case TextField::EventType::DETACH_WITH_IME:
                    removeSpace(inputTextfield);
                    this->runAction(MoveBy::create(0.25, Vec2(0, -200)));
                    break;
#endif
                case TextField::EventType::INSERT_TEXT:
                {
                    trimSpace(inputTextfield);
                    
                    (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                    
                    break;
                }
                case TextField::EventType::DELETE_BACKWARD:
                {
                    trimSpace(inputTextfield);

                    (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                    
                   break;
                }
                default:
                    break;
            }
        });
    }
    
    if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"))
    {
        inputTextfield->setTextColor(Color4B::BLACK);
        if (m_from == START_FROM::ENTER_INFO || m_from == START_FROM::UPDATE_PHONE || m_from == START_FROM::UPDATE_EMAIL)
        {
            inputTextfield->setString(i_mailOrPhone);
        }
        else
        {
            inputTextfield->setString((m_type == TYPE::PHONE ? m_countryCode: "") + i_mailOrPhone);
        }
        inputTextfield->setEnabled(false);
        inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        inputTextfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
    }
    
	return true;
}

void APChangePasswordScreen::onSaveButtonClicked(cocos2d::Ref * sender)
{
    auto saveButton = dynamic_cast<ui::Button*>(sender);
    
    if(saveButton)
    {
        saveButton->setTouchEnabled(false);
    }
    
    auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input");
    
    auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password");
    
    auto reenteringPasswordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword");
    
    hideVirtualKeyboard(m_passwordTextfields);
    
    removeSpace(passwordTextfield);
    
    removeSpace(reenteringPasswordTextfield);

    if (passwordTextfield->getString().empty() )
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password"));
        
        saveButton->setTouchEnabled(true);
        
        return;
    }
    
    if (reenteringPasswordTextfield->getString().empty())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.reender.password"));
        
        saveButton->setTouchEnabled(true);
        
        return;
    }
    
    if (passwordTextfield->getString().size() < PASSWORD_MIN_LENGTH || reenteringPasswordTextfield->getString().size() < PASSWORD_MIN_LENGTH)
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_password"));

        saveButton->setTouchEnabled(true);

        return;
    }
    
    if (!passwordTextfield->getString().empty() && !reenteringPasswordTextfield->getString().empty() && passwordTextfield->getString() != reenteringPasswordTextfield->getString())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_match_password"));
        
        saveButton->setTouchEnabled(true);
        
        return;
    }
    
    const std::string k_phoneText   = ( m_type == TYPE::EMAIL ? "":m_phoneOrEmailInput);
    
    const std::string k_emailText   = ( m_type == TYPE::EMAIL ? inputTextfield->getString():"");

    const std::string k_password    = passwordTextfield->getString();
    
    showWaittingView();
    
    if(m_from == START_FROM::RESET_PASS_SCREEN)
    {        
        if(auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
        {
            if(m_type == TYPE::EMAIL)
            {
                loginView->onGoogle(this,false);
            }
            else
            {
                loginView->onPhone(this);
            }
        }
    }
    
    ap::api::APPasswordAPI::changePasswordV2Call(k_emailText, k_phoneText, k_password,m_tokenToChangePassword,m_countryCode, [=](bool i_success, std::string i_msg, const rapidjson::Value &i_data) {

        if (i_success)
        {
            m_passwordChangedSuccess = true;
            
            if (m_from == START_FROM::ENTER_INFO || m_from == START_FROM::UPDATE_EMAIL || m_from == START_FROM::UPDATE_PHONE)
            {
                // TODO: CleverTap-pushEvent: "forgot_password"
                try
                {
                    CleverTapManager::GetInstance()->pushEvent("forgot_password",
                    {
                        {"update_new_password", cocos2d::Value("yes")},
                        {"verify_otp_type", cocos2d::Value(m_type == TYPE::EMAIL ? "email" : "sms")},
                    });
                }
                catch (const std::exception&)
                {
                    
                }
            }
            
            if (m_from == START_FROM::ENTER_INFO)
            {
                removeWaittingView();
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, saveButton, k_phoneText,k_emailText,k_password, passwordTextfield] {
                    auto temp_email = MJDEFAULT->getStringForKey(DEF_TEMP_EMAIL_FOR_ENTER_INFO_THREAD);
                    auto screen_confirmEmail = APAccountConfirmOTP::createView(temp_email);
                    screen_confirmEmail->callAPISendOTP();
                    
                    auto runningScene = Director::getInstance()->getRunningScene();
                    runningScene->addChild(screen_confirmEmail, INT_MAX-1);
                    
                    if (auto screen_confirm = (APAccountConfirmPassword *)runningScene->getChildByName(name_screen_apaccount_confirm_password))
                    {
                        screen_confirm->removeFromParent();
                    }
                    
                    this->removeFromParent();
                });
            }
            else if(m_from == START_FROM::UPDATE_PHONE)
            {
                removeWaittingView();
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, saveButton, k_phoneText,k_emailText,k_password, passwordTextfield] {
                    auto runningScene = Director::getInstance()->getRunningScene();
                    auto screen_updatePhone = APAccountUpdatePhone::createView();
                    runningScene->addChild(screen_updatePhone, INT_MAX-1);
                    
                    auto screen_confirm = (APAccountConfirmPassword *)runningScene->getChildByName(name_screen_apaccount_confirm_password);
                    
                    if (screen_confirm)
                    {
                        screen_updatePhone->setAccountBackendLayer(screen_confirm->getAccountBackend());
                        screen_confirm->removeFromParent();
                    }
                    
                    this->removeFromParent();
                });
            }
            else if(m_from == START_FROM::UPDATE_EMAIL)
            {
                removeWaittingView();
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, saveButton, k_phoneText,k_emailText,k_password, passwordTextfield] {
                    auto runningScene = Director::getInstance()->getRunningScene();
                    auto screen_updateEmail = APAccountUpdateEmail::createView();
                    runningScene->addChild(screen_updateEmail, INT_MAX-1);
                    
                    auto screen_confirm = (APAccountConfirmPassword *)runningScene->getChildByName(name_screen_apaccount_confirm_password);
                    
                    if (screen_confirm)
                    {
                        screen_updateEmail->setAccountBackendLayer(screen_confirm->getAccountBackend());
                        screen_confirm->removeFromParent();
                    }
                    
                    this->removeFromParent();
                });
            }
            else if(m_from == START_FROM::LOGIN_SCREEN)
            {
                removeWaittingView();
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, saveButton, k_phoneText,k_emailText,k_password, passwordTextfield] {
                    
                    MJDEFAULT->setBoolForKey("account_unverified", false);
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
                    
                    this->removeFromParent();
                    
                });
            }
            else
            {
                //APLoginView
                ap::api::Login::callV3(m_type == TYPE::EMAIL ? ap::api::Login::LoginType::GOOGLE:ap::api::Login::LoginType::PHONE, "", k_phoneText, k_emailText, "", k_password,"","","", [=](int code, bool is_success, std::string msg2,std::string tokenToChangePassword, ap::api::Login::LoginDataInfo info)  {
                    
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, saveButton,is_success, msg2, code,tokenToChangePassword,k_phoneText] {
                        
                        if (is_success)
                        {
                            MJDEFAULT->setBoolForKey("account_unverified", false);
                            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
                            
                            this->removeFromParent();
                            
                        }
                        else
                        {
                            removeWaittingView();

                            showFailedPopup(msg2);
                            
                            saveButton->setTouchEnabled(true);
                        }
                    });
                    
                });
            }
        }
        else
        {
            removeWaittingView();
            
            showFailedPopup(i_msg);
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, i_success, i_msg, saveButton, k_phoneText,k_emailText,k_password, passwordTextfield] {
                
                saveButton->setTouchEnabled(true);
                
            });
        }
    });
}

void APChangePasswordScreen::onSkipButton(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);
    
    if (!button)
    {
        CCLOG("Error|APRegisterScreen::onShowPasswordButtonClicked");
        
        return;
    }
    
    button->setTouchEnabled(false);
    
    MJDEFAULT->setBoolForKey("account_unverified", false);
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
    
    this->removeFromParent();

}

void APChangePasswordScreen::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

    if (!button)
    {
        CCLOG("Error|APRegisterScreen::onShowPasswordButtonClicked");

        return;
    }

    const int k_tag = button->getTag();

    if(auto textFiled =  cocos2d::utils::findChild<ui::TextField*>(this,k_tag == SHOW_PASS_BUTTON_TAG_IN_PASSWORD_TEXTFIELD ? "textfield_password":"textfield_repassword"))
    {
        bool isShowPassword = textFiled->isPasswordEnabled();
        
        if(auto eye = utils::findChild<ImageView*>(button,"eye"))
        {
            eye->loadTexture(isShowPassword? "new_v2/eye.png": "new_v2/eye_enable.png");
        }
        
        textFiled->setPasswordEnabled(!isShowPassword);
    }
}

void APChangePasswordScreen::changeDisplayLanguage()
{
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "title"),
            utils::findChild<ui::Text *>(this, "small_title"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }
    
    if(auto skipButton = cocos2d::utils::findChild<ui::Button*>(this, "skip_button"))
    {
        skipButton->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        skipButton->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.skip"));
    }
    
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.reset.password.tiltle"));
	}
    
    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "small_title"))
    {
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.reset.password.tiltle2"));
    }

    if (auto passwordTextField = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"))
    {
        passwordTextField->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        passwordTextField->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_password"));
    }
    
    if (auto passwordTextField = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword"))
    {
        passwordTextField->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        passwordTextField->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_reenter_password"));
    }

    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "save_button"))
    {
        button->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
        button->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.save"));
    }
    
}

cocos2d::ui::Widget::ccWidgetClickCallback APChangePasswordScreen::onLocateClickCallback(const std::string & callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
		CLICK_MAP(APChangePasswordScreen, onSaveButtonClicked),
		CLICK_MAP(APChangePasswordScreen, onSkipButton),
		CLICK_MAP(APChangePasswordScreen, onShowPasswordButtonClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }
	return nullptr;
}


