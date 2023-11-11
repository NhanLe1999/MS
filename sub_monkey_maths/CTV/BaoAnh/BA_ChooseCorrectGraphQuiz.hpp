//
//  BA_ChooseCorrectGraphQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/3/17.
//

#ifndef BA_ChooseCorrectGraphQuiz_hpp
#define BA_ChooseCorrectGraphQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "PictoGraph.h"
#include "TallyChart.h"
#include "Table.h"
#include "GraphBase.h"
#include "cocos2d.h"
#include "GraphBase.h"

class BA_ChooseCorrectGraphQuiz : public Math_Quiz{
public:
    static BA_ChooseCorrectGraphQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    //int number_object_kind;
    int number_object;
    bool same_no;
    
    //int number_right;
    
    int number_total_max, number_total_min;
    int number_object_max, number_object_min;
    
   // int count ;
   // std::string compare_key;
    std::string display_type;
    int max_in_line;
    //int total_object;
    
    std::vector<math::resource::Image*> kind_list;
    //std::vector<std::string> key ;
    //std::vector<std::pair<int, std::vector<int>>> answer_vector;
    
    cocos2d::Size object_size;
    int gr_cap;
    //chart config
    bool title_chart, item_title, item_icon;
    std::string chart_type;
    std::string chart_kind;
    bool vertical;
    std::vector<std::string> chart_name;
    int chart_cap, title_size;
    cocos2d::Size item_size;
    //lists size
    int number_kind;
    
    //int autoCountList =0;
    //math::comparision::Key sort_key;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
        
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onTouchedAnswer(cocos2d::Node* node);
    bool sameNumber(std::vector<int> vector , int number);
    bool clickable = true;
    
    //rieng cua game nay
    math::PictoGraph* graph = nullptr;
    math::TallyChart* tally_graph = nullptr;
    math::Table* table_graph = nullptr;
     std::vector<math::GraphBase*> graph_list;
    float bar_h;
    
    // std::vector<Math_Object*> bg_obj_list;
   // std::vector<Math_Object*> temp_obj_list;
    std::vector<std::vector<Math_Object*>>  item_list;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};
#endif /* BA_ChooseCorrectGraphQuiz_hpp */
