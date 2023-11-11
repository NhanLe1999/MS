#include "APSecurityQuestionPopup.h"
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
#include "APMessagePopup.h"
#include "APEnterNewPasswordPopup.h"
USING_NS_CC;
INITIALIZE_READER(APSecurityQuestionPopup);

#define QUESION_ID_POS 4
#define FAILED_VERIFICATION_NUM 3

std::once_flag ap_security_question_popup_flag;

APSecurityQuestionPopup * APSecurityQuestionPopup::createView(RECOVERY_TYPE i_type, ReponseData data)
{
	std::call_once(ap_security_question_popup_flag, [] 
	{
		REGISTER_CSB_READER(APSecurityQuestionPopup);
	});

	auto p = static_cast<APSecurityQuestionPopup*>(CSLoader::createNodeWithVisibleSize("csb/account/new/APSecurityQuestionPopup.csb"));

	if (p && p->didLoadFromCSB(i_type,data))
	{
		return p;
	}

	CC_SAFE_DELETE(p);

	return nullptr;
}

bool APSecurityQuestionPopup::didLoadFromCSB(RECOVERY_TYPE i_type, ReponseData data)
{
	if (!APPopupBase::didLoadFromCSB())
	{
		return false;
	}

	m_type = i_type;

	m_responseData = data;

	m_failedVerificationCount = MJDEFAULT->getIntegerForKey(FAILED_VERIFICATION_COUNT_KEY, 0);

	showingAnimation(utils::findChild(this, "popup_bg"), [=]() {
	
	
	});

	m_uiQuestions =
	{
		cocos2d::utils::findChild<ui::Text*>(this, "quest_01"),
		cocos2d::utils::findChild<ui::Text*>(this, "quest_02")
	};

	m_uiAnswers =
	{
		{
			cocos2d::utils::findChild<ui::Text*>(this, "as1_1"),
			cocos2d::utils::findChild<ui::Text*>(this, "as1_2"),
			cocos2d::utils::findChild<ui::Text*>(this, "as1_3"),
			cocos2d::utils::findChild<ui::Text*>(this, "as1_4"),
		},
		{
			cocos2d::utils::findChild<ui::Text*>(this, "as2_1"),
			cocos2d::utils::findChild<ui::Text*>(this, "as2_2"),
			cocos2d::utils::findChild<ui::Text*>(this, "as2_3"),
			cocos2d::utils::findChild<ui::Text*>(this, "as2_4"),
		},
	};

	for (size_t i = 0; i < m_responseData.m_questions.size(); i++)
	{
		m_uiQuestions.at(i)->setString(m_responseData.m_questions.at(i).m_question);

		for (size_t j = 0; j < m_uiAnswers.at(i).size(); j++)
		{
			m_uiAnswers.at(i).at(j)->setString(m_responseData.m_questions.at(i).m_answers.at(j));
		
			auto button = Button::create();
			button->ignoreAnchorPointForPosition(false);
			button->setAnchorPoint(Point::ANCHOR_MIDDLE);
			button->setScale9Enabled(true);
			button->setContentSize(m_uiAnswers.at(i).at(j)->getContentSize());
			button->setPosition(m_uiAnswers.at(i).at(j)->getContentSize()/2);
			button->addClickEventListener(CC_CALLBACK_1(APSecurityQuestionPopup::onAnsClicked, this));
			m_uiAnswers.at(i).at(j)->addChild(button);

			auto checkbox = static_cast<ui::CheckBox*>(m_uiAnswers.at(i).at(j)->getParent());
			checkbox->loadTextures("cb_normal.png", "cb_normal.png", "cb_checked.png", "cb_normal.png", "cb_normal.png");
			checkbox->setTag(j);
			checkbox->addEventListener(CC_CALLBACK_2(APSecurityQuestionPopup::onAnsCheckboxClicked,this));

			if (j == 0)
			{
				checkbox->setSelected(true);
			}
		}
	}

	return true;
}

void APSecurityQuestionPopup::changeDisplayLanguage()
{
	if (auto title = cocos2d::utils::findChild<ui::Text*>(this, "title"))
	{
		title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_security_question_title"));
	}
}

void APSecurityQuestionPopup::onContinute(Ref * sender)
{
	showWaittingView();

	std::vector<bool> correctAns;

	for (size_t i = 0; i < m_responseData.m_questions.size(); i++)
	{
		correctAns.push_back(false);
	}

	for (size_t i = 0; i < m_uiAnswers.size(); i++)
	{
		for (size_t j = 0; j < m_uiAnswers.at(i).size(); j++)
		{
			if (auto checkbox = dynamic_cast<ui::CheckBox*>(m_uiAnswers.at(i).at(j)->getParent()))
			{
				if (checkbox->isSelected() &&
					m_uiAnswers.at(i).at(j)->getString() == m_responseData.m_questions.at(i).m_rightAnswer
					)
				{
					correctAns.at(i) = true;
				}
			}
		}
	}

	bool result = true;

	for (size_t i = 0; i < correctAns.size(); i++)
	{
		if (!correctAns.at(i))
		{
			result = false;
		}
	}

	if (result) // verify success
	{

		this->removeFromParentWithAnmation([=]() {

			auto popup = APEnterNewPasswordPopup::createView((APEnterNewPasswordPopup::RECOVERY_TYPE)m_type, m_type == RECOVERY_TYPE::PHONE ? m_responseData.m_phone : m_responseData.m_email);

			this->getParent()->addChild(popup, POPUP_ZORDER);

			auto msgPopup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::SUCCESS, LANGUAGE_MANAGER->getDisplayTextByKey("ap_successful_verification"));

			this->getParent()->addChild(msgPopup, POPUP_ZORDER);
		
		});
	}
	else //verify failed
	{
		m_failedVerificationCount++;

		if (m_failedVerificationCount > FAILED_VERIFICATION_NUM)
		{
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DISALE_SECURITY_QUESTION_EVENT_KEY);

			this->removeFromParentWithAnmation([=]() 
			{
				auto msgPopup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::UNSUCCESS,LANGUAGE_MANAGER->getDisplayTextByKey("ap_disable_security_question_verification"));

				this->getParent()->addChild(msgPopup, POPUP_ZORDER);
			});

			return;
		}

		MJDEFAULT->setIntegerForKey(FAILED_VERIFICATION_COUNT_KEY, m_failedVerificationCount);

		removeWaittingView();

		auto popup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::UNSUCCESS, LANGUAGE_MANAGER->getDisplayTextByKey("ap_failed_verification"));

		this->getParent()->addChild(popup, POPUP_ZORDER);

		if (m_failedVerificationCount >= FAILED_VERIFICATION_NUM)
		{
			MJDEFAULT->setBoolForKey(ENABLE_SECURITY_QUESTION_KEY, false);

			UserDefault::getInstance()->setIntegerForKey(HOURS_FOR_SECURITY_QUESTION, converMillisecondsToHours(cocos2d::utils::getTimeInMilliseconds()));
		}

	}

}

void APSecurityQuestionPopup::onAnsClicked(cocos2d::Ref * sender)
{
	if (auto s = static_cast<ui::Button*>(sender))
	{
		if (auto cb = static_cast<ui::CheckBox*>(s->getParent()->getParent()))
		{
			cb->setSelected(true);
			onAnsCheckboxClicked(cb, cocos2d::ui::CheckBox::EventType::SELECTED);
		}
	}
}

cocos2d::ui::Widget::ccWidgetClickCallback APSecurityQuestionPopup::onLocateClickCallback(const std::string & callBackName) 
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = 
	{
        CLICK_MAP(APSecurityQuestionPopup, onCancel),//locate at parent (APPopupBase)
		CLICK_MAP(APSecurityQuestionPopup, onContinute),
		CLICK_MAP(APSecurityQuestionPopup, onAnsClicked),
    };
    
    if (function_map.find(callBackName) != function_map.end()) 
	{
        return function_map.at(callBackName);
    }

	return nullptr;
}

void APSecurityQuestionPopup::onAnsCheckboxClicked(Ref * sender, cocos2d::ui::CheckBox::EventType type)
{
	auto checkbox = dynamic_cast<ui::CheckBox*>(sender);

	auto name = checkbox->getName();
	
	const int k_questionID	= (name[QUESION_ID_POS] - '0')-1;

	const int k_ansID		= checkbox->getTag();

	CCLOG("XXX|questionID %d", k_questionID);
	
	switch (type)
	{
		case cocos2d::ui::CheckBox::EventType::SELECTED:
		{
			for (size_t j = 0; j < m_uiAnswers.at(k_questionID).size(); j++)
			{
				if (j == k_ansID)
				{
					continue;
				}
				auto checkbox = static_cast<ui::CheckBox*>(m_uiAnswers.at(k_questionID).at(j)->getParent());
				checkbox->setSelected(false);
			}
			cocos2d::log("SELECTED");
			break;
		}
		case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		{
			auto checkbox = static_cast<ui::CheckBox*>(m_uiAnswers.at(k_questionID).at(k_ansID)->getParent());
			checkbox->setSelected(true);
			cocos2d::log("UNSELECTED");
			break;
		}
		default:
			break;
	}
}


