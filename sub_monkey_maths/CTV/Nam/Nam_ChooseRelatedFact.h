//
//  Nam_ChooseRelatedFact
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_ChooseRelatedFact_h
#define Nam_ChooseRelatedFact_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_ChooseRelatedFact : public Math_Quiz
{
public:
    static Nam_ChooseRelatedFact* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
    //Game config
	std::string game_mode;
	int minResult, maxResult;
	// result
	std::vector <std::string>  str_answers;
	// answer
	std::string str_result;
	std::string str_ques_cal;
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node, std::string str_ans);
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	//void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_ChooseRelatedFact_h */
