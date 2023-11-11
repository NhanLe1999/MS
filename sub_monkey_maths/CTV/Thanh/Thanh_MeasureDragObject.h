//
//  Thanh_MeasureDragObject.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_MeasureDragObject_h
#define Thanh_MeasureDragObject_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_MeasureDragObject : public Math_Quiz
{
public:
    enum ObjectOption{
        number,
        text
    };
    enum OptionGame{
        measure_leng,
        measure_height
    };
    static Thanh_MeasureDragObject* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedObject(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
private:
    bool init() override;
    std::vector<math::resource::Image*> list_object, list_ruler, list_number;
    std::vector<std::pair<int, int> > results;

    // ---------------
    // game config
    OptionGame option_game;
    float scale;

    // ---------------
    // Object config
    cocos2d::Size number_size;
    math::resource::Image* number_bg = nullptr;
    ObjectOption number_option;
    int obj_id;
    math::comparision::Key key_compare=math::comparision::number;
    std::string _path;

    //----------------
    // slot config
//    cocos2d::Size slot_size;
    math::resource::Image* slot_bg = nullptr;

    // ----------------
    // Question config
    std::vector<int> list_questiondata;

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_result;
    std::vector<math::resource::Image*> icons_object_drag;
    math::resource::Image* icon_ruler = nullptr;
    math::resource::Image* measure_box = nullptr;
	std::vector<cocos2d::ui::ImageView*> list_imageview;

    // danh sach cac object, group
    std::vector<std::pair<Math_Slot*, Math_Object*> > pair_slots;
    std::vector<Math_Object*> objects_drag;
    std::vector<Math_Object*> measure_box_list;
    Math_Object* ruler;
    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done;
};

#endif /* Thanh_MeasureDragObject_h */
