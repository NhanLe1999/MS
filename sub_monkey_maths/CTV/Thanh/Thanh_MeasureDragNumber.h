//
//  Thanh_MeasureDragNumber.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_MeasureDragNumber_h
#define Thanh_MeasureDragNumber_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_MeasureDragNumber : public Math_Quiz
{
public:
    enum ObjectOption{
        number,
        number_text
    };
    enum OptionGame{
        measure_leng,
        measure_height
    };
    static Thanh_MeasureDragNumber* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedObject(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
	void update(float dt);
private:
    bool init() override;

    // ---------------
    // game config
    OptionGame option_game;
    int max_ruler;
    float scale;
    std::string task;
    std::vector<std::string> tasks;
    bool next_task;
    int index_question;
    std::string _mode_question;
    bool disable_touch;
	float time_measurement;
	std::vector<bool> task1;
	Math_Object *current_object_measurement = nullptr;
	std::string sapxep;

    // ---------------
    // Object config
    cocos2d::Size number_size;
    math::resource::Image* number_bg = nullptr;
    ObjectOption number_option;
    int obj_id;
    math::comparision::Key key_compare=math::comparision::number;

    //----------------
    // slot config
    cocos2d::Size slot_size;
    math::resource::Image* slot_bg = nullptr;

    // ----------------
    // Question config
    std::vector<int> list_questiondata;

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_number_drag;
    std::vector<math::resource::Image*> icons_object;
    math::resource::Image* icon_ruler_drag = nullptr;

    // danh sach cac object, group
    std::vector<Math_Object*> numbers_drag;
    std::vector<Math_Slot*> slots;
    Math_Object* ruler_drag = nullptr;
    std::vector<Math_Object*> objects;

    // vector ket qua
    std::vector<std::pair<int, bool> > results_value;

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
    int equal;
};

#endif /* Thanh_MeasureDragNumber_h */
