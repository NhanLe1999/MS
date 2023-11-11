
#ifndef WordTv_Led_HPP
#define WordTv_Led_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#include "ui/UIVideoPlayer.h"
using namespace cocos2d::experimental::ui;
#endif
USING_NS_CC;
using namespace ui;



class WordTv_Led : public cocos2d::ui::Layout {
public:
    enum LED_STATE{
        IDLE,
        ACTION,
        WRONG,
        BLINK
    };
    CREATE_FUNC(WordTv_Led);
    static WordTv_Led * createItem();
   
    void playTv(LED_STATE state);
    LED_STATE _state;
private:
    void onEnter() override;
	void didLoadFromCSB();
    
    std::string getColorChar(int index);
    int color;//0 yellow, 1 red, 2 green,
    cocos2d::Size screen_size;
    std::vector<GAFWrapper*> leds;
};

CREATE_READER(WordTv_Led);

#endif
