//
//  Chicken.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#ifndef Chicken_hpp
#define Chicken_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
class GAFWrapper;
class MJChicken;

class MJChickenDelegate {
public:
    virtual void onChickenDraggedTo(MJChicken * chicken, cocos2d::Point position) = 0;
    virtual void onChickenTouchedTo() = 0;
};

class MJChicken : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJChicken);
    static MJChicken * createChicken(mj::GameObject word);
    void flipChicken(bool flipped);
    void run();
    void drop(std::function<void()> callback = nullptr);
    inline std::string getWord() { return _word.text; }
    inline std::string getAudio() { return _word.audio_path; }
    inline float getOriginPositionY() { return _origin.y; }
    void onTouchChicken(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);
    bool isPlayAudioChicken() { return _isPlayAudioChicken; };

private:
    void didLoadFromCSB(mj::GameObject word);

    mj::GameObject _word;
    GAFWrapper * _chicken_gaf = nullptr;

    bool _isPlayAudioChicken = true;

    CC_SYNTHESIZE(MJChickenDelegate *, _delegate, Delegate);
    cocos2d::Point _origin;
};

CREATE_READER(MJChicken);

#endif /* Chicken_hpp */
