#pragma once

#include "cocostudiomacro2.h"

class MSLanguageSelection : public cocos2d::ui::Layout,public cocostudio::WidgetCallBackHandlerProtocol 
{
public:

    CREATE_FUNC(MSLanguageSelection);

    static cocos2d::Scene*   createScene();
    
    static MSLanguageSelection * createView();
        
private:
    
    void didLoadFromCSB();
    
	void onLanguageSelected(cocos2d::Ref * sender);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    
private:
	
    bool m_isItemSelected = false;

    CC_SYNTHESIZE(std::function<void()>, m_callback, OnLoginCallback);
    
    int _timeStartScreen;
};

CREATE_READER(MSLanguageSelection);

