//
//  APLoginView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APLoginView.h"
#include "APAuth.h"
#include "APProfileNameView.h"
#include "APWaitView.h"
#include "api/APLoginAPI.h"
#include "api/APProfilesListAPI.h"
#include "api/APLoadUpdateAPI.h"
#include "APDatabase.h"
#include "APProfileManager.h"
#include "LicenseInfoModel.h"
#include "StoryLanguageManager.h"
#include "MSIntro.h"
#include "MSHome.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"
#include "APCalculator.h"
#include "APEmailLogin.h"
#include "APPhoneLogin.h"
#include "APPopup.h"
#include "APLoginAPI.h"
#include "StoryConfigManager.h"
#include "new/APEnterPhoneNumberPopup.h"
#include "APRegisterScreen.h"
#include "APLoginScreen.h"
USING_NS_CC;

INITIALIZE_READER(APLoginView);

std::once_flag ap_login_view_reader_flag;

APLoginView * APLoginView::createView(SCREEN_TYPE i_type)
{
	std::call_once(ap_login_view_reader_flag, []
    {
		REGISTER_CSB_READER(APLoginView);
	});

	auto node = reinterpret_cast<APLoginView *>(CSLoader::createNodeWithVisibleSize("csb/account/Login.csb"));
	
    if (node)
    {
		node->didLoadFromCSB(i_type);
		return node;
	}

	CC_SAFE_DELETE(node);
	
    return nullptr;
}

void APLoginView::didLoadFromCSB(SCREEN_TYPE i_type)
{
//    if (!StoryConfigManager::getInstance()->isCountryVN())
//    {
//        MJDEFAULT->setIntegerForKey(key_lang_display, 1);
//    }
//    else
//    {
//        MJDEFAULT->setIntegerForKey(key_lang_display, 4);
//    }
	
    _is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
    
    if (!_is_vn)
    {
        auto txt_skip = cocos2d::utils::findChild<Text*>(this, "text_skip");
        txt_skip->setString("Skip");
        auto gachngang = cocos2d::utils::findChild<Layout*>(this, "gachngang");
        gachngang->setContentSize(Size(60, 4));
        gachngang->setAnchorPoint(Vec2(0.5, 0.5));
        gachngang->setPositionX(txt_skip->getContentSize().width / 2);
    }
    
    APLoginView::initQuickLoginButton();
 
    if (MJDEFAULT->getBoolForKey("is_skip_login", false))
    {
        MJDEFAULT->setBoolForKey("logged_in_skip", true);
        MJDEFAULT->setBoolForKey("is_skip", false);
    }
	
    if (MJDEFAULT->getBoolForKey("is_skip", false))
    {
		auto delay = 8;
		this->scheduleOnce([=](float dt)
        {
			cocos2d::utils::findChild<Layout*>(this, "panel_skip")->setVisible(true);
            
            if(auto screen = dynamic_cast<APRegisterScreen*>(m_currentScreen))
            {
                screen->updateSkipUI();
            }
            
		}, delay, "show panel skip");
	}
    /*if (MJDEFAULT->getBoolForKey("is_skip", false) || MJDEFAULT->getBoolForKey("account_unverified", false)) {
        quick_login->setVisible(false);
    }(*/
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    
	utils::findChild<Layout*>(this, "Panel_4")->setVisible(true);

	if (auto scene = Director::getInstance()->getRunningScene())
	{
		auto button = utils::findChild<Layout*>(this, "Panel_4_0")->clone();

		button->setVisible(true);

		auto size = Director::getInstance()->getVisibleSize();

		button->setPosition(Director::getInstance()->getVisibleOrigin() + Vec2(size.width - button->getContentSize().width, size.height/2.f));

		button->addClickEventListener([=](Ref*) {

			this->onSkip(nullptr);
		});

		scene->addChild(button,INT_MAX);
	}

    utils::findChild<Layout*>(this, "Panel_4_0")->setVisible(true);
#endif
	
    if (CONFIG_MANAGER->isVietnameseStories())
    {
		utils::findChild<Layout*>(this, "Panel_1")->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
	}
    
    utils::findChild<Text*>(this, "Text_15_0")->setTextHorizontalAlignment(TextHAlignment::CENTER);
    utils::findChild<Text*>(this, "Text_15_0")->setTextVerticalAlignment(TextVAlignment::CENTER);

    auto screenSize = Director::getInstance()->getVisibleSize();

    const float k_currScreenRatio = (screenSize.width / screenSize.height);

    if(k_currScreenRatio >= 19.f/9.f) //20/9,21/9
	{
    	if(auto bg =  utils::findChild<Layout*>(this, "Panel_1"))
		{
			bg->setIgnoreAnchorPointForPosition(false);
			bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			bg->setScale((1.1f*screenSize.width)/bg->getContentSize().width);//110%
			bg->setPosition(bg->getParent()->getContentSize()/2);
		}
	}
    
    this->setVisible(false);
    
    if(i_type == SCREEN_TYPE::REGISTER)
    {
        auto registerScreen = APRegisterScreen::createView(this);
        APPopupBase::sAnimationMove(registerScreen, APPopupBase::AnimationDirection::RIGHT);
        Director::getInstance()->getRunningScene()->addChild(registerScreen,INT_MAX -1 );
        m_currentScreen = registerScreen;
    }
    else
    {
        auto loginScreen = APLoginScreen::createView(this);
        APPopupBase::sAnimationMove(loginScreen, APPopupBase::AnimationDirection::LEFT);
        Director::getInstance()->getRunningScene()->addChild(loginScreen,INT_MAX -1 );
        m_currentScreen = loginScreen;
    }
    
    auto check = MJDEFAULT->getBoolForKey("is_skip_login", false);
    
    if (!check)
    {
        auto calculator = APCalculator::createView();
        calculator->setAnchorPoint(Vec2(0.5, 0.5));
        calculator->setPosition(cocos2d::Director::getInstance()->getVisibleSize() / 2);
        calculator->setEnabled(true);
        calculator->setTouchEnabled(true);
//        calculator->setCallback([=](){
//            
//           // APPopupBase::sAnimationMove(m_currentScreen,(i_type == SCREEN_TYPE::REGISTER)? APPopupBase::AnimationDirection::RIGHT: APPopupBase::AnimationDirection::LEFT);
//
//        });
        Director::getInstance()->getRunningScene()->addChild(calculator,INT_MAX -1 );
    }
    
    addDeviceId();

    auto origin        = Director::getInstance()->getVisibleOrigin();

    auto blackLayer = LayerColor::create(cocos2d::Color4B::WHITE);
    blackLayer->setIgnoreAnchorPointForPosition(false);
    blackLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    blackLayer->setName("blackLayer");
    blackLayer->setPosition(origin+screenSize/2);
    this->addChild(blackLayer, INT_MAX-1);
}

void APLoginView::initQuickLoginButton()
{
    auto user_info = ap::Database::getInstance().getUserInfo();

    Button* quick_login  = cocos2d::utils::findChild<Button*>(this, "quick_login");
     
     auto txt_quick_login = quick_login->getChildByName<Text*>("text");
     
    std::string msg = CONFIG_MANAGER->isVietnameseStories() ? "" : LANGUAGE_MANAGER->getDisplayTextByKey("key.quick.login");
         
     if (!user_info._name.empty() && user_info._name != "")
     {
         quick_login->setVisible(true);
         msg += user_info._name;
         txt_quick_login->setString(msg);
     }
     else
     {
         if (!user_info._mail.empty() && user_info._mail != "")
         {
             quick_login->setVisible(true);
             msg += user_info._mail;
             txt_quick_login->setString(msg);
         }
         else
         {
             if (!user_info._phone.empty() && user_info._phone != "") {
                 quick_login->setVisible(true);
                 msg += user_info._phone;
                 txt_quick_login->setString(msg);
             }
         }
     }
}

void APLoginView::onEnter() 
{
	ui::Layout::onEnter();
	changeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
	ap::Auth::init();
    ms::EventLogger::getInstance().logEvent("view_login", {});
}

void APLoginView::onExit() 
{
	ui::Layout::onExit();
}

void APLoginView::onLoggedIn()
{
    MJDEFAULT->setBoolForKey("CL_Loggin_Called",true);
    
    CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN,nullptr);

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
    {
		MJDEFAULT->setBoolForKey(key_logged_in, true);
        
		if (_login_callback)
		{
			_login_callback();
		}
        
        if(m_currentScreen)
        {
            m_currentScreen->removeFromParent();
        }
        
		this->removeFromParent();
	});
}

void APLoginView::onFacebook(Ref * sender, std::string i_newAccessToken)
{
    auto currentScreen = dynamic_cast<Node*>(sender);
    
	ms::EventLogger::getInstance().logEvent("click_login", 
	{
		{ "login_type", cocos2d::Value("facebook") },
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
	});
    
	ms::EventLogger::getInstance().logEvent("open_success_screen_3nd",
	{
		{"open_3nd_type", cocos2d::Value("facebook")},
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
	});

	auto wait_view = APWaitView::createView(std::string("Please proceed on your browser"));
	wait_view->setName("waiting_view");
	currentScreen->addChild(wait_view);

	ap::Auth::authenticateWithFacebook(i_newAccessToken);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.facebooklogin.android.finished", [this,i_newAccessToken](EventCustom* e)
	{
		auto data = reinterpret_cast<std::pair<std::string*, std::string*>*>(e->getUserData());
		//auto access_token = reinterpret_cast<std::string *>(e->getUserData());
		CCLOG("nam access_token: %s %s", data->first->c_str(), data->second->c_str());
		ap::Auth::authenticateWithFacebookToken(*data->first, *data->second,i_newAccessToken);
	}), this);

	auto e_cancel = EventListenerCustom::create("event_login_cancelled", [this,currentScreen](EventCustom* e)
	{
		if (MJDEFAULT->getBoolForKey("is_skip", false))
        {
			cocos2d::utils::findChild<Layout*>(this, "panel_skip")->setVisible(true);
            if(auto screen = dynamic_cast<APRegisterScreen*>(m_currentScreen))
            {
                screen->updateSkipUI();
            }
		}
        
        pushCleverTapTrackingEvent("facebook", false, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

		currentScreen->removeChildByName("waiting_view");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_cancelled");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
        
        ms::EventLogger::getInstance().logEvent("event_login_cancelled", {{"login_type", cocos2d::Value("facebook") }});
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e_cancel, this);

	auto listener = EventListenerCustom::create("event_completed_login_browser", [this,currentScreen](EventCustom* e)
	{
        CCLOG("bắt event");
		currentScreen->removeChildByName("waiting_view");
		auto wait_view = APWaitView::createView(std::string("Sign in to Monkey Cloud"));
		wait_view->setName("waiting_view");
		currentScreen->addChild(wait_view);
		this->getEventDispatcher()->removeCustomEventListeners("event_completed_login_browser");
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto e = EventListenerCustom::create("event_login_successfully", [this](EventCustom*) 
	{
		ms::EventLogger::getInstance().logEvent("event_login_successfull", 
		{
			{"login_type",cocos2d::Value("facebook")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});

		pushCleverTapTrackingEvent("facebook", true, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

		MJDEFAULT->deleteValueForKey("is_skip_login");
        MJDEFAULT->deleteValueForKey("logged_in_skip");
        
		this->onLoggedIn();
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);
}

void APLoginView::onGoogle(Ref * sender,bool isGmail, std::string i_newAccessToken)
{
    auto currentScreen = dynamic_cast<Node*>(sender);

	ms::EventLogger::getInstance().logEvent("click_login",
    {
		{ "login_type", cocos2d::Value("email") },
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
	});

    auto wait_view = APWaitView::createView(std::string("Please proceed on your browser"));
    wait_view->setName("waiting_view");
    currentScreen->addChild(wait_view);
    
    if(isGmail)
    {
        ap::Auth::authenticateWithGoogle(i_newAccessToken);
    }

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_remove_waitting_view", [this, currentScreen](EventCustom* e)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			if (currentScreen)
			{
				if (auto waittingView = cocos2d::utils::findChild(currentScreen, "waiting_view"))
				{
					waittingView->removeFromParent();
				}
			}
		});

	}), this);

//    auto email_login = APEmailLogin::createView();
//    email_login->setName("email_login");
//    currentScreen->addChild(email_login);

	auto e_cancel = EventListenerCustom::create("event_login_cancelled", [this,currentScreen](EventCustom* e)
	{
		if (MJDEFAULT->getBoolForKey("is_skip", false)) 
		{
			cocos2d::utils::findChild<Layout*>(this, "panel_skip")->setVisible(true);
            if(auto screen = dynamic_cast<APRegisterScreen*>(m_currentScreen))
            {
                screen->updateSkipUI();
            }
		}
        
        pushCleverTapTrackingEvent("email", false, MJDEFAULT->getBoolForKey("is_skip_login", false),"cancel");

		currentScreen->removeChildByName("waiting_view");
        //currentScreen->removeChildByName("email_login");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_cancelled");
        this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
        
         ms::EventLogger::getInstance().logEvent("event_login_cancelled", {{"login_type", cocos2d::Value("google") }});
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e_cancel, this);

	auto listener = EventListenerCustom::create("event_completed_login_browser", [this,currentScreen](EventCustom* e)
	{
		currentScreen->removeChildByName("waiting_view");
		auto wait_view = APWaitView::createView(std::string("Sign in to Monkey Cloud"));
		wait_view->setName("waiting_view");
		currentScreen->addChild(wait_view);
		this->getEventDispatcher()->removeCustomEventListeners("event_completed_login_browser");
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto e = EventListenerCustom::create("event_login_successfully", [this](EventCustom* i_data)
    {
		//ms::EventLogger::getInstance().setUserProperties("is_unverified_account", "false");
		ms::EventLogger::getInstance().logEvent("event_login_successfull", {
			{ "login_type",cocos2d::Value("email") },
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
			});

		std::string type = "email";

		if(i_data!= nullptr)
		{
			if (auto data = reinterpret_cast<std::string*>(i_data->getUserData()))
			{
				CCLOG("login|event_login_successfully dynamic_cast:success", (*data).c_str());

				type = (*data);
			}
		}

		CCLOG("login|event_login_successfully data type:%s", type.c_str());

		pushCleverTapTrackingEvent(type,true, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

		MJDEFAULT->deleteValueForKey("is_skip_login");
        MJDEFAULT->deleteValueForKey("logged_in_skip");
		//nếu onboard thì gọi
		this->onLoggedIn();
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);

	auto fail_listener = cocos2d::EventListenerCustom::create("event.googleSignIn.failed", [this](cocos2d::EventCustom* e) {

		//             getEventDispatcher()->removeCustomEventListeners("event.googleSignIn.failed");

	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(fail_listener, this);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(cocos2d::EventListenerCustom::create("event.googleSignIn.finished", [this, i_newAccessToken](cocos2d::EventCustom* e) {

		CCLOG("login|called event.googleSignIn.finished");

		this->getEventDispatcher()->removeCustomEventListeners("event.googleSignIn.finished");
		auto info = reinterpret_cast<std::vector<std::string>*> (e->getUserData());
		auto token = info->at(0);
		auto mail = info->at(1);
		CCLOG("TEST TOKEN %s", token.c_str());
		CCLOG("TEST EMAIL %s", mail.c_str());
		ap::Auth::authenticateWithGoogleToken(token, mail, i_newAccessToken);
	}), this);
}

void APLoginView::onPhone(Ref * sender) 
{
	ms::EventLogger::getInstance().logEvent("click_login", 
	{
		{ "login_type", cocos2d::Value("phone") },
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
	});

//    auto phone_login_view = APEnterPhoneNumberPopup::createView();
//	phone_login_view->setName("phone_login_view");
//    this->addChild(phone_login_view);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.accountkit.android.finished", [this](EventCustom* e) 
	{
		auto data = reinterpret_cast<std::pair<std::string*, std::string*>*>(e->getUserData());
		//auto access_token = reinterpret_cast<std::string *>(e->getUserData());
		CCLOG("nam access_token: %s %s", data->first->c_str(), data->second->c_str());
		std::string phone_number = data->second->c_str();
		std::string token = data->first->c_str();
		ap::Auth::authenticateWithAKToken(token, phone_number);
	}), this);


	auto e_cancel = EventListenerCustom::create("event_login_cancelled", [this](EventCustom* e)
	{
		if (MJDEFAULT->getBoolForKey("is_skip", false)) 
		{
			cocos2d::utils::findChild<Layout*>(this, "panel_skip")->setVisible(true);
            if(auto screen = dynamic_cast<APRegisterScreen*>(m_currentScreen))
            {
                screen->updateSkipUI();
            }
		}

        pushCleverTapTrackingEvent("phone", false, MJDEFAULT->getBoolForKey("is_skip_login", false),"cancel");

		m_currentScreen->removeChildByName("waiting_view");
        //m_currentScreen->removeChildByName("phone_login_view");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_cancelled");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");

         ms::EventLogger::getInstance().logEvent("event_login_cancelled", {{"login_type", cocos2d::Value("phone") }});
	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e_cancel, this);

	auto listener = EventListenerCustom::create("event_completed_login_browser", [this](EventCustom* e) {
		m_currentScreen->removeChildByName("waiting_view");
		auto wait_view = APWaitView::createView(std::string("Sign in to Monkey Cloud"));
		wait_view->setName("waiting_view");
		m_currentScreen->addChild(wait_view);
		this->getEventDispatcher()->removeCustomEventListeners("event_completed_login_browser");
	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto evLoginSuccessfully = EventListenerCustom::create("event_login_successfully", [this](EventCustom*) 
	{
		ms::EventLogger::getInstance().logEvent("event_login_successfull", 
		{
		   { "login_type", cocos2d::Value("phone")},
		   {"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});
		
		pushCleverTapTrackingEvent("phone", true, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

		MJDEFAULT->deleteValueForKey("is_skip_login");
        MJDEFAULT->deleteValueForKey("logged_in_skip");

		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
		this->onLoggedIn();
	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(evLoginSuccessfully, this);
}
void APLoginView::onQuickLogin(Ref* sender) 
{
	auto wait_view = APWaitView::createView(std::string("Please proceed on your browser"), false, true);
	wait_view->setName("waiting_view");
	m_currentScreen->addChild(wait_view);
	this->onLoggedIn();
    
    pushCleverTapTrackingEvent(MJDEFAULT->getStringForKey("ap_login_type","reloginafterlogout"),true, false,"none");

    
    //MJDEFAULT->setBoolForKey("CL_Loggin_Called",false);

}

void APLoginView::onSignOut(Ref * sender) 
{
	ap::Auth::signOut();
	MJDEFAULT->setBoolForKey(key_logged_in, false);
	auto view = APLoginView::createView(SCREEN_TYPE::LOGIN);
	view->setName("demo_ap_login");
	Director::getInstance()->getRunningScene()->addChild(view);
}

void APLoginView::onSkipLogin(Ref* sender) 
{
	ms::EventLogger::getInstance().logEvent("click_login", {
			{"login_type", cocos2d::Value("skip") },
			{"is_upgrade", cocos2d::Value("false")}
		});
	auto wait_view = APWaitView::createView(std::string("Please proceed on your browser"));
	wait_view->setName("waiting_view");
	this->addChild(wait_view);
	auto e_cancel = EventListenerCustom::create("event_login_cancelled", [this](EventCustom* e) 
	{
		if (MJDEFAULT->getBoolForKey("is_skip", false))
        {
			cocos2d::utils::findChild<Layout*>(this, "panel_skip")->setVisible(true);
        
            if(auto screen = dynamic_cast<APRegisterScreen*>(m_currentScreen))
            {
                screen->updateSkipUI();
            }
		}
		this->removeChildByName("waiting_view");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_cancelled");
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");

		ms::EventLogger::getInstance().logEvent("event_login_cancelled", { {"login_type", cocos2d::Value("skip") } });
	});
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e_cancel, this);

	auto e = EventListenerCustom::create("event_login_successfully", [this](EventCustom*) 
	{
        MJDEFAULT->setBoolForKey("account_unverified", true);

		ms::EventLogger::getInstance().logEvent("event_login_successfull", 
		{
			{ "login_type", cocos2d::Value("skip")},
			{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
		});

		pushCleverTapTrackingEvent("skip", true, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

		this->onLoggedIn();
		this->getEventDispatcher()->removeCustomEventListeners("event_login_successfully");
	});
	
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);

	ms::EventLogger::getInstance().logEvent("popup_after_click_login_show", 
	{
		{"show_type", cocos2d::Value("skip")},
		{"is_upgrade", cocos2d::Value("false")}
	});

    auto msg = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("key.popup.skip.msg").c_str(),7);
    
    if (MJDEFAULT->getBoolForKey("is_skip_login", false))
    {
        msg = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("key.popup.skip.msg").c_str(),5);
    }
    
    if (MJDEFAULT->getBoolForKey(key_is_submit))
    {
        ap::api::Login::call(ap::api::Login::LoginType::SKIP, "", "", "", "", "", "", [](int error_code, bool is_success, std::string message) {
            if (is_success) {
                MJDEFAULT->setBoolForKey("is_skip_login", true);
                if (MJDEFAULT->getIntegerForKey("time_skip_popup_upgrade_account", 0) == 0)
                    MJDEFAULT->setIntegerForKey("time_skip_popup_upgrade_account", mj::helper::getTime());
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
            }
            else {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {
                    bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([message, is_vn] {
                        if (message == "") {
                            StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                        }
                        else {
                            //TODO: show notification
                            StoryAlert::showAlert(message, "OK", LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.back"));
                        }
                        });
                    });
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
            }
        });
        return;
    }
	auto popup = AccountPopup::show({ msg,LANGUAGE_MANAGER->getDisplayTextByKey("lb.playlist.play.continue"),LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"), },
		[=]() {
			ms::EventLogger::getInstance().logEvent("popup_after_click_login_continue", {
			{"continue_type", cocos2d::Value("skip")},
			{"is_upgrade", cocos2d::Value("false")}
				});
			ap::api::Login::call(ap::api::Login::LoginType::SKIP, "", "", "", "", "", "", [](int error_code, bool is_success, std::string message) {
				if (is_success) {
					MJDEFAULT->setBoolForKey("is_skip_login", true);
					if (MJDEFAULT->getIntegerForKey("time_skip_popup_upgrade_account", 0) == 0)
						MJDEFAULT->setIntegerForKey("time_skip_popup_upgrade_account", mj::helper::getTime());
					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
				}
				else {
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([message] {
						bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
						Director::getInstance()->getScheduler()->performFunctionInCocosThread([message, is_vn] {
							if (message == "") {
								StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
							}
							else {
								//TODO: show notification
								StoryAlert::showAlert(message, "OK", is_vn ? "Đóng" : "Close");
							}
							});
						});
					cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_cancelled");
				}
				});
		},[=]() {
			ms::EventLogger::getInstance().logEvent("popup_after_click_login_cancel", {
				{"cancel_type", cocos2d::Value("skip")},
				{"is_upgrade", cocos2d::Value("false")}
				});
			this->removeChildByName("waiting_view"); 
		});
    
	ui::Button* btn_right = utils::findChild<Button*>(popup, "btn_right");
    ui::Button* btn_left = utils::findChild<Button*>(popup, "btn_left");
    auto pos_left = btn_right->getPosition();
    auto pos_right = btn_left->getPosition();
    btn_right->setPosition(pos_right);
    btn_left->setPosition(pos_left);
    
}

void APLoginView::onLoginSuccess(std::string type)
{
	ms::EventLogger::getInstance().logEvent("event_login_successfull", {
		{ "login_type",cocos2d::Value(type) },
		{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
	});

	CCLOG("login|event_login_successfully data type:%s", type.c_str());

	MJDEFAULT->deleteValueForKey("is_skip_login");
	MJDEFAULT->deleteValueForKey("logged_in_skip");

	//nếu onboard thì gọi
	this->onLoggedIn();
    
    pushCleverTapTrackingEvent(type, true, MJDEFAULT->getBoolForKey("is_skip_login", false),"none");

}

void APLoginView::onClose(Ref * sender) {
	exit(0);
}

void APLoginView::onSkip(Ref * sender) {
	auto scene = MSHome::createScene();
    //MJDEFAULT->setBoolForKey(key_feature_phonics, true);
	Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
}

void APLoginView::changeDisplayLanguage(int lang_id) {
	utils::findChild<Text*>(this, "Text_15")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.login"));
	utils::findChild<Text*>(this, "Text_15_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.login.text"));

}

void APLoginView::addDeviceId()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
		this->removeChildByName("text_device_id");
		auto id = MJ_PLATFORM_CONFIG.getDeviceId();
		auto text = ui::Text::create(StringUtils::format("Device ID: %s", id.c_str()), "fonts/Montserrat-SemiBold.ttf", 35);
		text->setName("text_device_id");
		text->setFontSize(20);
		text->setTextColor(Color4B::BLACK);
		text->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
		text->setPosition(Point(Director::getInstance()->getVisibleSize().width - 20, Director::getInstance()->getVisibleSize().height - 20));
		text->setOpacity(0x99);
        
        Director::getInstance()->getRunningScene()->addChild(text, INT_MAX - 1);
	});
}

ui::Widget::ccWidgetClickCallback APLoginView::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APLoginView, onClose),
		//CLICK_MAP(APLoginView, onFacebook),
		//CLICK_MAP(APLoginView, onGoogle),
		CLICK_MAP(APLoginView, onPhone),
		CLICK_MAP(APLoginView, onSkip),
		CLICK_MAP(APLoginView, onQuickLogin),
		CLICK_MAP(APLoginView, onSkipLogin),
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}


void APLoginView::pushCleverTapTrackingEvent(std::string i_loginType, bool i_isSuccess, bool i_isUpgrade, std::string i_failedReason)
{
    if(auto scene = Director::getInstance()->getRunningScene())
    {
        if(i_isSuccess)
        {
            scene->runAction(Sequence::createWithTwoActions(DelayTime::create(2.0f),CallFunc::create([=]() {
                
                CleverTapManager::GetInstance()->pushPendingEvents();

                CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::UPDATE,nullptr);
                
                try
                {
                    MJDEFAULT->setStringForKey("ap_login_type",i_loginType);
                    
                    CleverTapManager::GetInstance()->pushEvent("event_login",
                                                               {
                        {"view_screen",cocos2d::Value(true)},
                        {"login_type",cocos2d::Value(i_loginType)},
                        {"login_successfull",cocos2d::Value(i_isSuccess)},
                        {"is_upgrade",cocos2d::Value(i_isUpgrade)},
                        {"login_failed_reason",cocos2d::Value(i_failedReason)},
                        {"forgot_password",cocos2d::Value(MJDEFAULT->getStringForKey("CL_FORGOT_PASSWORD","not_yet"))},
                    });
                    
                }
                catch (const std::exception&)
                {
                    
                }
                
            })));
        }
        else
        {
            try
            {
                MJDEFAULT->setStringForKey("ap_login_type",i_loginType);
                
                CleverTapManager::GetInstance()->pushEvent("event_login",
                                                           {
                    {"view_screen",cocos2d::Value(true)},
                    {"login_type",cocos2d::Value(i_loginType)},
                    {"login_successfull",cocos2d::Value(i_isSuccess)},
                    {"is_upgrade",cocos2d::Value(i_isUpgrade)},
                    {"login_failed_reason",cocos2d::Value(i_failedReason)},
                    {"forgot_password",cocos2d::Value(MJDEFAULT->getStringForKey("CL_FORGOT_PASSWORD","not_yet"))},
                });
                
            }
            catch (const std::exception&)
            {
                
            }
        }
    
    }
}
