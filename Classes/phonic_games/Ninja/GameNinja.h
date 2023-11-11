//
//  GameNinja.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/3/18.
//

#ifndef GameNinja_hpp
#define GameNinja_hpp

#include "cocostudiomacro2.h"
#include "MJMainGBase.h"

class GAFWrapper;

class GameNinja : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameNinja);
    static GameNinja * createView(std::string config_path);
    
private:
    struct WordInfo {
        std::vector<std::string> characters;
        std::vector<std::string> noise;
        std::vector<std::string> voice;
        std::string word;
        std::string sound_normal;
        std::string sound_slow;
        std::string image;
    };
    
    std::vector<WordInfo> _data;
    WordInfo _current_round;
    int _current_turn;
	std::string config_path;
    void didLoadFromCSB(std::string config_path);
    void onEnter() override;
    void onExit() override;
    void generateData();
    
    void introGame();
    void startGame();
    void startTurn();
    void newRound();
	void inactiveMode(float dt);
    void onRightAnswer(cocos2d::Ref * sender);
    void onWrongAnswer(cocos2d::Ref * sender);
    
    void updateBackground(float dt);
    
    cocos2d::ui::Layout * _jungle;
    cocos2d::ui::Layout * _ground;
    cocos2d::ui::Layout * _word;
    std::vector<bool> _char_filled;
    std::vector<float> _space;
    std::vector<cocos2d::ui::Layout *> _underline;
    GAFWrapper * _gaf;
    
    int _audio_id_background;
    int _audio_id_run;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

    std::vector<ImageView*> _shurikens;

    std::vector<int> _noicesSize;

    int _countTurn = 0;

    void randAnwser(int dt);
};

CREATE_READER(GameNinja);

#endif /* GameNinja_hpp */
