#ifndef __MY_SUBTRACT_ZERO_AND_ALL_H__
#define __MY_SUBTRACT_ZERO_AND_ALL_H__

#include "Math_Quiz.h"

class My_SubtractZeroAndAll : public Math_Quiz {
public:
	static My_SubtractZeroAndAll* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchAnswer(cocos2d::Node* node);

private:
	bool init() override;

	std::string operation_type;
	bool drag;

	cocos2d::Size object_size;

	int subtrahend;
	int minuend;
	int different;
	std::vector<int> answer_values;
	std::vector<math::resource::Image*> icons;
	std::vector<math::resource::Image*> answer_imgs;

	Math_Answer* answer = nullptr;

	std::vector<Math_Object*> _draggers;

	math::resource::Image* slot_bg = nullptr;
};

#endif // !__MY_SUBTRACT_ZERO_AND_ALL_H__
