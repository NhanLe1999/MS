#pragma once

#include "cocostudiomacro2.h"

class EndTrialLessonPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(EndTrialLessonPopup);

    static EndTrialLessonPopup* createView();//1.

    void show(cocos2d::Node* scene = nullptr, int z_order = INT_MAX - 1);
private:
    void didLoadFromCSB();//2.

    void onLanguageSelected(cocos2d::Ref* sender);

    void onClickLeftButton(cocos2d::Ref* sender);

    void onClickRightButton(cocos2d::Ref* sender);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.


    void registerEventTrackingClickForButton(std::string type);

private:
};

CREATE_READER(EndTrialLessonPopup);

