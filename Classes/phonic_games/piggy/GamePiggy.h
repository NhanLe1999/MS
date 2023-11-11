//
//  GamePiggy.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#ifndef GamePiggy_hpp
#define GamePiggy_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GAFWrapper.h"
//#include "../external/ GAF.h"
#include "Letter.h"
#include "Math_Libs.h"
//#include "MJBaseActivity.h"
#include "MJMainGBase.h"
#include "StoryConfigManager.h"
using namespace cocos2d;

class GamePiggy : public MJMainGBase {
public:
    enum STATE {
        kIdle,
        kRunLeft,
        kRunRight,
        kWin
    };
    
public:
    GamePiggy();
    virtual ~GamePiggy();
    CREATE_FUNC(GamePiggy);
    static GamePiggy * createGame(std::string json_file, bool isAutoPlay);
    
    virtual bool init() override;
	void onEnter() override;
    void onExit() override;

    bool onTouchesBegan(Touch* touch, Event *event);
    void onTouchEnded(Touch * touch, Event *event);
    void onTouchMove(Touch * touch, Event *event);
    
    void animateMoveLeft(const float & posX, float time = -1);
    void animateMoveRight(const float & posX, float time = -1);
    void animateMoveToCenter(std::function<void()> callback = nullptr);
    void animateMoveOut();
    void animateIdle();
    void animateWin();
    void registerTouchListener();
    void endGane();
    void checkGameEnd();
    
    void gameUpdate(float dt);
    void addFallingLetter(float dt);
    void createActionSoundMove();
    bool isBasketIntersectLetter(Letter * letter);
    void playAudioGetTheSound(float dt);
    void hideAllLetters();
    void redAudioLetter();
    void setPositionLetter();
    void checkPositionTextAndPig(float postx);

    //// < New methods for Playing with images
    bool isBasketIntersectSprite(Sprite* sprite);
    void addFallingSprite(float dt);
    void addFallingRandom(float dt);
    void hideAllSprites();
    void runActionPiggySuggetGame();
    void handSuggetionGame();
    void addSpeaker();
    void getDataConfig();
    void loadGame();
    void runActionNode(cocos2d::Node * node);

    void runActionLetter();
    void resetSizeLetter(Letter* _lbExpectedLetter);
    cocos2d::Vec2 checkPositionPig(cocos2d::Vec2 positionPig);

    //// >

    void onClickBack();
    //CC_SYNTHESIZE(ms::delegate::Activity *, _activitydelegate, ActivityDelegate);
private:
    Node * _arrows;
    bool _isTouchOnPig;
    int _chanceOfCreateWrongLetter;
    float _tickCountUserNotPlay;
    int _level;
    bool _isPause;

    //// < New attributes for Playing with images
    std::vector<std::string> _correctImagesPaths;
    std::vector<std::string> _wrongImagesPaths;
    std::string _lbExpectedImage;
    cocos2d::Vector<Sprite*> _vSprite;
    std::string _typePlay = "text";
    //// >

    int _zOrder;
    int _comboCollectCorrect;
    int _numCollectCorrect;
    int _numCollectWrong;
	int m_letter_index = 0;
    int _countHideSuggetGame = 0;
    Action * _playSoundMove;
    cocos2d::Vector<Letter*> _vLabel;
    Sprite * _basket;
    Vec2 _posLeft;
    Vec2 _posRight;
    STATE _state;
    Vec2 _center;
    Sprite* _imgSprite;
    Letter* _lbExpectedLetter = nullptr;
    gaf::GAFObject * _pig;
    gaf::GAFObject * _goodJobSprite;
    EventListenerTouchOneByOne * _listener;
    std::vector<std::string> _lettersCorrect;
    std::vector<std::string> _lettersWrong;
    std::string hoc_am_hoc_van = "";
	std::map<std::string, std::string> _audioLetters;
    cocos2d::ui::Layout* back_layout;
	bool is_word; // phan biet choi word hay sound
    std::string font_name;
    bool _isSuggetionGame = true;
    std::string _audioIntro = "";
    float _positonXLetter = 0.0;
    Vec2 _PositionLetter = Vec2(0, 0);
    std::string _textLearn = "";
    bool _isRunSuggetionGame = false;
    int _index = 0;
    cocos2d::Vec2 _sizeScreen = cocos2d::Vec2(0, 0);
    bool _isPlaySound = true;
};

#endif /* GamePiggy_hpp */
