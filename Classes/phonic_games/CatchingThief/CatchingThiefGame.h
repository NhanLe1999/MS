#pragma once
#include "MJMainGBase.h"
#include "CatchingThiefWordFrame.h"
#include "GameData.h"
#include "StoryConfigManager.h"
namespace CatchingThief {
	typedef std::vector<mj::GameObject> Question;
}

class CatchingThiefGame : public MJMainGBase {
public:
	CREATE_FUNC(CatchingThiefGame);
	static CatchingThiefGame * createGame(std::string json_file, bool isAutoPlay);
	virtual bool init() override;
	void onEnter() override;
    void onExit() override;
protected:
	CatchingThiefGame();
	void initBackground();
	void initCharacter();
	void startGame();
	void initWordFrame();
	void getGameData();
	void playStartingScript();
	void showNextQuestion();
	void showQuestion(CatchingThief::Question& answer, mj::GameObject& correctAnswer);
	void clearAnswerBtns();
	void onWrongAnswer(cocos2d::ui::Button* btn);
	void onCorrectAnswer(cocos2d::ui::Button* btn);
	void playWrongScript();
	void playCorrectScript();
	void playEndingScript();
	bool checkWinCondition();
	void scheduleGuidingGame(mj::GameObject& correctAnswer);
	void jumpCharacter(float catchDuration,GAFWrapper* character);
	void hitTrash(float hitDuration,cocos2d::Node* trash);
	void playSynctext(std::vector<Label*> nodes, std::function<void()> callback = nullptr);
private:
	std::string m_configPath;
	std::string m_question_path;
	cocos2d::Node* m_backgroundNode;
	GAFWrapper* m_police;
	GAFWrapper* m_thief;
	CatchingThief::WordFrame* m_wordFrame;
	std::vector<CatchingThief::Question> m_questions;
	std::vector<mj::GameObject> m_correctLetters;
 
	std::vector<cocos2d::ui::Button*> m_answerBtn;
	mj::GameObject* m_correctAnswer = nullptr;
	int m_letterIndex;
	mj::GameObject m_correctWord;
	cocos2d::Node* m_gameNode;
	bool m_clickable;
	bool is_word; // phan biet choi word hay sound// phan biet choi word hay sound
	void showTapGuideGame(Vec2 position, float delay_time = 6.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	//    void hideTapGuideGame();

	void inActiveMode(Vec2 position, float delay_time = 7.0f);
    std::string hoc_am_hoc_van;
};
