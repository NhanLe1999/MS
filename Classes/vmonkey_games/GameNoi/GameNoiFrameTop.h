//
//  GameNoiFrame.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//	Cloned by MinhTzy on 29/05/2019

#ifndef GameNoiFrame_hpp
#define GameNoiFrame_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class GameNoiFrameTop;
class GameNoiFrameBottom;

class GameNoiPlugDelegate {
public:
    virtual void onPlugBeginning(GameNoiFrameTop * frame) = 0;
    virtual void onPlugMoving(GameNoiFrameTop * frame, cocos2d::Point position) = 0;
    virtual void onPlugMoved(GameNoiFrameTop * frame, cocos2d::Point position) = 0;
};

class GameNoiFrameTop : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameNoiFrameTop);
    static GameNoiFrameTop * createFrame(mj::GameObject object, bool is_image);

    std::string getName();
    void movePlugBack();
    void turnBulbOn();
    void connectToBottom(GameNoiFrameBottom * bottom);

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onBgFrame(cocos2d::Ref *sender);
    void didLoadFromCSB(mj::GameObject object, bool is_image);

    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callback_name) override;

    void onTouchPlug(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
    void movePlug(cocos2d::Point position);

    CC_SYNTHESIZE(GameNoiPlugDelegate *, _delegate, Delegate);

    mj::GameObject _object;

    cocos2d::DrawNode * _rope = nullptr;
    cocos2d::Node * end1 = nullptr;
    cocos2d::Node * end2 = nullptr;
    cocos2d::Node * control_point = nullptr;
    int bg_id = -1;
    void updateRope(float dt);
};

CREATE_READER(GameNoiFrameTop);

#endif /* GameNoiFrame_hpp */
