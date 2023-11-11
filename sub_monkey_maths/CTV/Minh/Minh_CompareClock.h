//
// Minh_CompareClock.h
//
//

#ifndef Minh_CompareClock_h
#define Minh_CompareClock_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"
#include "DigitalClock.h"

class Minh_CompareClock : public Math_Quiz
{
public:
    static Minh_CompareClock* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //Game config

	int num_activity;
	std::string answer;
	
	std::string icon_same;
	std::string icon_filter;
	
	bool same_name;

	// Cau hinh dong ho
	int clock_size = 300;
	std::vector<math::ClockBase *> _clocks;
	
	std::string clock_type;
	int clock_mode;
	std::string clock_show;

	// Danh sach icon su dung trong game
	std::vector<math::resource::Image *> activities_icon;
	// Danh sách câu hỏi
	 
    //callback Func
	void onTouchedObject(cocos2d::Node * node);
	void onTouchedAnswer(cocos2d::Node * node);
    bool clickable=true;
    
	// debugging mode
	int testing_step = 0;
	//Smart autotest
	//debugging mode
	void autoTestingRun(float dt);

	// Game 
	private:
		math::resource::Image * findFirstActivityImage(std::vector<math::resource::Image *>);
		math::resource::Image * findLastActivityImage(std::vector<math::resource::Image *>);

		std::pair<int, int> getTimeFromImage(math::resource::Image * img);
};

#endif /* Minh_CompareClock */
