//
//  CompareTwoObjectsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef CompareTwoObjectsQuiz_h
#define CompareTwoObjectsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CompareTwoObjectsQuiz : public Math_Quiz
{
public:
    static CompareTwoObjectsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    std::string compare_type;
    std::string compare;
    std::string sub_compare;
    std::string game_play;
    bool align;
    bool re_order;
    
    cocos2d::Size object_size;
    std::string background_key;
    std::string img_same;
    std::string fit_object;

    bool answer_true;
    cocos2d::ui::Text* clause = nullptr;
    std::vector<math::resource::Image*> answer_list;
    
    math::resource::Image*img_bg;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    std::string clause_string;
   
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
#endif /* CompareTwoObjectsQuiz_h */
