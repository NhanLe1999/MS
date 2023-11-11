//
//	Drag Arrow To Measure
//  MJQuiz
//
//  Created by Tunado on 20/9/17.
//
//
#ifndef Nam_DragArrowToMeasure_h
#define Nam_DragArrowToMeasure_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_DragArrowToMeasure : public Math_Quiz
{
public:
    static Nam_DragArrowToMeasure* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
	void update(float dt)override;
    
private:
    bool init() override;
	float delay = 0;
	bool isMoving = false;
	void onTouchedObject(Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	float width;
    std::string mode_game;
	//result
	float result;
	int intResult;
	// object drager
	Size dragerSize;
	Math_Object* origin = nullptr;
	Math_Object* lineBrownOrigin = nullptr;
	Math_Object* lineBrownDrager = nullptr;
	Math_Object* drager = nullptr;
	Math_Object* lineYellow = nullptr;
	Math_Object* hiddenDrager = nullptr;
	//ruler
	math::resource::Image* rulerImg = nullptr;
	Math_Object* rulerObject = nullptr;
	Math_Object* lineRed = nullptr;
	//character
	math::resource::Image* characterImg = nullptr;
	Math_Object* characterObj;
    //Game config
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	bool isEnd;
	Math_Answer* btn_done = nullptr;
	void onAnswerDone(Node* node);
	void fixAnchorPoint(Math_Object* obj);
};

#endif /* Nam_DragArrowToMeasure_h */
