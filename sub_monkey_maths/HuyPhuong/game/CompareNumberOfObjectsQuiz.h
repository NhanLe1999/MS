//
//  CompareNumberOfObjectsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/28/17.
//
//

#ifndef CompareNumberOfObjectsQuiz_h
#define CompareNumberOfObjectsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class CompareNumberOfObjectsQuiz : public Math_Quiz
{
public:
    static CompareNumberOfObjectsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    enum GameRuleType {
        More = 0,
        Fewer,
        Enough
    };

    //Game config
    std::string game_type;
    std::string game_rule;
    int number_kind;
    bool arange_kind;
    bool answer_icon;
    int max_line;
    int max_numbericon;
    
    GameRuleType gamerule_type;
    
    cocos2d::Size object_size;
    int index_answer;
    std::string background_key;
    std::vector<std::vector<Math_Object*>> all_objects;

    math::resource::Image*img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    
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
#endif /* CompareNumberOfObjectsQuiz_h */
