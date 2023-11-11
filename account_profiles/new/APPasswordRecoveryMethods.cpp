#include "APPasswordRecoveryMethods.h"
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
#include "APOTPPopup.h"
#include "APSecurityQuestionPopup.h"
#include "APSendOTPAPI.h"
#include "APEmailOTP.h"

USING_NS_CC;
INITIALIZE_READER(APPasswordRecoveryMethods);

std::once_flag ap_password_recovery_methods_popup_flag;

APPasswordRecoveryMethods * APPasswordRecoveryMethods::createView(ReponseData data, RECOVERY_FROM i_from)
{
	std::call_once(ap_password_recovery_methods_popup_flag, [] {
		REGISTER_CSB_READER(APPasswordRecoveryMethods);
		});

	auto p = static_cast<APPasswordRecoveryMethods*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APPasswordRecoveryMethods.csb"));

	if (p && p->didLoadFromCSB(data, i_from))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APPasswordRecoveryMethods::didLoadFromCSB(ReponseData data, RECOVERY_FROM i_from)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_from = i_from;

	m_responseData = data;

	

	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	
	});

	m_uiCheckboxs =
	{
		utils::findChild<ui::CheckBox*>(this, "mail_cb"),
		utils::findChild<ui::CheckBox*>(this, "sms_cb"),
		utils::findChild<ui::CheckBox*>(this, "question_cb"),
	};

	//add events listener for checkboxs
	for (size_t i = 0; i < m_uiCheckboxs.size(); i++)
	{
		m_uiCheckboxs[i]->setTag(i);
		m_uiCheckboxs[i]->setSelected(false);
		m_uiCheckboxs[i]->loadTextures("cb_normal.png", "cb_normal.png", "cb_checked.png", "cb_normal.png", "cb_normal.png");
		m_uiCheckboxs[i]->addEventListener(CC_CALLBACK_2(APPasswordRecoveryMethods::onCheckboxClicked, this));
	}

	//init default value
	utils::findChild<ui::CheckBox*>(this, "mail_cb")->setSelected(i_from == RECOVERY_FROM::EMAIL);
	utils::findChild<ui::CheckBox*>(this, "sms_cb")->setSelected(i_from == RECOVERY_FROM::PHONE);

	//showing options
	if (!data.m_email.empty() && !data.m_phone.empty()) // 3 option for free user && 2 option for pay user and 
	{
		utils::findChild<ui::Text*>(this, "email_text")->setString(data.m_email);
		utils::findChild<ui::Text*>(this, "phone_number_text")->setString(data.m_phone);

		utils::findChild<ui::CheckBox*>(this, "question_cb")->setVisible(data.m_isFree);

		if (data.m_isFree)
		{
			utils::findChild<ui::CheckBox*>(this, "question_cb")->setVisible(MJDEFAULT->getBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true));
		}
	}
	else if (!data.m_email.empty() && data.m_phone.empty()) // 2 option for free user
	{
		utils::findChild<ui::Text*>(this, "email_text")->setString(data.m_email);

		utils::findChild<ui::CheckBox*>(this, "sms_cb")->setVisible(false);
		
		utils::findChild<ui::CheckBox*>(this, "question_cb")->setPosition(utils::findChild<ui::CheckBox*>(this, "sms_cb")->getPosition());

		utils::findChild<ui::CheckBox*>(this, "question_cb")->setVisible(MJDEFAULT->getBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true));
	}
	else if (data.m_email.empty() && !data.m_phone.empty()) // 2 option for free user
	{
		utils::findChild<ui::Text*>(this, "phone_number_text")->setString(data.m_phone);

		utils::findChild<ui::CheckBox*>(this, "mail_cb")->setVisible(false);

		utils::findChild<ui::CheckBox*>(this, "question_cb")->setPosition(utils::findChild<ui::CheckBox*>(this, "sms_cb")->getPosition());

		utils::findChild<ui::CheckBox*>(this, "sms_cb")->setPosition(utils::findChild<ui::CheckBox*>(this, "mail_cb")->getPosition());

		utils::findChild<ui::CheckBox*>(this, "question_cb")->setVisible(MJDEFAULT->getBoolForKey(ENABLE_SECURITY_QUESTION_KEY, true));
	}
	else
	{
		CCLOG("something wrong !");
	}

	Director::getInstance()->getEventDispatcher()->addCustomEventListener(DISALE_SECURITY_QUESTION_EVENT_KEY, [=](cocos2d::EventCustom *i_event)
	{
		//select other option
		for (auto cb : m_uiCheckboxs)
		{
			if (cb->getName() != "question_cb" && cb->isVisible())
			{
				cb->setSelected(true);
				break;
			}
		}

		if (auto questionCheckbox = utils::findChild<ui::CheckBox*>(this, "question_cb"))
		{
			utils::findChild<ui::CheckBox*>(this, "question_cb")->setSelected(false);
			utils::findChild<ui::CheckBox*>(this, "question_cb")->setVisible(false);
		}

	});

	auto  texts =
	{
		cocos2d::utils::findChild<ui::Text*>(this, "mail_text"),
		cocos2d::utils::findChild<ui::Text*>(this, "sms_text"),
		cocos2d::utils::findChild<ui::Text*>(this, "question_text"),
	};

	for (auto text : texts)
	{
		auto button = Button::create();
		button->ignoreAnchorPointForPosition(false);
		button->setAnchorPoint(Point::ANCHOR_MIDDLE);
		button->setScale9Enabled(true);
		button->setContentSize(text->getContentSize());
		button->setPosition(text->getContentSize() / 2);
		button->addClickEventListener([=](Ref* sender)
		{
			if (auto s = static_cast<ui::Button*>(sender))
			{
				if (auto cb = static_cast<ui::CheckBox*>(s->getParent()->getParent()))
				{
					cb->setSelected(true);
					onCheckboxClicked(cb, cocos2d::ui::CheckBox::EventType::SELECTED);
				}
			}

		});
		text->addChild(button);
	}

	return true;
}

void APPasswordRecoveryMethods::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_password_recovery_method_title"));
	}

	if (auto continueButtonTitle = cocos2d::utils::findChild<ui::Text*>(this, "continue_text_title"))
	{
		continueButtonTitle->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_continue_button_title"));
	}

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "mail_text"))
	{
		text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_code_to_email_question"));
	}

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "sms_text"))
	{
		text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_send_code_to_sms_question"));
	}

	if (auto text = cocos2d::utils::findChild<ui::Text*>(this, "question_text"))
	{
		text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_open_security_question"));
	}
}

void APPasswordRecoveryMethods::onCheckboxClicked(Ref * sender, cocos2d::ui::CheckBox::EventType type)
{
	auto checkbox = dynamic_cast<ui::CheckBox*>(sender);

	const int k_methodID = checkbox->getTag();

	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
	{
		for (size_t j = 0; j < m_uiCheckboxs.size(); j++)
		{
			if (j == k_methodID)
			{
				continue;
			}
			m_uiCheckboxs[j]->setSelected(false);
		}
		break;
	}
	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
	{
		m_uiCheckboxs[k_methodID]->setSelected(true);
		break;
	}
	default:
		break;
	}
}

void APPasswordRecoveryMethods::onExit()
{
	APPopupBase::onExit();

	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(DISALE_SECURITY_QUESTION_EVENT_KEY);
}

void APPasswordRecoveryMethods::onContinute(Ref * sender)
{
	showWaittingView();

	if (utils::findChild<ui::CheckBox*>(this, "mail_cb")->isSelected())
	{
		//1=phone
		//2=email
		ap::api::APSendOTPAPI::call(AP_API_SEND_OTP_EMAIL_TYPE,"", m_responseData.m_email,[=](bool success, std::string msg, const rapidjson::Value &sendAPIOTPData)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				removeWaittingView();

				if (success)
				{
						auto popup = APOTPPopup::createView(APOTPPopup::OTP_TYPE::EMAIL, m_responseData.m_email,"");
						this->getParent()->addChild(popup, POPUP_ZORDER);
				}
				else
				{
					this->showFailedPopup(msg); //in cocos thread
				}
			});

		});
		
		return;
	}

	if (utils::findChild<ui::CheckBox*>(this, "sms_cb")->isSelected())
	{
		//1=phone
		//2=email
		ap::api::APSendOTPAPI::call(AP_API_SEND_OTP_PHONE_TYPE, m_responseData.m_phone,"",[=](bool success, std::string msg, const rapidjson::Value &sendAPIOTPData)
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				removeWaittingView();

				if (success)
				{
					auto popup = APOTPPopup::createView(APOTPPopup::OTP_TYPE::PHONE, m_responseData.m_phone,"+84");
					this->getParent()->addChild(popup, POPUP_ZORDER);
				}
				else
				{
					this->showFailedPopup(msg); //in cocos thread
				}
			});

		});

		return;
	}

	if (utils::findChild<ui::CheckBox*>(this, "question_cb")->isSelected())
	{
		removeWaittingView();

		auto popup = APSecurityQuestionPopup::createView((APSecurityQuestionPopup::RECOVERY_TYPE)m_from,m_responseData);

		this->getParent()->addChild(popup, POPUP_ZORDER);

		//this->removeFromParentWithAnmation();

		return;
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback APPasswordRecoveryMethods::onLocateClickCallback(const std::string & callBackName) {
    
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(APPasswordRecoveryMethods, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APPasswordRecoveryMethods, onContinute),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}


