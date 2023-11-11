#pragma once

#include "cocos2d.h"
#include "TickGroup.h"
#include "LetterCloud.h"
#include "MJMainGBase.h"
#include "StoryConfigManager.h"
namespace GymNS {
	enum GameState {
		INTRO,
		GUIDING,
		WAITING,
		PLAYING,
		ENDING,
		ENDED,
		NONE
	};
	enum MonkeyState {
		FADE_IN,
		FALL,
		WALK,
		JOG,
		RUN,
		FADE_OUT
	};
}

class GAFWrapper;

struct SingleGame {
	std::vector<std::pair<std::string, std::string>> answers;
	std::string correctAnswer;
	std::string audio;
};

class GymGame : public MJMainGBase, public CloudSelectListener
{
public:
	//static GymGame* createScene();
	static GymGame* createGame(std::string json_file, bool isAutoPlay);
	virtual ~GymGame();

	virtual bool init();
	void startGame();
	void onSelectLetter(std::string letter) override;
	void setGameState(GymNS::GameState state);
	void setMonkeyState(GymNS::MonkeyState state);
	void transitionMonkeyState(GymNS::MonkeyState lastState, GymNS::MonkeyState state);
	void updateRunAnimation();
	void parseGameData();
	bool nextQuestion();
	void setLetters(std::string answer[3], std::string correctAnswer);
	void setCloudsTexture(int textureColor);
	void InActiveMode(bool active);
	GymNS::GameState getGameState();
	virtual void onEnter() override;
	virtual void onExit() override;
	// implement the "static create()" method manually
	CREATE_FUNC(GymGame);

protected:
	GymGame();
	TickGroup* m_tickGroup;
	LetterCloud* m_clouds[3];
	GymNS::GameState m_gameState;
	GymNS::MonkeyState m_monkeyState;
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_endingNode;
	GAFWrapper* m_monkey;
	GAFWrapper* m_treadmill;
	GAFWrapper* m_endMonkey;
	std::string m_correctLetter;
	std::vector<SingleGame> m_gameData;
	std::string m_configPath;
	SingleGame m_current_game;
	int game_index;
	int number_tick = 0;
	int soundQuesId = -1;
	int soundGuidingId = -1;
	cocos2d::ui::Button* speaker;
	void onClickedSpeaker(cocos2d::Ref* sender);
	std::string hoc_am_hoc_dau;
	void guideGame();
	void stopGuideGame();
	LetterCloud* cloud;
	float _audio = -1;
private:
	void stopMonkeySounds();
	bool initBackground();
	bool initTickGroup();
	bool initCloud();
	bool initMonkey();
	bool initEndingNode();
};
