#pragma once
#include "cocos2d.h"
//#include "cocostudio/CocoStudio.h"
#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"

class Pipe: public cocos2d::Node {
public:
	CREATE_FUNC(Pipe);
	bool init() override;
	void showUp();
	void viberate();
protected:
	cocostudio::timeline::ActionTimeline* pipeTimeline;
	void onExit() override;
	
};

