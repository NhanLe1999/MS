//
//  FireFighter.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/6/18.
//

#ifndef FireFighter_hpp
#define FireFighter_hpp

#include <stdio.h>
#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "JsonDump.h"
#include "GameData.h"

USING_NS_CC;
using namespace ui;

class FireFighter : public MJMainGBase {
public:
    CREATE_FUNC(FireFighter);
    static FireFighter* createGame(std::string json, bool is_Autoplay);
    
private:
    void onExit() override;
    void didLoadFromCSB(std::string json, bool is_Autoplay);
    void loadFrame();
    void onGameStart();
    void onEnter() override;
    void onFinishGame() override;
    void generateData(rapidjson::Value& json_data, std::vector<math::resource::Image*> images);
    void onExitTransitionDidStart() override;
    
    void onTouched(Ref* sender, ui::Widget::TouchEventType type);
    void learningReminder();
    float calculateAngle(cocos2d::Vec2 pos, Vec2 center);
    void showText(Node* fire_hold);
    void playFighter(float goc);
    void playWater(Vec2 pos);
    void updateFire(float time);
    void waterOff();
    void fireOff();
	void onTransitionGame();

    Layout* _rootLayout = nullptr;
    Layout* _touchLayout = nullptr;
    GAFWrapper* _truck = nullptr;
    GAFWrapper* _waterSpray = nullptr;
    Layout* _water = nullptr;
    //Layout* _waterJet;
    Layout* _ChumNuoc = nullptr;
    Layout* _fireFighter = nullptr;
    ImageView* _fireHouse = nullptr;
    GAFWrapper* _fighter = nullptr;
    
    std::vector<GAFWrapper*> _fireList;
    
    std::string _learningReminderScheduleName = "learning reminder";
    std::string _introSoundPath = CONFIG_MANAGER->isGameVMonkey() ? "sounds/firefighter/intro_vn.mp3" : "sounds/firefighter/intro_new.mp3";
    int _introLearningReminderSoundId = -1;
    int _introSpeakerSoundId = -1;

    float _edgeX = -1;
    bool _touchable = false;
    int _fireId = -1, _waterId = -1;
    bool _waterActive = false;
    int _maxCount = 11; //configurable
    int _count = 0;
    mj::GameObject _word;
	std::vector<mj::GameObject> _gameDataSet;
	int _gameIndex = -1;
    
    Size _screenSize;
    bool _isStartGame = true;
};

CREATE_READER(FireFighter);

#endif /* FireFighter_hpp */
