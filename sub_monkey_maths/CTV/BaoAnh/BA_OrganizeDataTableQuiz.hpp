//
//  BA_OrganizeDataTableQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/5/17.
//

#ifndef BA_OrganizeDataTableQuiz_hpp
#define BA_OrganizeDataTableQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"

#include "Table.h"
#include "cocos2d.h"
#include "GraphBase.h"

class BA_OrganizeDataTableQuiz : public Math_Quiz{
public:
    static BA_OrganizeDataTableQuiz* createQuiz();
    
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
    int index;
    bool answer_checked;
    cocos2d::Color4B text_color;
    
   // int number_total_min, number_total_max;
    int kind_min, kind_max;
    std::string display_type;
    int max_in_line;
    
    std::vector<math::resource::Image*> kind_list;
    
    //object config
    cocos2d::Size object_size;
    int number_object_max, number_object_min;
    
    //chart config
    bool title_chart, item_title, item_icon;
    std::string chart_type;
    int chart_cap;
    double title_size;
    std::string chart_kind;
    bool vertical;
    bool same_no;
    std::vector<std::string> chart_name;
    
    //lists size
    int number_kind;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> number_img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
 
    std::vector<Math_Object*> dragers;
    //std::vector<Math_Slot*> drag_slots;
    std::vector<Math_Object*> bg_obj_list;
    math::resource::Image* drag_bg = nullptr;
    
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    //callback Func
    void onAnswerDone(bool correct);
    void onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 point);
    bool clickable = true;
    
    //rieng cua game nay
    std::vector<math::Table*> table_list;
    math::Table* table_graph = nullptr;
    float bar_w;
     int k_index;
    cocos2d::Size graph_items_sz;
    cocos2d::ui::ImageView* gr_label = nullptr;
    cocos2d::ui::Text* label_text = nullptr;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};

#endif /* BA_OrganizeDataTableQuiz_hpp */
