//
//  GameBirdieAnswerOverlay.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/5/17.
//

#ifndef GameBirdieAnswerOverlay_hpp
#define GameBirdieAnswerOverlay_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameBirdieAnswer;

class MJGameBirdieAnswerDelegate {
public:
    virtual void onAnswerDropped(MJGameBirdieAnswer * answer, cocos2d::Point position) = 0;
    virtual void onAnswerTouchBegan(){};
    virtual void onAnswerTouchEnded(){};
};

class MJGameBirdieAnswerOverlay : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJGameBirdieAnswerOverlay);
    static MJGameBirdieAnswerOverlay * createOverlay(mj::BirdieDataSet data);
    
    cocos2d::Point getAnswerPosition(std::string word);
private:
    void didLoadFromCSB(mj::BirdieDataSet data);

    mj::BirdieDataSet _data;
    
    std::vector<MJGameBirdieAnswer *> _answers;

    void onAnswerTouch(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
    CC_SYNTHESIZE(MJGameBirdieAnswerDelegate *, _delegate, Delegate);
};

CREATE_READER(MJGameBirdieAnswerOverlay);

#endif /* GameBirdieAnswerOverlay_hpp */
