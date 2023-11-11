#include "APLoginPopup.h"
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
#include "APPasswordRecoveryMethods.h"

USING_NS_CC;
INITIALIZE_READER(APLoginPopup);

std::once_flag ap_login_popup_reader_flag;

APLoginPopup * APLoginPopup::createView(LOGIN_TYPE i_type, std::string i_mailOrPhoneText)
{
	std::call_once(ap_login_popup_reader_flag, [] 
	{
		REGISTER_CSB_READER(APLoginPopup);
	});

	auto p = static_cast<APLoginPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APLoginPopup.csb"));

	if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneText))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APLoginPopup::didLoadFromCSB(LOGIN_TYPE i_type, std::string i_mailOrPhoneText)
{
	m_mailOrPhoneText = i_mailOrPhoneText;

	m_loginType = i_type;

	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}


	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	
	});

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "input_textfield"))
	{
		reduceFontSizeOfTextfieldBasedOnLength(inputTextfield, 22);

		inputTextfield->setEnabled(false);
		inputTextfield->setString(i_mailOrPhoneText);
	}

	if (auto forgotPassButton = cocos2d::utils::findChild<ui::Button*>(this, "forget_pasword_button"))
	{
		forgotPassButton->getTitleLabel()->enableUnderline();
		forgotPassButton->getTitleLabel()->updateContent();
	}

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield"))
	{
		inputTextfield->setPlaceHolderColor(Color4B(155, 155, 155, 255));
		inputTextfield->setTextColor(Color4B(155, 155, 155, 255));
		inputTextfield->setCursorChar('|');
		inputTextfield->setCursorEnabled(true);
		inputTextfield->setMaxLengthEnabled(true);
		inputTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
		//enableAButtonBasedOnTextfieldLength(cocos2d::utils::findChild<ui::Button*>(this, "login"), inputTextfield, PASSWORD_MIN_LENGTH);

		enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PASSWORD_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "login"));

		inputTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
		{
			auto box = utils::findChild(this, "popup_bg");

			switch (type)
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			case TextField::EventType::ATTACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
				break;
			case TextField::EventType::DETACH_WITH_IME:
				box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
				break;
#endif
			case TextField::EventType::INSERT_TEXT:
			{
				(inputTextfield->getString().size() == 0)?inputTextfield->setTextColor(Color4B(155, 155, 155, 255)): inputTextfield->setTextColor(Color4B::BLACK);
				enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PASSWORD_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "login"));
			}
			case TextField::EventType::DELETE_BACKWARD:
			{
				(inputTextfield->getString().size() == 0) ? inputTextfield->setTextColor(Color4B(155, 155, 155, 255)) : inputTextfield->setTextColor(Color4B::BLACK);
				enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), PASSWORD_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "login"));
			}
			break;
			default:
				break;
			}
		});
	}

	return true;
}

void APLoginPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_login_title"));
	}

	if (auto inputTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield"))
	{
		inputTextfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("ap_enter_password"));
	}

	if (auto loginButtonTitle = cocos2d::utils::findChild<ui::Text*>(this, "login_button_title"))
	{
		loginButtonTitle->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_login_button_title"));
	}

	if (auto forgotPassButton = cocos2d::utils::findChild<ui::Button*>(this, "forget_pasword_button"))
	{
		forgotPassButton->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("ap_forgot_pass_title"));
	}
}

void APLoginPopup::onLogin(Ref * sender)
{
	auto passwordTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "password_textfield");

	hideVirtualKeyboard(passwordTextfield);

	auto loginButton = cocos2d::utils::findChild<ui::Button*>(this, "login");

	loginButton->setTouchEnabled(false);

	if (passwordTextfield->getString().empty())
	{
		showFailedPopup(LANGUAGE_MANAGER->getDisplayTextByKey("ap_waring_entering_password"));

		loginButton->setTouchEnabled(true);

		return;
	}

	showWaittingView();

	const std::string k_emailText = (m_loginType == LOGIN_TYPE::EMAIL ? m_mailOrPhoneText : "");

	const std::string k_phoneText = (m_loginType == LOGIN_TYPE::EMAIL ? "" : m_mailOrPhoneText);

	ap::api::Login::call(m_loginType == LOGIN_TYPE::EMAIL ? ap::api::Login::GOOGLE: ap::api::Login::PHONE, "", k_phoneText, k_emailText, "", "", "", passwordTextfield->getString(),[=](int error_code, bool is_success2, std::string msg2)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success2, msg2, error_code, loginButton] {
			
			loginButton->setTouchEnabled(true);

			if (is_success2)
			{
				MJDEFAULT->setBoolForKey("account_unverified", false);
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_login_successfully");
			}
			else
			{
				removeWaittingView();

				showFailedPopup(msg2);
			}
		});

	});
}

void APLoginPopup::onForgetPasswordClicked(Ref * sender)
{
	showWaittingView();

	const std::string k_emailText = (m_loginType == LOGIN_TYPE::EMAIL ? m_mailOrPhoneText : "");

	const std::string k_phoneText = (m_loginType == LOGIN_TYPE::EMAIL ? "" : m_mailOrPhoneText);

	ap::api::APPasswordAPI::forgotPasswordCall(k_emailText, k_phoneText, [=](bool i_success, std::string i_msg, const rapidjson::Value &i_data)
	{
		//if(!i_data.Empty() && i_data.HasMember("")

		removeWaittingView();

		if (i_success)
		{
			CCLOG("success");

			MJDEFAULT->setStringForKey("CL_FORGOT_PASSWORD", "success");

			ReponseData responseData;

			if (JSON_SAFE_CHECK(i_data, "email", String))
			{
				responseData.m_email = i_data["email"].GetString();
			}

			if (JSON_SAFE_CHECK(i_data, "phone", String))
			{
				responseData.m_phone = i_data["phone"].GetString();
			}

			if (JSON_SAFE_CHECK(i_data, "is_free", Bool))
			{
				responseData.m_isFree = i_data["is_free"].GetBool();
			}

			if (JSON_SAFE_CHECK(i_data, "verify_answer", Array))
			{
				auto questions = i_data["verify_answer"].GetArray();

				for (size_t i = 0; i < questions.Size(); i++)
				{
					auto quesObjet = questions[i].GetObject();

					SecurityQuestion question;

					//question name
					question.m_question = quesObjet["question"].GetString();

					//question answers
					if (JSON_SAFE_CHECK(quesObjet, "answer", Array))
					{
						auto  answerArr = quesObjet["answer"].GetArray();

						for (size_t j = 0; j < answerArr.Size(); j++)
						{
							auto ansObj = answerArr[j].GetObject();

							const std::string k_ansText = ansObj["text"].GetString();

							question.m_answers.push_back(k_ansText);

							if (ansObj["is_result"].GetBool())
							{
								question.m_rightAnswer = k_ansText;
							}
						}
					}

					responseData.m_questions.push_back(question);
				}
			}

			if (!MJDEFAULT->getBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true))
			{
				const auto k_oldDay = MJDEFAULT->getIntegerForKey(HOURS_FOR_SECURITY_QUESTION, -1);

				const auto k_currDay = converMillisecondsToHours(cocos2d::utils::getTimeInMilliseconds());

				if (k_oldDay != -1 && (k_currDay - k_oldDay >= 24))
				{
					MJDEFAULT->deleteValueForKey(HOURS_FOR_SECURITY_QUESTION);
					MJDEFAULT->setBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true);
					MJDEFAULT->setIntegerForKey(FAILED_VERIFICATION_COUNT_KEY, 0);
				}
			}

			// !(A||B) = !A && !B
			// !(A && B) = !A || !A
			const bool k_isVerificatonOTPCodePopupShow = !(responseData.m_isFree || (!responseData.m_isFree && !responseData.m_email.empty() && !responseData.m_phone.empty()))
				|| (responseData.m_isFree && !MJDEFAULT->getBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true) && (responseData.m_email.empty() || responseData.m_phone.empty()));

			if (k_isVerificatonOTPCodePopupShow) // if pay user && user only have 1 option -> we will redirect to APOTPPopup screen 
			{
				ap::api::APSendOTPAPI::call(m_loginType == APLoginPopup::LOGIN_TYPE::PHONE ? AP_API_SEND_OTP_PHONE_TYPE : AP_API_SEND_OTP_EMAIL_TYPE, k_phoneText, k_emailText,[=](bool success, std::string msg, const rapidjson::Value &sendAPIOTPData)
				{
					if (success) 
					{
						Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
						{
							auto popup = APOTPPopup::createView((APOTPPopup::OTP_TYPE)m_loginType, m_mailOrPhoneText,"");

							this->getParent()->addChild(popup, POPUP_ZORDER);

							//this->removeFromParentWithAnmation();
						});
					}
					else
					{
						this->showFailedPopup(msg); //in cocos thread
					}

				});
			}
			else // this case for free user || user have >= 2 options => we will redirect to 
			{
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
				{
					auto popup = APPasswordRecoveryMethods::createView(responseData, (APPasswordRecoveryMethods::RECOVERY_FROM)m_loginType);

					this->getParent()->addChild(popup, POPUP_ZORDER);

					//this->removeFromParentWithAnmation();
				});
			}
		}
		else
		{
			MJDEFAULT->setStringForKey("CL_FORGOT_PASSWORD", "fail");

			this->showFailedPopup(i_msg);
		}
	
	});
}

cocos2d::ui::Widget::ccWidgetClickCallback APLoginPopup::onLocateClickCallback(const std::string & callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(APLoginPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APLoginPopup, onLogin),
		CLICK_MAP(APLoginPopup, onForgetPasswordClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) {
        return function_map.at(callBackName);
    }
	return nullptr;
}


