//
//  MJOwl.hpp
//  Azu
//
//  Created by To Ngoc Linh on 6/28/17.
//
//

#ifndef MJOwl_hpp
#define MJOwl_hpp

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "GameData.h"

class MJOwl;

class MJOwlDelegate {
public:
    virtual void onOwlFliedToStand(MJOwl * sender) = 0;
    virtual void onOwlRead(MJOwl * sender, int index, bool with_sound) = 0;
    virtual void onOwlDroppedOnPosition(MJOwl * sender, cocos2d::Point position) = 0;
};

class MJOwl : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MJOwl);
    static MJOwl * createOwl(mj::GameObject word, int index, bool is_night);
    void flyIn(int index, int total);
    void read(bool with_sound);
    void readAfterDelay(mj::TextTiming timing);
    void moveOwlToSlot(int slot);
    void enableTouch();
    void flyBackToStand(bool is_notify = false);
    void flyBack();
	void setOwlSize(float rate);
    cocos2d::Point getOwlPosition();
    cocos2d::Point getOriginalPosition();

    void setTouchEnabled(bool is_enabled) override;
private:
    void didLoadFromCSB(mj::GameObject word, int index, bool is_night);

    void idle();
    void fly();
    void shake();
    void land(bool is_notify = false);
    void flyTo(cocos2d::Point position, bool notify_when_landed);

    void onTouch(Ref * sender, cocos2d::ui::Widget::TouchEventType event_type);

    CC_SYNTHESIZE(MJOwlDelegate *, _delegate, Delegate);
    GAFWrapper * _owl = nullptr;

    mj::GameObject _word;
    int _index;

    cocos2d::Point _origin;
    bool _enable_touch;

    void onExit() override;

    cocos2d::Color4B _text_color, _tint_text_color;

    inline  mj::GameObject getWordObject() {return _word;};

    inline std::string getWord() {return _word.text;};
};

CREATE_READER(MJOwl);

#endif /* MJOwl_hpp */
