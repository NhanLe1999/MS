//
//  Nam_CompareObjectVsGroup.h
//  MJQuiz
//
//  Created by Tunado on 8/10/17.
//
//

#ifndef Nam_CompareObjectVsGroup_h
#define Nam_CompareObjectVsGroup_h

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"
//#include "AudioEngineHelper.h"
using namespace cocos2d;
class Nam_CompareObjectVsGroup : public Math_Quiz
{
public:
	static Nam_CompareObjectVsGroup* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart()override;
	void startQuestion()override;
	void onExplanation(bool correct)override;
	void onComplete()override;
	void autoTesting() override;	
	void autoTestingRun(float dt);
private:
	bool init() override;
	// Game config
	cocos2d::Size panel_size = cocos2d::Size(900, 750);
	std::string game_mode;
	int number_object, number_object_in_group;
	math::resource::Image* image_group = nullptr;
	std::string key;
	// object
    Math_Object* object = nullptr;
	cocos2d::Size object_size;
	std::string object_bg;
	std::string object_type;
	math::resource::Image* img_bg = nullptr;
	std::vector <math::resource::Image*> all_images_operator;
	std::string value_type_object;
	// drager
	Math_Object* object_greather = nullptr;
	Math_Object* object_equals = nullptr;
	Math_Object* object_less = nullptr;
	std::vector <Math_Object*> object_dot;
	cocos2d::Size drager_size;
	std::string display_drager;
	Vec2 first_pos;
	//slot
	Math_Slot* slot = nullptr;
	//group
	Size group_size;
	Size object_group_size;
	std::string object_align;
	std::string group_type;
	int group_column;
	int group_row;
	void createGroupObject(Math_Object* object);
	int maxObjectInGroup;
	math::group::GroupType gr_type;
	Math_Object* selected_object = NULL;
	//done
	Math_Answer* btn_done = nullptr;
	//touch
	void onAnswerDone(Node* node);
	void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void onTouchedObject(cocos2d::Node* node);
	//autotest
	int testing_step = 0;
	std::vector <Math_Object*> objects_dot;
	
};

#endif /* Nam_CompareObjectVsGroup_h */
