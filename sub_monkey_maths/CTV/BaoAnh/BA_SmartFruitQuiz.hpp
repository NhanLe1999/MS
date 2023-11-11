//
//  BA_SmartFruitQuiz.hpp
//  MJQuiz
//
//  Created by AnhB Nguyen on 8/31/17.
//
//

#ifndef BA_SmartFruitQuiz_hpp
#define BA_SmartFruitQuiz_hpp


#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"

class BA_SmartFruitQuiz : public Math_Quiz{
public:
    static BA_SmartFruitQuiz* createQuiz();
    
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
    int number_right;
    int display_mode;
    
    cocos2d::Size object_size;
    
    
    int number_object_max, number_object_min,value_object_max, value_object_min;
    
    int result;

    
    //lists size
    int number_kind;
    
    int group_cap;
    cocos2d::Size group_size;
    
    std::vector<std::pair<std::string, int>> expr_list;
    std::vector<Math_Object*> fruitList;
    
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    //danh sach so luong cho moi loai obj
    std::vector<int> number_list;
    //    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    //callback Func
    void onTouchedObject(Node* node);
    void onTouchedGroup(Node* node);
    void DoNothing(Node* node);

    void onAnswerDone(bool correct);
    
    
    //rieng cua game nay
    std::vector<Math_Object*> createFruitList(int result);
    std::vector<std::pair<std::string, int>> createExprList(int result);
//    std::vector<int> countList;
    
    std::vector<Math_Object*> resultList;
    
    std::pair<std::string, int> randomExpression(int result, int count);
    int randomWrongResult(int result);
//    void checkGroup(int gr_id, Math_Object* object);
    void lockedObj(bool lock);
    bool checkSameExpr(std::string expr);
    bool checkSelectedObj();
    float bar_h;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);
    
};


#endif /* BA_SmartFruitQuiz_hpp */
