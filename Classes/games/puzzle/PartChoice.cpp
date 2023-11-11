#include "PartChoice.h"

#define sqr(x) (x) * (x)

INITIALIZE_READER(PartChoice);

using namespace cocos2d;

PartChoice* PartChoice::createView() {
	REGISTER_CSB_READER(PartChoice);
	auto view = reinterpret_cast<PartChoice*>(CSLoader::createNode("csb/game/puzzle/PartChoice.csb"));
	if (view) {
		return view;
	}
	return nullptr;
}

cocos2d::ui::Widget::ccWidgetTouchCallback PartChoice::onLocateTouchCallback(const std::string &callBackName) {
	if (callBackName == "onTouch") {
		return CC_CALLBACK_2(PartChoice::choiceTouched, this);
	}
	return nullptr;
}

void PartChoice::choiceTouched(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type) {

	if (!this->touchable) {
		return;
	}

	if (!this->canStillBeTouch) {
		return;
	}

	CC_UNUSED_PARAM(object);

	if (type == cocos2d::ui::Widget::TouchEventType::BEGAN) {
		this->partChoiceDelegate->touched(this);
		this->setLocalZOrder(4000);
		++this->cntTouched;
	}
	else if (type == cocos2d::ui::Widget::TouchEventType::MOVED) {
		if (this->cntTouched != 0) {
			this->setPosition(this->getChildByName<ui::Layout*>("son")->getTouchMovePosition() - this->getParentPosition());
		}
	}
	else if (type == cocos2d::ui::Widget::TouchEventType::ENDED || type == cocos2d::ui::Widget::TouchEventType::CANCELED) {
		if (this->cntTouched != 0) {
			this->partChoiceDelegate->released(this);
			this->cntTouched = 0;
		}
	}
}

void PartChoice::initPuzzleDelegate(PartChoiceDelegate* root) {
	this->partChoiceDelegate = root;
}

void PartChoice::makeTouchable() {
	this->touchable = true;
	this->cntTouched = 0;
}

void PartChoice::makeUntouchable() {
	this->touchable = false;
}

void PartChoice::initImage(cocos2d::Size _size, std::string file_name) {
	this->makeUntouchable();
	this->canStillBeTouch = true;
	this->originalSize = _size;
	auto son = this->getChildByName<ui::Layout*>("son");
	ui::ImageView* image = ui::ImageView::create(file_name);
	this->img = image;
	this->setContentSize(_size);
	son->setContentSize(_size);

	this->mainScaleX = _size.width / image->getContentSize().width;
	this->mainScaleY = _size.height / image->getContentSize().height;
	image->setScaleX(this->mainScaleX);
	image->setScaleY(this->mainScaleY);

	//--------------------------------------
	image->setAnchorPoint(Vec2(0, 0));
	image->setPosition(Vec2(0, 0));
	this->addChild(image);
	this->originalSize = _size;
}

cocos2d::Vec2 PartChoice::getOriginalPosition() {
	return this->originalPosition;
}

void PartChoice::setOriginalPosition(cocos2d::Vec2 _position) {
	this->originalPosition = _position;
}

void PartChoice::setParentPosition(cocos2d::Vec2 _position) {
	this->parentPosition = _position;
}

cocos2d::Vec2 PartChoice::getParentPosition() {
	return this->parentPosition;
}

bool PartChoice::inItsRect() {
	Vec2 pos = this->getPosition();
	Vec2 posRect = this->getOriginalPosition();
	return abs(pos.x - posRect.x) < this->originalSize.width * 0.5
		&& abs(pos.y - posRect.y) < this->originalSize.height * 0.5;
}

void PartChoice::setNewSize(cocos2d::Size _size) {
	this->newSize = _size;
}

cocos2d::Size PartChoice::getNewSize() {
	return this->newSize;
}

void PartChoice::makeSize(cocos2d::Size _size) {
	this->setContentSize(_size);
	auto son = this->getChildByName<ui::Layout*>("son");
	this->img->setContentSize(_size);
	son->setContentSize(_size);
}

cocos2d::Size PartChoice::getOriginalSize() {
	return this->originalSize;
}

cocos2d::Vec2 PartChoice::getNewPosition() {
	return this->newPosition;
}

void PartChoice::setNewPosition(cocos2d::Vec2 _position) {
	this->newPosition = _position;
}

void PartChoice::makeCannotBeTouchedAnymore() {
	this->canStillBeTouch = false;
}