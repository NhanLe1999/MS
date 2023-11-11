//
//  BA_PictoGraphQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 11/23/17.
//

#ifndef BA_PictoGraphQuiz_hpp
#define BA_PictoGraphQuiz_hpp

#include <stdio.h>
#include <algorithm>
#include "Math_Quiz.h"
#include "PictoGraph.h"
#include "cocos2d.h"
#include "GraphBase.h"

class BA_PictoGraphQuiz : public Math_Quiz{
public:
    static BA_PictoGraphQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int number_object;
    int number_total_max, number_total_min;

    //object config
    cocos2d::Size object_size;
    int number_object_max, number_object_min;
    std::string display_type;
    int max_in_line;
    int gr_cap;
    //chart config
    bool title_chart, item_title, item_icon;
    std::string chart_type;
    double title_size;
    bool vertical;
    std::string chart_title, chart_icon_title;
    
    //lists size
    int number_kind;
    
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> kind_list;
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    //std::vector<math::common::QuestionData*> list_questiondata;

    
    //callback Func
    void onAnswerDone(bool correct);
   // bool clickable = true;
    
    //rieng cua game nay
    math::PictoGraph* graph = nullptr;
    std::vector<Math_Object*> item_list;
    float bar_h;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_PictoGraphQuiz_hpp */
