//
//  Nam_SortAndDisplayResult
//  MJQuiz
//
//  Created by Tunado on 18/9/17.
//
//
#ifndef Nam_SortAndDisplayResult_h
#define Nam_SortAndDisplayResult_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_SortAndDisplayResult : public Math_Quiz
{
public:
    static Nam_SortAndDisplayResult* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
	
	//ui
	std::vector <Math_Object*> objFooterGraph;
	math::resource::Image* boxObject = nullptr;
	math::resource::Image* boxBlue = nullptr;
	math::resource::Image* boxYellow = nullptr;
	// object 
	int numberOfObjectType;
	std::vector<int> objectsNumber;
	std::vector<int> currentAnswer;
	Size objectSize;
	// graph
	int maxRow;
	Size graphSize = cocos2d::Size(70,70);
	math::resource::Image* imgSlot = nullptr;
	void onTouchedObject(cocos2d::Node* node);
	
	Math_Object* graphObject[10][10];
	cocos2d::DrawNode* points[10][10];
	
	std::vector<cocos2d::DrawNode*> pointsVec;
	// 
	void onAnswerDone(Node* node);
	//Arrow
	std::vector <math::resource::Image*> objectImages;
    //Game config
	Math_Answer* btn_done = nullptr;
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
};

#endif /* Nam_SortAndDisplayResult_h */
