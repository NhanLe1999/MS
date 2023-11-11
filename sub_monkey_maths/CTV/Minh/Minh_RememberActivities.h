//
// Minh_RememberActivities.h
//
//

#ifndef Minh_RememberActivities_h
#define Minh_RememberActivities_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "AnalogClock.h"

class Minh_RememberActivities : public Math_Quiz
{
public:
    static Minh_RememberActivities* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    std::pair<int, int> getTimeFromImage(math::resource::Image * img);
    
    //Game config
	int num_activity;
	bool sorted;
	std::string icon_same;
	std::string icon_different;
	
	int current_index = 0;
    std::vector<math::AnalogClock *> _clocks;
    cocos2d::Size image_size = cocos2d::Size(500, 500);
    std::vector<bool> correct_answers;
    std::vector<Math_Answer*> answers;
        
    //math::AnalogClock * clock;
    Math_Object * clock_touch = nullptr;
    std::vector<std::pair<int, int>> hour_ques;
    std::vector<std::pair<int, int>> hour_ans;

	// Danh sach icon su dung trong game
	std::vector<math::resource::Image *> activities_icon;
    std::vector<math::resource::Image *> answer_img;
    
    void onTouchedAnswer(cocos2d::Node * node);
    
	// debugging mode
	int testing_step = 0;
	//debugging mode
	void autoTestingRun(float dt);
};

#endif /* Minh_RememberActivities */
