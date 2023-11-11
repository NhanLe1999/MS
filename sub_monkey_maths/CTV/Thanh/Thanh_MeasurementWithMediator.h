//
//  Thanh_MeasurementWithMediator.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_MeasurementWithMediator_h
#define Thanh_MeasurementWithMediator_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_MeasurementWithMediator : public Math_Quiz
{
public:
    enum GameMode {
        measurememt_use_box_touch_object,
        measurememt_use_box_touch_answer,
        measurement_use_ruler
    };
    enum OptionGame{
        measure_leng,
        measure_height
    };
    static Thanh_MeasurementWithMediator* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedObject(cocos2d::Node* node); 
	void onTouchedObject2(cocos2d::Node* node);
	void onMoveObjectEnded(cocos2d::Node *node, cocos2d::Vec2 point);
private:
    bool init() override;

    // ---------------
    // game config
    OptionGame option_game;
    GameMode _mode;
    int number_object;
    bool disable_touch; 
	cocos2d::Vec2 position_on_touch_begin;

    // ---------------
    // Object config
    cocos2d::Size object_size;
    math::resource::Image* object_bg = nullptr;
    math::comparision::Key key_compare=math::comparision::number;
    float scale;

    // ---------------
    // Group config
    cocos2d::Size unit_size;
    std::vector<cocos2d::ui::ImageView*> img_ruler;

    // ----------------
    // Question config
    int index_question;
    int number_question;

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_object;
    math::resource::Image* icon_ruler = nullptr;
    std::vector<math::resource::Image*> icons_question;
    // danh sach cac object, group

    // vector ket qua
    std::vector<Math_Object* > objects;


    //callback Func
    void onAnswerDone(cocos2d::Node*);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done = nullptr;
    std::vector<Math_Answer*> list_answer;
    std::vector<int> list_value_answer;
    cocos2d::Node *correct_answer = nullptr;
    // in game 
    Math_Object *curr_object = nullptr;
};

#endif /* Thanh_MeasurementWithMediator_h */
