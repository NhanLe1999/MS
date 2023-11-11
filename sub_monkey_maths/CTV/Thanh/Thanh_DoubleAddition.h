//
//  Thanh_DoubleAddition.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_DoubleAddition_h
#define Thanh_DoubleAddition_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_DoubleAddition : public Math_Quiz
{
public:
    enum ObjectOption{
        number,
        text
    };
    enum Hide{
        operand_1,
        operand_2,
		operand_3,
        equal
    };
    static Thanh_DoubleAddition* createQuiz();

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

    // game config
    Hide hide;
    bool operand_is_the_same;
    bool slot_in_group_row;
    bool hide_group;
    int counting_xx;
	bool add_three_operand;
	bool disable_touch;
    
    // ---------------
    // Object config
    cocos2d::Size object_size;
    math::resource::Image* object_bg = nullptr;
    ObjectOption object_option;
    bool status_copy;
    math::comparision::Key key_compare=math::comparision::number;
    std::vector<std::pair<int, cocos2d::Vec2> > root_position;

    // ---------------
    // Group config
    math::group::GroupType group_type;
    cocos2d::Size group_size;
    cocos2d::Size element_group_size;
    math::resource::Image* group_bg = nullptr;
    int number_row, number_column;


    //----------------
    // slot config
    cocos2d::Size slot_size;
    math::resource::Image* slot_bg = nullptr;

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    math::resource::Image *img_a = nullptr, *img_b = nullptr, *img_c = nullptr;
    std::vector<math::resource::Image*> icons_result;
    math::resource::Image* icon_group = nullptr;
    std::vector<cocos2d::ui::ImageView*> image_line;

    // danh sach cac object, group
    std::vector<Math_Object*> results;
    std::vector<Math_Object*> objects_copy;
    std::vector<Math_Slot*> slots;
    std::vector<cocos2d::Node*> row_1, row_2; // vector node để xoá theo từng hàng

    std::vector<int> results_value;

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
    int a, b, c, result;
	std::vector<Math_Object*> all_objects_row_1, all_objects_row_2;
	std::vector<Math_Group*> all_groups;
	Math_Slot *slot_1 = nullptr, *slot_2 = nullptr;
};

#endif /* Thanh_DoubleAddition_h */
