#pragma once
#include "MJMainGBase.h"
#include "ui/CocosGUI.h"
#include "GameData.h"
#include"MsHelperGame.h"
class TextLeaf;
struct WordSync {
	mj::GameObject word;
	float spawnTime;
	std::vector<mj::GameObject> answers;
};
using namespace ms::game;

namespace FrogSentence {
	enum GameState
	{
		STARTING,
		PLAYING,
		ENDING,
		TRANSITIONING,
        FINISH,
		NONE
	};
}

class SentenceGroup;

class FrogSentenceGame : public MJMainGBase
{
public:
	static FrogSentenceGame* createGame(std::string json_file, bool isAutoPlay);
	CREATE_FUNC(FrogSentenceGame);
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void update(float dt) override;
	bool onTouchBegan(Touch *touch, Event *unusedEvent);
	void onTouchMoved(Touch *touch, Event *unusedEvent);
	void onTouchEnded(Touch *touch, Event *unusedEvent);
protected:
	FrogSentenceGame();
	void initBackground();
	void initFrog();
	void initLeafSlot();
	void initSelectors();
	void showWords(std::vector<mj::GameObject> strs);
	void showNextWordOrEndGame();
	void getGameData();
	void onDropLeaf(TextLeaf* leaf);
	void playIntroGame();
	void playStartingScript();
	void playEndingScript();
	void playTransitioningScript();
	void setGameState(FrogSentence::GameState state);
	void showGuideGame();
	void showDragGuideGame(float delay = 1.5f);
    void getAudioVMonkey();
private:
	std::string m_configPath;
	cocos2d::Node* m_bgRootNode;
	GAFWrapper* m_frog;
	ui::Scale9Sprite* m_selectFrame;
	cocos2d::Node* m_slot;
	std::vector<WordSync> m_words;
	std::vector<WordSync> m_wordsClone;
	std::vector<WordSync> m_wordsClone2;
	std::string m_audioPath;
    std::string m_imagePath;
	mj::GameObject m_correctWord;
	cocos2d::Node* m_rightTree;
	cocos2d::Node* m_leftTree;
	FrogSentence::GameState m_gameState;
	std::vector<TextLeaf*> m_leaves;
	cocos2d::Node* m_bgSprite;
    cocos2d::Node* m_bgSprite1;
    cocos2d::Node* m_bgSprite2;
	cocos2d::Vec2 m_bgStartLocation;
	cocos2d::Vec2 m_lastTouchLocation;
	cocos2d::Node* m_guideNode;
	std::vector<math::resource::Sync_Text> m_words_SyncText;
	SentenceGroup* m_sentenceGroup;
	bool m_firstTimePlay, _finishActivity = false, _soundAvaiable = true;
    int max_game =1 , count_game = 0;
    std::string question_vi;
	int m_beginHaveAction = 0;
	int _countTimer = 0;
	const float _age_check = 3.5;
	Button* guideBtn;
};
