#include "APAccountConnectionScreen.h"
#include "APLoginScreen.h"
#include "APConnectWithCurrentAccount.h"
#include "APHaveUsedMonkeyApps.h"
#include "APLoginView.h"
#include "APUpdatePhoneNumberScreen.h"
#include "APLoginMethodsScreen.h"
#include "CleverTapManager.h"
#include "HelperManager.h"

INITIALIZE_READER(APAccountConnectionScreen);

std::once_flag v4_ap_account_connection_screen_flag;

APAccountConnectionScreen::APAccountConnectionScreen()
{
	m_isFromNotHaveUsedApps = false;
}

#pragma mark - Initialize

APAccountConnectionScreen * APAccountConnectionScreen::createView(ap::api::Login::LoginDataInfo info)
{
	std::call_once(v4_ap_account_connection_screen_flag, []
	{
		REGISTER_CSB_READER(APAccountConnectionScreen);
	});

	auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APAccountConnectionScreen.csb");

	auto p = static_cast<APAccountConnectionScreen*>(CSLoader::createNodeWithVisibleSize(csb_name));

	if (p && p->didLoadFromCSB(info))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

void APAccountConnectionScreen::fillPhoneNumber(std::string i_phoneNumber, std::string i_country, std::string i_code)
{
	if (auto text = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
	{
		text->setTextColor(Color4B::BLACK);
		text->setString(i_phoneNumber);
	}
}

void APAccountConnectionScreen::changeTextTitle()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"), "key.v4.connection.with.a.account2",APPopupBase::TextType::BOLD},
	});
}

bool APAccountConnectionScreen::didLoadFromCSB(ap::api::Login::LoginDataInfo info)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_info = info;

	HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

	setupForPhoneNumberTextfield(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"));

	APPopupBase::setupForPasswordTextfield(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"),this);
	
	animationMoveToLeft(cocos2d::utils::findChild(this, "white_background"));

	m_onCancelCallBack = [=]() 
	{
		hideVirtualKeyboardOnScreen();
	};
	
	if(CONFIG_MANAGER->isVietnameseStories())
	{
		if(auto connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "connection_button"))
		{
			connectionButton->loadTextures("account/new_v4/connection_button_vm.png","account/new_v4/connection_button_vm.png", "account/new_v4/connection_button_vm.png");
		}

		if (auto backButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_back"))
		{
			backButton->loadTextures("account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png");
		}

		if (auto  title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
			title->setTextColor(Color4B(66, 46, 135, 255));
		}

		if (auto backButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_back"))
		{
			backButton->loadTextures("account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png");
		}

		if (auto image = cocos2d::utils::findChild<ui::ImageView*>(cocos2d::utils::findChild(this,"textfield_phone_bg"), "textfield_enable"))
		{
			image->loadTexture("account/new_v2/textfied_enable2_vm.png");
		}

		if (auto image = cocos2d::utils::findChild<ui::ImageView*>(cocos2d::utils::findChild(this, "textfield_password_bg"), "textfield_enable"))
		{
			image->loadTexture("account/new_v2/textfied_enable2_vm.png");
		}
	}

	return true;
}

void APAccountConnectionScreen::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"), "key.v4.connection.with.a.account",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "small_title"), "key.v4.connection.with.a.account.small",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "or_txt"), "key.v4.txt.or",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"), "key.accountv3.screen.updatephone.placeholder",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"), "key.accountv3.text.password",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "connection_button"), "key.v4.connection",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "skip_button"), "key.skip",APPopupBase::TextType::BOLD},
		});
}

void APAccountConnectionScreen::onExit()
{
	APPopupBase::onExit();
}

void APAccountConnectionScreen::onConnectionClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APAccountConnectionScreen::onConnectionClicked");
		return;
	}

	button->setTouchEnabled(false);

	playClickedSoundEffect();

	auto phoneNumberTextfield  = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone");

	auto passwordTextfield  = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password");

	hideVirtualKeyboard(std::vector<cocos2d::Node*>{ phoneNumberTextfield, passwordTextfield });

	removeSpace(std::vector<cocos2d::Node*>{ phoneNumberTextfield, passwordTextfield });

	if (phoneNumberTextfield->getString().empty())
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.please.enter.phone.number"));

		button->setTouchEnabled(true);

		return;
	}

	if (phoneNumberTextfield->getString().length() < PHONE_MIN_LENGTH || phoneNumberTextfield->getString().length() > PHONE_MAX_LENGTH)
	{
		showFailedPopup(getDisplayTextByKey("key.invalid.phone.number"));

		button->setTouchEnabled(true);

		return;
	}

	if (passwordTextfield->getString().empty())
	{
		showFailedPopup(getDisplayTextByKey("ap_waring_entering_password"));

		button->setTouchEnabled(true);

		return;
	}

	if (cocos2d::StringUtils::StringUTF8(passwordTextfield->getString()).length() < PASSWORD_MIN_LENGTH )
	{
		showFailedPopup(getDisplayTextByKey("ap_waring_password"));

		button->setTouchEnabled(true);

		return;
	}

	showWaittingView();

	ap::api::Login::callV3(ap::api::Login::LoginType::PHONE, "", phoneNumberTextfield->getString(), "", "", passwordTextfield->getString(), "", "", m_info.newAccessToken,[=](int code, bool is_success, std::string msg2, std::string tokenToChangePassword, ap::api::Login::LoginDataInfo info)
	{
		removeWaittingView();

		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg2, code,tokenToChangePassword, info, button] {

			if (is_success)
			{
				if (code == 200)
				{
                    clevertapTracking("connect_with_phone","yes","no");

					switch (info.type)
					{
					case ap::api::Login::V4Type::DEFAULT:
					{
						MJDEFAULT->setBoolForKey("account_unverified", false);
						cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");

						break;
					}
					case ap::api::Login::V4Type::DEFAULT_WITH_OLD_USER_INFO:
					{
						auto newInfo = info;

						newInfo.currentUserInfo = m_info.currentUserInfo;

						newInfo.newAccessToken = m_info.newAccessToken;//importance: use for change password

						auto screen = APLoginMethodsScreen::createView(newInfo, APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD);

						screen->setNeedToCallLinkAccount(true);

						this->getParent()->addChild(screen, this->getLocalZOrder());

						this->removeFromParent();

						break;
					}
					case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT:
					{
						auto screen = APLoginMethodsScreen::createView(info, APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD);

						screen->setNeedToCallLinkAccount(true);

						this->getParent()->addChild(screen, this->getLocalZOrder());

						this->removeFromParent();

						break;
					}
					case ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_DONT_HAVE_OLD_ACCOUNT:
					{
						auto screen = APHaveUsedMonkeyApps::createView(info);

						Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);

						break;
					}
					default:
						break;
					}
				}
			}
			else
			{
                clevertapTracking("connect_with_phone","no","no");

				this->removeChildByName("waiting_view");

				showFailedPopup(msg2);

				button->setTouchEnabled(true);
			}
		});
	});

}

void APAccountConnectionScreen::onFacebookButtonClicked(cocos2d::Ref * sender)
{
	hideVirtualKeyboardOnScreen();

	if (auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
	{
		if (auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
		{
            auto eventFacebookDefaut = EventListenerCustom::create("event_show_change_default_password_screen", [this](EventCustom* i_userData)
                                                                   {
                clevertapTracking("connect_with_fb","yes","no");
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    
                    ap::api::Login::LoginDataInfo* data = nullptr;
                    
                    if (i_userData)
                    {
                        data = static_cast<ap::api::Login::LoginDataInfo*>(i_userData->getUserData());
                    }
                    
                    if (data)
                    {
                        ap::api::Login::LoginDataInfo info(*data);
                        
                        info.currentUserInfo = m_info.currentUserInfo;
                        
                        info.newAccessToken = m_info.newAccessToken;
                        
                        auto screen = APLoginMethodsScreen::createView(info, APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD);
                        
                        screen->setNeedToCallLinkAccount(true);
                        
                        this->getParent()->addChild(screen, this->getLocalZOrder());
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_show_change_default_password_screen");
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_show_update_phone_number_screen");
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_login_when_link_account_failed");
                        
                        this->removeFromParent();
                    }
                });
            });
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventFacebookDefaut, this);
			
            auto failedEvent = EventListenerCustom::create("event_login_when_link_account_failed", [this](EventCustom* i_userData)
                                                           {
                clevertapTracking("connect_with_fb","no","no");
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    
                    this->getEventDispatcher()->removeCustomEventListeners("event_show_change_default_password_screen");
                    
                    this->getEventDispatcher()->removeCustomEventListeners("event_show_update_phone_number_screen");
                    
                    this->getEventDispatcher()->removeCustomEventListeners("event_login_when_link_account_failed");
                    
                    //this->removeFromParent();
                    
                });
            });
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(failedEvent, this);
         
            auto eventUpdatePhoneNumber = EventListenerCustom::create("event_show_update_phone_number_screen", [this](EventCustom* i_userData) {
                
                clevertapTracking("connect_with_fb","no","yes");
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                    
                    ap::api::Login::LoginDataInfo* data = nullptr;
                    
                    if (i_userData)
                    {
                        data = static_cast<ap::api::Login::LoginDataInfo*>(i_userData->getUserData());
                    }
                    
                    if (data)
                    {
                        ap::api::Login::LoginDataInfo info(*data);
                        
                        info.currentUserInfo = m_info.currentUserInfo;
                        
                        info.newAccessToken = m_info.newAccessToken;
                        
                        auto screen = APUpdatePhoneNumberScreen::createView(info, APUpdatePhoneNumberScreen::START_FROM::CONNECTION_SCREEN_WITH_FACEBOOK);
                        
                        this->getParent()->addChild(screen, this->getLocalZOrder());
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_show_change_default_password_screen");
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_show_update_phone_number_screen");
                        
                        this->getEventDispatcher()->removeCustomEventListeners("event_login_when_link_account_failed");
                        
                        this->removeFromParent();
                    }
                    
                    delete data;
                    
                });
            });
            
            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventUpdatePhoneNumber, this);
			
		}

		loginView->onFacebook(this, m_info.newAccessToken);

	}
}

void APAccountConnectionScreen::onGmailButtonClicked(cocos2d::Ref * sender)
{
	hideVirtualKeyboardOnScreen();

	if (auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
	{
       
        auto failedEvent = EventListenerCustom::create("event_login_when_link_account_failed", [this](EventCustom* i_userData)
                                                       {
            clevertapTracking("connect_with_gmail","no","no");
            
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                
                this->getEventDispatcher()->removeCustomEventListeners("event_show_change_default_password_screen");
                
                this->getEventDispatcher()->removeCustomEventListeners("event_show_update_phone_number_screen");
                
                this->getEventDispatcher()->removeCustomEventListeners("event_login_when_link_account_failed");
                
                //this->removeFromParent();
                
            });
        });
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(failedEvent, this);
       
        
		auto ev = EventListenerCustom::create("event_show_change_default_password_screen", [this](EventCustom* i_userData)
		{
            clevertapTracking("connect_with_gmail","yes","no");

            
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				ap::api::Login::LoginDataInfo* data = nullptr;

				if (i_userData)
				{
					data = static_cast<ap::api::Login::LoginDataInfo*>(i_userData->getUserData());
				}

				if (data)
				{
					ap::api::Login::LoginDataInfo info(*data);

					info.currentUserInfo = m_info.currentUserInfo;

					info.newAccessToken = m_info.newAccessToken;

					auto screen = APLoginMethodsScreen::createView(info, APLoginMethodsScreen::ScreenType::CONTINUE_TO_CHANGE_PASSWORD);

					screen->setNeedToCallLinkAccount(true);

					this->getParent()->addChild(screen, this->getLocalZOrder());

					this->getEventDispatcher()->removeCustomEventListeners("event_show_change_default_password_screen");

					this->removeFromParent();
				}

				delete data;
			});
		});

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(ev, this);

		loginView->onGoogle(this,true, m_info.newAccessToken);
	}
}

void APAccountConnectionScreen::onSkipButton(cocos2d::Ref * sender)
{
	hideVirtualKeyboardOnScreen();

	auto screen = APUpdatePhoneNumberScreen::createView(m_info, m_isFromNotHaveUsedApps ? APUpdatePhoneNumberScreen::START_FROM::HAVE_USED_APPS :  APUpdatePhoneNumberScreen::START_FROM::CONNECTION_SCREEN);

	this->getParent()->addChild(screen, this->getLocalZOrder());

	this->removeFromParent();
}

void APAccountConnectionScreen::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
	hideVirtualKeyboardOnScreen();

	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APAccountConnectionScreen::onShowPasswordButtonClicked");
		return;
	}

	if (auto textFiled = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"))
	{
		bool isShowPassword = textFiled->isPasswordEnabled();

		if (auto eye = utils::findChild<ImageView*>(button, "eye"))
		{
			eye->loadTexture(isShowPassword ? "new_v2/eye.png" : "new_v2/eye_enable.png");
		}

		textFiled->setPasswordEnabled(!isShowPassword);
	}
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APAccountConnectionScreen::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APAccountConnectionScreen, onCancel),
		CLICK_MAP(APAccountConnectionScreen, onConnectionClicked),
		CLICK_MAP(APAccountConnectionScreen, onFacebookButtonClicked),
		CLICK_MAP(APAccountConnectionScreen, onGmailButtonClicked),
		CLICK_MAP(APAccountConnectionScreen, onSkipButton),
		CLICK_MAP(APAccountConnectionScreen, onShowPasswordButtonClicked),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void APAccountConnectionScreen::hideVirtualKeyboardOnScreen()
{
	hideVirtualKeyboard(std::vector<cocos2d::Node*>{ cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"), cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password") });
}

void APAccountConnectionScreen::clevertapTracking(std::string connectionType, std::string connectSuccessfull,std::string fbUpdate)
{
    try
    {
          CleverTapManager::GetInstance()->storePendingEvent("login_connection",
          {
              {"device_type",cocos2d::Value(m_startFrom == START_FROM::ConnectWithCurrentAccount ? "old":"new")},
              {"connection_type",cocos2d::Value(connectionType)},
              {"connect_successfull",cocos2d::Value(connectSuccessfull)},
              {"facebook_update_phone",cocos2d::Value(fbUpdate)},
          });
    }
    catch (const std::runtime_error& e)
    {
       
    }
}
