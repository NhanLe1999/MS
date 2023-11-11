//
//  SumAndSubQuiz.hpp
//  MJQuiz
//
//  Created by Admin on 8/7/17.
//
//

#ifndef SumAndSubQuiz_hpp
#define SumAndSubQuiz_hpp


#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class BA_SumAndSubQuiz : public Math_Quiz{
public:
    static BA_SumAndSubQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    void genOperator(std::string oprt);
    void genObject(math::resource::Image* drag_img);
    void genGroup(int i, int random_img, math::group::GroupType gr_type, math::resource::Image* group_bg,   std::vector<math::resource::Image*> ob_list);
    void genDrager(math::resource::Image* drag_img);
    void genSlot(math::resource::Image* slot_bg);
    void genQuestion();
    

protected:
    bool init() override;
    
    int grade;
    int game_mode ;
    //Game config
    int number_group;
    bool copy_object;
    std::string sort_by;
    std::string sorted;
    std::string align_group_object;
    math::comparision::Key sort_key;
 
    
    cocos2d::Size object_size;
    std::vector<std::string> list_groupname;
    
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    
    cocos2d::Size group_size;
    cocos2d::Size oprt_size;
    cocos2d::Size slot_size;
    
    int obj_id=0;
    

    //value range
    int min_range;
    int max_range;
    
    //lists size
   // int number_object;
    int number_kind;
    int number_drager;
    int number_slot;
    int number_oprt;
    
    //object random numbers
    int number_max, number_min;
    
    //drager values
    int value_min;
    int value_max;
    
    //---------------------------
    std::vector <int> slot_index;
    std::vector <int> drag_values;
    
    cocos2d:: Size drager_size;
    
    //operator
    std::string oprt_json;
    std::vector<std::string> oprt_List;
    
    int group_cap;
    
    int counting = 1;
    bool display_number;
    
    //merge group
    cocos2d::Vec2 center_pos;
 
    
    
    //danh sach doi tuong keo tha
    std::vector<Math_Object*> _dragers;
    std::vector<Node*> _slot_object;
    std::vector<int> number_objects;
    std::vector<Math_Object*> math_symbols;
    std::vector<Math_Object*> _group_list;
    std::vector<int> answer_list;
    
    std::vector<int> value_list;
    
    std::vector<Math_Answer*> multiChoice;
    
    std::vector<Math_Object*> copy_obj;
    //
    std::vector<int> list_number_drag; //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    //std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*>icon_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    //Math_Object* edge_object;
    
    std::vector<math::resource::Image*> bg_list;
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(bool correct);
    
    void onTouchedAnswer(Node* node);
    bool clickable = true;
    //rieng cua game nay
    std::pair<int, int> randomColRow(int number);
    std::vector<Math_Object*> createGroupList(int obj_id, int obj, std::vector<math::resource::Image*> obList, int random );
    int checkResult();
    bool checkSum(int check);
    
    int sum = 0;
    int number_counting = 0;
    
    void createObjectAndSum();
    void createSum(bool repeatNo);
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

};


#endif /* SumAndSubQuiz_hpp */
