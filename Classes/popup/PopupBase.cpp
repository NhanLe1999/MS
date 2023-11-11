#include "PopupBase.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "StoryLanguageManager.h"
#include "LicenseInfoModel.h"
#include "MSIntro.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "MJPlatformConfig.h"
#include "StoryAlert.h"
#include <regex>

USING_NS_CC;

#define SAFE_CHECK_OBJET(x, ...) if(!x){ return __VA_ARGS__; } 

bool PopupBase::didLoadFromCSB()
{
	m_visibleSize	= Director::getInstance()->getVisibleSize();
	m_origin		= Director::getInstance()->getVisibleOrigin();
	
	//add black layer
	auto blackLayer = LayerColor::create(cocos2d::Color4B::BLACK);

	blackLayer->setIgnoreAnchorPointForPosition(false);

	blackLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
	
	blackLayer->setOpacity(0.5f*255.0f);

	blackLayer->setPosition(m_origin+m_visibleSize/2);
	
	this->addChild(blackLayer, -1);

	//lock
	auto lockButton = Button::create();

	lockButton->setAnchorPoint(Point::ZERO);

	lockButton->setScale9Enabled(true);

	lockButton->setContentSize(m_visibleSize);

	lockButton->setPosition(m_origin);

	this->addChild(lockButton, -1);

	//language update.
	//MJDEFAULT->setIntegerForKey(key_lang_display, LICENSE_INFO_MODEL->isDisplayLanguageVN()? LANGUAGE_VN_CODE : LANGUAGE_EN_CODE);
	//LANGUAGE_MANAGER->updateLanguageWithID(LICENSE_INFO_MODEL->isDisplayLanguageVN() ? LANGUAGE_VN_CODE : LANGUAGE_EN_CODE);

	changeDisplayLanguage();


	return true;
}

void PopupBase::onCancel(cocos2d::Ref * sender)
{
	if (m_onCancelCallBack)
	{
		m_onCancelCallBack();
	}

	this->removeFromParentAndCleanup(true);
	//removeFromParentWithAnmation();
}

void PopupBase::showingAnimation(cocos2d::Node *i_node, std::function<void()> i_callback)
{
	const float k_currentScaleFactor = i_node->getScale();

	i_node->setVisible(false);
	
	i_node->setScale(0.0f);

	i_node->runAction(Sequence::create(DelayTime::create(0.125f), Show::create(), EaseBackOut::create(ScaleTo::create(0.3f, 1.0f*k_currentScaleFactor)), nullptr));
}

void PopupBase::hidingAnimation(cocos2d::Node * i_node,std::function<void()> i_callback)
{
	i_node->setVisible(false);

	i_node->runAction(Sequence::create(DelayTime::create(0.1f), Show::create(), EaseBackIn::create(ScaleTo::create(0.1f, 0.0f)), DelayTime::create(0.1f), CallFunc::create([=]() {
	
		i_callback();

	}), nullptr));
}

void PopupBase::validPhoneInput(ui::TextField * i_textField)
{
	if (!i_textField)
	{
		return;
	}

	cocos2d::StringUtils::StringUTF8 phoneText = cocos2d::StringUtils::StringUTF8(i_textField->getString());

	if (phoneText.getString().empty())
	{
		return;
	}

	auto new_char = phoneText.getString().at(phoneText.length() - 1)._char;

	while ((new_char == "+" && phoneText.getString().size() > 1 ) || (!phoneText.getString().empty() &&  !isUTF8StringNumberValid(new_char)))
	{
		phoneText.deleteChar(phoneText.length() - 1);

		if (phoneText.getString().empty())
		{
			break;
		}

		new_char = phoneText.getString().at(phoneText.length() - 1)._char;
	}
 

    const int k_phoneCodeLength = (!phoneText.getString().empty() && phoneText.getAsCharSequence().front()=='+') ? 6 : 0;
    
	while (phoneText.getString().size() > PHONE_MAX_LENGTH + k_phoneCodeLength)
	{
		phoneText.deleteChar(phoneText.length() - 1);
	}

	i_textField->setString(phoneText.getAsCharSequence());
}

void PopupBase::validNumberInput(ui::TextField * i_textField, std::function<void()> i_callback)
{
	if (!i_textField)
	{
		return;
	}

	cocos2d::StringUtils::StringUTF8 numberText = cocos2d::StringUtils::StringUTF8(i_textField->getString());

	if (numberText.getString().empty())
	{
		return;
	}

	auto new_char = numberText.getString().at(numberText.length() - 1)._char;

	while (!numberText.getString().empty() && !mj::helper::isUTF8StringValidNumber(new_char))
	{
		numberText.deleteChar(numberText.length() - 1);

		if (numberText.getString().empty())
		{
			break;
		}
		new_char = numberText.getString().at(numberText.length() - 1)._char;
	}

	i_textField->setString(numberText.getAsCharSequence());

	i_callback();
}

void PopupBase::enableButtonBasedOnValidPhoneNumber(std::string i_phoneNumber,ui::Button * i_button)
{
	if (!i_button || i_phoneNumber.empty())
	{
		return;
	}

	i_button->setEnabled(isUTF8StringPhoneNumberValid(i_phoneNumber));
}

void PopupBase::enableButtonBasedOnValidNumber(std::string i_Number, ui::Button * i_button)
{
	if (!i_button || i_Number.empty())
	{
		return;
	}

	i_button->setEnabled(mj::helper::isUTF8StringValidNumber(i_Number));
}

void PopupBase::enableWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget* i_node)
{
	if (!i_node || i_leghth <=0 )
	{
		return;
	}

	i_node->setEnabled(i_text.size() >= i_leghth);
}

void PopupBase::visibleWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget * i_node)
{
	if (!i_node || i_leghth <= 0)
	{
		return;
	}

	i_node->setVisible(i_text.size() < i_leghth);
}

void PopupBase::showErrorAlert(std::string msg)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg] {
		
		if (msg == "") 
		{
			StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
		}
		else 
		{
			StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4) ? "\xc4\x90\xc3\xb3\x6e\x67" : "Close");
		}
	});
}

void PopupBase::showSuccessPopup(std::string msg)
{
	//Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg] 
	//{
	// 	auto msgPopup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::SUCCESS, msg);

	//	Director::getInstance()->getRunningScene()->addChild(msgPopup, INT_MAX - 1);
	//	
	//});
}

void PopupBase::showFailedPopup(std::string msg)
{
	/*Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg]
	{
		auto msgPopup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::UNSUCCESS, msg);

		Director::getInstance()->getRunningScene()->addChild(msgPopup, INT_MAX - 1);

	});*/
}


void PopupBase::removeFromParentWithAnmation(std::function<void()> i_callback)
{
	if (auto popupBg = utils::findChild(this, "popup_bg"))
	{
		hidingAnimation(popupBg, [=]() 
		{
			if (i_callback)
			{
				i_callback();
			}

			this->removeFromParentAndCleanup(true);
		});
	}
	else
	{
		if (i_callback)
		{
			i_callback();
		}

		this->removeFromParentAndCleanup(true);
	}
}

void PopupBase::runWarningActionOnBackgroundNode(std::string i_bgName)
{
	if (auto bg = utils::findChild(this, i_bgName))
	{
		cocos2d::Device::vibrate(1.0f);

		bg->runAction(Repeat::create(Sequence::createWithTwoActions(MoveBy::create(0.1f, Vec2(+25, 0)), MoveBy::create(0.1f, Vec2(-25, 0))), 2));
	}
}

void PopupBase::enableAButtonBasedOnTextfieldLength(cocos2d::ui::Button * i_button, cocos2d::ui::TextField * i_textField, int i_length)
{
	if (!i_button || !i_textField)
	{
		return;
	}

	enableWidgetNodeBasedOnTextLength(i_textField->getString(), i_length, i_button);

	i_textField->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
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
			enableWidgetNodeBasedOnTextLength(i_textField->getString(), i_length, i_button);
		}
		case TextField::EventType::DELETE_BACKWARD:
		{
			enableWidgetNodeBasedOnTextLength(i_textField->getString(), i_length, i_button);
		}
		break;
		default:
			break;
		}
	});
}

void PopupBase::enableAButtonBasedOnTextfieldsLength(cocos2d::ui::Button * i_button, std::vector<cocos2d::ui::TextField*> i_textFields, cocos2d::ui::Widget * i_node, int i_length)
{
	if (!i_button || i_textFields.empty())
	{
		return;
	}

	for (auto inputTextfield : i_textFields)
	{
		if (inputTextfield)
		{
			enableWidgetNodeBasedOnTextLength(inputTextfield->getString(), i_length, i_button);

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
					bool isEnable = true;

					for (auto textfield : i_textFields)
					{
						if (textfield == inputTextfield)
						{
							continue;
						}

						if (textfield->getString().size() < i_length)
						{
							isEnable = false;
							break;
						}
					}

					if (isEnable)
					{
						i_button->setEnabled(inputTextfield->getString().size() >= i_length);
					}
				}
				case TextField::EventType::DELETE_BACKWARD:
				{
					bool isEnable = true;

					for (auto textfield : i_textFields)
					{
						if (textfield == inputTextfield)
						{
							continue;
						}

						if (textfield->getString().size() < i_length)
						{
							isEnable = false;
							break;
						}
					}

					if (isEnable)
					{
						i_button->setEnabled(inputTextfield->getString().size() >= i_length);
					}
				}
				break;
				default:
					break;
				}
			});
		}
	}

	

}

void PopupBase::showWaittingView()
{
	//auto waitView = APWaitView::createView(std::string(""), false, true);
	//waitView->setName("waitView");
	//this->addChild(waitView,POPUP_ZORDER);
}

void PopupBase::removeWaittingView()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
	{
		if (auto waitView = this->getChildByName("waitView"))
		{
			waitView->removeFromParent();
		}
	});
}

int PopupBase::converMillisecondsToHours(long long i_milliseconds)
{
	return (i_milliseconds/ 3600000);
}

bool PopupBase::isUTF8StringPhoneNumberValid(std::string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		auto c = s.at(i);

		if (i == 0 && c == '+')
		{
			continue;
		}

		if (!iswdigit(c))
		{
			return false;
		}
	}

	//vietname 
	//
	if ((CONFIG_MANAGER->isVietnameseStories() && s.length() < 10)|| (!CONFIG_MANAGER->isVietnameseStories() && s.length() < 7))
	{
		return false;
	}

	return true;
}

bool PopupBase::isUTF8StringNumberValid(std::string i_number)
{
	for (int i=0; i<i_number.size();i++) 
	{
		auto c = i_number.at(i);
		
		if (!CONFIG_MANAGER->isVietnameseStories() && i == 0 && c == '+')
		{
			continue;
		}

		if (!iswdigit(c)) 
		{
			return false;
		}
	}

	return true;
}

void PopupBase::reduceFontSizeOfTextfieldBasedOnLength(cocos2d::ui::TextField * i_textField, const int i_defaultLength)
{
	if (!i_textField)
	{
		return;
	}

	const auto k_defautFontSize = i_textField->getFontSize();

	int textFieldLength = StringUtils::StringUTF8(i_textField->getString()).length();

	while ((textFieldLength + 5) > i_defaultLength) //add 5 character -> we will reduce fontsize 2 
	{
		i_textField->setFontSize(k_defautFontSize - 2);
		
		textFieldLength -= 5;
	}

}

void PopupBase::hideVirtualKeyboard(cocos2d::ui::TextField * i_textField)
{
	if (i_textField)
	{
		UICCTextField *ime = dynamic_cast<UICCTextField*>(i_textField->getVirtualRenderer());
		ime->closeIME();
	}
}

void PopupBase::hideVirtualKeyboard(std::vector<cocos2d::ui::TextField*> i_textFields)
{
	for (auto textField : i_textFields)
	{
		hideVirtualKeyboard(textField);
	}
}

void PopupBase::replaceString(std::string & i_source, std::string i_subString, std::string i_newSubString)
{
	if (i_source.empty() || i_subString.empty() || i_newSubString.empty())
	{
		return;
	}

	i_source = std::regex_replace(i_source, std::regex(i_subString), i_newSubString);
}

