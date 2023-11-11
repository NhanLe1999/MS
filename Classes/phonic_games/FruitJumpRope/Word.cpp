#include "Word.h"
#include "HSAudioEngine.h"
#include "StoryConfigManager.h"
using namespace cocos2d;

INITIALIZE_READER(Word);

Word * Word::Init(string w)
{
	REGISTER_CSB_READER(Word);

	auto word = reinterpret_cast<Word*>(CSLoader::createNode("csb/game/FruitJumpRope/FruitJumpRope_Word.csb"));
    if (word)
    {
        word->text = w;
        std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
        if (CONFIG_MANAGER->isGameVMonkey()){
            font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
        }
		auto screenSize = Director::getInstance()->getVisibleSize();
		if (screenSize.width / screenSize.height < 1.5) {
			word->textField = TextFieldTTF::create(w, font_name, 80);
		}
		else {
			word->textField = TextFieldTTF::create(w, font_name, 100);
		}
        
        word->addChild(word->textField);
        word->textField->setColor(Color3B::BLACK);
        word->textField->setVisible(true);
        word->textField->setPosition(Vec2(0, 0));

        word->layout = reinterpret_cast<cocos2d::ui::Layout*>(cocos2d::utils::findChild(word, "panel"));

        word->layout->setContentSize(Size(word->textField->getContentSize().width + 10, word->textField->getContentSize().height+20));

        //set up touch handling
        word->touchable = true;
        word->layout->setTouchEnabled(true);
        word->layout->setVisible(true);
        word->layout->setLocalZOrder(1000);
        word->layout->addClickEventListener([=] (Ref* ref) {
            word->addClick();
        });
		return word;
	}
	return nullptr;
}

void Word::setDelegate(WordDelegate* delegate1)
{
	this->delegate = delegate1;
}

void Word::addClick()
{
	if (!this->touchable) return;
	AUDIO_ENGINE->stopAllAudio();
	AUDIO_ENGINE->playSound(wordAudio, false, nullptr, 1.0);
	if (delegate) delegate->WordClick(this);
}

void Word::setText(string word)
{
	this->textField->setString(word);
	this->text = word;
	this->layout->setContentSize(Size(this->textField->getContentSize().width + 10, this->textField->getContentSize().height + 20));
}

void Word::wrong()
{
	//smaller and set opacity
	auto tb = cocos2d::utils::findChild(this, "tb");
	tb->runAction(ScaleTo::create(0.25, 0.5));
	textField->getParent()->runAction(Sequence::create(ScaleTo::create(0.25, 0),DelayTime::create(0.3f) ,CallFunc::create([=] {
		
		textField->getParent()->setVisible(false);
		}),NULL));
	
}

void Word::correct()
{
	//bigger
    if (CONFIG_MANAGER->isGameVMonkey()){
        AUDIO_ENGINE->playSound(wordAudio);
    }
	textField->runAction(Spawn::create(ScaleTo::create(0.25, 1.3), CallFunc::create([=] {
		textField->getParent()->setColor(Color3B::GREEN);
		}),NULL));
}

void Word::changeColor(Color3B color)
{
	this->textField->setColor(color);
}

void Word::setAudio(string wordAudio)
{
	this->wordAudio = wordAudio;
}

void Word::showTB(int state)
{
	auto tb = cocos2d::utils::findChild(this, "tb");
	auto tb_r = cocos2d::utils::findChild(this, "tb_r");

	switch (state)
	{
	case 0:
		tb->setVisible(false);
		tb_r->setVisible(false);
		break;
	case 1:
		tb->setVisible(true);
		tb_r->setVisible(false);
		break;
	case 2:
		tb->setVisible(false);
		tb_r->setVisible(true);
		break;
	}
}

void Word::fixFit()
{
	auto tb = reinterpret_cast<cocos2d::Node*>(cocos2d::utils::findChild(this, "tb"));
	this->layout->setContentSize(tb->getContentSize());
}
