#include "CatchingThiefWordFrame.h"
#include "ui/CocosGUI.h"
#include "Shake.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"

using namespace CatchingThief;
USING_NS_CC;

WordFrame * WordFrame::createWithLetters(const std::vector<std::string> &letters)
{
	WordFrame * ret = new (std::nothrow) WordFrame();
	if (ret && ret->initWithLetters(letters))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool WordFrame::initWithLetters(const std::vector<std::string>& letters)
{
	//bg
	auto bg = ui::Scale9Sprite::create(Rect(6, 6, 1, 1), "games/catchingthief/word_frame.png");
	bg->setPreferredSize(cocos2d::Size(420, 94));
	bg->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	addChild(bg);

	auto highestHeight = 0;
	auto width = 0;
	auto margin = 10;
	if (m_letterWrapperNode)
		m_letterWrapperNode->removeFromParent();
	m_letterWrapperNode = cocos2d::Node::create();
	addChild(m_letterWrapperNode);
	m_letterNodes.clear();
	revealIndex = 0;
	float widestWidth = 0;
	std::vector<std::string> arrangedLetters(letters);
	std::vector<std::pair<int, int>> strPair;

	for (auto i = 0; i < arrangedLetters.size(); i++) {
		auto str = arrangedLetters.at(i);
		auto firstUnderscore = str.find('_');
		if (firstUnderscore == std::string::npos) {
			auto offset = 1;
			while (offset < str.length())
			{
				arrangedLetters.insert(arrangedLetters.begin() + i + offset, str.substr(offset, 1));
				strPair.push_back(std::make_pair(i, i + offset));
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
        std::string font_name = CONFIG_MANAGER->isGameVMonkey()?PHONIC_MANAGER->getInstance()->getGameFontName():"fonts/KGWHATTHETEACHERWANTS.TTF";
		auto label = cocos2d::Label::create(*it, font_name, 75);
		label->setAnchorPoint(cocos2d::Vec2::ZERO);
		label->setTextColor(Color4B(0xff, 0x33, 0, 255));

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

		// underscore
		auto underscore = Sprite::create("games/catchingthief/underscore.png");
		underscore->setAnchorPoint(cocos2d::Vec2::ZERO);
		underscore->setScale(widestWidth / underscore->getContentSize().width, 3 / underscore->getContentSize().height);
		underscore->setPosition(cocos2d::Vec2(width, -5));
		m_letterWrapperNode->addChild(underscore);

		width += (widestWidth + margin);
	}

	m_letterWrapperNode->setPosition(-width / 2 + margin, -highestHeight / 2 + 10);

	return true;
}

const std::vector<cocos2d::Label*>& WordFrame::getLetterNodes()
{
	return m_letterNodes;
}

void WordFrame::clearLetterNodes()
{
	m_letterNodes.clear();
}

void WordFrame::reveal()
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

WordFrame::WordFrame() :
	m_letterWrapperNode(nullptr),
	revealIndex(0)
{
}

void WordFrame::createLetterPair(cocos2d::Node * head, cocos2d::Node * tail, int offset)
{
	tail->setUserData((void*)1);
	head->setUserObject(tail);
}
