//
//  PatternCopyQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#ifndef PatternCopyQuiz_h
#define PatternCopyQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

#include "PatternCreator.h"

class PatternCopyQuiz : public Math_Quiz
{
public:
    static PatternCopyQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:

    ~PatternCopyQuiz();

    bool init() override;
    
    //Game config
    int number_object;
    int number_slot;
    bool make_copy;
    bool is_flip_x;
    bool object_align_bot = false;

    cocos2d::Size object_size;
    std::string object_bg;
    
    PatternCreator* pattern = nullptr;
    math::comparision::Key key_compare;
    
    
    math::resource::Image* img_bg = nullptr;
    std::vector<math::resource::Image*> img_list;

    //Danh sach cac doi tuong keo tha.
    std::vector<Math_Object*> _dragers;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch;
    void autoTestingRun(float dt);
};

#endif /* PatternCopyQuiz_h */
