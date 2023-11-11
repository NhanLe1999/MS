//
//  BA_TableValueQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/4/17.
//

#ifndef BA_TableValueQuiz_hpp
#define BA_TableValueQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "Table.h"
#include "cocos2d.h"
#include "GraphBase.h"

class BA_TableValueQuiz : public Math_Quiz{
public:
    static BA_TableValueQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int number_max, number_min;
    
    //obj config
    int obj_id =0;
    cocos2d::Size object_size;
    
    //chart config
    bool title_chart, item_title, item_icon;
    std::string chart_type;
    bool vertical;
    double chart_size;
    
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> number_list;
    math::resource::Image* drag_bg_img = nullptr;
    //danh sach keo tha
    std::vector<Math_Object*> dragers;
    std::vector<Math_Object*> dis_slots;

    //danh sach chu so
    std::vector<int> number_obj;
    
    
    //callback Func
    void onAnswerDone(bool correct);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    
    //rieng cua game nay
//    math::Table* table;
    Math_Object* general_object = nullptr;
    int display_level;
    cocos2d::ui::ImageView* background;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_TableValueQuiz_hpp */
