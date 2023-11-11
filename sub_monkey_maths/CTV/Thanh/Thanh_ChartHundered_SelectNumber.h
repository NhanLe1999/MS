//
//  Thanh_ChartHundered_SelectNumber.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_ChartHundered_SelectNumber_h
#define Thanh_ChartHundered_SelectNumber_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_ChartHundered_SelectNumber : public Math_Quiz
{
public:
    enum OptionGame{
        select_1,
        select_n
    };
    enum Operator{
        add,
        sub,
        random
    };
    enum TouchOption{
        block,
        cell,
        a_to_b,
        cell_and_block,
        cell_and_a_to_b
    };

    static Thanh_ChartHundered_SelectNumber* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancel(cocos2d::Touch * touch, cocos2d::Event * event);

private:
    bool init() override;

    // ---------------
    // game config
    OptionGame game_option;
    int origin;
    Operator oper;
    int range;
    int total_number_need_select;
    TouchOption touch_option;
    bool reset_before_touch, only_more, only_less;

    // ---------------
    // Chart config
    Math_Group *chart = nullptr;
    cocos2d::Size cell_size;
    math::resource::Image *background = nullptr;
    int number_row, number_column;
    int min_number, max_number;
    int first_row;
    bool first_number_is_1;

    // ----------------
    // Question config 

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_number;

    std::set<Math_Object*> choose_list, tmp_choose_list;  // danh sách các object được chọn

    Math_Object *object_be_touch = nullptr;

    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done = nullptr;

    // in game
    bool touch_off;
    bool only_touch;
    cocos2d::Point first_position_touch, second_position_touch;

    void addEvents();
    static bool cmpImage(math::resource::Image *a, math::resource::Image *b);
    static bool cmpObject(Math_Object *a, Math_Object *b);
    std::set<Math_Object*> getObjectInArea(cocos2d::Point first_point, cocos2d::Point second_point, TouchOption touch_opt);
};

#endif /* Thanh_ChartHundered_SelectNumber_h */
