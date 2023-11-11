#ifndef MJ_GAME_SAY_WHY_QUESTION_H__
#define MJ_GAME_SAY_WHY_QUESTION_H__
#include <stdio.h>
#include "ui/CocosGUI.h"

class SayWhyQuestion : public cocos2d::ui::Layout {

public:
	struct SayWhyQuestionInfo {
		std::string question;
		std::string audio_path;
		std::string image_path;
	};
	SayWhyQuestion();
	static SayWhyQuestion* createQuestion(SayWhyQuestionInfo question_info);
	void setQuestionInfo(SayWhyQuestionInfo question_info);
	cocos2d::Size question_size;
	void typeQuestion(std::function<void()> call_back = nullptr);

private:
	void didLoadFromCSB(SayWhyQuestionInfo question_info);
	SayWhyQuestionInfo	m_question_info;
	cocos2d::ui::Text* m_lbl_question;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";



};
#endif // !MJ_GAME_SAY_WHY_QUESTION_H__