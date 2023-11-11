
#ifndef WordTv_Tv_HPP
#define WordTv_Tv_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#include "ui/UIVideoPlayer.h"
using namespace cocos2d::experimental::ui;
#endif
USING_NS_CC;
using namespace ui;



class WordTv_Tv : public cocos2d::ui::Layout {
public:
    
    enum WORD_TV_STATE{
        LOCK,
        ACTIVE,
        ACTION,
        IDLE,
        WRONG,
        CORRECT
    };
    CREATE_FUNC(WordTv_Tv);
    static WordTv_Tv * createItem(mj::PHONIC_WORD data);
   
    void setCallBackW(const std::function<void()> afunc);
    void setCallBackC(const std::function<void()> afunc);
    void playTv(WORD_TV_STATE state, const std::function<void()> afunc = nullptr);
    WORD_TV_STATE _state;
    Text* text;
    int color;//0 red, 1 green , 2 yellow, 3 blue,
    mj::PHONIC_WORD _data;
    
    void voice();
    void playSoundVoice(bool* mybool);
private:
    void onEnter() override;
	void didLoadFromCSB(mj::PHONIC_WORD data);
    
    std::string getColorChar();
    
    cocos2d::Size screen_size;
    GAFWrapper *tv;
    std::function<void()> callBackWrong=nullptr;
    std::function<void()> callBackCorrect=nullptr;
};

CREATE_READER(WordTv_Tv);

#endif
