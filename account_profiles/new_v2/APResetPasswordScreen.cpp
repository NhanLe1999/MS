#include "APResetPasswordScreen.h"
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
#include "APOTPPopup.h"
#include "APPhoneCodePopup.h"
#include "CleverTapManager.h"
#include <regex>

USING_NS_CC;

INITIALIZE_READER(APResetPasswordScreen);

std::once_flag v2_ap_reset_password_screen_reader_flag;

APResetPasswordScreen * APResetPasswordScreen::createView(RESET_TYPE i_type, APPopupBase::Flow i_flow)
{
	std::call_once(v2_ap_reset_password_screen_reader_flag, []
	{
		REGISTER_CSB_READER(APResetPasswordScreen);
	});

	auto p = static_cast<APResetPasswordScreen*>(CSLoader::createNodeWithVisibleSize(CONFIG_MANAGER->isVietnameseStories()?"csb/account/new_v2/APResetPasswordScreen_VN.csb":"csb/account/new_v2/APResetPasswordScreen.csb"));

	if (p && p->didLoadFromCSB(i_type, i_flow))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APResetPasswordScreen::didLoadFromCSB(RESET_TYPE i_type, APPopupBase::Flow i_flow)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}
	m_resetType = i_type;
    m_flow = i_flow;
    
	if (auto continueButton = cocos2d::utils::findChild<ui::Button*>(this, "continue_button"))
	{

	}

	if (i_type == RESET_TYPE::EMAIL)
	{
		if (auto panelPhone = cocos2d::utils::findChild(this, "panel_phone"))
		{
			panelPhone->removeFromParent();
		}
	}
	else 
	{
		if (auto panelEmail = cocos2d::utils::findChild(this, "panel_email"))
		{
			panelEmail->removeFromParent();
		}
	}

	auto phoneCheckbox = utils::findChild<ui::CheckBox*>(this, "cb_phone");

	auto emailCheckbox = utils::findChild<ui::CheckBox*>(this, "cb_email");
    
    emailCheckbox->loadTextures("new_v2/cb_normal.png","new_v2/cb_normal.png", "new_v2/cb_selected.png", "new_v2/cb_normal.png", "new_v2/cb_normal.png");

    
    phoneCheckbox->loadTextures("new_v2/cb_normal.png","new_v2/cb_normal.png", "new_v2/cb_selected.png", "new_v2/cb_normal.png", "new_v2/cb_normal.png");
    
    phoneCheckbox->setSelected(i_type == RESET_TYPE::PHONE);
    
    emailCheckbox->setSelected(i_type == RESET_TYPE::EMAIL);

    phoneCheckbox->addEventListener([=](Ref * sender, cocos2d::ui::CheckBox::EventType type) {
	
        phoneCheckbox->setEnabled(false);

		if (m_resetType == RESET_TYPE::EMAIL)
		{
            hideVirtualKeyBoard();

			auto screen = APResetPasswordScreen::createView(RESET_TYPE::PHONE, m_flow);
            
            if (m_fixedConfig) screen->setTextfieldsWithFixedConfig(m_fixedPhone, m_fixedEmail, m_button_continue_counter);

			Director::getInstance()->getRunningScene()->addChild(screen, this->getLocalZOrder());

			this->removeFromParent();
		}
		else
		{
            phoneCheckbox->setEnabled(false);
		}
		
	});
    
    auto  texts =
    {
        cocos2d::utils::findChild<ui::Text*>(this, "email_text"),
        cocos2d::utils::findChild<ui::Text*>(this, "phone_text"),
    };
    
   for (auto text : texts)
    {
        auto button = Button::create();
        button->setIgnoreAnchorPointForPosition(false);
        button->setAnchorPoint(Point::ANCHOR_MIDDLE);
        button->setScale9Enabled(true);
        button->setContentSize(text->getContentSize());
        button->setPosition(text->getContentSize() / 2);
        button->addClickEventListener([=](Ref* sender)
        {
           if(text->getName() == "email_text")
           {
               emailCheckbox->setEnabled(false);
               
               if (m_resetType == RESET_TYPE::PHONE)
               {
                   hideVirtualKeyBoard();

                   auto screen = APResetPasswordScreen::createView(RESET_TYPE::EMAIL, m_flow);
                   
                   if (m_fixedConfig) screen->setTextfieldsWithFixedConfig(m_fixedPhone, m_fixedEmail, m_button_continue_counter);

                   Director::getInstance()->getRunningScene()->addChild(screen, this->getLocalZOrder());

                   this->removeFromParent();
               }
               else
               {
                   emailCheckbox->setEnabled(true);
               }
           }
           else
           {
               phoneCheckbox->setEnabled(false);

               if (m_resetType == RESET_TYPE::EMAIL)
               {
                   hideVirtualKeyBoard();

                   auto screen = APResetPasswordScreen::createView(RESET_TYPE::PHONE, m_flow);
                   
                   if (m_fixedConfig) screen->setTextfieldsWithFixedConfig(m_fixedPhone, m_fixedEmail, m_button_continue_counter);
                   
                   Director::getInstance()->getRunningScene()->addChild(screen, this->getLocalZOrder());
                   
                   this->removeFromParent();
               }
               else
               {
                   phoneCheckbox->setEnabled(false);
               }
           }

        });
        
        text->addChild(button);
    }

	emailCheckbox->addEventListener([=](Ref * sender, cocos2d::ui::CheckBox::EventType type)
    {
        emailCheckbox->setEnabled(false);
        
		if (m_resetType == RESET_TYPE::PHONE)
		{
            hideVirtualKeyBoard();

			auto screen = APResetPasswordScreen::createView(RESET_TYPE::EMAIL, m_flow);
            
            if (m_fixedConfig) screen->setTextfieldsWithFixedConfig(m_fixedPhone, m_fixedEmail, m_button_continue_counter);

			Director::getInstance()->getRunningScene()->addChild(screen, this->getLocalZOrder());

			this->removeFromParent();

		}
		else
		{
            emailCheckbox->setEnabled(true);
		}
        
	});

    if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_email"))
    {
        inputTextfield->setCursorChar('|');
        inputTextfield->setCursorEnabled(true);
//        inputTextfield->setMaxLengthEnabled(true);
        inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        inputTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
        //inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
        inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
        {
            switch (type)
            {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            case TextField::EventType::ATTACH_WITH_IME:
                this->runAction(MoveBy::create(0.25, Vec2(0, 200)));
                break;
            case TextField::EventType::DETACH_WITH_IME:
                enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PHONE_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "continue_button"));
                this->runAction(MoveBy::create(0.25, Vec2(0, -200)));
                removeSpace(inputTextfield);
                break;
#endif
            case TextField::EventType::INSERT_TEXT:
            {
                validEmaiInput(inputTextfield);
                
                (inputTextfield->getString().size() == 0) ? inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                
                enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PHONE_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "continue_button"));

                break;
            }
            case TextField::EventType::DELETE_BACKWARD:
            {
                validEmaiInput(inputTextfield);

                enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PHONE_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "continue_button"));
                
                (inputTextfield->getString().size() == 0) ? inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                
                break;
            }
            default:
                break;
            }
        });
    }
    
    changeDisplayLanguage();
    
	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
	{
		inputTextfield->setCursorChar('|');
		inputTextfield->setCursorEnabled(true);
		inputTextfield->setMaxLengthEnabled(true);
        inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        inputTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
        if(m_flow == APPopupBase::Flow::ENTER_INFO || m_flow == APPopupBase::Flow::UPDATE_PHONE || m_flow == APPopupBase::Flow::UPDATE_EMAIL)
        {
            inputTextfield->setMaxLength(30); // trường hợp này sẽ fix cứng số điện thoại (không cho sửa), nên cho MaxLength thoải mái con gà mái đi
        }
        else
        {
            inputTextfield->setMaxLength(PHONE_MAX_LENGTH);
        }
		inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
		{
			switch (type)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			case TextField::EventType::ATTACH_WITH_IME:
                    this->runAction(MoveBy::create(0.25, Vec2(0, 200)));
				break;
			case TextField::EventType::DETACH_WITH_IME:
                    this->runAction(MoveBy::create(0.25, Vec2(0, -200)));
                    removeSpace(inputTextfield);
				break;
#endif
			case TextField::EventType::INSERT_TEXT:
			{
                validPhoneInput(inputTextfield);

                (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                
                enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PHONE_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "continue_button"));

			}
			case TextField::EventType::DELETE_BACKWARD:
			{
                validPhoneInput(inputTextfield);

                (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                
                enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PHONE_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "continue_button"));
			}
			break;
			default:
				break;
			}
		});
	}

    m_onCancelCallBack = [=]()
    {
        hideVirtualKeyBoard();
    };
    
    if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_country"))
    {
        text->setString(MJDEFAULT->getStringForKey("phonecode_id","VN"));
    }
    
    if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
    {
        text->setString(MJDEFAULT->getStringForKey("phonecode_code","+84"));
    }
    
    // MARK: for Flows: UPDATE_EMAIL, UPDATE_PHONE, ENTER_INFO
    if (m_flow == APPopupBase::Flow::UPDATE_EMAIL || m_flow == APPopupBase::Flow::UPDATE_PHONE || m_flow == APPopupBase::Flow::ENTER_INFO)
    {
        if (auto exit_button = cocos2d::utils::findChild<ui::Button *>(this, "exit_button"))
        {
            // MARK: -- for VMonkey --
            if (CONFIG_MANAGER->isVietnameseStories())
            {
                // Point(80, 700) là vị trí của nút back trong các form của Account V3
                exit_button->setAnchorPoint(Point::ANCHOR_MIDDLE);
                exit_button->setPosition(exit_button->getParent()->convertToNodeSpace(Point(80, 700)));
                
                exit_button->loadTextureNormal("account/new_v3_vn/button_back_left.png");
                exit_button->loadTexturePressed("account/new_v3_vn/button_back_left.png");
                exit_button->resetNormalRender();
                exit_button->resetPressedRender();
                exit_button->resetDisabledRender();
                exit_button->getRendererNormal()->setPosition(exit_button->getContentSize()/2);
                exit_button->getRendererClicked()->setPosition(exit_button->getContentSize()/2);
                exit_button->getRendererDisabled()->setPosition(exit_button->getContentSize()/2);
            }
        }
    }
    
	return true;
}

void APResetPasswordScreen::setTextfieldsWithFixedConfig(const std::string& phone, const std::string& email, int button_continue_counter)
{
    if (phone.length() == 0 && email.length() == 0)
    {
        CCLOG("APResetPasswordScreen: Both phone and email are null!!!");
        return;
    }
    
    m_fixedConfig = true;
    m_fixedPhone = phone;
    m_fixedEmail = email;
    m_button_continue_counter = button_continue_counter;
    
    auto textfield_phone = cocos2d::utils::findChild<ui::TextField *>(this, "textfield_phone");
    auto textfield_email = cocos2d::utils::findChild<ui::TextField *>(this, "textfield_email");
    auto checkbox_phone = utils::findChild<ui::CheckBox *>(this, "cb_phone");
    auto checkbox_email = utils::findChild<ui::CheckBox *>(this, "cb_email");
    
    // có số điện thoại, không có email
    if (phone.length() && email.length() == 0)
    {
        if (auto text_desc = utils::findChild<ui::Text *>(this, "small_title")) text_desc->setVisible(false);
        
        checkbox_email ? checkbox_email->setVisible(false) : (void) nullptr;
        
        if (checkbox_phone)
        {
            if(auto spr = checkbox_phone->getRendererBackground()) spr->setVisible(false);
            if(auto spr = checkbox_phone->getRendererFrontCross()) spr->setVisible(false);
            if(auto spr = checkbox_phone->getRendererBackgroundDisabled()) spr->setVisible(false);
            if(auto spr = checkbox_phone->getRendererBackgroundSelected()) spr->setVisible(false);
            if(auto spr = checkbox_phone->getRendererFrontCrossDisabled()) spr->setVisible(false);
        }
    }
    // có email, không có số điện thoại
    else if (email.length() && phone.length() == 0)
    {
        if (auto text_desc = utils::findChild<ui::Text *>(this, "small_title")) text_desc->setVisible(false);
        
        checkbox_phone ? checkbox_phone->setVisible(false) : (void) nullptr;
        
        if (checkbox_email)
        {
            if (textfield_email)
            {
                auto ppp = textfield_email->getParent();
                ppp->setPosition(Point(ppp->getPosition().x, 394.f));
            }
            
//            checkbox_email->setPosition(checkbox_email->getPosition() + Point(0, 164));
            
            if(auto spr = checkbox_email->getRendererBackground()) spr->setVisible(false);
            if(auto spr = checkbox_email->getRendererFrontCross()) spr->setVisible(false);
            if(auto spr = checkbox_email->getRendererBackgroundDisabled()) spr->setVisible(false);
            if(auto spr = checkbox_email->getRendererBackgroundSelected()) spr->setVisible(false);
            if(auto spr = checkbox_email->getRendererFrontCrossDisabled()) spr->setVisible(false);
        }
    }
    
    if (textfield_phone)
    {
        textfield_phone->setPosition(textfield_phone->getPosition() - Point(160, 0));
        
        textfield_phone->setString(phone);
        textfield_phone->setTextColor(Color4B::BLACK);
        textfield_phone->setTouchEnabled(false);
        
        if (auto button = cocos2d::utils::findChild<ui::Button *>(this, "phone_code_button"))
        {
            button->setVisible(false);
        }
    }
    
    if (textfield_email)
    {
        textfield_email->setString(email);
        textfield_email->setTextColor(Color4B::BLACK);
        textfield_email->setTouchEnabled(false);
    }
    
    if (auto continueButton = cocos2d::utils::findChild<ui::Button*>(this, "continue_button"))
    {
        continueButton->setEnabled(true);
    }
}

void APResetPasswordScreen::changeDisplayLanguage()
{
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "title"),
            utils::findChild<ui::Text *>(this, "small_title"),
            utils::findChild<ui::Text *>(this, "phone_text"),
            utils::findChild<ui::Text *>(this, "email_text"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }
    
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.reset.password.tiltle"));
	}
    
    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "small_title"))
    {
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_password_recovery_method_title"));
    }

    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "phone_text"))
    {
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_code_to_sms_question"));
    }

    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "email_text"))
    {
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_code_to_email_question"));
    }
    
	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
	{
        inputTextfield->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
		inputTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_phone"));
	}

    if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_email"))
    {
        inputTextfield->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
        inputTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.enter.email"));
    }

	if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "continue_button"))
	{
        button->setEnabled(false);
        button->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
		button->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("ap_continue_button_title"));
	}
}

void APResetPasswordScreen::onContineButtonClicked(Ref * sender)
{
    m_button_continue_counter++;
    
    hideVirtualKeyBoard();
    
	auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, (m_resetType == RESET_TYPE::EMAIL)? "textfield_email" :"textfield_phone");

	if (!inputTextfield)
	{
		CCLOG("APResetPasswordScreen::onContineButtonClicked|inputTextfield = nullptr !");

		return;
	}


    removeSpace(inputTextfield);
    
	const std::string k_text = inputTextfield->getString();

	if (k_text.empty())
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("forgot.license.invalid"));

		return;
	}
    
    bool  isStartFromUpdateInfo = ( m_flow == APPopupBase::Flow::ENTER_INFO || m_flow == APPopupBase::Flow::UPDATE_PHONE || m_flow == APPopupBase::Flow::UPDATE_EMAIL);
    
    if(m_resetType == RESET_TYPE::EMAIL && isGMail(k_text) && !isStartFromUpdateInfo)
    {
        auto txt = LANGUAGE_MANAGER->getDisplayTextByKey("key.please.login.with.gmail");
        
        txt = std::regex_replace(txt, std::regex("APPNAME"), k_text);

        //replaceAll(txt,"APPNAME",k_text);
        if (MJDEFAULT->getBoolForKey(key_is_submit, false)){
            txt = "We dont't support gmail!";
        }
        showFailedPopup(txt);
        
        return;
    }
    
    std::string countryCode = "";
    
    if(auto txtCode = cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
    {
        countryCode = txtCode->getString();
    }
    
    showWaittingView();
    
    APPopupBase::Flow curFlow = m_flow;

	ap::api::APSendOTPAPI::call((m_resetType == RESET_TYPE::EMAIL)?AP_API_SEND_OTP_EMAIL_TYPE: AP_API_SEND_OTP_PHONE_TYPE, (m_resetType == RESET_TYPE::EMAIL) ? "": k_text, (m_resetType == RESET_TYPE::EMAIL) ? k_text:"",countryCode, [=](bool success, std::string msg, const rapidjson::Value &sendAPIOTPData)
	{
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			if (success)
			{
				auto popup = APOTPPopup::createView(m_resetType == RESET_TYPE::EMAIL?APOTPPopup::OTP_TYPE::EMAIL: APOTPPopup::OTP_TYPE::PHONE, k_text,countryCode, curFlow);

				this->addChild(popup, POPUP_ZORDER);
			}
			else
			{
				this->showFailedPopup(msg);
			}
            
            removeWaittingView();
		});
	});
	
}

void APResetPasswordScreen::onPhoneCodeButtonClicked(Ref * sender)
{
    auto popup = APPhoneCodePopup::createView();
    
    popup->setDelegate(this);
    
    this->addChild(popup,POPUP_ZORDER);
}

void APResetPasswordScreen::onPhoneCodeItemSelected(PhoneCodeInfo i_info)
{
  if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_country"))
  {
     text->setString(i_info.id);
  }
  
  if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
  {
     text->setString(i_info.code);
  }
    
}

cocos2d::ui::Widget::ccWidgetClickCallback APResetPasswordScreen::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APResetPasswordScreen, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APResetPasswordScreen, onContineButtonClicked),
		CLICK_MAP(APResetPasswordScreen, onPhoneCodeButtonClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }
	return nullptr;
}

void APResetPasswordScreen::hideVirtualKeyBoard()
{
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"));
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_email"));
}

void APResetPasswordScreen::onCancel(cocos2d::Ref *sender)
{
    if (m_flow == APPopupBase::Flow::UPDATE_EMAIL || m_flow == APPopupBase::Flow::UPDATE_PHONE || m_flow == APPopupBase::Flow::ENTER_INFO)
    {
        // TODO: CleverTap-pushEvent: "forgot_password"
        try
        {
            CleverTapManager::GetInstance()->pushEvent("forgot_password",
            {
                {"update_new_password", cocos2d::Value(m_button_continue_counter ? "no" : "none")},
                {"verify_otp_type", cocos2d::Value(m_resetType == RESET_TYPE::EMAIL ? "email" : "sms")},
            });
        }
        catch (const std::exception&)
        {
            
        }
        
        this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this, sender] {
            APPopupBase::onCancel(sender);
        }), NULL));
    }
    else
    {
        APPopupBase::onCancel(sender);
    }
}
