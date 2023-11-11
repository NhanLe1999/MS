#include "APUpdatePhoneNumberScreen.h"
#include "APLoadUpdateAPI.h"
#include "APUpdatePhoneNumberAPI.h"
#include "APChangeDefaultPasswordScreen.h"
#include "APAccountConnectionScreen.h"
#include "APLinkUserCrmToAccount.h"
#include "HelperManager.h"
#include "CleverTapManager.h"

USING_NS_CC;
INITIALIZE_READER(APUpdatePhoneNumberScreen);

std::once_flag v4_ap_update_phone_number_screen_flag;

#pragma mark - Initialize
APUpdatePhoneNumberScreen * APUpdatePhoneNumberScreen::createView(ap::api::Login::LoginDataInfo info, START_FROM i_from)
{
    std::call_once(v4_ap_update_phone_number_screen_flag, []
    {
        REGISTER_CSB_READER(APUpdatePhoneNumberScreen);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/account/new_v4/APUpdatePhoneNumberScreen.csb");

    auto p = static_cast<APUpdatePhoneNumberScreen *>(CSLoader::createNodeWithVisibleSize(csb_name));

    if (p && p->didLoadFromCSB(info, i_from))
    {
        return p;
    }

    CC_SAFE_DELETE(p);

    return nullptr;
}

bool APUpdatePhoneNumberScreen::didLoadFromCSB(ap::api::Login::LoginDataInfo info, START_FROM i_from)
{
	m_info = info;

	m_from = i_from;

	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	animationMoveToLeft(cocos2d::utils::findChild(this, "layout"));

	HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

	setupForPhoneNumberTextfield(cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"),PHONE_MAX_LENGTH_WITH_COUNTRY_CODE);

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "txt_country"))
	{
		text->setString(MJDEFAULT->getStringForKey("phonecode_id", "VN"));
	}

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
	{
		text->setString(MJDEFAULT->getStringForKey("phonecode_code", "+84"));
	}

	m_onCancelCallBack = [=]() {

		//write code for back function.

	};

	if (CONFIG_MANAGER->isVietnameseStories())
	{
		if (auto connectionButton = cocos2d::utils::findChild<ui::Button*>(this, "update_button"))
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

		if (auto image = cocos2d::utils::findChild<ui::ImageView*>(cocos2d::utils::findChild(this, "textfield_phone_bg"), "textfield_enable"))
		{
			image->loadTexture("account/new_v2/textfied_enable2_vm.png");
		}
	}

	return true;
}

void APUpdatePhoneNumberScreen::changeDisplayLanguage()
{
	setTextAndFontForNodes({
		{cocos2d::utils::findChild<ui::Text*>(this, "title"), "key.accountv3.screen.updatephone.title",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Text*>(this, "small_title"), "key.accountv4.screen.updatephone.desc",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"), "key.accountv3.screen.updatephone.placeholder",APPopupBase::TextType::BOLD},
		{cocos2d::utils::findChild<ui::Button*>(this, "update_button"), "key.accountv3.button.title.update",APPopupBase::TextType::BOLD}
	});
}

void APUpdatePhoneNumberScreen::onExit()
{
	APPopupBase::onExit();
}

void APUpdatePhoneNumberScreen::onUpdateButtonClicked(cocos2d::Ref * sender)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(sender);

	if (!button)
	{
		CCLOG("Error|APUpdatePhoneNumberScreen::onUpdateButtonClicked");
		return;
	}

	auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone");

	if (!inputTextfield)
	{
		CCLOG("APResetPasswordScreen::onContineButtonClicked|inputTextfield = nullptr !");

		return;
	}

	button->setTouchEnabled(false);

	hideVirtualKeyboard(inputTextfield);

	removeSpace(inputTextfield);

	const std::string k_phoneNumber = inputTextfield->getString();

	if (k_phoneNumber.empty())
	{
		showFailedPopup(getDisplayTextByKey("key.please.enter.phone.number"));

		button->setTouchEnabled(true);

		return;
	}

	if (k_phoneNumber.length() < PHONE_MIN_LENGTH || inputTextfield->getString().length() > PHONE_MAX_LENGTH_WITH_COUNTRY_CODE)
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("key.invalid.phone.number"));

		button->setTouchEnabled(true);

		return;
	}

	if (!isDigitString(k_phoneNumber))
	{
		APPopupBase::showErrorAlert(getDisplayTextByKey("key.invalid.phone.number"));

		button->setTouchEnabled(true);

		return;
	}

	std::string countryCode = "";

	if (auto txtCode = cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
	{
		countryCode = txtCode->getString();
	}

	Login::LoginType type = (m_from == START_FROM::CONNECTION_SCREEN_WITH_FACEBOOK ? Login::LoginType::FACEBOOK: Login::LoginType::USER_NAME);

	bool isAppUsed = (!(m_from == START_FROM::HAVE_USED_APPS));
	
	//	const std::string & i_newAccessToken,
	//	const std::string & i_phone,
	//	const std::string &i_countryCode,
	//	const bool& i_isWeb,
	//	const std::string& i_oldAccessToken,
	//	const bool& i_isOldTokenUse,
	//	const bool& i_isAppUsed

	APUpdatePhoneNumberAPI::call(type, m_info.newAccessToken, k_phoneNumber, countryCode, type == Login::LoginType::USER_NAME, m_info.oldAccessToken, m_from == START_FROM::CONNECTION_SCREEN_WITH_FACEBOOK, isAppUsed,[=](bool i_isSuccess, int code, std::string i_msg, std::string i_token, int i_userID)
	{
		if (i_isSuccess)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				auto gotoChangePasswordScreen = [=](APChangeDefaultPasswordScreen::START_FROM i_from,ap::api::Login::LoginDataInfo i_info) {
				
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
					{
						auto screen = APChangeDefaultPasswordScreen::createView(i_info, i_from);

						screen->setPhoneNumber(k_phoneNumber);

						screen->setCountryCode(countryCode);

						this->getParent()->addChild(screen, this->getLocalZOrder());
					});
				
				};

				if (m_from == START_FROM::CONNECTION_SCREEN_WITH_FACEBOOK)
				{
					linkAccountAPICall(m_info, [=]() {
					
						gotoChangePasswordScreen(APChangeDefaultPasswordScreen::START_FROM::UPDATE_PHONE_NUMBER_SCREEN_FOR_FACEBOOK,m_info);

					});
				}
				else
				{
                    auto newInfo = m_info;
                    
					newInfo.oldAccessToken  = i_token;
					newInfo.oldUserInfo._id = i_userID;

                    pushCleverTapEvent("yes");
                    
					gotoChangePasswordScreen(APChangeDefaultPasswordScreen::START_FROM::UPDATE_PHONE_NUMBER_SCREEN,newInfo);
				}

			});
		}
		else
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				if (code == 316)
				{
					StoryAlert::showAlert2Button(i_msg, getDisplayTextByKey("key.skip"), getDisplayTextByKey("key.accountv3.button.title.confirm"), false, 32, this,0,"csb/StoryAlert_2button_new.csb");
				}
				else
				{
                    if (m_from != START_FROM::CONNECTION_SCREEN_WITH_FACEBOOK)
                    {
                       pushCleverTapEvent("no");
                    }
                    
					showErrorAlert(i_msg);
				}

				button->setTouchEnabled(true);

			});

		}
	});

}

void APUpdatePhoneNumberScreen::onPhoneCodeButtonClicked(cocos2d::Ref * sender)
{
	hideVirtualKeyboardOnScreen();

	auto popup = APPhoneCodePopup::createView();

	popup->setDelegate(this);

	this->addChild(popup, POPUP_ZORDER);
}

void APUpdatePhoneNumberScreen::onPhoneCodeItemSelected(PhoneCodeInfo i_info)
{
	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "txt_country"))
	{
		text->setString(i_info.id);

		CCLOG("");
	}

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "txt_code"))
	{
		text->setString(i_info.code);
	}
}

void APUpdatePhoneNumberScreen::onAlertOK(cocos2d::Ref * sender)
{

}

void APUpdatePhoneNumberScreen::onAlertCancel(cocos2d::Ref * sender)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]
	{

		auto phoneTxt = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone");

		auto countryTxt = cocos2d::utils::findChild<ui::Text*>(this, "txt_country");
	
		auto codeTxt = cocos2d::utils::findChild<ui::Text*>(this, "txt_code");

		auto screen = APAccountConnectionScreen::createView(m_info);

        screen->setStartFrom(APAccountConnectionScreen::START_FROM::UpdatePhoneNumberScreen);

		screen->setIsFromNotHaveUsedApps(true);

		this->getParent()->addChild(screen, this->getLocalZOrder());

		if (phoneTxt && countryTxt && codeTxt)
		{
			screen->fillPhoneNumber(phoneTxt->getString(), countryTxt->getString(), codeTxt->getString());
		}

		//this->removeFromParent();
	});
}

#pragma mark - Button Handling
cocos2d::ui::Widget::ccWidgetClickCallback APUpdatePhoneNumberScreen::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(APUpdatePhoneNumberScreen, onCancel),//on APPopupBase
		CLICK_MAP(APUpdatePhoneNumberScreen, onUpdateButtonClicked),
		CLICK_MAP(APUpdatePhoneNumberScreen, onPhoneCodeButtonClicked)
	};

	if (function_map.find(callBackName) != function_map.end())
	{
		return function_map.at(callBackName);
	}

	return nullptr;
}

void APUpdatePhoneNumberScreen::hideVirtualKeyboardOnScreen()
{
	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield_phone"))
	{
		hideVirtualKeyboard(inputTextfield);
	}
}

void APUpdatePhoneNumberScreen::linkAccountAPICall(ap::api::Login::LoginDataInfo i_info, std::function<void()> i_callback )
{
	showWaittingViewInCocosThread();

	APLinkUserCrmToAccount::call(i_info.newAccessToken, i_info.oldAccessToken, [=](bool i_isSuccess, std::string i_msg) {

		removeWaittingView();

		if (i_isSuccess)
		{
			if (i_callback)
			{
				i_callback();
			}
		}
		else
		{
			if (!i_msg.empty())
			{
				showErrorAlert(i_msg);
			}
			else
			{
				showNetworkDownAlert(nullptr, [=]()
				{
					linkAccountAPICall(i_info, i_callback);
				});
			}
		}
	});
}

void APUpdatePhoneNumberScreen::pushCleverTapEvent(std::string i_connectionStatus)
{
      try
      {
            CleverTapManager::GetInstance()->storePendingEvent("login_connection",
            {
                {"device_type",cocos2d::Value( m_info.type ==ap::api::Login::V4Type::LOGIN_WITH_USER_NAME_AND_DEVICE_HAVE_OLD_ACCOUNT ? "old":"new")},
                {"connection_type",cocos2d::Value("connect_with_new_phone")},
                {"connect_successfull",cocos2d::Value(i_connectionStatus)},
                {"facebook_update_phone",cocos2d::Value("no")},
            });
      }
      catch (const std::runtime_error& e)
      {
         
      }
}
