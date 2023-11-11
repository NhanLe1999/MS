#include "WordFrame_VM.h"
#include "ui/CocosGUI.h"
#include "Shake.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"

using namespace WizardCauldron_VM;
USING_NS_CC;

WordFrame_VM * WordFrame_VM::createWithLetters(const std::vector<std::string> &letters, bool co_dau,int pos_dau)
{
	WordFrame_VM * ret = new (std::nothrow) WordFrame_VM();
    ret->_co_dau = co_dau;
    ret->_pos_dau = pos_dau;
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

WordFrame_VM * WordFrame_VM::createWithLetters(const std::vector<mj::GameObject> &objs, bool co_dau, int pos_dau)
{
	std::vector<std::string> letters;
	for (auto it = objs.begin(); it != objs.end(); it++) {
		letters.push_back(it->text);
	}
	return createWithLetters(letters,co_dau,pos_dau);
}

bool WizardCauldron_VM::WordFrame_VM::initWithLetters(const std::vector<std::string>& letters)
{
	//bg
	auto bg = ui::Scale9Sprite::create(Rect(23, 23, 1, 1), "games/wizardcauldron/3.png");
	bg->setPreferredSize(cocos2d::Size(500, 94));
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
		cocos2d::StringUtils::StringUTF8 str = cocos2d::StringUtils::StringUTF8(arrangedLetters.at(i));
        auto firstUnderscore = str.getAsCharSequence().find('_');
		if (firstUnderscore == std::string::npos) {
			auto offset = 1;
            auto a= str.length();
			while (offset < str.length())
			{
				arrangedLetters.insert(arrangedLetters.begin() + i + offset, str.getAsCharSequence(offset, 1));
				strPair.push_back(std::make_pair(i + offset - 1, i + offset));
				offset++;
			}
			arrangedLetters.at(i) = str.getAsCharSequence(0, 1);
			continue;
		}

        auto baseStr = str.getAsCharSequence(0, firstUnderscore);
		auto index = firstUnderscore;
		auto appendStr = str.getAsCharSequence(firstUnderscore + 1);
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
		auto label = cocos2d::Label::create(*it, CONFIG_MANAGER->isGameVMonkey()?"fonts/danhvan_center.ttf": "fonts/KGWHATTHETEACHERWANTS.TTF", 75);
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
    unders.clear();
	for (auto it = m_letterNodes.begin(); it != m_letterNodes.end(); it++) {
		auto label = *it;
		label->setPosition(cocos2d::Vec2(width + widestWidth / 2 - label->getContentSize().width / 2, 0));
            
		// underscore
		auto underscore = Sprite::create("games/wizardcauldron/10.png");
		underscore->setAnchorPoint(cocos2d::Vec2::ZERO);
		underscore->setScale(widestWidth / underscore->getContentSize().width, 3 / underscore->getContentSize().height);
		underscore->setPosition(cocos2d::Vec2(width, -5));
		m_letterWrapperNode->addChild(underscore);
        unders.push_back(underscore);
		width += (widestWidth + margin);
	}

	m_letterWrapperNode->setPosition(-width / 2 + margin, -highestHeight / 2);
    if (_co_dau){
        auto height = m_letterNodes[_pos_dau]->getContentSize().height;
        auto width = m_letterNodes[_pos_dau]->getContentSize().width;
        m_letterNodes.back()->setAnchorPoint(Vec2(0.5,0.5));
        if (letters.back() == "."){
            m_letterNodes.back()->setPosition(m_letterNodes[_pos_dau]->getPosition()+Vec2(width/2,8));
            unders.back()->setPosition(unders[_pos_dau]->getPosition()+Vec2(0,-15));
        }else{
            std::vector<std::string> nguyen_am = {"a","e","y","u","o"};
            int yy = 14 ;
            for (auto am : nguyen_am){
                if (m_letterNodes[_pos_dau]->getString() == am){
                    yy = 0;
                    break;
                }
            }
            int xx = 0;
            if (m_letterNodes.back()->getString() == "´"){
                xx = 5;
            }
            m_letterNodes.back()->setPosition(m_letterNodes[_pos_dau]->getPosition()+Vec2(width/2+xx,height + yy));
            unders.back()->setPosition(unders[_pos_dau]->getPosition()+Vec2(0,80));
        }
        
    }
	return true;
}

const std::vector<cocos2d::Label*>& WizardCauldron_VM::WordFrame_VM::getLetterNodes()
{
	return m_letterNodes;
}

void WizardCauldron_VM::WordFrame_VM::clearLetterNodes()
{
	m_letterNodes.clear();
}

void WizardCauldron_VM::WordFrame_VM::reveal()
{
	if (revealIndex >= m_letterNodes.size())
		return;
	auto label = m_letterNodes.at(revealIndex);
    
	while ((uintptr_t)label->getUserData() ) // tail node
	{
		if (revealIndex >= m_letterNodes.size())
			return;
		label = m_letterNodes.at(revealIndex++);
	}
	while (label) {
        unders[revealIndex]->setVisible(false);
        revealIndex++;
		auto scaleTo = ScaleTo::create(0.5, 1, 1);
		label->runAction(EaseBackOut::create(scaleTo));
		label = dynamic_cast<cocos2d::Label*>(label->getUserObject());
	}
}

WizardCauldron_VM::WordFrame_VM::WordFrame_VM() :
	m_letterWrapperNode(nullptr),
	revealIndex(0)
{
}

void WizardCauldron_VM::WordFrame_VM::createLetterPair(cocos2d::Node * head, cocos2d::Node * tail, int offset)
{
	tail->setUserData((void*)1);
	head->setUserObject(tail);
}
