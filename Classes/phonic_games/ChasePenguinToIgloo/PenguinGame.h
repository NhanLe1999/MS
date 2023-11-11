#pragma once
#include "MJMainGBase.h"
#include "GameData.h"

class BigFoot;
class Penguin;

enum PenguinGameState {
	WAITING,
	GUIDING,
	SCRIPT_GUIDING,
	PLAYING,
	ENDED,
	NONE
};

class PenguinGame : public MJMainGBase {
public:
	static PenguinGame * createGame(std::string json_file, bool isAutoPlay);

	virtual bool init();
	CREATE_FUNC(PenguinGame);
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float delta) override;
	void startGame();
	void movePenguinToRandomPosition(Penguin* penguin);
	void setGameState(PenguinGameState state);
	void releaseAllPenguins();
	bool checkWinCondition();
	void endGame();
	void idleAction();
protected:
	PenguinGame();
	cocos2d::Node* m_gameNode;
	cocos2d::Node* m_endingNode;
	cocos2d::ui::Button* _guideBtn;
	std::vector<cocos2d::Sprite*> m_igloos;
	std::vector<cocos2d::Label*> m_labels;
	std::vector<Penguin*> m_penguins;
	PenguinGameState m_gameState;
	BigFoot* m_bigFoot;
	GAFWrapper* m_endingBigFoot;
	int m_correctAnswer;
    std::string hoc_am_hoc_van = "";
	mj::GameObject m_correctLetter;
	bool m_touchEnabled;
	std::string m_configPath;
	bool m_scriptGuided;
    int randomIndex = 0;
    int max_game = 0;
	bool _enablePenguinIntoIglooWithGuiding = true;
	bool _needGuidingByHand = true;
	int level_index = 0;
//    std::vector<>
private:
	bool initBackground();
	bool initIgloos();
	bool initBigFoot();
	bool initPenguin();
	bool initEndingNode();
	void showGuideGame(std::function<void()> callback = nullptr);
	void checkBigFootContact();
	void playGuideScript();
	void showHandGuideTap(cocos2d::Vec2 pos, std::function<void()> callback = nullptr);
	void reachIgloo(int index);
	bool handleTouch(Touch* touch, Event * event);
	void zoomIgloo(cocos2d::Sprite* igloo);
    void inActiveMode();
	void scheduleAudioAndActiveGuiding();
};
