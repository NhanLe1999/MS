//
//  Nam_PartitionShapeColor
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_PartitionShapeColor_h
#define Nam_PartitionShapeColor_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_PartitionShapeColor : public Math_Quiz
{
public:
    static Nam_PartitionShapeColor* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
	int tu_so, mau_so;
    //Game config
	std::vector <math::resource::Image*> images_shape;
	// answer
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node);
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
};

#endif /* Nam_PartitionShapeColor_h */
