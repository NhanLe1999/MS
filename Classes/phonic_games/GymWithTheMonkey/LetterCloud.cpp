#include "LetterCloud.h"
#include "HSAudioEngine.h"

USING_NS_CC;

bool LetterCloud::init()
{
	if (!Node::init())
		return false;

	m_listener = nullptr;
	this->runAction(Sequence::create(DelayTime::create(5.0f), CallFunc::create([=] {
		cloudBtn = cocos2d::ui::Button::create("games/gymwiththemonkey/gym_cloudB.png");
		cloudBtn->setScale(0.1);
		//cloudBtn->setScale(0.75f);
		cloudBtn->addClickEventListener([=](Ref* ref) {
			if (this->m_listener)
				this->m_listener->onSelectLetter(this->m_currentLetter);
			});
		this->addChild(cloudBtn);
		cloudBtn->setVisible(false);
		this->runAction(Sequence::create(DelayTime::create(3.5f), CallFunc::create([=] {
			cloudBtn->setVisible(true);
			cloudBtn->runAction(ScaleTo::create(1.0f, 1));
			}), NULL));
		m_button = cloudBtn;
		m_button->setTouchEnabled(false);
		cloudBtn->setTouchEnabled(false);
		std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
		if (CONFIG_MANAGER->isGameVMonkey()) {
			font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
		}
		m_label = cocos2d::Label::create(" ", font_name, 80);
		m_label->setColor(cocos2d::Color3B(255, 255, 255));
		this->addChild(m_label);
		}), NULL));

	return true;
}

void LetterCloud::setLetter(std::string letter, bool skipAnimation)
{
	//m_button->setTouchEnabled(false);

	const float fadeDuration = 0.2f;

	AUDIO_ENGINE->playSound("sounds/gymwiththemonkey/gym_textshow.mp3", false);

	auto fadeOut = cocos2d::FadeOut::create(fadeDuration);
	auto scaleDown = cocos2d::ScaleTo::create(fadeDuration, 0);
	auto preAction = fadeOut; //cocos2d::Spawn::createWithTwoActions(fadeOut, scaleDown);

	auto setLetterAction = cocos2d::CallFunc::create([=]() {
		m_currentLetter = letter;
		m_label->setString(letter);
		fitValueTextSize();
		});

	auto fadeIn = cocos2d::FadeIn::create(fadeDuration);
	auto scaleUp = cocos2d::ScaleTo::create(fadeDuration, 1);
	auto postAction = fadeIn; //cocos2d::Spawn::createWithTwoActions(fadeIn, scaleUp);
	auto enableTouchAction = cocos2d::CallFunc::create([=]() {
		this->runAction(Sequence::create(DelayTime::create(3.0f), CallFunc::create([=] {
			m_button->setTouchEnabled(true);
			if (cloudBtn) {
				cloudBtn->setTouchEnabled(true);
			}
			}), NULL));
		});
	if (m_label != nullptr) {
		if (!skipAnimation)
			m_label->runAction(cocos2d::Sequence::create(preAction, setLetterAction, postAction, enableTouchAction, nullptr));
		else
			m_label->runAction(cocos2d::Sequence::create(setLetterAction, enableTouchAction, nullptr));
	}
}

void LetterCloud::setTextureColor(int textureColor)
{
	m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudB.png");
	//switch (textureColor) {
	//case 0:
	//	m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudB.png");
	//	break;
	//case 1:
	//	m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudG.png");
	//	break;
	//case 2:
	//	m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudR.png");
	//	break;
	//}
	m_textureColor = textureColor;
}

std::string LetterCloud::getLetter()
{
	return m_currentLetter;
}

cocos2d::Label* LetterCloud::getLabel()
{
	return m_label;
}

void LetterCloud::setListener(CloudSelectListener* listener)
{
	m_listener = listener;
}

void LetterCloud::playAnimation(bool correct, std::function<void()> callback)
{
	if (correct) {
		m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudG.png");
	}
	else {
		m_button->loadTextureNormal("games/gymwiththemonkey/gym_cloudR.png");
	}
	auto basePos = m_button->getPosition();
	Vector<FiniteTimeAction*> actions;
	//for (int i = 0; i < 3; i++) {
	//	actions.pushBack(MoveTo::create(0.1f, basePos - cocos2d::Vec2(10, 0)));
	//	actions.pushBack(MoveTo::create(0.1f, basePos + cocos2d::Vec2(10, 0)));
	//}
	auto scale = m_button->getScale();
	actions.pushBack(CallFunc::create([=]() {
		m_button->setTouchEnabled(false);
		cloudBtn->setTouchEnabled(false);
		}));
	actions.pushBack(ScaleTo::create(0.25, scale + 0.25f));
	actions.pushBack(ScaleTo::create(0.25, scale));
	actions.pushBack(CallFunc::create([=]() {
		m_button->setTouchEnabled(true);
		cloudBtn->setTouchEnabled(true);
		}));
	actions.pushBack(MoveTo::create(0.1f, basePos));
	actions.pushBack(CallFunc::create([=]() {
		setTextureColor(m_textureColor);
		if (callback)
			callback();
		}));
	m_button->runAction(Sequence::create(actions));
}

void LetterCloud::fitValueTextSize()
{
	auto size = m_button->getContentSize();
	auto label_size = m_label->getContentSize();
	if (label_size.width > (size.width - 40)) {
		m_label->setScale((size.width - 40) / label_size.width);

	}

}

void LetterCloud::setClickAbleStatus(bool isStautus)
{
	cloudBtn->setTouchEnabled(isStautus);
	m_button->setTouchEnabled(isStautus);
}
