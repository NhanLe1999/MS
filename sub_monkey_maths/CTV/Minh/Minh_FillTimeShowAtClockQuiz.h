//
// Minh_FillTimeShowAtClockQuiz.h
// Create 5/3/2018
// Dev by minhtzy
//

#ifndef Minh_FillTimeShowAtClockQuiz_h
#define Minh_FillTimeShowAtClockQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class Minh_FillTimeShowAtClockQuiz : public Math_Quiz
{
public:
	static Minh_FillTimeShowAtClockQuiz* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;

private:
	bool init() override;
	// Config for clock
	math::AnalogClock * clock = nullptr;
	int clock_size = 350;
	bool use_text;
	bool use_roman;
	// Config game
	bool fill_hour;
	bool fill_minute;
	bool fill_random;

	int num_slot_fill;
	bool is_clone_drag = false;

	int round_minute;

	std::vector<bool> _hours_show;
	std::vector<bool> _minutes_show;
	std::vector<int> _hours_fill;
	std::vector<int> _minutes_fill;

	cocos2d::Size drager_size = cocos2d::Size(100, 100);

	std::vector<Math_Slot *> _hour_slots;
	std::vector<Math_Slot *> _minute_slots;

	std::vector<Math_Object *> _hour_dragers;
	std::vector<Math_Object *> _minute_dragers;

	std::vector<Math_Object *> _hour_objects;
	std::vector<Math_Object *> _minute_objects;

	Math_Object * hour_object = nullptr;
	Math_Object * minute_object = nullptr;
	Math_Object * device_object = nullptr;

	std::string level;
	math::comparision::Key key_compare = math::comparision::Key::number;
	// Callback func
	void onTouchedObject(cocos2d::Node * node);
	void onMoveObjectEnded(cocos2d::Node * node, cocos2d::Vec2 position);
	void onAnswerDone(cocos2d::Node * node);
	bool clickable;
	// debugging mode
	int testing_step = 0;
	//Smart autotest
	//debugging mode
	void autoTestingRun(float dt);
	void playAnimGaf(Node * object);

};

#endif /* Minh_FillTimeShowAtClockQuiz_h */
