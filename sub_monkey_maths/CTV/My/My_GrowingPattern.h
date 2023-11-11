#ifndef __MY_GROWING_PATTERN_H__
#define __MY_GROWING_PATTERN_H__

#include "Math_Quiz.h"

class My_GrowingPattern : public Math_Quiz {
public:
	static My_GrowingPattern* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	/*void autoTesting() override;*/

	void onTouchAnswer(cocos2d::Node* node);
	void onTouchButtonDone(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Point point);

private:
	bool init() override;

	bool clickable = false;
	int count = 0;
	bool drag;

	std::string pattern_type;
	std::string pattern_kind;
	int min_number;
	int row_number;
	int pattern_piece;

	int slot_index = 0;

	cocos2d::Size object_size;
	std::vector<std::vector<Math_Object*>> objects;

	std::vector<math::resource::Image*> pattern_imgs;
	std::vector<math::resource::Image*> img_list;

	math::resource::Image* background = nullptr;

	std::vector<Math_Object*> _draggers;

	cocos2d::Vec2 drag_pos;
    bool touch_slot;
};

#endif // !__MY_GROWING_PATTERN_H__
