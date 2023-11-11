//
//  Nam_DragObjectToMeasureHeight.h
//  MJQuiz
//
//  Created by Tunado on 8/30/17.
//
//

#ifndef Nam_DragObjectToMeasureHeight_h
#define Nam_DragObjectToMeasureHeight_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_DragObjectToMeasureHeight : public Math_Quiz
{
public:
	static Nam_DragObjectToMeasureHeight* createQuiz();

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
    std::string mode_game;
    math::resource::Image* imageBlock = nullptr;
	// character
	Math_Object* character = nullptr;
	int heighOfCharacter;
	math::resource::Image* imageCharacter = nullptr;
	//slot
	Math_Object* line1 = nullptr;
	Math_Object* line2 = nullptr;
	int indexSlotCurrent;
	// drager
	Size dragerSize;
	math::resource::Image* imageDrager = nullptr;
    math::resource::Image* image_answer = nullptr;
	void updateSlotHeightOfCharacter();
	std::vector <Math_Slot*> slotHeightOfCharacter;
	math::resource::Image* box_slot = nullptr;
	void onTouchedDrager(cocos2d::Node* node);
	void onMoveDragerEnded(cocos2d::Node* node, Vec2 point);
	Math_Object* selected_object = nullptr;
	Math_Object* objectDrager = nullptr;
	std::vector <Math_Object*> _dragers;
    void fixAnchorPoint(Math_Slot* slot);
	void genDrager(Math_Object* obj);
	// react drager
	cocos2d::Rect dragersRect;
	//auto test
	int testing_step = 0;
	// numbers answer
	std::string displayType;
	std::vector <int> numbers_result;
	void onAnswerDone(Node* node, int index);
	
};


#endif /* Nam_DragObjectToMeasureHeight.h */
