#ifndef GameBaseState_hpp
#define GameBaseState_hpp


#include "MJBaseActivity.h"



USING_NS_CC;
using namespace ui;

class BaseState {
public:
	enum GAME_BASE_STATE
	{
		GBS_NONE = -1,
		GBS_PLAYING,
		GBS_PLAY_VIDEO,
		GBS_PAUSE
	};
	
protected:
	//bool game_play_enable = true;
	GAME_BASE_STATE game_state = GAME_BASE_STATE::GBS_NONE;
//    void pauseGame(bool isPause = true){
//        if (isPause){
//            game_state = GAME_BASE_STATE::GBS_PAUSE;
//            cocos2d::MessageBox("pause", "pau roi nhe");
//        }
//    };
};

#endif /* GameBase_hpp */
