#ifndef MJ_GAME_MULTIPLE_CHOICE_ANSWER_H__
#define MJ_GAME_MULTIPLE_CHOICE_ANSWER_H__

#include "GAFWrapper.h"
class MultipleChoiceAnswerDelegate {
public:
	virtual void onClickedAnswerDelegate(cocos2d::Ref * sender) {};
	virtual void onClickedSpeakerDelegate(cocos2d::Ref * sender) {};
};
class MultipleChoiceAnswer : public cocos2d::ui::Layout
{
public:
	struct MultipleChoiceAnswerInfo {
		std::string answer;
		std::string audio_path;
		bool correct;
		MultipleChoiceAnswerInfo() : answer{ " " }, audio_path{ " " }, correct{ false } {};
	};
	MultipleChoiceAnswer();
	static MultipleChoiceAnswer * createAnswer(MultipleChoiceAnswerInfo info = MultipleChoiceAnswerInfo());
	void setAnswerInfo(MultipleChoiceAnswerInfo info);
	void showAnswer(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void hideAnswer(float dt = 0.0f,std::function<void()> call_back = nullptr, bool side_down = false);
	void showSpeaker();
	void hideSpeaker();
	void highlightText(cocos2d::Color3B color);
	bool isAnswerCorrect();
	void showImageAnswer(int type);
	MultipleChoiceAnswerInfo m_info;
	void setDelegate(MultipleChoiceAnswerDelegate * delegate);
	cocos2d::ui::Button * getSpeaker();
protected:
	MultipleChoiceAnswerDelegate *_delegate;
private:
	
	void didLoadFromCSB(MultipleChoiceAnswerInfo info);
	void loadFrames();
	cocos2d::ui::Button * m_speaker;
	cocos2d::ui::Layout * layout_root;
	cocos2d::ui::Text * m_lbl_answer;
	cocos2d::Size answer_size;
	//cocos2d::ui::Layout * m_layout_touch;
	std::string font_name = "fonts/Montserrat-Regular.ttf";
};
#endif // !MJ_GAME_MULTIPLE_CHOICE_ANSWER_H__
