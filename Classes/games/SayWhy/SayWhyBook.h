#ifndef MJ_GAME_SAY_WHY_BOOK_H
#define MJ_GAME_SAY_WHY_BOOK_H

#include <stdio.h>
#include "GAFWrapper.h"

#include "SayWhyAnswer.h"
#include "SayWhyQuestion.h"

class SayWhy;
class SayWhyBookDelegate {
public:
	virtual void onClickedAnswer(cocos2d::Ref* sender) {};
};

class SayWhyBook : public cocos2d::ui::Layout {
public:
	struct SayWhyBookInfo {
		SayWhyQuestion::SayWhyQuestionInfo question;
		std::vector<SayWhyAnswer::SayWhyAnswerInfo> list_answer;
		bool flag = false;
	};
	SayWhyBook();
	static SayWhyBook* createBook(std::vector<SayWhyBookInfo> info);
	SayWhyBookInfo getInfo() const;
	void openBook(std::function<void()> call_back = nullptr);
	void showQuestion(float delay_time);
	void showAnswers(int index = 0);
	void setDelegate(SayWhyBookDelegate* delegate);
	bool onNextQuestion();
	int getQuestionIndex() const;
	void setAnswersTouchable(bool touch);
	void showTapGuideGame(cocos2d::Vec2 pos, int loop = 100, float delay_time = 10.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void stopAudio();
	void pauseAudio();
	void guideGame();
	void stopGuideGame();
protected:
	void didLoadFromCSB(std::vector<SayWhyBookInfo> info);
	void loadFrames();
	void initQuestion();
	void initAnswers();
	void onLoadNextQuestion();
	void radioQuestion();
	SayWhyBookDelegate* _delegate;
	CC_SYNTHESIZE(SayWhy*, _sayWhy, SayWhy1);

private:
	std::vector<SayWhyBookInfo> m_list_info;
	int question_index;
	GAFWrapper* m_book_front;
	GAFWrapper* m_book_back;
	cocos2d::ui::ImageView* m_img_book;
	cocos2d::ui::ImageView* m_picture_bg;
	cocos2d::ui::ImageView* m_picture_ref;
	cocos2d::ui::ImageView* m_picture;
	cocos2d::Size book_size;

	SayWhyQuestion* m_question;
	std::vector<SayWhyAnswer* > m_answers;
	int count_answer = 0;
	cocos2d::Size screen_size;
	int _idAudioImage = -1;
	int _idAudioText = -1;
	int _flag = 0;
	int _audio = -1;
};

#endif // !MJ_GAME_SAY_WHY_BOOK_H