//
//  Nam_IspyWithSortInLineQuiz
//  MJQuiz
//
//  Created by Tunado on 15/11/17.
//
//

#ifndef Nam_IspyWithSortInLineQuiz_h
#define Nam_IspyWithSortInLineQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"
using namespace cocos2d;
class Nam_IspyWithSortInLineQuiz : public Math_Quiz
{
public:
    static Nam_IspyWithSortInLineQuiz* createQuiz();
    
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
    int current_index;
	void onAnswerDone(Node* node, std::string str_ans);
    Size shape_size;
    int number_shape;
    std::string yeu_cau_1, yeu_cau_2;
    int number_line;
    std::vector <Math_Object*> shapes_question;
    std::vector <Math_Slot*> lines;
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
	void onTouchedObject(cocos2d::Node* node);
    void onMoveObjectEnded(Node* node, Vec2 position);
    Math_Object* selected_object = nullptr;
};

#endif /* Nam_IspyWithSortInLineQuiz_h */
