#pragma once

#include "cocostudiomacro2.h"

class ProfileMiniScene : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(ProfileMiniScene);

    static ProfileMiniScene* createView();//1.

private:
    void didLoadFromCSB();//2.

    void gotoNextScreen(float dt = 0);

    void onTouchEnded();

    void onTouchBegan();

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;//3.

private:
    bool _isTouchActive = false;
    float _countTime = 0;
    float _loopTime = 0.001;
    std::string _countdownScheduleName = "Count Down Schedule";
};

CREATE_READER(ProfileMiniScene);

