//
//  ChoosePerfectObjectsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef ChoosePerfectObjectsQuiz_h
#define ChoosePerfectObjectsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class ChoosePerfectObjectsQuiz : public Math_Quiz
{
public:
    static ChoosePerfectObjectsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    //callback Func
    void onTouchObject(Node* node);
    
    //debugging mode
    int testing_step=0;
    void autoTestingRun(float dt);
    
    bool revert_answer = false;
    
    //att
    bool is_out_cmp = false;
    std::string out_cmp_prop;
    std::string display_object;
    cocos2d::Size object_size;
    
    int number_total;
    int number_correct;
    
    bool is_cmp_size = false;
    bool is_fuz_size = false;
    bool is_big = false;
    
    std::vector<std::pair<std::string, std::string>> prop_value_list;
    
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_list_wrong;

    math::resource::Image* out_img = nullptr;
    int number_touch = 0;
    
};
#endif /* ChoosePerfectObjectsQuiz_h */
