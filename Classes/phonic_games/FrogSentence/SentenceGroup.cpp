#include "SentenceGroup.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
#include "TextLeaf.h"

bool SentenceGroup::init()
{
	if (!Node::init())
		return false;
	return true;
}

void SentenceGroup::showSentence(float initialDelay, const std::vector<WordSync>& words, std::string audio, std::function<void()> callback, float callbackDelay)
{
	initLabels(words);
	scheduleOnce([this, &words, audio, callback, callbackDelay](float) {
		showSentence(words, audio, callback, callbackDelay);
	}, initialDelay, "show_sentence_delay");
}

void SentenceGroup::showSentence(const std::vector<WordSync> &words, std::string audio,
	std::function<void()> callback, float callbackDelay)
{
	initLabels(words);
	auto counter = 0;
	AUDIO_ENGINE->playSound(audio, false);
	float lastDelay = 0;
	for (auto it = words.begin(); it != words.end(); it++)
	{
		auto label = m_labels[counter];
		auto delay = DelayTime::create(it->spawnTime);
		lastDelay = it->spawnTime + 0.4f;
		
		auto flash = CallFunc::create([=]() {
			flashLabel(label);
		});

		label->runAction(Sequence::create(delay, flash, nullptr));
		counter++;
	}

	lastDelay += callbackDelay;
	if (!callback)
		return;
	scheduleOnce([=](float) {
		callback();
	}, lastDelay, "callback_delay");
}

void SentenceGroup::initLabels(const std::vector<WordSync>& words)
{
	if (!m_dirtySentence)
		return;

	removeAllChildren();
	m_labels.clear();    setFontSz(words);
	float totalWidth = 0;
	auto margin = 20;
    std::string font_name = "fonts/LeapingFrogFont.ttf";
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_name = PHONIC_MANAGER->getGameFontName();
    }
	for (auto it = words.begin(); it != words.end(); it++) {
		auto label = Text::create(it->word.text, font_name/*"fonts/KGWHATTHETEACHERWANTS.TTF"*/, font_sz);
		addChild(label);
		label->setTextColor(Color4B::WHITE);
		//label->enableOutline(Color4B::RED, 2);
		m_labels.push_back(label);
		totalWidth += label->getContentSize().width;
	}

	// refresh position
	totalWidth += ((m_labels.size() - 1) * margin);
    
    
    auto screen_width = cocos2d::Director::getInstance()->getWinSize().width;
    if(totalWidth > screen_width)
    {
        float fixWidth = 0;
        auto lb_it = m_labels.end();
        lb_it--;
        while(totalWidth > screen_width * 0.8)
        {
            auto label = *lb_it;
            totalWidth -= label->getContentSize().width + 20;
            fixWidth += label->getContentSize().width + 20;
            lb_it--;
        }
        fixWidth -= 20;
        auto basePos = cocos2d::Vec2(-totalWidth / 2, -10);
        for(auto it = m_labels.begin(); it != lb_it; it++)
        {
            auto label = *it;
            basePos.x += label->getContentSize().width / 2;
            label->setPosition(basePos);
            basePos.x += (label->getContentSize().width / 2 + margin);
        }
        auto basePos1 = cocos2d::Vec2(-fixWidth / 2, -90);
        for(auto it = lb_it; it != m_labels.end(); it++)
        {
            auto label = *it;
            basePos1.x += label->getContentSize().width / 2;
            label->setPosition(basePos1);
            basePos1.x += (label->getContentSize().width / 2 + margin);
        }
    }
    else
    {
        auto basePos = cocos2d::Vec2(-totalWidth / 2, -10);
        for (auto it = m_labels.begin(); it != m_labels.end(); it++) {
            auto label = *it;
            basePos.x += label->getContentSize().width / 2;
            label->setPosition(basePos);
            basePos.x += (label->getContentSize().width / 2 + margin);
        }
    }
	m_dirtySentence = false;
}

void SentenceGroup::repeatSentence(const std::vector<WordSync>& words, const std::vector<WordSync>::iterator it, std::function<void(int auId, std::string auPath)> callback,
	std::vector<math::resource::Sync_Text> listTextHight, std::string m_audioPath, std::vector<TextLeaf*> m_leaves, cocos2d::Color4B initilSyncTextColor, cocos2d::Color4B startSyncTextColor, cocos2d::Color4B endSyncTextColor)
{
	initLabels(words);
	if (it == words.begin()) {
		m_label_used.clear();
	}
	if (it == words.end()) {
		if (callback)
			callback(-1, "");
		return;
	}
	int kt = 1;

	if (listTextHight.size() > 0)
	{
		//auto text = m_labels.at(0);
		for (int i = 0; i < m_labels.size(); i++)
		{
			if(i!= 0)
			m_labels.at(i)->setVisible(false);
			m_labels.at(i)->setTextColor(initilSyncTextColor);
	
		}

		ms::game::startHighlightText(listTextHight, m_labels.at(0), m_audioPath, callback, "", 15, startSyncTextColor, endSyncTextColor);
		if (m_leaves.size() > 0) {
			Vector<FiniteTimeAction*> actions1;
			for (int i = 0; i < listTextHight.size(); i++)
			{
				for (int j = 0; j < m_leaves.size(); j++)
				{
					if (m_leaves.at(j) != nullptr && (m_leaves.at(j))->getText() == listTextHight.at(i).w && ((m_leaves.at(j))->getLabel())) {
						//(m_leaves.at(i))->getLabel()->setTextColor(Color4B::RED);

						actions1.pushBack(CallFunc::create([=]() {
								(m_leaves.at(j))->getLabel()->setTextColor(startSyncTextColor);
						}));
						float delayTime = ((float)listTextHight.at(i).e - (float)listTextHight.at(i).s) / (float)1000;
						actions1.pushBack(DelayTime::create(delayTime));
						actions1.pushBack(CallFunc::create([=]() {
							(m_leaves.at(j))->getLabel()->setTextColor(endSyncTextColor);
							}));
						break;
					}
				}
			}
			if(m_leaves.at(0)->getParent())
				m_leaves.at(0)->getParent()->runAction(Sequence::create(actions1));
		}


	}

	else
	{
		for (auto labelIt = m_labels.begin(); labelIt != m_labels.end(); labelIt++)
		{
			if ((*labelIt)->getString() == it->word.text) {
				flashLabel(*labelIt);
				if (kt) m_label_used.insert(*labelIt);
				kt = 0;
				//break;
			}
		}

		AUDIO_ENGINE->playSound(it->word.audio_path, false, [this,&words,it,callback]() {

		auto repeatIt = it;
	
		if(words.size() >0 && repeatIt != words.end())
		{
			repeatIt++;
			repeatSentence(words, repeatIt,callback);
		}
	});
	}


}

SentenceGroup::SentenceGroup() :
	m_completeCallback(nullptr),
	m_dirtySentence(true)
{
}

void SentenceGroup::flashLabel(Text* label)
{
	auto move1 = CallFunc::create([=]() {
		label->setTextColor(Color4B::RED);
		//label->enableOutline(Color4B::WHITE, 2);
	});
	auto move2 = ScaleTo::create(0.2f, 1.2f);
	auto move3 = ScaleTo::create(0.2f, 1);
	auto move4 = CallFunc::create([=]() {
		label->setTextColor(Color4B::WHITE);
		//label->enableOutline(Color4B::RED, 2);
	});
	label->runAction(Sequence::create(move1, move2, move3, move4, nullptr));
}

void SentenceGroup::setFontSz(const std::vector<WordSync>& words){
    auto margin = 20;
    std::string str = "";
    for (auto it = words.begin(); it != words.end(); it++) {
        str.append(it->word.text);
        str.append(" ");
    }
    
    auto text = ui::Text::create(str, "fonts/LeapingFrogFont.ttf", font_sz);
    float totalW = text->getVirtualRendererSize().width + ((words.size() - 1) * margin);
    while (totalW > Director::getInstance()->getWinSize().width * 0.9) {
        font_sz -= 2;
        text->setFontSize(font_sz);
        totalW = text->getVirtualRendererSize().width + ((words.size() - 1) * margin);
    }
}
