#include "APEnterPhoneNumberPopup.h"
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
#include "StoryLanguageManager.h"
#include "APLoginPopup.h"
#include "APRegisterPopup.h"
#include "APLoginAPI.h"
#include "APEmailOTP.h"

USING_NS_CC;
INITIALIZE_READER(APEnterPhoneNumberPopup);

std::once_flag ap_enter_phone_number_popup_reader_flag;

APEnterPhoneNumberPopup * APEnterPhoneNumberPopup::createView() 
{
	std::call_once(ap_enter_phone_number_popup_reader_flag, [] {
		REGISTER_CSB_READER(APEnterPhoneNumberPopup);
		});

	auto p = static_cast<APEnterPhoneNumberPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APEnterPhoneNumberPopup.csb"));

	if (p && p->didLoadFromCSB())
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APEnterPhoneNumberPopup::didLoadFromCSB()
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	showingAnimation(utils::findChild(this, "popup_bg"));

	if (auto phoneTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield"))
	{
		phoneTextfield->setPlaceHolderColor(Color4B(155, 155, 155, 255));
		phoneTextfield->setCursorChar('|');
		phoneTextfield->setCursorEnabled(true);
		//phoneTextfield->setMaxLength(10);
		phoneTextfield->addEventListener([=](Ref *pSender, cocos2d::ui::TextField::EventType i_type) {

			auto box = utils::findChild(this, "popup_bg");

			switch (i_type)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			case TextField::EventType::ATTACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
				break;
			case TextField::EventType::DETACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
				break;
#endif
			case cocos2d::ui::TextField::EventType::INSERT_TEXT:
			{
				validPhoneInput(phoneTextfield);

				(phoneTextfield->getString().size() == 0) ? phoneTextfield->setTextColor(Color4B(155, 155, 155, 255)) : phoneTextfield->setTextColor(Color4B::BLACK);

				enableButtonBasedOnValidPhoneNumber(phoneTextfield->getString(), cocos2d::utils::findChild<ui::Button*>(this, "send_button"));

				break;
			}
			case cocos2d::ui::TextField::EventType::DELETE_BACKWARD:
			{
				validPhoneInput(phoneTextfield);

				(phoneTextfield->getString().size() == 0) ? phoneTextfield->setTextColor(Color4B(155, 155, 155, 255)) : phoneTextfield->setTextColor(Color4B::BLACK);

				enableButtonBasedOnValidPhoneNumber(phoneTextfield->getString(), cocos2d::utils::findChild<ui::Button*>(this, "send_button"));

				break;
			}
			default:
				break;
			}

		});

	}

	return true;
}

void APEnterPhoneNumberPopup::onEnter() 
{
	ui::Layout::onEnter();
}

void APEnterPhoneNumberPopup::onExit() 
{
	ui::Layout::onExit();
}

void APEnterPhoneNumberPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_phone_number_title"));
	}

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield"))
	{

		inputTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_phone"));
	}

	if (auto sendTitle = cocos2d::utils::findChild<ui::Text*>(this, "send_title"))
	{
		sendTitle->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_title"));
	}
}

void APEnterPhoneNumberPopup::onSend(Ref * sender)
{
	auto sendButton = cocos2d::utils::findChild<ui::Button*>(this, "send_button");

	sendButton->setTouchEnabled(false);

	auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "phone_textfield");

	hideVirtualKeyboard(inputTextfield);

	const std::string  k_phoneText = inputTextfield->getString();

	auto warningLabel = cocos2d::utils::findChild<ui::Text*>(this, "warning_label");

	const bool k_isPhoneNumberInvalided = (k_phoneText.empty() 
		|| (LICENSE_INFO_MODEL->isDisplayLanguageVN()  && k_phoneText.front() != '0' && (k_phoneText.size() > 12 || k_phoneText.size() < 9))
		|| (!LICENSE_INFO_MODEL->isDisplayLanguageVN() && k_phoneText.front() != '+' && k_phoneText.size() < 10 ));
	
	warningLabel->setVisible(k_isPhoneNumberInvalided);

	if (k_isPhoneNumberInvalided)
	{
		warningLabel->setVisible(true);
		warningLabel->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_wrong_phone_number"));

		runWarningActionOnBackgroundNode();

		sendButton->setTouchEnabled(true);
		//show arlert
		//showErrorAlert(LANGUAGE_MANAGER->getDisplayTextByKey("ap_wrong_phone_number"));
		return;
	}

	showWaittingView();

	//call API
	ap::api::Login::call(ap::api::Login::LoginType::PHONE, "", k_phoneText, "", "", "", "","", [=](int error_code, bool is_success, std::string msg)
	{
		sendButton->setTouchEnabled(true);
		removeWaittingView();

		if (is_success) 
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] 
			{
				MJDEFAULT->setBoolForKey("account_unverified", false);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			});
		}
		else 
		{
			if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_OTP_POPUP)
			{
				// verify otp verification
				cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, k_phoneText, error_code]
				{
					MJDEFAULT->setBoolForKey("account_unverified", false);

					auto email_otp = APEmailOTP::createView(k_phoneText, APEmailOTP::OTP_TYPE::PHONE, (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP);
					this->addChild(email_otp);

				});
			}
			else
			{
				if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_GOOGLE_LOGIN_POPUP) //303
				{
					// verify google verification
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});
						ap::Auth::authenticateWithGoogle("");
						this->removeFromParent();
					});
				}
				else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_CREATING_PASSWORD_POPUP) //306
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, k_phoneText] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});

						auto popup = APRegisterPopup::createView(APRegisterPopup::REGISTER_TYPE::PHONE, k_phoneText);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else if (error_code == (int)ap::api::RESPONSE_CODE_FROM_REGISTER_API::OPEN_ENTERING_PASSWORD_POPUP)  //307
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, k_phoneText] {
						ms::EventLogger::getInstance().logEvent("open_success_screen_3nd", {
							{"open_3nd_type", cocos2d::Value("email")},
							{"is_upgrade", cocos2d::Value(MJDEFAULT->getBoolForKey("is_skip_login", false) ? "true" : "false")}
							});

						auto popup = APLoginPopup::createView(APLoginPopup::LOGIN_TYPE::PHONE, k_phoneText);
						this->getParent()->addChild(popup);
						this->removeFromParent();
					});
				}
				else
				{
					cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
						this->removeChildByName("waiting_view");
					});

					CCLOG("error: %s", msg.c_str());
					showFailedPopup(msg);
				}
			}
		}
	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APEnterPhoneNumberPopup::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APEnterPhoneNumberPopup, onCancel),
        CLICK_MAP(APEnterPhoneNumberPopup, onSend),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }
	return nullptr;
}


