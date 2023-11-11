//
//  MoveFruitToBagQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/20/17.
//
//

#ifndef MoveFruitToBagQuiz_h
#define MoveFruitToBagQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class MoveFruitToBagQuiz : public Math_Quiz
{
public:
    static MoveFruitToBagQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    std::string game_mode;
    int number_group;
    bool check_move;
    
    int number_kind;
    cocos2d::Size object_size;
    bool names_enable;
    std::string group_name_type;
    std::vector<std::string> list_groupname;    
    cocos2d::Size group_size;
    
    int number_object;
    int group_cap;
    
    std::vector<int> list_number_drag;
    std::vector<int> number_list;
    
    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> img_number_list;
    std::vector<math::resource::Image*> group_name_img;

    
    //danh sach so luong cho moi loai obj
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
    Math_Group* tree;
    int count_move=0;
    Math_Object* selected_object=NULL;
    
    //callback Func
    void onTouchedObject(Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void onAnswerDone(Node* node);
    bool clickable=true;
    
    //debugging mode
    int testing_step=0;
    //Smart autotesy
    void autoTestingRun(float dt);
};

#endif /* MoveFruitToBagQuiz_h */

