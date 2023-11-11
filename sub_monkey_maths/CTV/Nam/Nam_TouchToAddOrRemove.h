//
//  AddObjectAndAnswerQuiz_Nam.h
//  MJQuiz
//
//  Created by Tunado on 4/10/17.
//
//

#ifndef Nam_TouchToAddOrRemove_H
#define Nam_TouchToAddOrRemove_H
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_TouchToAddOrRemove : public Math_Quiz {
public:
	static Nam_TouchToAddOrRemove* createQuiz();

	void initQuiz(mj::model::Game game) override;
	void onStart() override;
	void startQuestion() override;
	void onExplanation(bool correct) override;
	void onComplete() override;
	void autoTesting() override;
	void update(float dt) override;
private:
	bool init() override;
	void addObjectToGroup(cocos2d::Node* node);
	void removeObject(cocos2d::Node* node);
	void onAnswerDone(cocos2d::Node* node);
	void autoTestingRun(float delaytime);
	//game config
	int objectNumberAddOrRemove;
	int result;
	int testing_step = 0;
	//group
	int groupNumber;
	Size objectSize = Size(72, 72);
	math::resource::Image* objectImage = nullptr;
	// genObject(Math_Object* obj, math::resource::Image* img);
	// add and  remove
	void genAddAndRemove(Math_Object* obj, math::resource::Image* img, int num, math::resource::Image* background);
	Math_Object* add = nullptr;
	std::vector<Math_Object*> objects_gen_truoc;
};

#endif
