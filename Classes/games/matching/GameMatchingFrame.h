//
//  GameMatchingFrame.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#ifndef GameMatchingFrame_hpp
#define GameMatchingFrame_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameMatchingAnswer;
class MJGameMatching;
class MJGameMatchingFrame : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJGameMatchingFrame);
    static MJGameMatchingFrame* createFrame(mj::GameObject object, bool is_undersea);
    std::string getObjectName();
    void addRightAnswer(MJGameMatchingAnswer* answer, cocos2d::Point position);
    void showWrongAnswer();
    void setCallbackName(std::function<void()> _callback);
    void shake();
    void appearByAngle(float angle, float distance);
    CC_SYNTHESIZE(MJGameMatching*, _gameMatching2, GameMatching2);
    CC_SYNTHESIZE_READONLY(bool, _is_answered, Answered);
private:
    void didLoadFromCSB(mj::GameObject object);

    

    mj::GameObject _data;

    bool _is_undersea;
};

CREATE_READER(MJGameMatchingFrame);

#endif /* GameMatchingFrame_hpp */
