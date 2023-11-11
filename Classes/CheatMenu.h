#pragma once

#include "cocostudiomacro2.h"

using namespace cocos2d;

class CheatMenu : public ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	CREATE_FUNC(CheatMenu);

	static CheatMenu * createView();
    
    static void show(Node* i_parent = nullptr);


private:

	bool didLoadFromCSB();

	void onCancel(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

    void onReset(std::string enviroment);
	
	static bool _iShow;

};

CREATE_READER(CheatMenu);
