#include "APOTPPopup.h"
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
#include "APWaitView.h"
#include "StoryAlert.h"
#include "APVerifyOptForEmailAPI.h"
#include "APChangePasswordPopup.h"
#include "APSendOTPAPI.h"
#include "APChangePasswordScreen.h"
#include "APAccountUpdatePassword.h"

#define TIME_COUNTDOWN 60

USING_NS_CC;
INITIALIZE_READER(APOTPPopup);

std::once_flag ap_otp_popup_reader_flag;

APOTPPopup * APOTPPopup::createView(OTP_TYPE i_type, std::string i_mailOrPhoneText,std::string i_countryCode, APPopupBase::Flow i_flow)
{
	std::call_once(ap_otp_popup_reader_flag, []
	{
		REGISTER_CSB_READER(APOTPPopup);
	});

	auto p = static_cast<APOTPPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APOTPPopup.csb"));

    if (p && p->didLoadFromCSB(i_type, i_mailOrPhoneText, i_countryCode, i_flow))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APOTPPopup::didLoadFromCSB(OTP_TYPE i_type, std::string i_mailOrPhoneText,std::string i_countryCode, APPopupBase::Flow i_flow)
{
	m_OTPType = i_type;

	m_mailOrPhoneText = i_mailOrPhoneText;

    m_countryCode = i_countryCode;
    
    m_flow = i_flow;
    
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	
	});

	if (auto inputText = cocos2d::utils::findChild<ui::Text*>(this, "input_text"))
	{
        if (m_flow == APPopupBase::Flow::ENTER_INFO || m_flow == APPopupBase::Flow::UPDATE_EMAIL || m_flow == APPopupBase::Flow::UPDATE_PHONE)
        {
            inputText->setString(i_mailOrPhoneText);
        }
        else
        {
            inputText->setString((m_OTPType == OTP_TYPE::PHONE ? m_countryCode: "") +  i_mailOrPhoneText);
        }
	}
	
	m_textNumbers.push_back(cocos2d::utils::findChild<ui::Text*>(this, "Text1"));
	m_textNumbers.push_back(cocos2d::utils::findChild<ui::Text*>(this, "Text2"));
	m_textNumbers.push_back(cocos2d::utils::findChild<ui::Text*>(this, "Text3"));
	m_textNumbers.push_back(cocos2d::utils::findChild<ui::Text*>(this, "Text4"));

	auto inputTextfield = utils::findChild<ui::TextField *>(this, "textfield");
	inputTextfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
	inputTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	inputTextfield->setMaxLengthEnabled(true);
	inputTextfield->setMaxLength(4);
	inputTextfield->addEventListener(CC_CALLBACK_2(APOTPPopup::onTextfieldChanged, this));
	inputTextfield->setOpacity(0);

	enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), OTP_MIN_LENGTH, cocos2d::utils::findChild<ui::Button*>(this, "btn_continue"));

	updateCusor();
	
	resendOTP();

	return true;
}

void APOTPPopup::changeDisplayLanguage()
{
	std::string type = (m_OTPType == OTP_TYPE::EMAIL ? "email" : "phone");

	//ap_email_otp_title
	//ap_phone_otp_title
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(StringUtils::format("ap_%s_otp_title", type.c_str())));
	}

	if (auto continueButtonTitle = cocos2d::utils::findChild<ui::Text*>(this, "continue_text_title"))
	{
        continueButtonTitle->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");

		continueButtonTitle->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_continue_button_title"));
	}
    
}

void APOTPPopup::onContinute(Ref * sender)
{
    this->hideWarning();
    
	auto inputTextfield = utils::findChild<ui::TextField *>(this, "textfield");

	hideVirtualKeyboard(inputTextfield);

	auto continueButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_continue");

	continueButton->setTouchEnabled(false);

    showWaittingView();
    
	ap::api::CommomVerifyOTP::call(inputTextfield->getString(), (ap::api::CommomVerifyOTP::VERIFY_TYPE) m_OTPType, m_mailOrPhoneText,m_countryCode, [=](bool is_success, std::string msg, int code)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, is_success, msg, continueButton]
		{
            removeWaittingView();
            
			continueButton->setTouchEnabled(true);

			if (is_success)
			{
                ms::EventLogger::getInstance().logEvent("success_verify_otp_3nd",
                {
                    {"verify_otp", cocos2d::Value(m_OTPType == OTP_TYPE::EMAIL? "email":"phone")},
                });

                cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this,msg]
                {
                    //tokenToChangePassword = msg
                    
                    APChangePasswordScreen::START_FROM start_from = APChangePasswordScreen::START_FROM::RESET_PASS_SCREEN;
                    
                    if (m_flow == APPopupBase::Flow::ENTER_INFO)
                    {
                        start_from = APChangePasswordScreen::START_FROM::ENTER_INFO;
                    }
                    else if (m_flow == APPopupBase::Flow::UPDATE_PHONE)
                    {
                        start_from = APChangePasswordScreen::START_FROM::UPDATE_PHONE;
                    }
                    else if (m_flow == APPopupBase::Flow::UPDATE_EMAIL)
                    {
                        start_from = APChangePasswordScreen::START_FROM::UPDATE_EMAIL;
                    }

                    auto changePasswordScreen =  APChangePasswordScreen::createView(start_from, (APChangePasswordScreen::TYPE) m_OTPType,m_mailOrPhoneText,msg,m_countryCode);
                                        
                    animationMoveToLeft(changePasswordScreen);
                    
                    Director::getInstance()->getRunningScene()->addChild(changePasswordScreen,INT_MAX-1);
                    
//                    auto popup = APChangePasswordPopup::createView((APChangePasswordPopup::CHANGE_TYPE) m_OTPType, m_mailOrPhoneText);
//
                    this->getParent()->removeFromParent();//remove resetpasswordscreen.

                    //this->removeFromParentWithAnmation();
                });
			}
			else
			{
				ms::EventLogger::getInstance().logEvent("fail_verify_otp_3nd", 
				{
							{"verify_otp",  cocos2d::Value(m_OTPType == OTP_TYPE::EMAIL ? "email" : "phone")},
				});

//				showFailedPopup(msg);
                this->showWarning(msg);
			}

		});
	});
}

void APOTPPopup::onResend(Ref * sender)
{
	UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(this, "textfield")->getVirtualRenderer());
	ime->closeIME();
	Button* btn = (Button*)sender;
	btn->setEnabled(false);
	btn->setTouchEnabled(false);
	
	resendOTP();

	const std::string k_type		= (m_OTPType == OTP_TYPE::EMAIL ? AP_API_SEND_OTP_EMAIL_TYPE : AP_API_SEND_OTP_PHONE_TYPE);
	const std::string k_email		= (m_OTPType == OTP_TYPE::EMAIL ? m_mailOrPhoneText : "");
	const std::string k_phoneNumber	= (m_OTPType == OTP_TYPE::EMAIL ? "" : m_mailOrPhoneText);

	//1=phone
	//2=email
	ap::api::APSendOTPAPI::call(k_type, k_phoneNumber, k_email, m_countryCode,[=](bool success, std::string msg, const rapidjson::Value &sendAPIOTPData)
	{
		if (success)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				auto popup = APOTPPopup::createView((APOTPPopup::OTP_TYPE)m_OTPType, m_mailOrPhoneText,m_countryCode);
				this->getParent()->addChild(popup, POPUP_ZORDER);

				this->removeFromParent();

				//this->removeFromParentWithAnmation();
			});
		}
		else
		{
			this->showErrorAlert(msg); //in cocos thread
		}
	});

}

void APOTPPopup::onTextfieldChanged(cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
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
		auto otpTextfield = cocos2d::utils::findChild<ui::TextField*>(this, "textfield");

		validNumberInput(otpTextfield, [this]() {
		
			this->insertOTP();

		});

		enableWidgetNodeBasedOnTextLength(otpTextfield->getString(), OTP_MIN_LENGTH , cocos2d::utils::findChild<ui::Button*>(this, "btn_continue"));
	}
	case TextField::EventType::DELETE_BACKWARD:
	{
		this->insertOTP();
	}
	break;
	default:
		break;
	}

}

void APOTPPopup::insertOTP() 
{
	auto inputTextfield = utils::findChild<ui::TextField*>(this, "textfield");

	auto text = inputTextfield->getString();
	
	for (int i = 0; i < text.size(); i++) 
	{
		std::string s(1, text[i]);
		m_textNumbers[i]->setString(s);
	}

	for (int i = text.size(); i < m_textNumbers.size(); i++)
	{
		m_textNumbers[i]->setString("");
	}

	updateCusor();
}

void APOTPPopup::updateCusor() 
{
	auto inputTextfield = utils::findChild<ui::TextField*>(this, "textfield");

	for (size_t i = 0; i < m_textNumbers.size(); i++)
	{
		m_textNumbers[i]->stopAllActions();
		m_textNumbers[i]->setOpacity(255);
	}

	int cusorID = inputTextfield->getString().size();

	if (cusorID >= 0 && cusorID < m_textNumbers.size())
	{
		m_textNumbers[cusorID]->setString("|");
		m_textNumbers[cusorID]->runAction(RepeatForever::create(Sequence::create(FadeIn::create(0), DelayTime::create(0.5f), FadeOut::create(0), DelayTime::create(0.5f), nullptr)));
	}
}

void APOTPPopup::resendOTP()
{
	this->unschedule("resendCodeButtonUpdate");

	m_resendingCodeTimer = 0.0f;

	auto resendCodeButton = cocos2d::utils::findChild<ui::Button*>(this, "btn_resend");
	resendCodeButton->setTouchEnabled(false);
	resendCodeButton->setEnabled(false);

    auto countDownFunction = [=](){
        
        if (m_resendingCodeTimer >= TIME_COUNTDOWN)
            {
                resendCodeButton->setEnabled(true);
                resendCodeButton->setTouchEnabled(true);
                resendCodeButton->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey("ap_resend_code_button_title2"));
                resendCodeButton->setTitleColor(Color3B(36, 172, 222));
                resendCodeButton->getTitleLabel()->enableBold();
                resendCodeButton->getTitleLabel()->enableUnderline();

                cocos2d::utils::findChild<ui::Button*>(resendCodeButton, "line")->setVisible(true);

                this->unschedule("resendCodeButtonUpdate");
            }
            else
            {
                std::string text = StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("ap_resend_code_button_title").c_str(), TIME_COUNTDOWN - (int)m_resendingCodeTimer);
                
                resendCodeButton->setTitleFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

                resendCodeButton->setTitleText(text);
            }
        
    };
    
    countDownFunction();
    
	this->schedule([=](float dt)
	{
		m_resendingCodeTimer += dt;

        countDownFunction();

	}, 1.0f, "resendCodeButtonUpdate");
}

cocos2d::ui::Widget::ccWidgetClickCallback APOTPPopup::onLocateClickCallback(const std::string & callBackName) 
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APOTPPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APOTPPopup, onContinute),
		CLICK_MAP(APOTPPopup, onResend),
	};

	if (function_map.find(callBackName) != function_map.end()) {
		return function_map.at(callBackName);
	}
	return nullptr;
}

void APOTPPopup::showWarning(const std::string& warning)
{
    if (auto warningIcon = cocos2d::utils::findChild<ui::ImageView *>(this, "icon_warning"))
    {
        warningIcon->setVisible(true);
        
        if(auto warningLbl = (ui::Text *)warningIcon->getChildByName("warning_label"))
        {
            setFontByLang(warningLbl);
            warningLbl->setString(warning);
            
            const float x = warningIcon->getContentSize().width*0.5f +  warningLbl->getContentSize().width*0.5f;
            
            warningIcon->setPositionX(warningIcon->getParent()->getContentSize().width*0.5f-x );
        }
    }
}

void APOTPPopup::hideWarning()
{
    if (auto warningIcon = cocos2d::utils::findChild<ui::ImageView *>(this, "icon_warning"))
    {
        warningIcon->setVisible(false);
    }
}
