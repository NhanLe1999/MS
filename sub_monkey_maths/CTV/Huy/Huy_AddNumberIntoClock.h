//
//  Huy_AddNumberIntoClock.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef Huy_AddNumberIntoClock_h
#define Huy_AddNumberIntoClock_h

#include "Math_Quiz.h"
#include "AnalogClock.h"

class Huy_AddNumberIntoClock : public Math_Quiz {
public:
	static Huy_AddNumberIntoClock* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;
private:
	bool init() override;
    
	void autoTestingRun(float delaytime);
    
    bool is_add_number = false;
    bool is_learn_time = false;
    
    cocos2d::Size object_size;
    
    math::AnalogClock* m_clock = nullptr;
    Math_Object* text_object = nullptr;
    
    int index_touch = 0;
    int minute = 0;
    int hour = 0;
    bool completed = true;
    
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    
    void runClock(float dt);

};

#endif /* Huy_AddNumberIntoClock_h */
