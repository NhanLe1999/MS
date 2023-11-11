//
//  Nam_WayToMakeANumber
//  MJQuiz
//
//  Created by Tunado on 1/11/17.
//
//

#ifndef Nam_WayToMakeANumber_h
#define Nam_WayToMakeANumber_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_WayToMakeANumber : public Math_Quiz
{
public:
    static Nam_WayToMakeANumber* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
	float delay = 0;
    //Game config
	// sentence
	int resultOfSentence;
	math::resource::Image* image_result = nullptr;
	cocos2d::ui::ImageView* frame_box = nullptr;
	math::resource::Image* image_slot = nullptr;
	math::resource::Image* bg_sentence = nullptr;
    math::resource::Image* bg_drager = nullptr;
	std::vector <Math_Object*> dragers;
    std::vector <Math_Object*> non_dragers;
    cocos2d::Size size_sentence;
    std::string game_mode;
	int numberOfSlot;
	std::vector <int> slotId;
	std::vector <int> bUsed;
	std::vector <int> aUsed;
	void genSentence(int id, Math_Object* object, int a, int b, int result);
	Math_Object* selected_object = nullptr;
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
	void onMoveObjectEndedSlot(Node* node, Vec2 point);
	void onAnswerDone(Node* node);
};

#endif /* Nam_WayToMakeANumber_h */
