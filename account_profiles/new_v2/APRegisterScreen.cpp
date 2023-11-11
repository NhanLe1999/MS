#include "APRegisterScreen.h"
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
#include "APRegisterAPI.h"
#include "APLoginScreen.h"
#include "APPhoneCodePopup.h"
#include "TILLabelAnimated.h"
#include "MJDefinitions.h"
#include "MSLanguageSelection.h"
#define IS_SKIP_LOGIN_ENABLE_FOR_DEBUG_MODE 0

USING_NS_CC;
INITIALIZE_READER(APRegisterScreen);

std::once_flag v2_ap_register_screen_reader_flag;

APRegisterScreen * APRegisterScreen::createView(cocos2d::Node* i_loginView)
{
	std::call_once(v2_ap_register_screen_reader_flag, []
	{
		REGISTER_CSB_READER(APRegisterScreen);
	});

	auto p = static_cast<APRegisterScreen*>(CSLoader::createNodeWithVisibleSize(CONFIG_MANAGER->isVietnameseStories()?"csb/account/new_v2/APRegisterScreen_VN.csb":"csb/account/new_v2/APRegisterScreen.csb"));

	if (p && p->didLoadFromCSB(i_loginView))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APRegisterScreen::didLoadFromCSB(cocos2d::Node* i_loginView)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}
    if (CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild<ImageView*>(this, "background")->loadTexture("mjstory/new_ui_vn/bg.jpg");
       
    }
    m_loginView = i_loginView;
    
    updateSkipUI();
    
    m_passwordTextfields =
    {
         cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"),
         cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword"),
    };
        
    for (auto inputTextfield : m_passwordTextfields)
    {
		setupForPasswordTextfield(inputTextfield,this);

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

	if (auto textFiled = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
	{
		setupForPhoneNumberTextfield(textFiled, PHONE_MAX_LENGTH_WITH_COUNTRY_CODE);

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		auto editbox = APPopupBase::createEditBoxFromTextfield(textFiled, ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS, ui::EditBox::InputMode::PHONE_NUMBER);
		editbox->setName(textFiled->getName());
		editbox->setDelegate(this);
        
        if (auto node = cocos2d::utils::findChild(editbox->getParent(), "show_password_btn"))
        {
            node->setZOrder(editbox->getLocalZOrder() + 1);
        }
        
		textFiled->removeFromParent();
#endif

	}

    if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_country"))
    {
        text->setString(MJDEFAULT->getStringForKey("phonecode_id","VN"));
    }
    
    if(auto text =  cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
    {
        text->setString(MJDEFAULT->getStringForKey("phonecode_code","+84"));
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
        
    if (auto monkey = cocos2d::utils::findChild(this, "monkey"))
    {
        auto monkeyEffect = ParticleSystemQuad::create("plist/particle/monkey_effect.plist");
        monkeyEffect->setPosition(monkey->getPosition()+Vec2(0,-monkey->getContentSize().height*0.5f));
        monkey->getParent()->addChild(monkeyEffect, 10000);

		monkey->runAction(
			Sequence::createWithTwoActions(DelayTime::create(0.5f + 0.8f), CallFunc::create([=]() {
		
			monkey->runAction(RepeatForever::create( 
				Sequence::createWithTwoActions(
				Repeat::create(Sequence::createWithTwoActions(RotateBy::create(0.08f, -3.0f), RotateBy::create(0.08f, +3.0f)), 5), DelayTime::create(5.0f))));
			})));
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

    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "register_button"))
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

    if (auto registerTitle = cocos2d::utils::findChild<ui::Text*>(this, "register_title"))
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

//	animationMoveForNodes
//	({
//			cocos2d::utils::findChild(this, "textfield_phone_bg"),
//			cocos2d::utils::findChild(this, "textfield_password_bg"),
//			cocos2d::utils::findChild(this, "textfield_repassword_bg"),
//			cocos2d::utils::findChild(this, "register_button"),
//	}, AnimationDirection::RIGHT, 0.6f,0.5f);
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

void APRegisterScreen::onExit()
{
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"));
    
    hideVirtualKeyboard(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_input"));
    
    APPopupBase::onExit();
}

void APRegisterScreen::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "register_title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"));
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
    
    if (auto phoneTextField = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
    {
        phoneTextField->setFontSize(22.0f);
        phoneTextField->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        phoneTextField->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_phone"));
    }

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "txt_or"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.or"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "txt_login_1"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.have.account"));
	}

	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "txt_login_2"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_login_button_title"));
        
        title->setPositionX(title->getParent()->getContentSize().width);
	}
    
    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "register_button"))
    {
        button->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
        button->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"));
    }

    if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "skip_button"))
    {
        button->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        button->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("key.skip"));
    }
}

void APRegisterScreen::onRegisterButtonClicked(Ref * sender)
{
    playClickedSoundEffect();

    #if (defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0))
        #if (IS_SKIP_LOGIN_ENABLE_FOR_DEBUG_MODE > 0)
            MJDEFAULT->setBoolForKey("is_skip_login", true);
            if(auto skipButton = cocos2d::utils::findChild(this, "skip_button"))
            {
                skipButton->setVisible(true);
            }
        #endif
    #endif
    
    auto registerButton = dynamic_cast<ui::Button*>(sender);

    if(registerButton)
    {
        registerButton->setTouchEnabled(false);
    }

    auto inputTextfield = cocos2d::utils::findChild(this, "textfield_phone");
    
    auto passwordTextfield = cocos2d::utils::findChild(this, "textfield_password");
    
    auto reenteringPasswordTextfield = cocos2d::utils::findChild(this, "textfield_repassword");
    
    hideVirtualKeyboard(inputTextfield);

    hideVirtualKeyboard(std::vector<Node*>{passwordTextfield,reenteringPasswordTextfield});

	std::string inputText		= "";

	std::string passwordText	= "";

	std::string rePasswordText	= "";

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (auto editbox = dynamic_cast<ui::EditBox*>(inputTextfield))
	{
		inputText = editbox->getText();
	}

	if (auto editbox = dynamic_cast<ui::EditBox*>(passwordTextfield))
	{
		passwordText = editbox->getText();
	}

	if (auto editbox = dynamic_cast<ui::EditBox*>(reenteringPasswordTextfield))
	{
		rePasswordText = editbox->getText();
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

	if (auto editbox = dynamic_cast<ui::TextField*>(reenteringPasswordTextfield))
	{
		rePasswordText = editbox->getString();
	}
#endif

    if (inputText.empty())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.please.enter.phone.number"));

        registerButton->setTouchEnabled(true);
        
        return;
    }
    
    if (inputText.length() < PHONE_MIN_LENGTH || inputText.length() > PHONE_MAX_LENGTH)
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.invalid.phone.number"));

        registerButton->setTouchEnabled(true);
        
        return;
    }
    
    if (passwordText.empty() || rePasswordText.empty())
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password"));

        registerButton->setTouchEnabled(true);

        return;
    }
    if (passwordText.length() < PASSWORD_MIN_LENGTH || rePasswordText.length() < PASSWORD_MIN_LENGTH)
    {
       showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_password"));

       registerButton->setTouchEnabled(true);

       return;
    }
    
    if (!passwordText.empty() && !rePasswordText.empty() && passwordText != rePasswordText)
    {
        showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_match_password"));

        registerButton->setTouchEnabled(true);

        return;
    }
    
    std::string k_phoneText     = inputText;
    
    std::string k_countryCode   = cocos2d::utils::findChild<ui::Text*>(this, "txt_code")->getString();
    
    std::string k_password      = passwordText;

    CCLOG("APRegisterScreen::onRegisterButtonClicked| phone:%s, code:%s ,k_password: %s",k_phoneText.c_str(),k_countryCode.c_str(),k_password.c_str());
    
    showWaittingView();
    
    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onPhone(this);
    }
    
    //new logic upgrade account v3 -> cannot register with email ( except gmail ).
    
    ap::api::APRegisterAPI::call(ap::api::APRegisterAPI::LoginType::PHONE, "", k_phoneText, "", "",k_countryCode ,k_password, [=](int code, bool isSuccess, std::string message)
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, isSuccess, message, code,registerButton]
        {
            removeWaittingView();

            registerButton->setTouchEnabled(true);

            if (isSuccess)
            {
                MJDEFAULT->setBoolForKey("account_unverified", false);
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
            }
            else
            {
                showFailedPopup(message);
            }
        });

    });
}

void APRegisterScreen::onFacebookButtonClicked(Ref * sender)
{
    playClickedSoundEffect();
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));

	if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onFacebook(this);
    }
}

void APRegisterScreen::onGmailButtonClicked(Ref * sender)
{
    playClickedSoundEffect();
    
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));

    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onGoogle(this);
    }
}

void APRegisterScreen::onSkipButton(Ref * sender)
{
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));

    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->onSkipLogin(this);
    }
}

void APRegisterScreen::onLoginButtonClicked(Ref * sender)
{
    auto button = dynamic_cast<ui::Button*>(sender);

    if(button)
    {
        button->setTouchEnabled(false);
    }
    
    auto screen = APLoginScreen::createView(m_loginView);
        
    Director::getInstance()->getRunningScene()->addChild(screen,INT_MAX -1);
    
    animationMoveToLeft(screen);

    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
        loginView->setCurrentScreen(screen);
    }
    
    this->removeFromParent();
}

void APRegisterScreen::onPhoneCodeButtonClicked(Ref * sender)
{
    avoidSpamOnButton(dynamic_cast<ui::Button*>(sender));
    
    auto popup = APPhoneCodePopup::createView();
    
    popup->setDelegate(this);
    
    this->addChild(popup,POPUP_ZORDER);
    
}

void APRegisterScreen::onShowPasswordButtonClicked(Ref * sender)
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
    else if (auto textFiled = cocos2d::utils::findChild<ui::EditBox*>(this, k_tag == SHOW_PASS_BUTTON_TAG_IN_PASSWORD_TEXTFIELD ? "textfield_password" : "textfield_repassword"))
    {
        bool isShowPassword = (textFiled->getInputFlag() == ui::EditBox::InputFlag::PASSWORD);
        
        if (auto eye = utils::findChild<ImageView*>(button, "eye"))
        {
            eye->loadTexture(isShowPassword ? "new_v2/eye.png" : "new_v2/eye_enable.png");
        }
        
        textFiled->setInputFlag(isShowPassword ? ui::EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS : ui::EditBox::InputFlag::PASSWORD);
        textFiled->setText(textFiled->getText());
    }
}

void APRegisterScreen::updateSkipUI()
{
    if(auto loginView = dynamic_cast<APLoginView*>(m_loginView))
    {
       if(auto panel = cocos2d::utils::findChild<Layout*>(loginView, "panel_skip"))
       {
           if(auto skipButton = cocos2d::utils::findChild(this, "skip_button"))
           {
               skipButton->setVisible(panel->isVisible());
           }
       }
    }
}

void APRegisterScreen::onPhoneCodeItemSelected(PhoneCodeInfo i_info)
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

cocos2d::ui::Widget::ccWidgetClickCallback APRegisterScreen::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(APRegisterScreen, onRegisterButtonClicked),//locate at parent (APPopupBase)
		CLICK_MAP(APRegisterScreen, onFacebookButtonClicked),
		CLICK_MAP(APRegisterScreen, onGmailButtonClicked),
		CLICK_MAP(APRegisterScreen, onSkipButton),
		CLICK_MAP(APRegisterScreen, onLoginButtonClicked),
		CLICK_MAP(APRegisterScreen, onPhoneCodeButtonClicked),
		CLICK_MAP(APRegisterScreen, onShowPasswordButtonClicked),
        CLICK_MAP(APRegisterScreen, onBack),
        
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void APRegisterScreen::editBoxEditingDidBegin(cocos2d::ui::EditBox * editBox)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	moveScreenWhenUseTextfiled(this, APPopupBase::AnimationDirection::TOP);
#endif
	log("editBox %p DidBegin !", editBox);
}

void APRegisterScreen::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox * editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)
{
	log("editBox %p DidEnd with action %d!", editBox, action);
}

void APRegisterScreen::editBoxTextChanged(cocos2d::ui::EditBox * editBox, const std::string & text)
{
	if (editBox->getName() == "textfield_phone")
	{
		validPhoneInput(editBox, text);
	}

	if (editBox && editBox->getName() == "textfield_password")
	{
		trimSpace(editBox);
	}
	else if (editBox && editBox->getName() == "textfield_repassword")
	{
		trimSpace(editBox);
	}

	//log("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void APRegisterScreen::editBoxReturn(cocos2d::ui::EditBox * editBox)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	moveScreenWhenUseTextfiled(this, APPopupBase::AnimationDirection::DOWN);
#endif
	//log("editBox %p was returned !", editBox->getName().c_str());
}
void APRegisterScreen::onBack(cocos2d::Ref * sender)
{
    auto scene = MSLanguageSelection::createScene();
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
    this->removeFromParent();
}

