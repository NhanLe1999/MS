//
//  Minh_Calendar.h
//  MJQuiz
//
//  Created by MinTzy on 7/25/17.
//
//

#ifndef Minh_Calendar_h
#define Minh_Calendar_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "CalendarBase.h"
class Minh_Calendar : public Math_Quiz
{
public:
	static Minh_Calendar* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;

private:
	bool init() override;

	//Game config
	//Calendar config
	math::calendar::Calendar * calendar = nullptr;
	cocos2d::Size calendar_size = cocos2d::Size(250, 250);
	bool monday_start;
	std::string date_format;
	// Question config
	std::string question_type;
	std::string date_ask;
	bool hight_light_date_ask;
	//callback Func
	void onTouchedAnswer(Node* node);
	bool clickable = true;
	//debugging mode
	int testing_step = 0;
	//Smart autotesy
	void autoTestingRun(float dt);
};

#endif /* Minh_Calendar_h */
