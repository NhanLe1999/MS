//
//  DragObjectToTwoGroupAndCaculate
//  MJQuiz
//
//  Created by Tunado on 7/9/17.
//
//

#ifndef Nam_DragObjectToTwoGroupAndCaculate_h
#define Nam_DragObjectToTwoGroupAndCaculate_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_DragObjectToTwoGroupAndCaculate : public Math_Quiz
{
public:
	static Nam_DragObjectToTwoGroupAndCaculate* createQuiz();
	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;	
	void autoTestingRun(float dt);
private:
	bool init() override;
	int number1, number2, result;
	// drager
	cocos2d::Size dragerSize;
	// group
	std::string alignGroup, typeGroup;
	math::group::GroupType gr_type;
	cocos2d::Size groupSize;
	int colFrame, rowFrame;
	void genGroup(Math_Group* gr,int number);
	void createGroupObject(Math_Object* obj);
    cocos2d::ui::ImageView* line1 = nullptr;
    cocos2d::ui::ImageView* line2 = nullptr;
	//drager
	math::resource::Image* img_drager = nullptr;
	math::resource::Image* slot_bg = nullptr;
	math::resource::Image* drag_bg = nullptr;
	math::resource::Image* img_bg_number = nullptr;
	std::vector <Math_Object*> objects_drager;
	void onTouchedObject(Node* node);
	void onMoveObjectEnded(Node* node, cocos2d::Vec2 point);
	void onMoveObjectEndedSlot(Node* node, cocos2d::Vec2 point);
	Math_Object* selected_object;
	//autotest
	int testing_step = 0;
	// answer
	std::vector <int> number_result;
	void onAnswerDone(cocos2d::Node* node);
	void onAnswerResult(cocos2d::Node* node, int index);
	// caculation
	std::vector <Math_Object*> objectsCaculation;
	void createCaculationObject(Math_Object*, std::string valueText, bool looked);
	Math_Slot* slotResult = nullptr;
};

#endif /* Nam_DragObjectToTwoGroupAndCaculate_h */
