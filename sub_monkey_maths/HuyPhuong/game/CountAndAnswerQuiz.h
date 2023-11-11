//
//  CountAndAnswerQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/15/17.
//
//

#ifndef CountAndAnswerQuiz_h
#define CountAndAnswerQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CountAndAnswerQuiz : public Math_Quiz
{
public:
    static CountAndAnswerQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;
        
private:
    bool init() override;

    //Game config
    bool counting;
    bool count_sequence;
    bool count_revert;
    int revert_total;

    std::string missing_type;
    int index_missing;
    bool frame_show;
    int frame_column;
    int frame_row;
    bool arrange_line = true;
    int start_number = 0;
    int skip_counting = 1;
    bool ordinal = false;

    cocos2d::Size object_size;
    cocos2d::Size answer_size;

    std::string object_bg;
    bool object_value;
    std::string object_value_position;
    int number_object;
    int number_kind;
    int number_answer;
    int correct_index;
    std::vector<cocos2d::Vec2> frames;
    

    std::vector<Math_Object*> list_skipcount;

    //danh sach image cho moi loai obj
    math::resource::Image*img_bg;
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> list_answer_image;

    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    bool answer_display_number = true;

    int number_counting=0;
    int count_index=0;

    
    //callback Func
    void onTouchedObject(Node* node);    
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
	math::lang::MathLanguage lang;
};

#endif /* CountAndAnswerQuiz_h */
