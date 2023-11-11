#include "Choice.h"

USING_NS_CC;

std::string Choice::getText() {
	return this->text;
}

bool Choice::canStillBeTouched() {
	return !this->cantTouchAnyMore;
}

void Choice::setText(std::string str) {
	this->text = str;
}

bool Choice::init() {
	if ( !cocos2d::Node::init() ) {
		return false;
	}
	this->choiceTimeline = cocos2d::CSLoader::createTimeline("csb/game/WaterPipe/Choice.csb");
	this->choiceTimeline->retain();
	this->isAnswer = false;
	this->touchable = true;
	this->canBeClicked = true;
	return true;
}

void Choice::onExit() {
	this->choiceTimeline->release();
	Node::onExit();
}

void Choice::setIsAnswer(const bool _isAnswer) {
	this->isAnswer = _isAnswer;
}

bool Choice::getIsAnswer() {
	return this->isAnswer;
}

void Choice::runRightAnimation() {
	//this->stopAllActions();
	this->runAction(this->choiceTimeline);
	this->choiceTimeline->play("right", false);
}

void Choice::runWrongAnimation() {
	//this->stopAllActions();
	this->runAction(this->choiceTimeline);
	this->choiceTimeline->play("wrong", false);
}

void Choice::makeCantTouchAnyMore(bool _bool) {
	if (_bool) {
		this->cantTouchAnyMore = true;
	}
	else {
		this->cantTouchAnyMore = false;
	}
}

float Choice::getMainScaleX() {
	return this->getChildByName<cocos2d::Sprite*>("choice")->getScaleX();
}

float Choice::getMainScaleY() {
	return this->getChildByName<cocos2d::Sprite*>("choice")->getScaleY();
}

float Choice::getRealWidth() {
	return this->getChildByName<cocos2d::Sprite*>("choice")->getContentSize().width * this->getMainScaleX();
}

float Choice::getRealHeight() {
	return this->getChildByName<cocos2d::Sprite*>("choice")->getContentSize().height;
}

cocos2d::Size Choice::getOriginalPosition() {
	return this->originalPosition;
}

void Choice::setOriginalPosition(cocos2d::Size size) {
	this->originalPosition = size;	
}

bool Choice::havePoint(cocos2d::Point point) {
	return this->originalPosition.width <= point.x && point.x <= this->originalPosition.width + this->getRealWidth()
		&& this->originalPosition.height <= point.y && point.y <= this->originalPosition.height + this->getRealHeight();
}

cocos2d::Size Choice::getOriginalLocalPosition() {
	return this->originalLocalPosition;
}

void Choice::setOriginalLocalPosition(cocos2d::Size size) {
	this->originalLocalPosition = size;
}

void Choice::makeUntouchable() {
	this->touchable = false;
}

void Choice::makeTouchable() {
	this->touchable = true;
}

cocos2d::ui::Widget::ccWidgetTouchCallback Choice::onLocateTouchCallback(const std::string &callBackName) {
	if (callBackName == "onClick") {
		//CCLOG("dz");+++++++++++++++++++++++++++++++++++++++++
		return CC_CALLBACK_2(Choice::choiceTouched, this);
	}
	return nullptr;
}

void Choice::choiceTouched(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type) {

	if (!this->touchable) {
		return;
	}
	if (this->cantTouchAnyMore) {
		return;
	}

	CC_UNUSED_PARAM(object);
	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		
		if (this->canBeClicked) {

			CCLOG("i was touched!!!!");

			this->isBeingMoved = true;

			this->setLocalZOrder(10000);
			
			this->Delegate->clickedd(this);

			this->canBeClicked = false;

		}

	}
	else if (type == cocos2d::ui::Widget::TouchEventType::MOVED) {
		
		this->setPosition(this->getChildByName<cocos2d::ui::ImageView*>("choice")->getTouchMovePosition()
			- cocos2d::Vec2(this->originalX, this->originalY) - this->getHalfSize()
		);
		//this->Delegate->inRectt(this);

	}
	else if (type == cocos2d::ui::Widget::TouchEventType::ENDED || type == cocos2d::ui::Widget::TouchEventType::CANCELED) {

		this->makeUntouchable();

		this->Delegate->cancled(this);

		this->isBeingMoved = false;
		this->setZOrder(10);
	}
}

void Choice::setOriginal(const float &_x, const float&_y) {
	this->originalX = _x;
	this->originalY = _y;
}

cocos2d::Vec2 Choice::getHalfSize() {
	return cocos2d::Vec2(
			0.3f * this->getChildByName<cocos2d::ui::ImageView*>("choice")->getContentSize().width / 2.0f,
			0.28f * this->getChildByName<cocos2d::ui::ImageView*>("choice")->getContentSize().height / 2.0f
		);
}

void Choice::setupDelegate(MyClassDelegate* _Delegate) {
	this->Delegate = _Delegate;
}

bool Choice::getIsBeingMoved() {
	return this->isBeingMoved;
}

