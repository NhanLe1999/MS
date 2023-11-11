#ifndef PART_CHOICE_H
#define PART_CHOICE_H

#include "cocostudiomacro2.h"
#include <string>
#include <cmath>

class PartChoice;
class PartChoiceDelegate {
public:
	virtual void touched(PartChoice* partChoice) = 0;
	virtual void released(PartChoice* partChoice) = 0;
};

class PartChoice : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(PartChoice);
	static PartChoice* createView();
	void initImage(cocos2d::Size _size, std::string file_name);
	void initPuzzleDelegate(PartChoiceDelegate* root);

	void makeTouchable();
	void makeUntouchable();

	cocos2d::Vec2 getOriginalPosition();
	void setOriginalPosition(cocos2d::Vec2 _position);
	cocos2d::Vec2 getParentPosition();
	void setParentPosition(cocos2d::Vec2 _position);

	cocos2d::Size getOriginalSize();

	void setNewSize(cocos2d::Size _size);
	cocos2d::Size getNewSize();

	cocos2d::Vec2 getNewPosition();
	void setNewPosition(cocos2d::Vec2 _position);

	void makeSize(cocos2d::Size _size);

	bool inItsRect();

	void makeCannotBeTouchedAnymore();

	float newScale = 0;
	cocos2d::ui::ImageView* img;

private:
	virtual cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callBackName);
	void choiceTouched(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);

	PartChoiceDelegate* partChoiceDelegate;
	bool touchable;
	bool canStillBeTouch;

	cocos2d::Vec2 originalPosition;
	cocos2d::Vec2 parentPosition;
	cocos2d::Vec2 newPosition;
	cocos2d::Size originalSize;
	cocos2d::Size newSize;


	float mainScaleX;
	float mainScaleY;

	int cntTouched;

};

CREATE_READER(PartChoice);

#endif