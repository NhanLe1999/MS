//
//  BA_ChooseTwoMakeOneQuiz.hpp
//  MJQuiz
//
//  Created by Admin on 8/23/17.
//
//

#ifndef BA_ChooseTwoMakeOneQuiz_hpp
#define BA_ChooseTwoMakeOneQuiz_hpp

#include <stdio.h>
#include "Math_Quiz.h"
#include "cocos2d.h"


class BA_ChooseTwoMakeOneQuiz : public Math_Quiz{
public:
    static BA_ChooseTwoMakeOneQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    
private:
    bool init() override;
    
    //Game config
    int number_object_list;
    int number_object;
    int number_right;

    int obj_id =0;
    
    bool autoCheck;
    
    
    cocos2d::Size object_size;
    cocos2d::Size result_size;

    
    int number_object_max, number_object_min,value_object_max, value_object_min;
    
    
    //lists size
    int number_kind;

    int autoCountList =0;
    
    std::vector<std::vector<Math_Object*>> object_list;
    std::vector<std::pair<int, std::vector <int>>> answers_lists;
    //Danh cho game keo tha so luong
    
    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> img_list;
    std::vector<math::resource::Image*> icon_list;
    //danh sach so luong cho moi loai obj
    
    //    //danh sach size cho tung doi tuong
    std::vector<int> size_list;
    //std::vector<math::common::QuestionData*> list_questiondata;
    
    
    
    //callback Func
    void onTouchedObject(Node* node);
    void onAnswerDone(bool correct);
    
    
    //rieng cua game nay
    std::vector<int> createValueList();
    bool checkSameValue(int number, std::vector<int> value_list);
    int checkSelectedObject(int i);
    void lockObj(int i, bool lock);
    bool checkResult(int i);
    void checkInline(Math_Object* object);
    cocos2d::DrawNode* draw;
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    //debugging mode
    void autoTestingRun(float dt);
    
};


#endif /* BA_ChooseTwoMakeOneQuiz_hpp */
