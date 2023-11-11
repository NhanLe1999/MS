#include "WordFrame.h"
#include "ui/CocosGUI.h"
#include "Shake.h"

#include "TextNode.h"
#include "WizardCauldronGame.h"
#include "json/rapidjson.h"
#include "json/schema.h"
#include "json/stringbuffer.h"
#include "cocos2d.h"

using namespace WizardCauldron;
USING_NS_CC;

WordFrame * WordFrame::createWithLetters(const std::vector<std::string> &letters, bool hideText)
{
	WordFrame * ret = new (std::nothrow) WordFrame();
	if (ret && ret->initWithLetters(letters, hideText))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

WordFrame * WordFrame::createWithLetters(const std::vector<mj::GameObject> &objs, bool hideText)
{
	std::vector<std::string> letters;
	for (auto it = objs.begin(); it != objs.end(); it++) {
		letters.push_back(it->text);
	}
	return createWithLetters(letters, hideText);
}

bool WizardCauldron::WordFrame::initWithLetters(const std::vector<std::string>& letters, bool hideText)
{
	_hideText = hideText;
	float gach_chan = 0;
	auto vibisize = Director::getInstance()->getVisibleSize();
	//bg
	_max = 0;
	auto bg = ui::Scale9Sprite::create(Rect(23, 23, 1, 1), "games/wizardcauldron/3.png");
	bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	

	auto highestHeight = 0;
	auto width = 0;
	auto margin = 10;
	if (m_letterWrapperNode)
		m_letterWrapperNode->removeFromParent();
	m_letterWrapperNode = cocos2d::Node::create();
	m_letterNodes.clear();
	revealIndex = 0;
	float widestWidth = 0;
	std::vector<std::string> arrangedLetters(letters);
	std::vector<std::pair<int, int>> strPair;

	for (auto i = 0; i < arrangedLetters.size(); i++) {
		
		auto str = arrangedLetters.at(i);
		auto firstUnderscore = str.find('_');
		CCLOG("%s tu can tim va do dai la % d", str.c_str(), str.length());
		_max = _max + 1;
		if (firstUnderscore == std::string::npos) {
			auto offset = 1;
			while (offset < str.length())
			{
				arrangedLetters.insert(arrangedLetters.begin() + i + offset, str.substr(offset, 1));
				strPair.push_back(std::make_pair(i + offset - 1, i + offset));
				offset++;
			}
			arrangedLetters.at(i) = str.substr(0, 1);
			continue;
		}

		auto baseStr = str.substr(0, firstUnderscore);
		auto index = firstUnderscore;
		auto appendStr = str.substr(firstUnderscore + 1);
		arrangedLetters.at(i) = baseStr;
		do {
			index++;
			auto nextUnderscore = appendStr.find('_');
			if (nextUnderscore == std::string::npos) {
				arrangedLetters.insert(arrangedLetters.begin() + i + index, appendStr);
				strPair.push_back(std::make_pair(i, i + index));
				break;
			}
			if (nextUnderscore > 0) {
				arrangedLetters.insert(arrangedLetters.begin() + i + index, appendStr.substr(0, nextUnderscore));
				strPair.push_back(std::make_pair(i, i + index));
			}
			appendStr = appendStr.substr(nextUnderscore + 1);
		} while (1);
	}

	for (auto it = arrangedLetters.begin(); it != arrangedLetters.end(); it++) {
		Label* label = cocos2d::Label::create(*it, "fonts/KGWHATTHETEACHERWANTS.TTF", (_hideText ? vibisize.width * 0.035 : 80));
		label->setAnchorPoint(cocos2d::Vec2::ZERO);

		widestWidth = std::max(widestWidth, label->getContentSize().width);
		highestHeight = highestHeight > label->getContentSize().height ? highestHeight : label->getContentSize().height;
		m_letterWrapperNode->addChild(label);
		label->setScaleY(0);

		// pair-data
		label->setUserData(0);
		label->setUserObject(0);

		m_letterNodes.push_back(label);
	}

	for (auto it = strPair.begin(); it != strPair.end(); it++) {
		createLetterPair(m_letterNodes.at(it->first), m_letterNodes.at(it->second),it->second - it->first);
	}

	// reposition
	for (auto it = m_letterNodes.begin(); it != m_letterNodes.end(); it++) {
		auto label = *it;
		label->setPosition(cocos2d::Vec2(width + widestWidth / 2 - label->getContentSize().width / 2, 0));
		if (_hideText)
		{
			if (label->getString() == " ")
			{
				auto label__ = cocos2d::Label::create(label->getString(), "fonts/KGWHATTHETEACHERWANTS.TTF", 2);
				label__->setAnchorPoint(cocos2d::Vec2::ZERO);
				label__->setScale(widestWidth / label__->getContentSize().width, 3 / label__->getContentSize().height);
				label__->setPosition(cocos2d::Vec2(width, -5));
				m_letterWrapperNode->addChild(label__);

				width += (widestWidth + margin - 11);
			}
			else
			{
				// underscore
				auto underscore = Sprite::create("games/wizardcauldron/10.png");
				underscore->setAnchorPoint(cocos2d::Vec2::ZERO);
				underscore->setScale(widestWidth / underscore->getContentSize().width, 3 / underscore->getContentSize().height);
				underscore->setPosition(cocos2d::Vec2(width, -5));
				m_letterWrapperNode->addChild(underscore); 
				gach_chan = underscore->getBoundingBox().size.width;

				width += (widestWidth + margin);
			}
		}
		else
		{
			auto underscore = Sprite::create("games/wizardcauldron/10.png");
			underscore->setAnchorPoint(cocos2d::Vec2::ZERO);
			underscore->setScale(widestWidth / underscore->getContentSize().width, 3 / underscore->getContentSize().height);
			underscore->setPosition(cocos2d::Vec2(width, -5));
			m_letterWrapperNode->addChild(underscore);

			width += (widestWidth + margin);
		}
	}

	m_letterWrapperNode->setPosition(-width / 2 + margin, -highestHeight / 2 + 10);
	bg->setName("background_answer");
	bg->setPreferredSize(_hideText ? cocos2d::Size(_max * (gach_chan + 10), 94) : cocos2d::Size(500, 94));

	addChild(bg);
	addChild(m_letterWrapperNode);
	

	return true;
}

const std::vector<cocos2d::Label*>& WizardCauldron::WordFrame::getLetterNodes()
{
	return m_letterNodes;
}

void WizardCauldron::WordFrame::clearLetterNodes()
{
	m_letterNodes.clear();
}

void WizardCauldron::WordFrame::reveal()
{
	if (revealIndex >= m_letterNodes.size())
		return;

	auto label = m_letterNodes.at(revealIndex++);
	while ((uintptr_t)label->getUserData() ) // tail node
	{
		if (revealIndex >= m_letterNodes.size())
			return;
		label = m_letterNodes.at(revealIndex++);
	}
	while (label) {
		auto scaleTo = ScaleTo::create(0.5, 1, 1);
		label->runAction(EaseBackOut::create(scaleTo));
		label = dynamic_cast<cocos2d::Label*>(label->getUserObject());
	}
}

WizardCauldron::WordFrame::WordFrame() :
	m_letterWrapperNode(nullptr),
	revealIndex(0)
{
}

void WizardCauldron::WordFrame::createLetterPair(cocos2d::Node * head, cocos2d::Node * tail, int offset)
{
	tail->setUserData((void*)1);
	head->setUserObject(tail);
}
