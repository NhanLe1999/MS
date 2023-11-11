//
//  CountAndDragQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/18/17.
//
//

#ifndef CountAndDragQuiz_h
#define CountAndDragQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CountAndDragQuiz : public Math_Quiz
{
public:
    static CountAndDragQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    bool counting;
    bool count_sequence;
    bool display_number;
    int number_slot;
    int skip_count=1;
    bool ordinal = false;
    
    cocos2d::Size object_size;
    std::string object_bg;
    bool object_value;
    std::string object_value_position;
    int number_object;
    int number_kind;
    int number_drager;
    cocos2d::Size drager_size;
    math::comparision::Key key_compare=math::comparision::number;
    
    math::resource::Image* img_bg = nullptr;

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> img_drager;

    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    std::vector<int> drag_values;
    std::vector<int> slot_index;
    int number_counting=0;
    int count_index=0;
    
    //Danh sach cac doi tuong keo tha.
    std::vector<Math_Object*> _dragers;
    std::vector<Math_Object*> _added_objects;
    std::vector<Node*> _slot_object;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
};

#endif /* CountAndDragQuiz_h */
