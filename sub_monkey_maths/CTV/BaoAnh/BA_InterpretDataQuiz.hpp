//
//  BA_InterpretDataQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 12/2/17.
//

#ifndef BA_InterpretDataQuiz_hpp
#define BA_InterpretDataQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "PictoGraph.h"
#include "TallyChart.h"
#include "Table.h"
#include "cocos2d.h"
#include "GraphBase.h"

class BA_InterpretDataQuiz : public Math_Quiz{
public:
    static BA_InterpretDataQuiz* createQuiz();
    
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
    
    
    
    int kind_max, kind_min;
    
    int obj_id =0;
    //int count ;
    //std::string compare_key;
    //std::string display_type;
    //int max_in_line;
    
    
    std::vector<math::resource::Image*> kind_list;
    // std::vector<std::string> key ;
    //std::vector<std::pair<int, std::vector<int>>> answer_vector;
    
    cocos2d::Size object_size;
    int number_object_max, number_object_min;
    
    //chart config
    bool title_chart, item_title, item_icon;
    std::string chart_type;
    bool different;
    bool vertical;
    std::string chart_title, chart_icon_title;
    int title_size;
    
    //answer config
    //int right_answer; //so luong dap an dung
   // std::string answer_display; //kieu hien thi answer
    int count = 0;
    
    //lists size
    int number_kind;
    
    //    int autoCountList =0;
    //    math::comparision::Key sort_key;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    //    std::vector<int> size_list;
    //
    // std::vector<int> answer_list;
    
//    std::vector<Math_Object*> choice_list;
    // std::vector<std::pair<int, std::vector <int>>> answers_lists;
    //Danh cho game keo tha so luong
//    std::vector<std::pair<int, std::vector<Math_Answer*>>> answers_list;
    
//    std::vector<std::pair<int, std::vector<int>>> answer_list;
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
 //   std::vector<math::common::QuestionData*> list_questiondata;
    
    
    
    //callback Func
//    void onTouchedObject( cocos2d::Node *node);
    void onTouchedAnswer(cocos2d::Node *node);
//    void onAnswerDone(bool correct);
    bool sameNumber(std::vector<int> vector , int number);
    bool clickable = true;
    
    //rieng cua game nay
    std::vector<Math_Answer*> answers;
    std::vector<math::GraphBase*> graph_base;
    math::PictoGraph* graph = nullptr;
    math::TallyChart* tally_graph = nullptr;
    math::Table* table = nullptr;
    std::vector<math::GraphBase::GraphItem> graph_items;
//    std::vector<std::string> name_full;
    std::vector<int> right_answer;
    float bar_h;
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};


#endif /* BA_InterpretDataQuiz_hpp */
