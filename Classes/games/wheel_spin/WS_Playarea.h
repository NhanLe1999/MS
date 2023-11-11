#ifndef WS_Playarea_h
#define WS_Playarea_h
//#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include <stdio.h>
#include "cocos2d.h"
#include "MJBaseActivity.h"
#include "ui/UIScale9Sprite.h"

#define ARC4RANDOM_MAX 0x100000000
#define screenSize cocos2d::Director::getInstance()->getWinSize()
#define ScreenWidth screenSize.width
#define ScreenHeight screenSize.height

#define IPAD_WIDTH (float)(screenSize.width)
#define IPAD_HEIGHT (float)(screenSize.height)

#define POSX 1
#define POSY 1

#define SCALE_X 1
#define SCALE_Y 1

#define SCALE_XY 1

#define RAND_FLOAT_BT(MIN,MAX) ((float)arc4random() / ARC4RANDOM_MAX) * (MAX  - MIN) + MIN

#define RAND_INT_BT(MIN,MAX) (int)(arc4random()%(MAX - MIN) + MIN)

#define outOfScreen Vec2(RAND_INT_BT(9000,10000),RAND_INT_BT(9000,10000))

#define CCLOG_SIZE(size) CCLOG("Width = %2.0f || Height = %2.0f",size.width,size.height)

#define CCLOG_POINT(point) CCLOG("X = %2.0f || Y = %2.0f",point.x,point.y)

#define AUDIO_LETS_SPIN "sounds/wheel_spin/letspin.mp3"

#include "ui/CocosGUI.h"
using namespace cocos2d;

class WS_Playarea :public ms::BaseActivity
{
public:
	~WS_Playarea();
    Point location;
    void onExit() override;
	EventListenerTouchOneByOne* listener = nullptr;
	bool onTouchesBegan(Touch* touch, Event* event);
	void onTouchesMoved(Touch* touch, Event* event);
	void onTouchesEnded(Touch* touch, Event* event);

    Sprite *background = nullptr,*move_tool = nullptr;
    void playSound(std::string str);
    void changeImage(Sprite *sp,std::string str);
    void spinRemind();
    void answerRemind(float delay);
    void startGame();
    void endGame();

    void runActionSuggetGame();

	std::string getImage(std::string str);
    
    bool _isSpeaking;
    int NoOfPart;
    
    //Start Screen
    void setupStartScreen();
    Sprite *_sp_No = nullptr,*_sp_start = nullptr;
    
	int currentRotation = 0;
    
    //Read Data From Plist
    __Dictionary *dir_objects_detail,*dir_objects[10];
    
    //Wheel
    void setupWheel();
    void startWheel();
    void enableSpine();
    int Object_No[10],Object_counter,currentScene,rotationValue;
    Point pos_object[10],pos_object_button[10];
	Sprite *_sp_stand = nullptr, *_sp_wheel = nullptr, *_sp_pin = nullptr, *_sp_spine = nullptr, *_sp_spine1 = nullptr;
	ImageView* _sp_object[10];
	cocos2d::ui::Scale9Sprite*_sp_object_button[10];
    LayerColor *lyl_wheel = nullptr;
    Label *Object_Name[10],*Object_Name_Button[10];
    __String Object_Name_str[10];
    Button* _speaker;
    //Scene 2
    void setupScene2();
    void startObjectButton(int SelectedObject);
    void enableObjectButton();
    Sprite *_sp_star[10];
    int Object_No_scene2[10],Object_No_scene2_temp[10],Object_counter_scene2;
    int NoOfWrongAnswer;
    
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
    void enableKeypad();
    void radio();
    
    bool touch_tool;
    void first_effect();
    void touch_true();
    void home_Effect();
    int add_y = 40, origin_wheel = 0;
//    virtual bool init();
	static WS_Playarea *createGame(std::vector<math::resource::Image*> images);
	void initOptions(std::vector<math::resource::Image*> images);
	void loadData(std::vector<math::resource::Image*> images);
    CREATE_FUNC(WS_Playarea);

private:
    struct WheelSpinSet {
        std::string text;
        std::string audio_path;
        std::string img_path;
        
    };
    std::vector<WheelSpinSet> list_object;

    bool _isSuggetGame = true;

    void onEnter() override;
    //bool game_play_enable = true;
    //void pauseGame();
};

#endif /* WS_Playarea_hpp */
