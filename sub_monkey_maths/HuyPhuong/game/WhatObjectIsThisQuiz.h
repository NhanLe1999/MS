//
//  WhatObjectIsThisQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef WhatObjectIsThisQuiz_h
#define WhatObjectIsThisQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class WhatObjectIsThisQuiz : public Math_Quiz
{
public:
    static WhatObjectIsThisQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    std::string game_type;
    bool rotation;
    int number_object;
    bool random_size;
    bool answer_true;
    
    cocos2d::Size object_size;
    std::string background_key;
    std::string fit_object;
    std::string display;
    int max_in_line;
    
    std::vector<math::resource::Image*> answer_list;

    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    std::vector<int> rotate_list;
    
    //callback Func
    void onAnswered(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    int last_index;
    std::vector <Math_Answer*> answer_tested;
    void autoTestingRun(float dt);
};
#endif /* WhatObjectIsThisQuiz_h */
