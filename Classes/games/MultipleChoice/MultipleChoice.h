#ifndef MJ_GAME_MULTIPLE_CHOICE_H__
#define MJ_GAME_MULTIPLE_CHOICE_H__

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "MultipleChoiceAnswer.h"
#include "MultipleChoiceQuestion.h"
class MultipleChoice : public MJMainGBase, public MultipleChoiceAnswerDelegate,public MultipleChoiceQuestionDelegate {
public:
	struct MultipleChoiceInfo {
		std::vector<MultipleChoiceAnswer::MultipleChoiceAnswerInfo> _answers;
		MultipleChoiceQuestion::MultipleChoiceQuestionInfo _question;
	};
	MultipleChoice();
	virtual bool init() override;
 	static MultipleChoice * createGame();
	void generateData();
	void onEnter() override;
	void onExit() override;
	void onExitTransitionDidStart() override;
	void onClickedAnswerDelegate(cocos2d::Ref * sender) override;
	void onClickedSpeakerDelegate(cocos2d::Ref * sender) override;
	void onClickedQuestionDelegate(cocos2d::Ref  * sender) override;
protected:
	void loadFrames();
	void initQuestionAndAnswers();
	void setGamePlay(int index);
	void showIntroGames();
	void showGamename();
	void startGame();
	void onChooseRight(MultipleChoiceAnswer * answer);
	void onChooseWrong(MultipleChoiceAnswer * answer);
	void onNextQuestion();
	void endGame();
	void showQuestion(std::function<void()> call_back = nullptr);
	void showAnswers(int index = 0,float dt = 0.5f);
	void hideWrongAnswers();
	void nextQuestion(std::function<void()> call_back = nullptr);
	void lockTouch(bool lock);
	void setQAPosition();
private:
	cocos2d::ui::Layout * layout_root;
	GAFWrapper * m_navy;
	std::string m_config_path;
	MultipleChoiceQuestion * m_question;
	std::vector<MultipleChoiceAnswer * > m_answers;
	int question_index;
	std::vector<MultipleChoiceInfo> list_game_info;
	void initSampleGameInfo();
    int answer_id = -1;
    int count_answer = 0;
    int q_id = -1;
    int ans_id = -1;
};

#endif // !MJ_GAME_MULTIPLE_CHOICE_H__
