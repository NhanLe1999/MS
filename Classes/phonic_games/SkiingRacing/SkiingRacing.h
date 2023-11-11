//
//  SkiingRacing.hpp
//  mj_gamelib-mobile
//
//  Created by AnhB Nguyen on 3/20/19.
//

#ifndef SkiingRacing_hpp
#define SkiingRacing_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "JsonDump.h"
#include "GameData.h"
#include "SkiingRacing_Race.h"
#include "SkiingRacing_Map.h"

USING_NS_CC;
using namespace ui;


class SkiingRacing: public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol {
    
public:
    CREATE_FUNC(SkiingRacing);
    static SkiingRacing* createGame(std::string json_file, bool is_Autoplay = false);
private:
    void didLoadFromCSB(std::string json, bool is_Autoplay);
    void loadFrame();
    void startGame();
    void update_character_other(float dt);
	void update_character_other_speed(float dt = 4.f);
	void update_main_character_jump(float dt);
//    void onEnter() override;
//    void onFinishGame() ;
    void generateData(std::string json_file);
    void showAudioCorrect();
    void showQuestion();
    void createObstacle();
    void onSelectCharacter(Ref* sender);
	void onEndGame();
	void removeAllAnswerBtns();
	void onCorrectAnswer();
    void showIntro();
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    Layout* root_layout;
    Layout* select_layout;
    Layout* _bg;
	Layout* hidden_layout;
    Button* _speaker;
	int current_question;
	SkiingRacing_Map* map_race;
	std::vector<SkiingRacing_Race*>races;
	SkiingRacing_Race* race_finish;
    std::vector<Layout*> bg_layouts;
    GAFWrapper* main_character;
    std::vector<GAFWrapper*> characters;
	bool look_select_character = false;
    float move_speed = 500;
	float race_width = 3628;
	float long_races;
	int result;
    std::vector<SkiingRacingQuestion> m_questions = {};
    std::vector<ui::Button*> m_answerBtns;
	bool look_for_jump = false;
	bool is_visible_answer = false;
	void inActiveMode(float delay_time = 7);
	Vec2 pos_right;
	void showTapGuide(Vec2 position, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
//    bool m_clickable;
	std::vector<bool>kt_characters; // cai nay de ve dich nhanh
    std::string question_vi;
};
CREATE_READER(SkiingRacing);
#endif /* SkiingRacing_hpp */
