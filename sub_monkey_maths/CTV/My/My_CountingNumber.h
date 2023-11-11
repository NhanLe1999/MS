#ifndef __COUNT_NUMBER_H__
#define __COUNT_NUMBER_H__

#include "Math_Quiz.h"

class My_CountingNumber : public Math_Quiz {
public:
	static My_CountingNumber* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;

	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);

	void autoTestingRun(float delaytime);
private:
	bool init() override;

	Math_Object* first = nullptr;
    int skipping_count = 1;
	int testing_step = 0;

	std::string count_type = "";
    std::string game_type = "";
    
	int number_dragger;
	cocos2d::Size dragger_size;
	//Số đầu tiên trong dãy số
	int dragger_start;
	std::vector<Math_Object*> test_vector;

	int count = 0;
	int number_slot;
	cocos2d::Size slot_size;

	//Vector lưu các số đếm
	std::vector<math::resource::Image*> numbers;

	//Vector lưu các dragger
	std::vector<Math_Object*> _draggers;
	std::vector<cocos2d::ui::ImageView*> _arrows;

	math::resource::Image* slot_bg = nullptr;
	math::resource::Image* drag_bg = nullptr;
};

#endif //__COUNT_NUMBER_H__
