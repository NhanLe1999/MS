//
//  Thanh_ChartHundered.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_ChartHundered_h
#define Thanh_ChartHundered_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_ChartHundered : public Math_Quiz
{
public:
    enum OptionGame{
        drag_number,
        draw_row,
        find_number
    };
    enum OptionChart {
        full_chart,
        half_chart
    };
    enum TouchMode {
        only_row,
        only_column,
        only_block,
        cell_and_block,
        only_cell
    };

    static Thanh_ChartHundered* createQuiz();

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

    void onTouchedObject(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
private:
    bool init() override;

    // ---------------
    // game config
    OptionGame option_game;
    TouchMode touch_mode;
    bool consecutive;
    int number_miss;
    int number_result;
    int min_number, max_number;
    bool enable_sound;
    bool only_touch;

    // ---------------
    // Object config
    cocos2d::Size number_size;
    int obj_id;
    math::comparision::Key key_compare=math::comparision::number;

    // ---------------
    // Chart config
    Math_Group *chart = nullptr;
    cocos2d::Size cell_size;
    math::resource::Image *background = nullptr;
    OptionChart option_chart;
    int number_row, number_column;

 

    // ----------------
    // Question config 

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_number;
    std::vector<math::resource::Image*> icons_result;

    // danh sach cac object, group
    std::vector<Math_Object*> numbers_drag;
    std::vector<Math_Slot*> slots;
    std::vector<Math_Object*> numbers;

    std::vector<std::pair<Math_Slot*, Math_Object*> > pair_slots; // dễ kiểm tra xem đúng sai
    std::set<Math_Object*> choose_list;  // danh sách các object được chọn

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
    Math_Object *object_be_touch = nullptr;
    bool disable_touch;
    cocos2d::Point top_left_touch, bottom_right_touch, first_position_touch;
	 
    static bool cmpImage(math::resource::Image *a, math::resource::Image *b);
    static bool cmpObject(Math_Object *a, Math_Object *b);

	// audio
	bool read_left_2_right;
	int index_audio; 
	math::AudioHelper *my_audio = nullptr;
	std::vector<Math_Object*> list_audio;
	void play_next_audio();
};

#endif /* Thanh_ChartHundered_h */
