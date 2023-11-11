//
//  Nam_Commutative.h
//  MJQuiz
//
//  Created by Tunado on 30/9/17.
//
//

#ifndef Nam_Commutative_h
#define Nam_Commutative_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_Commutative : public Math_Quiz
{
public:
	static Nam_Commutative* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;	
	void autoTestingRun(float dt);
private:
	bool init() override;
	//ui
	math::resource::Image* img_box_slot = nullptr;
	// Game config
	cocos2d::Size panel_size = cocos2d::Size(900, 750);
	int result, number1, number2;
	// object
	Size objectSize = Size(80,80);
	
	math::resource::Image* imageObject = nullptr;
	std::vector<Math_Object*> caculationObjects1[2];
	std::vector<Math_Object*> caculationObjects2[2];
	// drager
	Size dragerSize = Size(120, 120);
	std::vector<Math_Object*> dragers;
	std::vector<int> intResult;
	std::vector<Math_Slot*> resultSlots;
	//slot
	math::resource::Image* imgSlot = nullptr;
	std::vector<Node*> calculation1;
	std::vector<Node*> calculation2;
	std::vector<Math_Object*> operatorObjs;
	void genOperator(Math_Object* obj, std::string text);
	//touch
	void onAnswerDone(Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchedObject(cocos2d::Node* node);
	//autotest
	int testing_step = 0;
	Math_Answer* btn_done = nullptr;
};

#endif /* Nam_Commutative_h */
