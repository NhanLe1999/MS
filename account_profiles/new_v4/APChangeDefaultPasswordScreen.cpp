#include "APChangeDefaultPasswordScreen.h"
#include "APUpdatePasswordAPI.h"
#include "APLinkUserCrmToAccount.h"
#include "APLoginMethodsScreen.h"
#include "APUpdatePhoneNumberAPI.h"
#include "APPasswordAPI.h"
#include "StoryAlert.h"
#include "APLoginView.h"
#include "CleverTapManager.h"

INITIALIZE_READER(APChangeDefaultPasswordScreen);

std::once_flag v4_ap_change_default_password_screen_flag;

#pragma mark - Initialize

APChangeDefaultPasswordScreen * APChangeDefaultPasswordScreen::createView(ap::api::Login::LoginDataInfo info, START_FROM i_from)
{
	std::call_once(v4_ap_change_default_password_screen_flag, []
	{
		REGISTER_CSB_READER(APChangeDefaultPasswordScreen);
	});

	auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APChangeDefaultPasswordScreen.csb");

	auto p = static_cast<APChangeDefaultPasswordScreen*>(CSLoader::createNodeWithVisibleSize(csb_name));

	if (p && p->didLoadFromCSB(info, i_from))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APChangeDefaultPasswordScreen::didLoadFromCSB(ap::api::Login::LoginDataInfo info, START_FROM i_from)
{
	m_info = info;

	m_from = i_from;

	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}
	
	m_passwordTextfields =
	{
		 cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"),
		 cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword"),
	};

	for (auto inputTextfield : m_passwordTextfields)
	{
		setupForPasswordTextfield(inputTextfield, utils::findChild(this, "background"));
	}

	animationMoveToLeft(cocos2d::utils::findChild(this, "background"));

	if (auto btnBack = cocos2d::utils::findChild<ui::Button*>(this, "btn_back"))
	{
		btnBack->removeFromParent();
	}

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		if (auto connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "connection_button"))
		{
			connectionButton->loadTextures("account/new_v4/connection_button_vm.png", "account/new_v4/connection_button_vm.png", "account/new_v4/connection_button_vm.png");
		}

		if (auto backButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_back"))
		{
			backButton->loadTextures("account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png", "account/new_v3/button_back_left_vm.png");
		}

		if (auto  title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
		{
			title->setTextColor(Color4B(66, 46, 135, 255));
		}

		if (auto image = cocos2d::utils::findChild<ui::ImageView*>(cocos2d::utils::findChild(this, "textfield_repassword_bg"), "textfield_enable"))
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

void APChangeDefaultPasswordScreen::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"), "key.v4.change.password.title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "small_title"), "key.v4.default.password.small.title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password"), "key.accountv3.screen.updatepassword.new.password",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword"), "key.accountv3.screen.updatepassword.renew.password",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "connection_button"), "key.v4.change.password.button.title",APPopupBase::TextType::BOLD}
		});
}

void APChangeDefaultPasswordScreen::onExit()
{
	APPopupBase::onExit();
}

void APChangeDefaultPasswordScreen::onConnectionClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APChangeDefaultPasswordScreen::onShowPasswordButtonClicked");
		return;
	}

	button->setTouchEnabled(false);

	playClickedSoundEffect();

	hideVirtualKeyboard(m_passwordTextfields);

	removeSpace(m_passwordTextfields);

	auto passwordTextfield				= cocos2d::utils::findChild<ui::TextField*>(this, "textfield_password");

	auto reenteringPasswordTextfield	= cocos2d::utils::findChild<ui::TextField*>(this, "textfield_repassword");

	if (passwordTextfield->getString().empty())
	{
		showFailedPopup(getDisplayTextByKey("ap_waring_entering_password"));

		button->setTouchEnabled(true);

		return;
	}

	if (reenteringPasswordTextfield->getString().empty())
	{
		showFailedPopup(getDisplayTextByKey("key.reender.password"));

		button->setTouchEnabled(true);

		return;
	}

	if (cocos2d::StringUtils::StringUTF8(passwordTextfield->getString()).length() < PASSWORD_MIN_LENGTH || cocos2d::StringUtils::StringUTF8(reenteringPasswordTextfield->getString()).length() < PASSWORD_MIN_LENGTH)
	{
		showFailedPopup(getDisplayTextByKey("ap_waring_password"));

		button->setTouchEnabled(true);

		return;
	}

	if (!passwordTextfield->getString().empty() && !reenteringPasswordTextfield->getString().empty() && passwordTextfield->getString() != reenteringPasswordTextfield->getString())
	{
		showFailedPopup(getDisplayTextByKey("ap_match_password"));

		button->setTouchEnabled(true);

		return;
	}

	m_connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "connection_button");

	const std::string k_password = passwordTextfield->getString();


	if (m_from == START_FROM::LOGIN_SCREEN)
	{
		changePasswordV3APICall(k_password);
	}
	else if (m_from == START_FROM::UPDATE_PHONE_NUMBER_SCREEN)
	{
        updatePasswordAPICall(k_password);
	}
	else if (m_from == START_FROM::UPDATE_PHONE_NUMBER_SCREEN_FOR_FACEBOOK)
	{
		updatePasswordAPICall(k_password);
	}
	else
	{
		updatePasswordAPICall(k_password);
	}
}

void APChangeDefaultPasswordScreen::onShowPasswordButtonClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APChangeDefaultPasswordScreen::onShowPasswordButtonClicked");
		return;
	}

	const int k_tag = button->getTag();

	if (auto textFiled = cocos2d::utils::findChild<ui::TextField*>(this, k_tag == SHOW_PASS_BUTTON_TAG_IN_PASSWORD_TEXTFIELD ? "textfield_password" : "textfield_repassword"))
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
cocos2d::ui::Widget::ccWidgetClickCallback APChangeDefaultPasswordScreen::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APChangeDefaultPasswordScreen, onCancel),
		CLICK_MAP(APChangeDefaultPasswordScreen, onConnectionClicked),
		CLICK_MAP(APChangeDefaultPasswordScreen, onShowPasswordButtonClicked),
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void APChangeDefaultPasswordScreen::updatePasswordAPICall(std::string i_password)
{
	showWaittingViewInCocosThread();

    auto enableButton = [=]()
    {
        if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "connection_button"))
        {
            button->setTouchEnabled(true);
        }
    };
    
	ap::api::UpdatePassword::callDefaulPasswordChange(m_info.newAccessToken, i_password, m_info.oldAccessToken, [=](bool is_success, std::string msg) {

		removeWaittingView();

		if (is_success)
		{
            clevertapTracking("login_change_password",true);
            
            auto info = m_info;
            
			if (m_from == START_FROM::UPDATE_PHONE_NUMBER_SCREEN || m_from == START_FROM::UPDATE_PHONE_NUMBER_SCREEN_FOR_FACEBOOK)
			{
				info.oldUserInfo._type.push_back(ap::api::Login::PHONE);
				info.oldUserInfo._phone = m_phoneNumber;
			}

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				auto screen = APLoginMethodsScreen::createView(info, APLoginMethodsScreen::ScreenType::CONTINUE_TO_SHOW_PROFILE);

				this->getParent()->addChild(screen, this->getLocalZOrder());

                enableButton();
                

				//this->removeFromParent();
			});
		}
		else
		{
            clevertapTracking("login_change_password",false);
            
            enableButton();

			//retry
			showNetworkDownAlert([=]() {
					
				updatePasswordAPICall(i_password);

			});
		}
	});
}

void APChangeDefaultPasswordScreen::changePasswordV3APICall(std::string i_password)
{
	showWaittingViewInCocosThread();

	ap::api::APPasswordAPI::changePasswordV3Call( i_password, [=](bool is_success, std::string msg ,const rapidjson::Value &data) {

		removeWaittingView();

		if (is_success)
		{
            clevertapTracking("login_change_password_after_exited",true);
            
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				if (auto loginView = dynamic_cast<APLoginView*>(Director::getInstance()->getRunningScene()->getChildByName("loginView")))
				{
					loginView->onLoginSuccess("user_name");
				}

				this->removeFromParent();
			});
		}
		else
		{
            clevertapTracking("login_change_password_after_exited",false);

			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				if (auto button = cocos2d::utils::findChild<ui::Button*>(this, "connection_button"))
				{
					button->setTouchEnabled(true);
				}
			});

			//retry
			showNetworkDownAlert(nullptr,[=]() {

				changePasswordV3APICall(i_password);

			});
		}
	});
}

void APChangeDefaultPasswordScreen::clevertapTracking(std::string i_evName,bool isSuccessfull)
{
    try
    {
        CleverTapManager::GetInstance()->storePendingEvent(i_evName,
        {
                {"is_successfull", cocos2d::Value(isSuccessfull) },
        });
    }
    catch (const std::exception&)
    {

    }
}
