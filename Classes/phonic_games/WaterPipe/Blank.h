#pragma once
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"


class Blank : public cocos2d::Node {
public:
	CREATE_FUNC(Blank);

	static Blank* createView();

	float getRealWidth();
	float getRealHeight();
	void runWrongAnimation();
	void setRealPosition(cocos2d::Vec2 point);
	cocos2d::Vec2 getRealPosition();
	cocos2d::Vec2 getRealPositionn();
	bool inBlank(cocos2d::Point point);

	bool getRed();
	void setRed(bool red);

	std::string getItsAnswer();
	void setItsAnswer(std::string str);

protected:
	cocostudio::timeline::ActionTimeline* blankTimeline;

	float getMainScaleX();
	float getMainScaleY();

	cocos2d::Vec2 pos;

	bool reded;

	std::string answer;
};

CREATE_READER(Blank);

