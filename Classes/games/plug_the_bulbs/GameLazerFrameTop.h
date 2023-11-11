//
//  GameLazerFrame.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//

#ifndef GameLazerFrame_hpp
#define GameLazerFrame_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameLazerFrameTop;
class MJGameLazerFrameBottom;

class MJGameLazerPlugDelegate {
public:
    virtual void onPlugMoving(MJGameLazerFrameTop * frame, cocos2d::Point position) = 0;
    virtual void onPlugMoved(MJGameLazerFrameTop * frame, cocos2d::Point position) = 0;
};

class MJGameLazerFrameTop : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameLazerFrameTop);
    static MJGameLazerFrameTop * createFrame(mj::GameObject object, bool is_image, bool isTextLength = false);

    std::string getName();
    void movePlugBack();
    void turnBulbOn();
    void connectToBottom(MJGameLazerFrameBottom * bottom);

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onBgFrame(cocos2d::Ref *sender);
    void didLoadFromCSB(mj::GameObject object, bool is_image, bool isTextLength = false);

    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callback_name) override;

    void onTouchPlug(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
    void movePlug(cocos2d::Point position);

    void resetSuggetionGame();
    void removeSuggetionGame();
    void playAudioObject(std::string path);

    CC_SYNTHESIZE(MJGameLazerPlugDelegate *, _delegate, Delegate);

    mj::GameObject _object;

    cocos2d::DrawNode * _rope = nullptr;
    cocos2d::Node * end1 = nullptr;
    cocos2d::Node * end2 = nullptr;
    cocos2d::Node * control_point = nullptr;
    bool _isPlayAudio = true;

    void updateRope(float dt);
};

CREATE_READER(MJGameLazerFrameTop);

#endif /* GameLazerFrame_hpp */
