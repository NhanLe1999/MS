#pragma once

#include "MJMainGBase.h"
#include "Goat.h"
#include "GameData.h"
class BackgroundNode;

namespace Dinosaur {
	enum GameState {
		GUIDING,
		STARTING,
		PLAYING,
		ENDING,
		NONE
	};
}

class DinosaurChasingGame : public MJMainGBase
{
public:
	static DinosaurChasingGame * createGame(std::string json_file, bool isAutoPlay);
	CREATE_FUNC(DinosaurChasingGame);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt) override;
	void setGameState(Dinosaur::GameState state);
	void createObstacle();
protected:
	DinosaurChasingGame();
	std::string m_configPath;
	void initBackground();
	void initGoat();
	void initDinosaur();
	void playGuidingScript();
	void playStartingScript();
	void playEndingScript();
	void comeCloseToObstacle();
	void onTouchObstacle();
	void setTimeScale(float timescale);
	void jumpGoat();
	void getNextLetterOrEndGame();
	void getDataFromConfig();
	void onSpeakerClicked(cocos2d::Ref* sender);
private:
	cocos2d::Node* m_bgRootNode;
	cocos2d::Node* m_gameNode;
	std::vector<BackgroundNode*> m_bgNodes;
	BackgroundNode* m_groundNode;
	cocos2d::Node* m_obstacleNode;
	cocos2d::Node* m_endNodeLayer1;
	cocos2d::Node* m_endNodeLayer2;
	cocos2d::Label* m_label;
	GAFWrapper* m_balloonNode;
	GAFWrapper* m_dinosaur;
	GAFWrapper* boom_effect;
	GAFWrapper* correct_effect;
	cocos2d::ui::Button* _speaker;

	//Camera m_camera;
	Goat* m_goat;
	Dinosaur::GameState m_gameState;
	float m_nextObstacleDistance;
	float m_timeScale;
	bool m_touchObstacleEnabled;
	bool m_isGoatOnAir;
	bool m_isGoatFallen;
	cocos2d::Node* m_obstacleSprite;
	std::vector<mj::GameObject> m_lettes;
	std::vector<mj::GameObject> m_phonics;
    std::vector<std::vector <mj::GameObject>> m_games;
	bool m_lastAnswerResult;
	int m_consecutiveCorrect;
	bool m_need_guide;
    
    int max_tap = 15, number_tap = 0;
	void inActiveMode(float delay_time = 7.);
};
