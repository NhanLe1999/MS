#include "SpacePipeSlot.h"

bool SpacePipeSlot::init()
{
	if (!Node::init())
		return false;

	auto sprite = Sprite::create("games/spacestation/8.png");
	sprite->setOpacity(0);
	sprite->runAction(Sequence::createWithTwoActions(
		DelayTime::create(0.5f),
		FadeIn::create(0.5f)
	));
	addChild(sprite);

	return true;
}

void SpacePipeSlot::setWord(mj::GameObject word)
{
	m_word = word;
}

mj::GameObject SpacePipeSlot::getWord()
{
	return m_word;
}

SpacePipeSlot::SpacePipeSlot()
{
}
