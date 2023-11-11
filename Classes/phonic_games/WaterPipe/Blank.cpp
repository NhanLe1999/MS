#include "Blank.h"

using namespace cocos2d;
INITIALIZE_READER(Blank);

Blank* Blank::createView() {

	REGISTER_CSB_READER(Blank);

	auto view = reinterpret_cast<Blank*>(CSLoader::createNode("csb/game/WaterPipe/Blank.csb"));
	if (view) {
		return view;
	}
	return nullptr;
}

std::string Blank::getItsAnswer() {
	return this->answer;
}

void Blank::setItsAnswer(std::string str) {
	this->answer = str;
}


bool Blank::getRed() {
	return this->reded;
}

void Blank::setRed(bool red) {
	this->reded = red;
}

void Blank::runWrongAnimation() {
	//this->stopAllActions();
	this->runAction(this->blankTimeline);
	this->blankTimeline->play("wrong", false);
}

float Blank::getMainScaleX() {
	return this->getChildByName<cocos2d::Sprite*>("blank")->getScaleX();
}

float Blank::getMainScaleY() {
	return this->getChildByName<cocos2d::Sprite*>("blank")->getScaleY();
}

float Blank::getRealWidth() {
	return this->getChildByName<cocos2d::Sprite*>("blank")->getContentSize().width * this->getMainScaleX();
}

float Blank::getRealHeight() {
	return this->getChildByName<cocos2d::Sprite*>("blank")->getContentSize().height * this->getMainScaleY();
}

void Blank::setRealPosition(cocos2d::Vec2 point) {
	this->pos = point;
}

cocos2d::Vec2 Blank::getRealPosition() {
	return this->pos;
}

#define sqr(x) (x) * (x)

bool Blank::inBlank(cocos2d::Point point) {
	float xnho = this->pos.x;
	float ynho = this->pos.y;
	
	float xlon = xnho + this->getChildByName<cocos2d::Sprite*>("blank")->getContentSize().height * this->getScaleY() / 2;
	float ylon = ynho + this->getChildByName<cocos2d::Sprite*>("blank")->getContentSize().width * this->getScaleX() / 2;
	return sqrt(sqr(point.x - xlon) + sqr(point.y - ylon)) <= 2 * (xlon - xnho);
}
