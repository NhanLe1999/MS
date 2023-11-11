#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudiomacro2.h"

class Choice;

class MyClassDelegate
{
public:
	virtual void clickedd(Choice* movingChoice) = 0;
	virtual void cancled(Choice* moveingChoice) = 0;
	//virtual Blank* inRectt(Choice* movingChoice) = 0;

};

class Choice : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(Choice);
	bool init() override;

	float getRealWidth();
	float getRealHeight();

	cocos2d::Size getOriginalPosition();
	cocos2d::Size getOriginalLocalPosition();
	void setOriginalPosition(cocos2d::Size size);
	void setOriginalLocalPosition(cocos2d::Size size);

	bool havePoint(cocos2d::Point point);
	void runRightAnimation();
	void runWrongAnimation();
	void setIsAnswer(const bool _isAnswer);
	bool getIsAnswer();

	void setOriginal(const float &_x, const float&_y);

	cocos2d::Vec2 getHalfSize();

	void setupDelegate(MyClassDelegate* _Delegate);

	bool getIsBeingMoved();

	void makeUntouchable();
	void makeTouchable ();

	bool canBeClicked;

	std::string getText();
	void setText(std::string str);

	void makeCantTouchAnyMore(bool _bool);

	bool canStillBeTouched();

	float originalScale = 2.0f;

protected:
	cocos2d::Size originalPosition;
	cocos2d::Size originalLocalPosition;
	virtual cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callBackName);
	void choiceTouched(cocos2d::Ref* object, cocos2d::ui::Widget::TouchEventType type);

	float originalX;
	float originalY;

	bool touchable;
	bool isAnswer;
	bool isBeingMoved;

	float getMainScaleX();
	float getMainScaleY();	

	cocostudio::timeline::ActionTimeline* choiceTimeline;
	void onExit() override;

	MyClassDelegate* Delegate = nullptr;

	std::string text;

	bool cantTouchAnyMore = false;
};

