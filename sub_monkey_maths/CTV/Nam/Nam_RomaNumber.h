//
//  Nam_RomaNumber
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#ifndef Nam_RomaNumber_h
#define Nam_RomaNumber_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "RomanNumeralsUtils.h"
using namespace cocos2d;
class Nam_RomaNumber : public Math_Quiz
{
public:
    static Nam_RomaNumber* createQuiz();
    
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
	std::string modeGame;
	// result
	int result;
	math::resource::Image* imageResult = nullptr;
	Math_Object* objResult = nullptr;
	// answer
	std::vector<int> answers;
	void onAnswerDone(Node* node, int ans);
    //debugging mode
    int testing_step=0;

    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_RomaNumber_h */
