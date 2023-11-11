#ifndef MJ_GAME_MULTIPLE_CHOICE_QUESTION_H__
#define MJ_GAME_MULTIPLE_CHOICE_QUESTION_H__
#include <stdio.h>
#include "ui/CocosGUI.h"

class MultipleChoiceQuestionDelegate {
public:
	virtual void onClickedQuestionDelegate(cocos2d::Ref * sender) {

	};
};
class MultipleChoiceQuestion : public cocos2d::ui::Layout {

public:
	struct MultipleChoiceQuestionInfo{
		std::string question;
		std::string audio_path;
		std::string image_path;
		bool show_image;
		MultipleChoiceQuestionInfo() : question{ " " }, audio_path{ " " }, image_path{ " " }, show_image{false} {};
	};
	MultipleChoiceQuestion();
	static MultipleChoiceQuestion * createQuestion(MultipleChoiceQuestionInfo question_info = MultipleChoiceQuestionInfo());
	void setQuestionInfo(MultipleChoiceQuestionInfo question_info);
	cocos2d::Size question_size;
	void typeQuestion(std::function<void()> call_back = nullptr);
	MultipleChoiceQuestionInfo	m_question_info;
	void showQuestion(float delay = 0.0f, std::function<void()> call_back = nullptr);
	void hideQuestion(float delay = 0.0f, std::function<void()> call_back = nullptr);
	void setQuestionTouchable(bool touchable);
	void setDelegate(MultipleChoiceQuestionDelegate * delegate);
private:
	MultipleChoiceQuestionDelegate * _delegate;
	void didLoadFromCSB(MultipleChoiceQuestionInfo question_info);
	cocos2d::ui::Text * m_lbl_question;	
	cocos2d::ui::ImageView * m_img_question;
	std::string font_name = "fonts/Montserrat-Regular.ttf";

};
#endif // !MJ_GAME_MULTIPLE_CHOICE_QUESTION_H__
