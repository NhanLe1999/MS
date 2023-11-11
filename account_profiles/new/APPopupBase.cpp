#include "APPopupBase.h"
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
#include "StoryAlert.h"
#include "APSendOTPAPI.h"
#include "APMessagePopup.h"
#include <regex>
#include <iostream>
#include <locale>         // std::locale, std::isdigit

USING_NS_CC;

#define SAFE_CHECK_OBJET(x, ...) if(!x){ return __VA_ARGS__; } 

//APPopupBaseReader * APPopupBaseReader::__instance_APPopupBaseReader = nullptr

bool APPopupBase::didLoadFromCSB()
{
	m_visibleSize	= Director::getInstance()->getVisibleSize();
	m_origin		= Director::getInstance()->getVisibleOrigin();
	
	//add black layer
	auto blackLayer = LayerColor::create(cocos2d::Color4B::BLACK);

	blackLayer->setIgnoreAnchorPointForPosition(false);

	blackLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
	
	blackLayer->setOpacity(0.5f*255.0f);

	blackLayer->setName("blackLayer");
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
//	MJDEFAULT->setIntegerForKey(key_lang_display, LICENSE_INFO_MODEL->isDisplayLanguageVN()? LANGUAGE_VN_CODE : LANGUAGE_EN_CODE);
//
//	LANGUAGE_MANAGER->updateLanguageWithID(LICENSE_INFO_MODEL->isDisplayLanguageVN() ? LANGUAGE_VN_CODE : LANGUAGE_EN_CODE);

	changeDisplayLanguage();

    //setupBackground();

	return true;
}

void APPopupBase::onCancel(cocos2d::Ref * sender)
{
	if (m_onCancelCallBack)
	{
		m_onCancelCallBack();
	}

	this->removeFromParent();
	//removeFromParentWithAnmation();
}

void APPopupBase::showingAnimation(cocos2d::Node *i_node, std::function<void()> i_callback)
{
	const float k_currentScaleFactor = i_node->getScale();

	i_node->setVisible(false);
	
	i_node->setScale(0.0f);

	i_node->runAction(Sequence::create(DelayTime::create(0.125f), Show::create(), EaseBackOut::create(ScaleTo::create(0.3f, 1.0f*k_currentScaleFactor)), nullptr));
}

void APPopupBase::hidingAnimation(cocos2d::Node * i_node,std::function<void()> i_callback)
{
	i_node->setVisible(false);

	i_node->runAction(Sequence::create(DelayTime::create(0.1f), Show::create(), EaseBackIn::create(ScaleTo::create(0.1f, 0.0f)), DelayTime::create(0.1f), CallFunc::create([=]() {
	
		i_callback();

	}), nullptr));
}

void APPopupBase::trimSpace(cocos2d::Node *i_node)
{
    if (!i_node)
    {
        return;
    }

	std::string input = "";
	
	if (auto textfield = dynamic_cast<ui::TextField*>(i_node))
	{
		input = textfield->getString();
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_node))
	{
		input = editbox->getText();
	}

    cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(input);
    
    if (txt.getString().empty())
    {
        return;
    }
    
    auto new_char = txt.getString().at(txt.length() - 1)._char;
    
    while ((new_char == " " && !txt.getString().empty()))
    {
        txt.deleteChar(txt.length() - 1);
        
        if (txt.getString().empty())
        {
            break;
        }
        
        new_char = txt.getString().at(txt.length() - 1)._char;
    }
    
	if (auto textfield = dynamic_cast<ui::TextField*>(i_node))
	{
		textfield->setString(txt.getAsCharSequence());
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_node))
	{
		editbox->setText(txt.getAsCharSequence().c_str());
	}

}

void APPopupBase::removeSpace(cocos2d::Node* i_textField)
{
	if (!i_textField)
	{
		return;
	}

	std::string txt = "";

	if (auto textfield = dynamic_cast<ui::TextField*>(i_textField))
	{
		txt = textfield->getString();
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_textField))
	{
		txt = editbox->getText();
	}

	if (txt.empty())
	{
		return;
	}

	txt.erase(std::remove_if(txt.begin(), txt.end(), [](const char i_char) {
	
		return i_char == ' ';

	}), txt.end());

	if (auto textfield = dynamic_cast<ui::TextField*>(i_textField))
	{
		textfield->setString(txt);
	}
	else if (auto editbox = dynamic_cast<ui::EditBox*>(i_textField))
	{
		editbox->setText(txt.c_str());
	}
}

void APPopupBase::removeSpace(std::vector<cocos2d::Node*> i_textFields)
{
	for (auto textFiled : i_textFields)
	{
		removeSpace(textFiled);
	}
}

void APPopupBase::removeSpace(std::vector<ui::TextField*> i_textFields)
{
	for (auto textFiled : i_textFields)
	{
		removeSpace(textFiled);
	}
}

void APPopupBase::validEmaiInput(ui::TextField* i_textField)
{
    if (!i_textField)
    {
        return;
    }

    cocos2d::StringUtils::StringUTF8 txt = cocos2d::StringUtils::StringUTF8(i_textField->getString());

    if (txt.getString().empty())
    {
        return;
    }

    auto new_char = txt.getString().at(txt.length() - 1)._char;
    
    while ((new_char == " " && !txt.getString().empty()))
    {
        txt.deleteChar(txt.length() - 1);

        if (txt.getString().empty())
        {
            break;
        }

        new_char = txt.getString().at(txt.length() - 1)._char;
    }

    i_textField->setString(txt.getAsCharSequence());

}

void APPopupBase::validPhoneInput(ui::TextField * i_textField)
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

void APPopupBase::validNumberInput(ui::TextField * i_textField, std::function<void()> i_callback)
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

void APPopupBase::enableButtonBasedOnValidPhoneNumber(std::string i_phoneNumber,ui::Button * i_button)
{
	if (!i_button || i_phoneNumber.empty())
	{
		return;
	}

	i_button->setEnabled(isUTF8StringPhoneNumberValid(i_phoneNumber));
}

void APPopupBase::enableButtonBasedOnValidNumber(std::string i_Number, ui::Button * i_button)
{
	if (!i_button || i_Number.empty())
	{
		return;
	}

	i_button->setEnabled(mj::helper::isUTF8StringValidNumber(i_Number));
}

void APPopupBase::enableWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget* i_node)
{
	if (!i_node || i_leghth <=0 )
	{
		return;
	}
    
    bool enable = (i_text.size() >= i_leghth);
    
    if(!i_text.empty() && isContainSpace(i_text))
    {
        enable = false;
    }

	i_node->setEnabled(enable);
}

void APPopupBase::visibleWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget * i_node)
{
	if (!i_node || i_leghth <= 0)
	{
		return;
	}

	i_node->setVisible(i_text.size() < i_leghth);
}

void APPopupBase::showErrorAlert(std::string msg)
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

void APPopupBase::showNetworkDownAlert(std::function<void()> i_okButtonCallback, std::function<void()> i_cacelButtonCallback)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() 
	{
		auto alert = static_cast<StoryAlert *>(CSLoader::createNode("csb/StoryNetworkDown.csb"));
		alert->setNetworkDownContent(false, true);
		alert->setOkButtonCallback([=]() {
			if (i_okButtonCallback)
			{
				i_okButtonCallback();
			}
		});
		alert->setCacelButtonCallback([=]() {
			if (i_cacelButtonCallback)
			{
				i_cacelButtonCallback();
			}
		});
		alert->show(Director::getInstance()->getRunningScene(), INT_MAX - 1);
	});
}

void APPopupBase::showSuccessPopup(std::string msg)
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, msg] 
	{
	 	auto msgPopup = APMessagePopup::createView(APMessagePopup::MESSAGE_TYPE::SUCCESS, msg);

		Director::getInstance()->getRunningScene()->addChild(msgPopup, INT_MAX - 1);
		
	});
}

void APPopupBase::showFailedPopup(std::string msg)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
    {
        if (msg == "")
        {
            StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "Close");
        }
        else
        {
            StoryAlert::showAlert(msg, "OK", "Close");
//            StoryAlert::showAlert(msg, "OK", (MJDEFAULT->getIntegerForKey(key_lang_display, 1) == 4) ? "\xc4\x90\xc3\xb3\x6e\x67" : "Close");
        }
    });
}

rapidjson::Document APPopupBase::readJsonFromString(const std::string i_json, rapidjson::Type i_type)
{
	std::string jsonStr = i_json;

	//check json content.
	if (jsonStr.empty())
	{
		CCLOG("%s: parse json with empty content %s", PREFIX_ERROR_TAG, i_json.c_str());

		return nullptr;
	}

	std::string jsonStrTemp = jsonStr;

	rapidjson::Document doc;

	doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(jsonStrTemp.c_str());

	//parse error
	if (doc.HasParseError())
	{
		switch (i_type)
		{
		case rapidjson::kNullType:
			break;
		case rapidjson::kFalseType:
			break;
		case rapidjson::kTrueType:
			break;
		case rapidjson::kObjectType:
			SAFE_CHECK_OBJET(!doc.IsObject(), nullptr)
				break;
		case rapidjson::kArrayType:
			SAFE_CHECK_OBJET(!doc.IsArray(), nullptr)
				break;
		case rapidjson::kStringType:
			SAFE_CHECK_OBJET(!doc.IsString(), nullptr)
				break;
		case rapidjson::kNumberType:
			SAFE_CHECK_OBJET(!doc.IsNumber(), nullptr)
				break;
		default:
			break;
		}

		CCLOG("%s:readJsonFromString|doc.HasParseError()|file path=%s", PREFIX_ERROR_TAG, i_json.c_str());

		return nullptr;
	}

	return doc;
}

void APPopupBase::sendOTPAPICall(std::string i_type)
{

}

void APPopupBase::removeFromParentWithAnmation(std::function<void()> i_callback)
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

void APPopupBase::runWarningActionOnBackgroundNode(std::string i_bgName)
{
	if (auto bg = utils::findChild(this, i_bgName))
	{
		cocos2d::Device::vibrate(1.0f);

		bg->runAction(Repeat::create(Sequence::createWithTwoActions(MoveBy::create(0.1f, Vec2(+25, 0)), MoveBy::create(0.1f, Vec2(-25, 0))), 2));
	}
}

void APPopupBase::enableAButtonBasedOnTextfieldLength(cocos2d::ui::Button * i_button, cocos2d::ui::TextField * i_textField, int i_length)
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

void APPopupBase::enableAButtonBasedOnTextfieldsLength(cocos2d::ui::Button * i_button, std::vector<cocos2d::ui::TextField*> i_textFields, cocos2d::ui::Widget * i_node, int i_length)
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
					for (auto textfield : i_textFields)
					{
						enableWidgetNodeBasedOnTextLength(textfield->getString(), i_length, i_button);

						visibleWidgetNodeBasedOnTextLength(textfield->getString(), i_length, i_node);
					}
				}
				case TextField::EventType::DELETE_BACKWARD:
				{
					for (auto textfield : i_textFields)
					{
						enableWidgetNodeBasedOnTextLength(textfield->getString(), i_length, i_button);

						visibleWidgetNodeBasedOnTextLength(textfield->getString(), i_length, i_node);
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

void APPopupBase::showWaittingView()
{
	auto waitView = APWaitView::createView(std::string(""), false, true);
	waitView->setName("waiting_view");
	this->addChild(waitView,POPUP_ZORDER);
}

void APPopupBase::showWaittingViewInCocosThread()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
	{
		showWaittingView();
	});
}

void APPopupBase::removeWaittingView()
{
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
	{
		if (auto waitView = this->getChildByName("waiting_view"))
		{
			waitView->removeFromParent();
		}
	});
}

int APPopupBase::converMillisecondsToHours(long long i_milliseconds)
{
	return (i_milliseconds/ 3600000);
}

bool APPopupBase::isUTF8StringPhoneNumberValid(std::string s)
{
	for (auto c: s)
	{
		if (!iswdigit(c))
		{
			return false;
		}
	}
    
    if (s.length() < PHONE_MIN_LENGTH || s.length() > PHONE_MAX_LENGTH)
    {
          return false;
    }
    
	return true;
}


bool APPopupBase::isUTF8StringNumberValid(std::string i_number)
{
	for (int i=0; i<i_number.size();i++) 
	{
		auto c = i_number.at(i);

		if (!iswdigit(c)) 
		{
			return false;
		}
	}

	return true;
}

bool APPopupBase::isGMail(std::string email)
{
    if(email.empty())
    {
        return false;
    }
    
    try
    {
        const std::regex pattern("^[a-z0-9](\\.?[a-z0-9]){5,}@g(oogle)?mail\\.com$");
        
        return std::regex_match(email, pattern);
    }
    catch (const std::runtime_error& e)
    {
        
    }
    
    return false;
}

void APPopupBase::reduceFontSizeOfTextfieldBasedOnLength(cocos2d::ui::TextField * i_textField, const int i_defaultLength)
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

void APPopupBase::hideVirtualKeyboard(Node* i_textField)
{
	if (i_textField)
	{
		if (auto textfield = dynamic_cast<ui::TextField*>(i_textField))
		{
			UICCTextField *ime = dynamic_cast<UICCTextField*>(textfield->getVirtualRenderer());
			ime->closeIME();
		}
		else if (auto editbox = dynamic_cast<ui::EditBox*>(i_textField))
		{
			/*UICCTextField *ime = dynamic_cast<UICCTextField*>(text->getVirtualRenderer());
				ime->closeIME();*/
		}
	}
}

void APPopupBase::hideVirtualKeyboard(std::vector<Node*> i_textFields)
{
	for (auto textField : i_textFields)
	{
		hideVirtualKeyboard(textField);
	}
}

void APPopupBase::hideVirtualKeyboard(std::vector<ui::TextField*> i_textFields)
{
	for (auto textField : i_textFields)
	{
		hideVirtualKeyboard(textField);
	}
}

void APPopupBase::animationMoveToLeft(Node *i_node,float i_duration)
{
    if(!i_node)
    {
        return;
    }
    
    auto layer = LayerColor::create(cocos2d::Color4B::WHITE);

    layer->setIgnoreAnchorPointForPosition(false);

    layer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    layer->setPosition(m_origin+m_visibleSize/2);
    
    Director::getInstance()->getRunningScene()->addChild(layer,i_node->getLocalZOrder()-1);
    
    auto currentPos = i_node->getPosition();
    
    i_node->setPosition({ m_origin.x + m_visibleSize.width,currentPos.y});
    
    i_node->setOpacity(0);
        
    i_node->runAction(Spawn::createWithTwoActions(MoveTo::create(i_duration, currentPos), FadeIn::create(i_duration)));
    
    layer->runAction(Sequence::createWithTwoActions(DelayTime::create(i_duration), RemoveSelf::create()));

}

void APPopupBase::animationMoveToRight(Node *i_node,float i_duration)
{
    if(!i_node)
    {
        return;
    }
    
    auto layer = LayerColor::create(cocos2d::Color4B::WHITE);

    layer->setIgnoreAnchorPointForPosition(false);

    layer->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    layer->setPosition(m_origin+m_visibleSize/2);
    
    Director::getInstance()->getRunningScene()->addChild(layer,i_node->getLocalZOrder()-1);
    
    auto currentPos = i_node->getPosition();
    
    i_node->setPosition({ - m_origin.x - m_visibleSize.width,currentPos.y});
    
    i_node->setOpacity(0);
        
    i_node->runAction(Spawn::createWithTwoActions(MoveTo::create(i_duration, currentPos), FadeIn::create(i_duration)));
    
    layer->runAction(Sequence::createWithTwoActions(DelayTime::create(i_duration), RemoveSelf::create()));

}

void APPopupBase::avoidSpamOnButton(cocos2d::ui::Button *i_button)
{
    if(!i_button)
    {
        return;
    }
    
    i_button->setTouchEnabled(false);

    i_button->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f),CallFunc::create([=](){
        
        if(i_button)
        {
            i_button->setTouchEnabled(true);
        }

    })));
}

void APPopupBase::replaceAll(std::string& str, const std::string from, const std::string to)
{
    if(str.empty()||from.empty()||to.empty())
    {
        return;
    }
    
    std::regex_replace(str, std::regex(from.c_str()), to);
    
//    size_t start_pos = 0;
//
//    while((start_pos = str.find(from, start_pos)) != std::string::npos)
//    {
//        str.replace(start_pos, from.length(), to);
//        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
//    }
}


void APPopupBase::animationMoveForNodes(std::vector<Node*> i_nodes, AnimationDirection i_direction, float i_duration,  float i_delayTime)
{
	if (i_nodes.empty())
	{
		return;
	}

	const float k_detalTime = i_duration / (float)i_nodes.size();

	auto itWidth = std::max_element(i_nodes.begin(), i_nodes.end(), [](Node* nodeA, Node* nodeB)
	{
		return nodeA->getContentSize().width > nodeB->getContentSize().width;
	});

	auto itHeight = std::max_element(i_nodes.begin(), i_nodes.end(), [](Node* nodeA, Node* nodeB)
	{
		return nodeA->getContentSize().height > nodeB->getContentSize().height;
	});

	const int k_maxWidth	= (*itWidth)->getContentSize().width;

	const int k_maxHeight	= (*itHeight)->getContentSize().height;

	for (int i=0 ; i < i_nodes.size(); i++)
	{
		auto node = i_nodes.at(i);

		if (!node)
		{
			continue;
		}

		auto currentPos = node->getPosition();

		switch (i_direction)
		{
		case APPopupBase::AnimationDirection::LEFT:
			node->setPosition({ currentPos.x + k_maxWidth,currentPos.y });
			break;
		case APPopupBase::AnimationDirection::RIGHT:
			node->setPosition({ currentPos.x - k_maxWidth,currentPos.y });
			break;
		case APPopupBase::AnimationDirection::TOP:
			node->setPosition({ currentPos.x ,currentPos.y - k_maxHeight });
			break;
		case APPopupBase::AnimationDirection::DOWN:
			node->setPosition({ currentPos.x ,currentPos.y + k_maxHeight });
			break;
		default:
			break;
		}

		node->setOpacity(0);

		const float k_actionTime = i_duration - (i_nodes.size() - i - 1) *k_detalTime;

		CCLOG("actionTime:%f", k_actionTime);

		node->runAction( Sequence::createWithTwoActions(DelayTime::create(i_delayTime),Spawn::createWithTwoActions(MoveTo::create(k_actionTime, currentPos), FadeIn::create(k_actionTime))));
	}
}

void APPopupBase::sAnimationMove(Node * i_node, AnimationDirection i_direction, float i_duration)
{
	if (!i_node)
	{
		return;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
    
	auto origin = Director::getInstance()->getVisibleOrigin();

    if(auto currentScene = Director::getInstance()->getRunningScene())
    {
        auto layer = LayerColor::create(cocos2d::Color4B::WHITE);

        layer->setIgnoreAnchorPointForPosition(false);

        layer->setAnchorPoint(Point::ANCHOR_MIDDLE);

        layer->setPosition(origin + visibleSize / 2);

        currentScene->addChild(layer, i_node->getLocalZOrder() - 1);
        
        layer->runAction(Sequence::createWithTwoActions(DelayTime::create(i_duration), RemoveSelf::create()));
    }

	auto currentPos = i_node->getPosition();

	i_node->setPosition({ origin.x + visibleSize.width,currentPos.y });

	switch (i_direction)
	{
	case APPopupBase::AnimationDirection::LEFT:
		i_node->setPosition({ origin.x + visibleSize.width,currentPos.y });
		break;
	case APPopupBase::AnimationDirection::RIGHT:
		i_node->setPosition({ origin.x - visibleSize.width,currentPos.y });
		break;
	case APPopupBase::AnimationDirection::TOP:
		break;
	case APPopupBase::AnimationDirection::DOWN:
		break;
	default:
		break;
	}

	i_node->setOpacity(0);

	i_node->runAction(Spawn::createWithTwoActions(MoveTo::create(i_duration, currentPos), FadeIn::create(i_duration)));

}

void APPopupBase::sAnimationMoveForNodes(std::vector<Node*> i_nodes, AnimationDirection i_direction, float i_duration,  float i_delayTime)
{
    if (i_nodes.empty())
    {
        return;
    }

    const float k_detalTime = i_duration / (float)i_nodes.size();

    auto itWidth = std::max_element(i_nodes.begin(), i_nodes.end(), [](Node* nodeA, Node* nodeB)
    {
        return nodeA->getContentSize().width > nodeB->getContentSize().width;
    });

    auto itHeight = std::max_element(i_nodes.begin(), i_nodes.end(), [](Node* nodeA, Node* nodeB)
    {
        return nodeA->getContentSize().height > nodeB->getContentSize().height;
    });

    const int k_maxWidth    = (*itWidth)->getContentSize().width;

    const int k_maxHeight    = (*itHeight)->getContentSize().height;

    for (int i=0 ; i < i_nodes.size(); i++)
    {
        auto node = i_nodes.at(i);

        if (!node)
        {
            continue;
        }

        auto currentPos = node->getPosition();

        switch (i_direction)
        {
        case APPopupBase::AnimationDirection::LEFT:
            node->setPosition({ currentPos.x + k_maxWidth,currentPos.y });
            break;
        case APPopupBase::AnimationDirection::RIGHT:
            node->setPosition({ currentPos.x - k_maxWidth,currentPos.y });
            break;
        case APPopupBase::AnimationDirection::TOP:
            node->setPosition({ currentPos.x ,currentPos.y - k_maxHeight });
            break;
        case APPopupBase::AnimationDirection::DOWN:
            node->setPosition({ currentPos.x ,currentPos.y + k_maxHeight });
            break;
        default:
            break;
        }

        node->setOpacity(0);

        const float k_actionTime = i_duration - (i_nodes.size() - i - 1) *k_detalTime;

        CCLOG("actionTime:%f", k_actionTime);

        node->runAction( Sequence::createWithTwoActions(DelayTime::create(i_delayTime),Spawn::createWithTwoActions(MoveTo::create(k_actionTime, currentPos), FadeIn::create(k_actionTime))));
    }
}

void APPopupBase::playClickedSoundEffect()
{
    AUDIO_ENGINE->playEffect("sounds/click.mp3");
}

void APPopupBase::visibleAllNodes(std::vector<cocos2d::Node*> i_nodes,bool i_visible)
{
    if(i_nodes.empty())
    {
        return;
    }
    
    for(auto node : i_nodes)
    {
        if(node)
        {
            node->setVisible(i_visible);
        }
    }
}


void APPopupBase::visibleBlackLayer(bool i_visible)
{
	if (auto blackLayer = this->getChildByName("blackLayer"))
	{
		blackLayer->setVisible(i_visible);
	}
}

bool APPopupBase::isContainSpace(const std::string i_input)
{
    if(i_input.empty())
    {
        return false;
    }
    
    for (auto s:i_input)
    {
        if(s == ' ')
        {
            return true;
        }
    }
    
    return false;
}

void APPopupBase::setFontByLang(Node * i_node, TextType type)
{
	LANGUAGE_MANAGER->setFontByLang(i_node,(StoryLanguageManager::TextType)type);
}

void APPopupBase::setFontByLang(std::vector<Node*> i_nodes, TextType type)
{
	LANGUAGE_MANAGER->setFontByLang(i_nodes, (StoryLanguageManager::TextType)type);
}

void APPopupBase::setTextAndFontForNodes(std::vector<std::tuple<Node*, std::string, TextType>> i_nodes)
{
	for (auto nodeInfo : i_nodes)
	{
		auto node		= std::get<0>(nodeInfo);
		auto textKey	= std::get<1>(nodeInfo);
		auto fontType	= std::get<2>(nodeInfo);

		if (!node)
		{
			continue;
		}

		setFontByLang(node, fontType);

		if (textKey.empty())
		{
			continue;
		}

		if (auto castedNode = dynamic_cast<ui::Text*>(node))
		{
			castedNode->setText(LANGUAGE_MANAGER->getDisplayTextByKey(textKey));
		}

		if (auto castedNode = dynamic_cast<ui::Button*>(node))
		{
			castedNode->setTitleText(LANGUAGE_MANAGER->getDisplayTextByKey(textKey));
		}

		if (auto castedNode = dynamic_cast<ui::TextField*>(node))
		{
			castedNode->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey(textKey));
		}

		if (auto castedNode = dynamic_cast<cocos2d::Label*>(node))
		{
			castedNode->setString(LANGUAGE_MANAGER->getDisplayTextByKey(textKey));
		}
	}
}

void APPopupBase::removeChildrenByNames(cocos2d::Node *i_parent, std::vector<std::string> i_names)
{
	if (!i_parent)
	{
		return;
	}

	for (auto name : i_names)
	{
		if (auto node = cocos2d::utils::findChild(i_parent, name))
		{
			node->removeFromParent();
		}
	}
}

std::string APPopupBase::getDisplayTextByKey(std::string i_key)
{
	return LANGUAGE_MANAGER->getDisplayTextByKey(i_key);
}

bool APPopupBase::isThaiLandLanguage()
{
	return LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE;
}

bool APPopupBase::isDigitString(const std::string & str)
{
	return mj::helper::isDigitString(str);
}

void APPopupBase::safeDisableButton(cocos2d::Ref * i_button)
{
	auto button = dynamic_cast<cocos2d::ui::Button*>(i_button);

	if (!button)
	{
		CCLOG("Error|APPopupBase::safeDisableButton");
		return;
	}

	button->setTouchEnabled(false);
}

void APPopupBase::moveScreenWhenUseTextfiled(Node* i_node,AnimationDirection i_direction)
{
	if (!i_node)
	{
		return;
	}

	switch (i_direction)
	{
	case APPopupBase::AnimationDirection::LEFT:
		break;
	case APPopupBase::AnimationDirection::RIGHT:
		break;
	case APPopupBase::AnimationDirection::TOP:
		i_node->runAction(MoveBy::create(0.25, Vec2(0, 200)));
		break;
	case APPopupBase::AnimationDirection::DOWN:
		i_node->runAction(MoveBy::create(0.25, Vec2(0, -200)));
		break;
	default:
		break;
	}

}

void APPopupBase::setupForPhoneNumberTextfield(cocos2d::ui::TextField * i_phoneNumberTextfield, const int i_maxPhoneLength)
{
	if (!i_phoneNumberTextfield)
	{
		CCLOG("<Error>: APPopupBase::setupForPhoneNumberTextfield");

		return;
	}

	i_phoneNumberTextfield->setCursorChar('|');
	i_phoneNumberTextfield->setCursorEnabled(true);
	i_phoneNumberTextfield->setMaxLengthEnabled(true);
	i_phoneNumberTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	i_phoneNumberTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
	i_phoneNumberTextfield->setMaxLength(i_maxPhoneLength);
	i_phoneNumberTextfield->setTextColor(Color4B::BLACK);
	i_phoneNumberTextfield->setPlaceHolderColor(Color4B(90, 90, 90, 255));
	i_phoneNumberTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
	{
		switch (type)
		{
		case TextField::EventType::ATTACH_WITH_IME:
			i_phoneNumberTextfield->setTextColor(Color4B::BLACK);
			break;
		case TextField::EventType::DETACH_WITH_IME:
			i_phoneNumberTextfield->setTextColor(Color4B::BLACK);
			removeSpace(i_phoneNumberTextfield);
			break;
		case TextField::EventType::INSERT_TEXT:
		{
			validPhoneInput(i_phoneNumberTextfield);

			(i_phoneNumberTextfield->getString().size() == 0) ? i_phoneNumberTextfield->setTextColor(Color4B::WHITE) : i_phoneNumberTextfield->setTextColor(Color4B::BLACK);
			break;
		}
		case TextField::EventType::DELETE_BACKWARD:
		{
			validPhoneInput(i_phoneNumberTextfield);

			(i_phoneNumberTextfield->getString().size() == 0) ? i_phoneNumberTextfield->setTextColor(Color4B::WHITE) : i_phoneNumberTextfield->setTextColor(Color4B::BLACK);

			break;
		}
		default:
			break;
		}
	});

}

void APPopupBase::setupForPasswordTextfield(cocos2d::ui::TextField * i_passwordTextfield, cocos2d::Node* i_parent)
{
	if (!i_passwordTextfield)
	{
		CCLOG("<Error>: APPopupBase::setupForPasswordTextfield");

		return;
	}

	i_passwordTextfield->setCursorChar('|');
	i_passwordTextfield->setCursorEnabled(true);
	i_passwordTextfield->setMaxLengthEnabled(true);
	i_passwordTextfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	i_passwordTextfield->setTextHorizontalAlignment(TextHAlignment::LEFT);
	i_passwordTextfield->setMaxLength(PASSWORD_MAX_LENGTH);
	i_passwordTextfield->addEventListener([=](cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
	{
		switch (type)
		{
			case TextField::EventType::ATTACH_WITH_IME:
			{
				i_passwordTextfield->setTextColor(Color4B::BLACK);
				i_passwordTextfield->setString(i_passwordTextfield->getString());
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				moveScreenWhenUseTextfiled(i_parent, APPopupBase::AnimationDirection::TOP);
	#endif
				break;
			}
			case TextField::EventType::DETACH_WITH_IME:
			{	
				i_passwordTextfield->setTextColor((i_passwordTextfield->getString().size() == 0) ? Color4B::WHITE : Color4B::BLACK);
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				moveScreenWhenUseTextfiled(i_parent, APPopupBase::AnimationDirection::DOWN);
	#endif
				removeSpace(i_passwordTextfield);
				break;
			}
			case TextField::EventType::INSERT_TEXT:
			{
				trimSpace(i_passwordTextfield);

				i_passwordTextfield->setTextColor((i_passwordTextfield->getString().size() == 0) ? Color4B::WHITE : Color4B::BLACK);

				break;
			}
			case TextField::EventType::DELETE_BACKWARD:
			{
				trimSpace(i_passwordTextfield);

				i_passwordTextfield->setTextColor((i_passwordTextfield->getString().size() == 0) ? Color4B::WHITE : Color4B::BLACK);

				break;
			}
			default:
				break;
		}
	});
}

std::string APPopupBase::resizeString(std::string i_string, int i_length)
{
	if (i_string.empty())
	{
		return i_string;
	}

	auto uft8String = StringUtils::StringUTF8(i_string);

	if (i_length > uft8String.length())
	{
		return i_string;
	}

	auto new_char = uft8String.getString().at(uft8String.length() - 1)._char;

	while (!uft8String.getString().empty() && i_length < uft8String.length())
	{
		uft8String.deleteChar(uft8String.length() - 1);
	}

	return uft8String.getAsCharSequence();
}

bool APPopupBase::isValidUserName(std::string i_userName)
{
	auto it = std::find_if(i_userName.begin(), i_userName.end(), [](char c) 
	{
		return c == '0';
	});

	if (it != i_userName.end())
	{
		return false;
	}

	const size_t k_numberCount = std::count_if(
		i_userName.begin(),
		i_userName.end(),
		[](char c)
	{ 
		return isdigit(c);
	});

	const size_t k_characterCount = std::count_if(
		i_userName.begin(),
		i_userName.end(),
		[=](char c)
	{
		auto it = std::find_if(k_alphabet.begin(), k_alphabet.end(), [c](char new_char) {

			return c == new_char;
		});

		return (it != k_alphabet.end()); //found

	});
	
	return ((int)i_userName.size() >= USER_NAME_MAX_LENGTH && k_numberCount == 6 && k_characterCount == 2);
}

cocos2d::ui::RichText * APPopupBase::createRichTextFromText(cocos2d::ui::Text * i_text, std::string i_xml)
{
	if (!i_text)
	{
		return nullptr;
	}

	auto richText = ui::RichText::createWithXML(i_xml);
	richText->ignoreAnchorPointForPosition(false);
	richText->setAnchorPoint(i_text->getAnchorPoint());
	richText->ignoreContentAdaptWithSize(false);
	richText->setContentSize(i_text->getContentSize());
	richText->setPosition(i_text->getPosition());
	richText->setName(i_text->getName());
	i_text->getParent()->addChild(richText, i_text->getZOrder());

	return nullptr;
}

cocos2d::ui::EditBox * APPopupBase::createEditBoxFromTextfield(cocos2d::ui::TextField * i_textField,ui::EditBox::InputFlag inputFlag,ui::EditBox::InputMode inputMode
	)
{
	if (!i_textField)
	{
		return nullptr;
	}

	auto _editName = ui::EditBox::create(i_textField->getContentSize(), "new_v4/empty_box.png",ui::Widget::TextureResType::LOCAL);
	_editName->setIgnoreAnchorPointForPosition(false);
	_editName->setAnchorPoint(i_textField->getAnchorPoint());
	_editName->setPosition(i_textField->getPosition());
	_editName->setFontSize(i_textField->getFontSize());
	_editName->setFontName(i_textField->getFontName().c_str());
	_editName->setFontColor(i_textField->getTextColor());
	_editName->setPlaceHolder(i_textField->getPlaceHolder().c_str());
	_editName->setPlaceholderFont(i_textField->getFontName().c_str(), i_textField->getFontSize());
	_editName->setPlaceholderFontColor(i_textField->getPlaceHolderColor());
	_editName->setMaxLength(i_textField->getMaxLength());
	_editName->setTextHorizontalAlignment(i_textField->getTextHorizontalAlignment());
	_editName->setText(i_textField->getString().c_str());
	_editName->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	//_editName->setDelegate(this);
	_editName->setInputMode(inputMode);
	_editName->setName(i_textField->getName());
	_editName->setVisible(i_textField->isVisible());
	_editName->setTag(i_textField->getTag());

	if (auto parent = i_textField->getParent())
	{
		parent->addChild(_editName, i_textField->getLocalZOrder());
	}

	_editName->setInputFlag(inputFlag);

	return _editName ;
}

void APPopupBase::validPhoneInput(ui::EditBox * i_editbox, std::string text)
{
	if (!i_editbox)
	{
		return;
	}

	cocos2d::StringUtils::StringUTF8 numberText = cocos2d::StringUtils::StringUTF8(i_editbox->getText());

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

	i_editbox->setText(numberText.getAsCharSequence().c_str());
}



