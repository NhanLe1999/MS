//
//  GameBirdieAnswer.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/5/17.
//

#ifndef GameBirdieAnswer_hpp
#define GameBirdieAnswer_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"

class MJGameBirdieAnswer : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJGameBirdieAnswer);
    static MJGameBirdieAnswer * createAnswer(mj::GameObject data);
    void moveBackToOrigin();
    inline std::string getWord() { return _data.text; }
    mj::GameObject _data;
private:
    void didLoadFromCSB(mj::GameObject data);
    
    CC_SYNTHESIZE(cocos2d::Point, _origin, StartedTouchPosition);

    void onExit();
};

CREATE_READER(MJGameBirdieAnswer);

#endif /* GameBirdieAnswer_hpp */
