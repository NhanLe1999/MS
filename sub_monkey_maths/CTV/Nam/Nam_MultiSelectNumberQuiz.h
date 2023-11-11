//
//  Nam_MultiSelectNumberQuiz
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_MultiSelectNumberQuiz_h
#define Nam_MultiSelectNumberQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_MultiSelectNumberQuiz : public Math_Quiz
{
public:
    static Nam_MultiSelectNumberQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    void update(float dt) override;
private:
    bool init() override;
	float delay = 0;
    //Game config
    bool isHide;
	void onAnswerDone(Node* node, std::string str_ans);
    Size object_size;
    std::vector<cocos2d::ui::ImageView*> images_box_choice;
    //std::vector<Math_Slot*> slots;
    std::vector<Math_Object*> true_answers;
    std::vector<Math_Object*> false_answers;
    std::vector<Math_Object*> objects_gen_truoc;
    void onTouchedObject(cocos2d::Node* node);
    void onTouchedAnswer(cocos2d::Node* node);
    bool onTouchBegan(Touch* touch, Event* event);
    void genNumberBox(int id, Math_Object* obj, int num,math::resource::Image* img_box, Color4B color);
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
};

#endif /* Nam_MultiSelectNumberQuiz_h */
