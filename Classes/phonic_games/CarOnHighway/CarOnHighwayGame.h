#pragma once
#include "MJMainGBase.h"
#include "GameData.h"
#include "StoryConfigManager.h"
struct CarOnHighwayQuestion {
	std::vector<mj::GameObject> answers;
	mj::GameObject correctAnswer;
};


class CarOnHighwayGame : public MJMainGBase {
public:
	static CarOnHighwayGame * createGame(std::string json_file, bool isAutoPlay);
	CREATE_FUNC(CarOnHighwayGame);
	virtual bool init() override;
protected:
	CarOnHighwayGame();
	void initBackground();
	void initCar();
	void createSideObject();
	void getGameData();
	void startGame();
	void playStartingScript();
	void playEndingScript();
	void showQuestion(std::vector<mj::GameObject> answers, mj::GameObject correctAnswer);
	void showNextQuestion();
	void onCorrectAnswer();
	void onWrongAnswer();
	void removeAllAnswerBtns();
	virtual void onEnter() override;
	virtual void onExit() override;
	void onFinishGame() ;
	void onExitTransitionDidStart() override;
	void showTapGuideGame(Vec2 pos, int loop = 100, float delay_time = 8.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void cacheGAF();
private:              
	std::string m_configPath;
	cocos2d::Node* m_bgNode;
	cocos2d::Node* m_sideObjectNode;
	cocos2d::Node* m_gameNode;
	GAFWrapper* m_car;
	std::vector<CarOnHighwayQuestion> m_questions;
	std::vector<cocos2d::ui::Button*> m_answerBtns;
	mj::GameObject m_correctAnswer;
	int m_laneIndex;
	bool m_clickable;
    Vec2 right_pos;
    std::string hoc_am_hoc_van;
	int count_lv = 0;
	void stopGuiding();
	int _audio = -1;
	Button* _speaker;
};
