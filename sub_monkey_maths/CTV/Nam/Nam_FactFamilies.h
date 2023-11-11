//
//  Nam_FactFamilies
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_FactFamilies_h
#define Nam_FactFamilies_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_FactFamilies : public Math_Quiz
{
public:
    static Nam_FactFamilies* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
	math::resource::Image* img_null;
    //Game config
	cocos2d::ui::ImageView* box_1 = nullptr;
	cocos2d::ui::ImageView* box_2 = nullptr;
	std::string game_mode;
	int min_result, max_result;
	Math_Object* obj_box = nullptr;
	std::vector <std::string > str_box;
	std::string str_result;
	std::vector <std::string > str_answers;
	void genObjCal(Math_Object* obj, std::string string);
	// answer
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node, std::string str_ans);

    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	//void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_FactFamilies_h */
