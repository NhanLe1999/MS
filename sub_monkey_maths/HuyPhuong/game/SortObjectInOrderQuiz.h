//
//  SortObjectInOrderQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef SortObjectInOrderQuiz_h
#define SortObjectInOrderQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class SortObjectInOrderQuiz : public Math_Quiz
{
public:
    static SortObjectInOrderQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    math::comparision::Key compare_key;
    bool sort_asc;
    
    cocos2d::Size slot_size;
    cocos2d::Size object_size;
    std::string fit_object;
    int number_object;
    std::string show_icon = "icon";
    
    //display
    std::string display_type;
    int max_in_line;
    
    //background image
    math::resource::Image*img_bg = nullptr;
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;

    std::string dif_type;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    cocos2d::DrawNode* drawer = nullptr;
    int move_aligned=0;
    
    std::vector<Math_Object*> answer_list;
    Math_Object* temp_object=NULL;
    Math_Object* selected_object=NULL;
    
    //callback Func
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
    
};


#endif /* SortObjectInOrderQuiz_h */
