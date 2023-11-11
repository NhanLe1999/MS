//
//  Thanh_Matching2C.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_Matching2C_h
#define Thanh_Matching2C_h

#include <stdio.h>
#include "Math_Quiz.h"

struct MyPair{
    cocos2d::Node *node = nullptr;
    cocos2d::ui::ImageView* plug = nullptr;
	cocos2d::Vec2 start_plug_position;
    cocos2d::ui::ImageView* light = nullptr;
    cocos2d::DrawNode* draw = nullptr;
    bool panel_1;
    bool disable;
    cocos2d::DrawNode *start, *end, *avg;
};

class Thanh_Matching2C : public Math_Quiz
{
public:
    enum GameType
    {
        OO,
        GO,
        OG,
        GG
    };
    enum ObjectOption{
        text,
        image,
        number,
        ordinal,
		roman
    };
    enum Direction
    {
        left,
        right,
        top,
        bottom
    };
    enum Option{
        row,
        column
    };


    static Thanh_Matching2C* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart() override;
    void startQuestion() override;
    void onExplanation(bool correct) override;
    void onComplete() override;
    void autoTesting() override;

    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;
	void update(float dt) override;
private:
    bool init() override;
    // game config
    GameType gameType;
    cocos2d::Size object_size;
    cocos2d::Size group_size;
    int number_pair;
    Option option;
    math::comparision::Key compare_key;
    bool make_sum = false;
    int min_range;
    int max_range;
    int number_line_drawed=0;
    bool touch_to_plug, touch_to_image;
    std::vector<cocos2d::ui::ImageView*> right_plugs, left_plugs, right_lights, left_lights;
    std::vector<MyPair*> my_pair;
    MyPair *current_mp = nullptr, *mp1 = nullptr, *mp2 = nullptr;
	bool touch_disable, game_end;
    
    // ---------------
    // Object config
    ObjectOption object_option_1, object_option_2;
    bool object_reuse;

    // ---------------
    // group config
    math::group::GroupType type_group_1, type_group_2;
    cocos2d::Size ele_group_1_size, ele_group_2_size;
    bool group_reuse;
    int frame_col_1, frame_col_2, frame_row_1, frame_row_2;
    math::resource::Image *group_bg = nullptr;

    // ----------------
    // Question config

    std::vector<int> list_questiondata;

    math::resource::Image* img_bg = nullptr;
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> icons_1;
    std::vector<math::resource::Image*> icons_2;
    std::vector<math::resource::Image*> icons_3;
    std::vector<math::resource::Image*> icons_4;

    // list doi tuong
    std::vector<int> status;
    std::vector<cocos2d::Node*> objects; // group & object vector
    std::vector<Math_Object*> objects_1, objects_2; // object vector
    std::vector<Math_Group*> groups_1, groups_2; // group vector

    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    bool onlyTouch;
    bool isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node* object);
    void setSelected(bool selected, cocos2d::Node* object);

    std::vector<cocos2d::DrawNode*> drawer;
    std::vector<cocos2d::Color4F> color;
    int index;
    int pass;
    int obj_id, gr_id;
    std::vector<int> value_list;
    std::vector<int> value_list2;
    int sum_value;
    std::vector<math::resource::Image*> image_list_1;
    std::vector<math::resource::Image*> image_list_2;
    math::resource::Image* img_group_1 = NULL;
    math::resource::Image* img_group_2 = NULL;
    
    // ham trong game
    bool isCorrect(int i, int j);
    int getValue(int i);
    cocos2d::Vec2 getPositionDraw(int i, Direction direction);
    std::vector<Math_Group*> createGroupList(std::vector<math::resource::Image*>,math::resource::Image*, math::group::GroupType, cocos2d::Size, int col=5, int row=2);

    std::vector<Math_Object*> createObjectList(std::vector<math::resource::Image*>, ObjectOption);
    void genObjectGroup();
    void connect(int i, int j);
};

#endif /* Thanh_Matching2C_h */
