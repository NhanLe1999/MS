//
//  BA_CountBlockQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/19/17.
//
//

#ifndef BA_CountBlockQuiz_hpp
#define BA_CountBlockQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class BA_CountBlockQuiz : public Math_Quiz{
public:
    static BA_CountBlockQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    int row, column;
    int total_object;
    int obj_id =0;
    
    int number_object_max, number_object_min;
    
    cocos2d::Size object_size;
    std::vector<int> answer_list;
    
    int group;
    bool game_mode;
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    std::vector<Math_Object*> blocks_object;
    std::vector<Math_Object*> ones_object;

    //callback Func
    void onTouchedAnswer(Node* node);
    bool clickable = true;
    //rieng cua game nay
    float bar_w, bar_h;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_CountBlockQuiz_hpp */
