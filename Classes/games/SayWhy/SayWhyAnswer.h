#ifndef MJ_GAME_SAY_WHY_ANSWER_H__
#define MJ_GAME_SAY_WHY_ANSWER_H__

#include "GAFWrapper.h"

class SayWhyAnswer : public cocos2d::ui::Layout
{
public:
	struct SayWhyAnswerInfo {
		std::string answer;
		std::string audio_path;
		bool correct;
	};
	SayWhyAnswer();
	static SayWhyAnswer * createAnswer(SayWhyAnswerInfo info);
	void showAnswer(float dt = 0.25f);
	void hideAnswer(float dt = 0.25f);
	void onClickedAnswer(std::function<void()> call_back = nullptr);
	bool isAnswerCorrect();
	SayWhyAnswerInfo m_info;
private:
	void didLoadFromCSB(SayWhyAnswerInfo info);
	void loadFrames();
	GAFWrapper * m_gaf_answer;
	cocos2d::ui::Text * m_lbl_answer;
	cocos2d::Size answer_size;
	//cocos2d::ui::Layout * m_layout_touch;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};
#endif // !MJ_GAME_SAY_WHY_ANSWER_H__
