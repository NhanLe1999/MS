//
//  Thanh_WhereDoesClockHandPointing.hpp
//  MJQuiz
//
//  Created by Daniel on 11/23/17.
//
//

#ifndef Thanh_WhereDoesClockHandPointing_hpp
#define Thanh_WhereDoesClockHandPointing_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class Thanh_WhereDoesClockHandPointing : public Math_Quiz
{
public:
	enum Mode {
		hour,
		minute
    };
	enum Option {
		grade_1,
		grade_1_plus_plus
	};
    static Thanh_WhereDoesClockHandPointing* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;


private:
    bool init() override;

    // ---------------
    // game config
    Mode _mode;
	Option _option;
	bool _random_time;
	std::string _answer_mode;

    // ---------------
    // clock config
    int _radius;
    math::resource::Image* _background = nullptr;
    math::AnalogClock* _analogClock = nullptr;
    int _hour, _minute;

    // ---------------
    // answer config
    int _number_answer;

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> _icons;
    std::vector<math::resource::Image*> img_number_list;

    //callback Func
	void onAnswerDone(cocos2d::Node *node);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    void autoTestingRun(float dt);

    // answer 
    std::vector<Math_Answer*> _list_answer;
    std::vector<int> _vector_answers;

    // in game
    bool _disable; // disable touch
};
#endif /* Thanh_WhereDoesClockHandPointing_hpp */
