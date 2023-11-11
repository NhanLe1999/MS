//
//  Nam_DragObjectToMatrixProperties.h
//  MJQuiz
//
//  Created by Tunado on 8/23/17.
//
//

#ifndef Nam_DragObjectToMatrixProperties_h
#define Nam_DragObjectToMatrixProperties_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_DragObjectToMatrixProperties : public Math_Quiz
{
public:
	static Nam_DragObjectToMatrixProperties* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	//void update(float dt)override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;
	void autoTestingRun(float dt);
private:
    bool init() override;

	std::string optionCol;
	std::string optionRow;
	//matrix config
	int rowNumberMathSlot;
	int colNumberMathSlot;
	int rowNumberMathDrager;
	int colNumberMathDrager;
	//object config
	Size objectSize;
	math::resource::Image* imageBackground;
	//question
	Math_Slot* matrixSlot[10][10];
	Math_Slot* matrixDrager[10][10];
	Math_Object* objectsDrager[10][10];
	std::string style_matrix;
	//object
    Math_Object* objectFrame = nullptr;
	std::vector <math::resource::Image*> imagesObjectCol;
	std::vector <math::resource::Image*> imagesObjectRow;
	math::resource::Image* imagesObjectResult[10][10];
	void genSlot(Math_Slot* slot, math::resource::Image* bg, Vec2 pos);
	void genObject(Math_Object* object, math::resource::Image* image, Vec2 pos,Size size, bool looked);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchedObject(cocos2d::Node* node);
	void onAnswerDone(Node* node);	
	Math_Object* selected_object = nullptr;
	Math_Slot* slotDragerForBack();
	// matrixSize
	float dSize = 0.15;
	//auto test
	int testing_step = 0;
};


#endif /* Nam_DragObjectToMatrixProperties.h */
