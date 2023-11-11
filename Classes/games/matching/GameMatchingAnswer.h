//
//  GameMatchingAnswer.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#ifndef GameMatchingAnswer_hpp
#define GameMatchingAnswer_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameMatchingAnswer;

class MJGameMatching;

class MJGameMatchingAnswerDelegate {
public:
    virtual void onAnswerMoved(MJGameMatchingAnswer* answer, cocos2d::Point position) = 0;
    virtual void onAnswerMoving(MJGameMatchingAnswer* answer, cocos2d::Point position) {};
    virtual void onAnswerBegan(MJGameMatchingAnswer* answer, cocos2d::Point position) {};
};

class MJGameMatchingAnswer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameMatchingAnswer);
    static MJGameMatchingAnswer* createAnswer(mj::GameObject object, bool is_undersea);
    static MJGameMatchingAnswer* clone(MJGameMatchingAnswer* other);

    void moveToPosition(cocos2d::Point position);
    void moveBackToOrigin();
    std::string getAnswer();

private:
    void didLoadFromCSB(mj::GameObject object);
    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string& callback_name) override;

    void onTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event_type);
    void onTouchMovedToPosition(cocos2d::Point position);

    mj::GameObject _data;
    cocos2d::Point _original_point;

    bool _is_undersea;

    CC_SYNTHESIZE(MJGameMatching*, _gameMatching, GameMatching);

    CC_SYNTHESIZE(MJGameMatchingAnswerDelegate*, _delegate, Delegate);
};

CREATE_READER(MJGameMatchingAnswer);

#endif /* GameMatchingAnswer_hpp */
