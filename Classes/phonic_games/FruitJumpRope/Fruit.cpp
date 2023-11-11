#include "Fruit.h"

#define GAF_LIMBS "gaf/FruitJumpRope/jumprope_limbs/jumprope_limbs.gaf"

Fruit* Fruit::Init(string gaf)
{
    auto fruit = new Fruit();
    fruit->gaf = gaf;
    fruit->init(GAF_LIMBS);
    fruit->playSequence("idle", true);
    fruit->setAnchorPoint(Vec2(0.5, 0.5));

    fruit->content = GAFWrapper::createGAF(gaf);
    fruit->content->playSequence("idle", true);
    fruit->content->setAnchorPoint(Vec2(0, 0));
    fruit->content->setPosition(Vec2(0, 0));
    fruit->addChild(fruit->content);

    fruit->isIdle = true;
	return fruit;
}

void Fruit::idle(bool loop)
{
	this->isIdle = true;
	this->playSequence("idle", loop);
	this->content->playSequence("idle", loop);
}

void Fruit::start(bool loop, function<void()> call_back)
{
	this->isIdle = false;
	this->playSequence("start", loop, [=](gaf::GAFObject *, const std::string & sqN) {
		if (call_back) {
			call_back();
		}
	});
	this->content->playSequence("start", loop);
}

void Fruit::fast(bool loop, function<void()> call_back)
{
	this->isIdle = false;
	this->playSequence("fast", loop, [=](gaf::GAFObject *, const std::string & sqN) {
		if (call_back) {
			call_back();
		}
	});
	this->content->playSequence("fast", loop);
}

void Fruit::stop(bool loop)
{
	this->playSequence("stop", loop);
	this->content->playSequence("stop", loop);
}

void Fruit::mchoice(bool loop)
{
	if (loop)
	{
		this->playSequence("mchoice", true);
		this->content->playSequence("mchoice", false);
		return;
	}

	this->runAction(Sequence::create(CallFunc::create([=] {
		this->playSequence("mchoice", false);
		this->content->playSequence("mchoice", false);
	}), DelayTime::create(27.0f/32), CallFunc::create([=] {
		this->idle(true);
	}), NULL));
}

void Fruit::wrong(bool loop)
{
	if (loop)
	{
		this->playSequence("wrong", true);
		this->content->playSequence("wrong", false);
		return;
	}

	this->runAction(Sequence::create(CallFunc::create([=] {
		this->playSequence("wrong", false);
		this->content->playSequence("wrong", false);
	}), DelayTime::create(1.0), CallFunc::create([=] {
		this->idle(true);
	}), NULL));
}

void Fruit::correct(bool loop)
{
	this->playSequence("correct", loop);
	this->content->playSequence("correct", loop);
}
