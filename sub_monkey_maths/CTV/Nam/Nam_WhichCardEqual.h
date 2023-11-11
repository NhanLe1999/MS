//
//  Nam_WhichCardEqual
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_WhichCardEqual_h
#define Nam_WhichCardEqual_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_WhichCardEqual : public Math_Quiz
{
public:
    static Nam_WhichCardEqual* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
	math::resource::Image* img_result;
    //Game config
	int max_result, min_result;
	int result;
	int cardCount;
	// answer
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node, std::string str_ans);
    math::resource::Image* bg_sentence = nullptr;
    math::resource::Image* bg_sentence_choosed = nullptr;
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_WhichCardEqual_h */
