//
//  Nam_PartitionShapeEqual
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_PartitionShapeEqual_h
#define Nam_PartitionShapeEqual_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_PartitionShapeEqual : public Math_Quiz
{
public:
    static Nam_PartitionShapeEqual* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
	std::vector <DrawNode*> draws;
    //Game config
	std::string game_mode;
	// result
	std::string str_result;
	math::resource::Image* img_result = nullptr;
	int int_result;
	// answer
	Math_Object* selected_object = nullptr;
	void onAnswerDone(Node* node, std::string str_ans);
	void genDrawNode(Math_Object* obj, int intType, bool isTrue);
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
    
};

#endif /* Nam_PartitionShapeEqual_h */
