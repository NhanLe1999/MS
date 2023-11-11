#include "APLoginScreen.h"
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
#include "APRegisterScreen.h"
#include "APLoginView.h"
#include "APLoginAPI.h"
#include "APChangePasswordScreen.h"
#include "APResetPasswordScreen.h"
#include "TILLabelAnimated.h"
#include "MJDefinitions.h"
#include "APHaveUsedMonkeyApps.h"
#include "APConnectWithCurrentAccount.h"
#include "APChangeDefaultPasswordScreen.h"
#include "MSLanguageSelection.h"
USING_NS_CC;
INITIALIZE_READER(APLoginScreen);

std::once_flag v2_ap_login_screen_reader_flag;

APLoginScreen * APLoginScreen::createView(cocos2d::Node* i_node)
{
	std::call_once(v2_ap_login_screen_reader_flag, []
	{
		REGISTER_CSB_READER(APLoginScreen);
	});

	auto p = static_cast<APLoginScreen*>(CSLoader::createNodeWithVisibleSize(CONFIG_MANAGER->isVietnameseStories()?"csb/account/new_v2/APLoginScreen_VN.csb":"csb/account/new_v2/APLoginScreen.csb"));

	if (p && p->didLoadFromCSB(i_node))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APLoginScreen::didLoadFromCSB(cocos2d::Node* i_node)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}
    
    if (CONFIG_MANAGER->isVietnameseStories())
    {
        utils::findChild<ImageView*>(this, "background")->loadTexture("mjstory/new_ui_vn/bg.jpg");
    }
    
    m_loginView = i_node;
    
    if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"))
    {
        inputTextfield->setCursorChar('|');
        inputTextfield->setCursorEnabled(true);
        inputTextfield->setMaxLengthEnabled(true);
        inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
        inputTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
        inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
		inputTextfield->setTextColor(Color4B::BLACK);
		inputTextfield->setPlaceHolderColor(Color4B(90, 90, 90, 255));

        //enableAButtonBasedOnTextfieldLength(cocos2d::utils::findChild<ui::Button*>(this, "login"), inputTextfield, PASSWORD_MIN_LENGTH);
        
        //enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PASSWORD_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "login"));
        
        inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)  {
            switch (type)
            {
                case TextField::EventType::ATTACH_WITH_IME:
					inputTextfield->setTextColor(Color4B::BLACK);
                    break;
                case TextField::EventType::DETACH_WITH_IME:
					inputTextfield->setTextColor(Color4B::BLACK);
                    removeSpace(inputTextfield);
                    break;
                case TextField::EventType::INSERT_TEXT:
                {
                    trimSpace(inputTextfield);
                    (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                    
                }
                case TextField::EventType::DELETE_BACKWARD:
                {
                    trimSpace(inputTextfield);
                    (inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B::WHITE): inputTextfield->setTextColor(Color4B::BLACK);
                }
                    break;
                default:
                    break;
            }
        });
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		auto editbox = APPopupBase::createEditBoxFromTextfield(inputTextfield);//allow input text and numbers, email...
		editbox->setName(inputTextfield->getName());
		editbox->setDelegate(this);

		inputTextfield->removeFromParent();
#endif

    }
    if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"))
	{
		inputTextfield->setCursorChar('|');
		inputTextfield->setCursorEnabled(true);
		inputTextfield->setMaxLengthEnabled(true);
		inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
		inputTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
		inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
		inputTextfield->setTextColor(Color4B::BLACK);
		inputTextfield->setPlaceHolderColor(Color4B(90,90,90,255));
		//enableAButtonBasedOnTextfieldLength(cocos2d::utils::findChild<ui::Button*>(this, "login"), inputTextfield, PASSWORD_MIN_LENGTH);

		//enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PASSWORD_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "login"));

		inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
		{
			switch (type)
			{
			case TextField::EventType::ATTACH_WITH_IME:
				inputTextfield->setTextColor(Color4B::BLACK);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				moveScreenWhenUseTextfiled(this, APPopupBase::AnimationDirection::TOP);
#endif
				break;
			case TextField::EventType::DETACH_WITH_IME:
				inputTextfield->setTextColor(Color4B::BLACK);
				removeSpace(inputTextfield);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				moveScreenWhenUseTextfiled(this, APPopupBase::AnimationDirection::DOWN);
#endif
				break;
			case TextField::EventType::INSERT_TEXT:
			{
				trimSpace(inputTextfield);

				(inputTextfield->getString().size() == 0) ? inputTextfield->setTextColor(Color4B::WHITE) : inputTextfield->setTextColor(Color4B::BLACK);

				break;
			}
			case TextField::EventType::DELETE_BACKWARD:
			{
				trimSpace(inputTextfield);

				(inputTextfield->getString().size() == 0) ? inputTextfield->setTextColor(Color4B::WHITE) : inputTextfield->setTextColor(Color4B::BLACK);

				break;
			}
			default:
				break;
			}
		});

		

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		auto editbox = APPopupBase::createEditBoxFromTextfield(inputTextfield, ui::EditBox::InputFlag::PASSWORD);
		editbox->setName(inputTextfield->getName());
		editbox->setDelegate(this);

		if (auto node = cocos2d::utils::findChild(editbox->getParent(), "show_password_btn"))
		{
			node->setZOrder(editbox->getLocalZOrder() + 1);
		}

		inputTextfield->removeFromParent();
#endif
	}

    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        Button* quickLoginInLoginView  = cocos2d::utils::findChild<Button*>(loginView, "quick_login");
        
        if(quickLoginInLoginView)
        {
            if(!CONFIG_MANAGER->isVietnameseStories())
            {
                auto txtQuickLoginInLoginView = quickLoginInLoginView->getChildByName<Text*>("text");
                
                Button* quickLoginButton = cocos2d::utils::findChild<Button*>(this, "quick_login");
                
                auto txtQuickLogin = quickLoginButton->getChildByName<Text*>("quick_login_title");
                
                auto label = Label::createWithTTF(txtQuickLoginInLoginView->getString(), txtQuickLogin->getFontName(), txtQuickLogin->getFontSize());
                
                const float k_padding			= 100.0f;
                const float k_quickButtonWidth	=  label->getContentSize().width + k_padding;
                
                quickLoginButton->setVisible(quickLoginInLoginView->isVisible());
                
                quickLoginButton->setContentSize({k_quickButtonWidth,quickLoginButton->getContentSize().height});
                
                txtQuickLogin->setPositionX(quickLoginButton->getContentSize().width/2);
                
                txtQuickLogin->ignoreContentAdaptWithSize(false);
                
                txtQuickLogin->setContentSize({label->getContentSize().width+ k_padding/2,txtQuickLogin->getContentSize().height});
                
                txtQuickLogin->setString(txtQuickLoginInLoginView->getString());
            }
            else
            {
                auto txtQuickLoginInLoginView = quickLoginInLoginView->getChildByName<Text*>("text");
                
                Button* quickLoginButton = cocos2d::utils::findChild<Button*>(this, "quick_login");
                
                auto txtQuickLogin = quickLoginButton->getChildByName<Text*>("quick_login_title");
                
                auto label = Label::createWithTTF(txtQuickLoginInLoginView->getString(), txtQuickLogin->getFontName(), txtQuickLogin->getFontSize());
                
                const float k_padding   = 100.0f;
                const float k_quickButtonWidth =  label->getContentSize().width + k_padding;
                
                quickLoginButton->setVisible(quickLoginInLoginView->isVisible());
                
                quickLoginButton->setContentSize({k_quickButtonWidth,quickLoginButton->getContentSize().height});
                
                txtQuickLogin->setPositionX(quickLoginButton->getContentSize().width/2);
                
                txtQuickLogin->ignoreContentAdaptWithSize(false);
                
                txtQuickLogin->setContentSize({label->getContentSize().width+ k_padding/2,txtQuickLogin->getContentSize().height});
                
                txtQuickLogin->setString(txtQuickLoginInLoginView->getString());
                
                if(quickLoginButton->isVisible())
                {
                    auto quickLoginRichText = ui::RichText::createWithXML("<font color='#323232' size='23' face='fonts/Lato-Bold.ttf'>" + LANGUAGE_MANAGER->getDisplayTextByKey("key.quick.login") +"<font color='#51075E' size='23' face='fonts/Lato-Bold.ttf'>" + txtQuickLoginInLoginView->getString()+ " </font> </font>");
                    //quickLoginRichText->ignoreContentAdaptWithSize(false);
                    //quickLoginRichText->setContentSize(quickLoginButton->getContentSize());
                    quickLoginRichText->setPosition(quickLoginButton->getPosition());
                    quickLoginRichText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
                    quickLoginButton->getParent()->addChild(quickLoginRichText, quickLoginButton->getZOrder() + 1);
                    
                    quickLoginButton->setOpacity(0);
                }
            }
        }
    }
        
    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "fb_button"))
    {
        auto parentSize = button->getParent()->getContentSize();
        
        button->setScale((parentSize.width*0.139f)/button->getContentSize().width);
    }
    
    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "gmail_button"))
    {
        auto parentSize = button->getParent()->getContentSize();
        
        button->setScale((parentSize.width*0.139f)/button->getContentSize().width);
    }
    
    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
#if !(defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0))
        visibleAllNodes({
            cocos2d::utils::findChild(this, "gmail_button"),
            cocos2d::utils::findChild(this, "fb_button"),
            cocos2d::utils::findChild(this, "txt_or"),
        },false);
#endif
    }
    
    
    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "forgot_account"))
    {
        button->setVisible(CONFIG_MANAGER->isCountryVN());
    }
    
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    if (auto monkey = cocos2d::utils::findChild(this, "monkey"))
    {
         auto monkeyEffect = ParticleSystemQuad::create("plist/particle/monkey_effect.plist");
         monkeyEffect->setPosition(monkey->getPosition()+Vec2(0,-monkey->getContentSize().height*0.5f));
         monkey->getParent()->addChild(monkeyEffect, 10000);
    }

     auto particleSystem = ParticleSystemQuad::create("plist/particle/click_effect_02.plist");
     this->addChild(particleSystem, 10);

     auto particleSystem2 = ParticleSystemQuad::create("plist/particle/click_effect_03.plist");
     this->addChild(particleSystem2, 10);

     auto touchListener = EventListenerTouchOneByOne::create();

     touchListener->onTouchBegan = [](Touch* touch, Event* event) {
         // your code
         return true; // if you are consuming it
     };

     // trigger when moving touch
     touchListener->onTouchMoved = [](Touch* touch, Event* event) {
         // your code
     };

     // trigger when you let up
     touchListener->onTouchEnded = [=](Touch* touch, Event* event) {

         particleSystem->resetSystem();
         particleSystem2->resetSystem();

         particleSystem->setPosition(touch->getLocation());
         particleSystem2->setPosition(touch->getLocation());
     };

     // Add listener
     _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

     if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "login_button"))
     {
         ClippingNode *clipper = ClippingNode::create();
         clipper->setPosition(button->getContentSize() / 2);
         button->addChild(clipper, 100);

         auto stencil = ui::Scale9Sprite::create("new_v2/register_button.png");
         stencil->setScale9Enabled(true);
         stencil->setContentSize(button->getContentSize());
         stencil->setAnchorPoint(Vec2(0.5, 0.5));
         clipper->setStencil(stencil);
         clipper->setInverted(false);
         clipper->setAlphaThreshold(0.25f);
         clipper->setName("clipper");

         auto content = Sprite::create("new_v2/menu_button_blend.png");
         content->setScale(2.0f);

         auto  pos = Vec2(-stencil->getContentSize().width,0);

         content->setPosition(pos);
         content->setName("content");
         //content->setBlendFunc({ GL_DST_COLOR,GL_ONE });
         //content->setOpacity(0.25f*255.0f);
         content->runAction(RepeatForever::create(Sequence::create(MoveBy::create(2.0f, Vec2(+2.5*stencil->getContentSize().width, 0)), DelayTime::create(2.0f),
                                                                   MoveTo::create(0.0f, pos), DelayTime::create(2.0f), nullptr)));
         clipper->addChild(content);
     }

     if (auto registerTitle = cocos2d::utils::findChild<ui::Text*>(this, "login_title"))
     {
         auto newTitle = TILLabelBMFontAnimated::createWithTTF(
                 registerTitle->getString(),
                 registerTitle->getFontName(),
                 registerTitle->getFontSize());

         newTitle->setPosition(registerTitle->getPosition());
         newTitle->setTextColor(registerTitle->getTextColor());
         //newTitle->enableGlow(Color4B(28, 121, 208, 255));
         newTitle->enableBold();
         newTitle->enableOutline(Color4B(registerTitle->getTextColor()), 1.0f);
         newTitle->setName(registerTitle->getName());

         registerTitle->getParent()->addChild(newTitle, registerTitle->getZOrder());

         newTitle->runAction(RepeatForever::create(Sequence::createWithTwoActions(CallFunc::create([=]() {

             newTitle->animateInSwell(1.0f);

         }), DelayTime::create(8.0f))));

         registerTitle->removeFromParent();

         ClippingNode *clipper = ClippingNode::create();
         clipper->setPosition(newTitle->getContentSize() / 2);
         newTitle->addChild(clipper, 100);

         auto stencil = TILLabelBMFontAnimated::createWithTTF(
                 registerTitle->getString(),
                 registerTitle->getFontName(),
                 registerTitle->getFontSize());

         newTitle->enableBold();
         newTitle->enableOutline(Color4B(registerTitle->getTextColor()), 1.0f);
         clipper->setStencil(stencil);
         clipper->setInverted(false);
         clipper->setAlphaThreshold(0.25f);
         clipper->setName("clipper");

         auto content = Sprite::create("account/new_v2/menu_button_blend.png");

         //content->setScale(2.0f);

         auto  pos = Vec2(-stencil->getContentSize().width, 0);

         content->setPosition(pos);
         content->setName("content");
         content->setBlendFunc({ GL_DST_COLOR,GL_ONE });
         content->setOpacity(0.25f*255.0f);
         content->runAction(RepeatForever::create(Sequence::create(DelayTime::create(2.0f), MoveBy::create(3.5f, Vec2(+2.5*stencil->getContentSize().width, 0)), DelayTime::create(2.0f), MoveTo::create(0.0f, pos), nullptr)));
         clipper->addChild(content);
     }
    
//	 animationMoveForNodes
//	 ({
//			 cocos2d::utils::findChild(this, "textfield_id_bg"),
//			 cocos2d::utils::findChild(this, "textfield_password_bg"),
//			 cocos2d::utils::findChild(this, "login_button"),
//	 }, AnimationDirection::LEFT, 0.6f, 0.5f);
//
//    if (auto rightPanel = cocos2d::utils::findChild<ui::ImageView*>(this, "right_object"))
//    {
//        rightPanel->setOpacity(0);
//
//        rightPanel->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f),FadeIn::create(0.6f)));
//    }
    
    AUDIO_ENGINE->playEffect("sounds/slide.mp3");

    return true;
}

void APLoginScreen::onExit()
{
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"));
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"));
    
    APPopupBase::onExit();
}

void APLoginScreen::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "txt_register_1"), "key.not.have.account",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "login_title"), "ap_login_button_title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"), "key.enter.phone.or.email",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"), "ap_enter_password",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "forgot_account_txt"), "key.forgot.account",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "forgot_password_txt"), "ap_forgot_pass_title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "login_button"), "ap_login_button_title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "txt_or"), "key.or",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "txt_register_2"), "ap_register_button_title",APPopupBase::TextType::BOLD},
	});

    if (auto phoneTextField = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"))
    {
        phoneTextField->setFontSize(phoneTextField->getFontSize()-1);
    }
    
	if (auto passwordTextField = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"))
	{
        passwordTextField->setFontSize(passwordTextField->getFontSize()-1);
	}

    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "quick_login_title"))
    {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
       // title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.or"));
    }
    
    if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "txt_register_2"))
    {
        title->setPositionX(title->getParent()->getContentSize().width);
    }
}


void APLoginScreen::onLoginButtonClicked(cocos2d::Ref * sender)
{
    playClickedSoundEffect();
    
    auto loginButton = dynamic_cast<ui::Button*>(sender);

    loginButton->setTouchEnabled(false);
  
    auto inputTextfield = cocos2d::utils::findChild(this, "textfield_input");
    auto passwordTextfield = cocos2d::utils::findChild(this, "textfield_password");

    hideVirtualKeyboard(inputTextfield);
    
    hideVirtualKeyboard(passwordTextfield);
    
    removeSpace(passwordTextfield);
    
    removeSpace(inputTextfield);

	std::string inputText = "";

	std::string passwordText = "";

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (auto editbox = dynamic_cast<ui::EditBox*>(inputTextfield))
	{
		inputText = editbox->getText();
	}

	if (auto editbox = dynamic_cast<ui::EditBox*>(passwordTextfield))
	{
		passwordText = editbox->getText();
	}
#else
	if (auto editbox = dynamic_cast<ui::TextField*>(inputTextfield))
	{
		inputText = editbox->getString();
	}

	if (auto editbox = dynamic_cast<ui::TextField*>(passwordTextfield))
	{
		passwordText = editbox->getString();
	}
#endif


    if(inputText.empty())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.please.enter.email.phone.number"));
        
        loginButton->setTouchEnabled(true);
        
        return;
    }
//    if(inputText.length() < PHONE_MIN_LENGTH)
//    {
//        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_wrong_phone_number"));
//
//        loginButton->setTouchEnabled(true);
//
//        return;
//    }
    
    if (passwordText.empty())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password2"));

        loginButton->setTouchEnabled(true);

        return;
    }

	if (cocos2d::StringUtils::StringUTF8(passwordText).length() < PASSWORD_MIN_LENGTH )
	{
		showFailedPopup(getDisplayTextByKey("ap_waring_password"));

		loginButton->setTouchEnabled(true);

		return;
	}

    std::string phoneText = "" ;
    
    std::string emailText = "";
    
	std::string userNameID = "";

    auto type = ap::api::Login::LoginType::PHONE;

    if(isUTF8StringPhoneNumberValid(inputText))//phone
    {
        phoneText = inputText;                                                                                                                      
        
        type = ap::api::Login::LoginType::PHONE;
   }
    else if(mj::helper::isValidEmail(inputText))//email
    {
        emailText = inputText;
        
        type = ap::api::Login::LoginType::GOOGLE;

    }
    else if (isValidUserName(inputText))//user name
    {
		userNameID = inputText;

		type = ap::api::Login::LoginType::USER_NAME;
    }
	else
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.email.phone.wrong"));

		loginButton->setTouchEnabled(true);

		return;
	}

    auto loginView = dynamic_cast<APLoginView*>(m_loginView);
    
    if(type==ap::api::Login::LoginType::PHONE)
    {
        loginView->onPhone(this);
    }
    else if (type == ap::api::Login::LoginType::GOOGLE)
    {
        loginView->onGoogle(this,false);
    }

    ap::api::Login::callV3(type, "", phoneText, emailText, "", passwordText,"", userNameID, "", [=](int code, bool is_success, std::string msg2,std::string tokenToChangePassword,ap::api::Login::LoginDataInfo info)
                           {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg2, code,loginButton,tokenToChangePassword,phoneText, info, type] {
            
            if (is_success)
            {
                if(code == 200)
                {
                    showWaittingView();
                   
					switch (info.type)
					{
					case ap::api::Login::V4Type::DEFAULT:
					{
						MJDEFAULT->setBoolForKey("account_unverified", false);
						cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");

						if (type == ap::api::Login::LoginType::USER_NAME)
						{
							if (info.isPasswordChanged)
							{
								auto screen = APChangeDefaultPasswordScreen::createView(info, APChangeDefaultPasswordScreen::START_FROM::LOGIN_SCREEN);

								Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
							}
							else
							{
								if (auto loginView = dynamic_cast<APLoginView*>(m_loginView))
								{
									loginView->onLoginSuccess("user_name");
								}
							}
						}

						break;
					}
					case ap::api::Login::V4Type::DEFAULT_WITH_OLD_USER_INFO:
					{

						break;
					}
					case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT:
					{
						auto screen = APConnectWithCurrentAccount::createView(info);

						Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);

						removeWaittingView();

						break;
					}
					case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT:
					{
						auto screen = APHaveUsedMonkeyApps::createView(info);

						Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);

						removeWaittingView();

						break;
					}
					default:
						break;
					}

					loginButton->setTouchEnabled(true);

                }
                else //code 315
                {
					auto changePasswordScreen = APChangePasswordScreen::createView(APChangePasswordScreen::START_FROM::LOGIN_SCREEN, APChangePasswordScreen::TYPE::PHONE, phoneText, tokenToChangePassword, "");

					animationMoveToLeft(changePasswordScreen);

					Director::getInstance()->getRunningScene()->addChild(changePasswordScreen, INT_MAX - 1);

                    //this->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f),RemoveSelf::create()));
                    
                    //this->removeFromParent();
                }
            }
            else
            {
                this->removeChildByName("waiting_view");
                
                showFailedPopup(msg2);
                
                loginButton->setTouchEnabled(true);
                
                APLoginView::pushCleverTapTrackingEvent("user_name", false, MJDEFAULT->getBoolForKey("is_skip_login", false), msg2);
            }
        });
    });
}

void APLoginScreen::onForgotPasswordClicked(cocos2d::Ref * sender)
{
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));

    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"));
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"));
    
    auto screen =  APResetPasswordScreen::createView(APResetPasswordScreen::RESET_TYPE::PHONE);
    
    animationMoveToLeft(screen);
    
    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX-1);
}

void APLoginScreen::onForgotAccountClicked(cocos2d::Ref * sender)
{
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));

    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"));
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"));
    
    std::string url = MJDEFAULT->getBoolForKey("debugkey_dev_environment", false) ?"https://dev.monkey.edu.vn/forgot-account.html?app_id=40":"https://monkey.edu.vn/forgot-account.html?app_id=40";
    if (CONFIG_MANAGER->isVietnameseStories()){
        url = MJDEFAULT->getBoolForKey("debugkey_dev_environment", false) ?"https://dev.monkey.edu.vn/forgot-account.html?app_id=51":"https://monkey.edu.vn/forgot-account.html?app_id=51";
    }
    Application::getInstance()->openURL(url);

}

void APLoginScreen::onQuickLoginClicked(cocos2d::Ref * sender)
{
    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onQuickLogin(this);
    }
}
void APLoginScreen::onFacebookButtonClicked(cocos2d::Ref * sender)
{
    playClickedSoundEffect();
    
    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onFacebook(this);
    }
}

void APLoginScreen::onGmailButtonClicked(cocos2d::Ref * sender)
{
    playClickedSoundEffect();
    
    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onGoogle(this);
    }
}

void APLoginScreen::onRegisterButtonClicked(cocos2d::Ref * sender)
{
    if(auto button = dynamic_cast<ui::Button*>(sender))
    {
        button->setTouchEnabled(false);
    }
        
    auto screen = APRegisterScreen::createView(m_loginView);
    
    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX -1);
    
    animationMoveToRight(screen);

    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->setCurrentScreen(screen);
    }
    
    this->removeFromParent();
}

void APLoginScreen::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
    auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

    if (!button)
    {
        CCLOG("Error|APRegisterScreen::onShowPasswordButtonClicked");

        return;
    }
    
    if(auto textFiled = cocos2d::utils::findChild<ui::EditBox*>(this,"textfield_password"))
    {
        bool isShowPassword = (textFiled->getInputFlag()== ui::EditBox::InputFlag::PASSWORD);
        
        if(auto eye = utils::findChild<ImageView*>(button,"eye"))
        {
            eye->loadTexture(isShowPassword? "new_v2/eye.png": "new_v2/eye_enable.png");
        }
        
        textFiled->setInputFlag(isShowPassword? ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS: ui::EditBox::InputFlag::PASSWORD);
		textFiled->setText(textFiled->getText());
    }
	else if (auto textFiled = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"))
	{
		bool isShowPassword = textFiled->isPasswordEnabled();

		if (auto eye = utils::findChild<ImageView*>(button, "eye"))
		{
			eye->loadTexture(isShowPassword ? "new_v2/eye.png" : "new_v2/eye_enable.png");
		}

		textFiled->setPasswordEnabled(!isShowPassword);
	}
}
void APLoginScreen::onBack(cocos2d::Ref * sender)
{
    auto scene = MSLanguageSelection::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
    this->removeFromParent();
}
cocos2d::ui::Widget::ccWidgetClickCallback APLoginScreen::onLocateClickCallback(const std::string & callBackName) {
    
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
		CLICK_MAP(APLoginScreen, onLoginButtonClicked),
		CLICK_MAP(APLoginScreen, onForgotPasswordClicked),
		CLICK_MAP(APLoginScreen, onForgotAccountClicked),
		CLICK_MAP(APLoginScreen, onQuickLoginClicked),
		CLICK_MAP(APLoginScreen, onFacebookButtonClicked),
		CLICK_MAP(APLoginScreen, onGmailButtonClicked),
		CLICK_MAP(APLoginScreen, onRegisterButtonClicked),
		CLICK_MAP(APLoginScreen, onShowPasswordButtonClicked),
        CLICK_MAP(APLoginScreen, onBack),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void APLoginScreen::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
	log("editBox %p DidBegin !", editBox);
}

void APLoginScreen::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox * editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)
{
	log("editBox %p DidEnd with action %d!", editBox, action);
}

void APLoginScreen::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
	if (editBox && editBox->getName() == "editboxPhone")
	{
		trimSpace(editBox);
	}
	else if (editBox && editBox->getName() == "editboxPassword")
	{
		trimSpace(editBox);
	}
	//log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void APLoginScreen::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
    if (editBox && editBox->getName() == "editboxPhone")
    {
        removeSpace(editBox);
    }
    else if (editBox && editBox->getName() == "editboxPassword")
    {
        removeSpace(editBox);
    }
    
	//log("editBox %p was returned !", editBox->getName().c_str());
}


