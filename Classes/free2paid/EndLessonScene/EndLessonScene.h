#pragma once

#include "cocostudiomacro2.h"
#include"F2PCommon.h"
class EndLessonScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(EndLessonScene);

    static cocos2d::Scene* createScene(ms::f2p::FreeContentOption freeContentOption = ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);
    
    static EndLessonScene* createView(ms::f2p::FreeContentOption freeContentOption = ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY);//1.

    void showScreenOneImage(std::string imageLessonPath = "");

    void showScreenTwoImage(std::string imageLessonPath_1 = "", std::string imageLessonPath_2 = "");

    void setUnlockAllCallback(std::function<void()> callback) { _onUnlockCallback = callback; };
    void setOnComeBackLaterCallback(std::function<void()> callback) { _onComeBackLaterCallback = callback; };
private:
    void didLoadFromCSB(ms::f2p::FreeContentOption freeContentOption);//2.

    void onClickTopButton(cocos2d::Ref* sender);

    void onClickBottomButton(cocos2d::Ref* sender);
    ms::f2p::FreeContentOption getFreeContentOption();
  
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

private:
    std::function<void()> _onUnlockCallback = nullptr;
    std::function<void()> _onComeBackLaterCallback = nullptr;
    ms::f2p::FreeContentOption _freeContentOption = ms::f2p::FreeContentOption::MONO_ITEM_ONE_DAY_ONLY_STORY;
};

CREATE_READER(EndLessonScene);

