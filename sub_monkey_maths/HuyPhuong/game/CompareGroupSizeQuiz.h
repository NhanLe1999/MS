//
//  CompareGroupSizeQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef CompareGroupSizeQuiz_h
#define CompareGroupSizeQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CompareGroupSizeQuiz : public Math_Quiz
{
public:
    static CompareGroupSizeQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum RelationType {
        Equal = 0,
        More,
        Less,
        Enough
    };

    enum GamePlayType {
        Yes_No = 0,
        Answer,
        Choose
    };

    //Game config
    int number_group;
    bool reorder;
    bool align;
    bool gr_same_kind;
    std::string relation;
    std::string game_type;
    bool is_more;
    
    RelationType relation_type;
    GamePlayType gameplay_type;
    
    cocos2d::Size object_size;
    std::vector<std::string> list_groupname;
    int kind_in_group;
    std::string sort_type;
    bool object_arrange;
    std::string object_align;
    std::string group_type;
    int group_column;
    int group_row;
    cocos2d::Size group_size;
    
    int index_answer;
    
    int number_object;
    int group_cap;
    
    Math_Object* selected_object = nullptr;
    
    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
   
    std::vector<math::resource::Image*> group_name_img;

    
    //callback Func
    void onTouchedGroup(Node* node);
    void onAnswered(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    int last_index;
    std::vector <Math_Answer*> answer_tested;
    void autoTestingRun(float dt);
};
#endif /* CompareGroupSizeQuiz_h */
