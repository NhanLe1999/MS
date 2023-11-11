//
//  Thanh_Matching3C.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_Matching3C_h
#define Thanh_Matching3C_h

#include <stdio.h>
#include "Math_Quiz.h"

struct Matching3CPair {
	cocos2d::Node *node = nullptr;
	cocos2d::ui::ImageView* plug_right = nullptr, *plug_left = nullptr;
	cocos2d::Vec2 start_plug_position;
	cocos2d::ui::ImageView* light = nullptr;
	cocos2d::DrawNode* draw = nullptr;
	int panel;
	bool disable;
	cocos2d::DrawNode *start = nullptr, *end = nullptr, *avg = nullptr;
	int value;
};

class Thanh_Matching3C : public Math_Quiz
{
public:
    enum GameType
    {
        GOG,
        OGO
    };
    enum ObjectOption{
        image,
        text,
        number
    };
    enum Direction
    {
        left,
        right
    };

    static Thanh_Matching3C* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
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
    int number_row;
	bool touch_disable;
	bool touch_to_plug;
	bool touch_to_image;
	std::vector<Matching3CPair*> my_pair;
	Matching3CPair *mp_current = nullptr, *start_mp = nullptr, *end_mp = nullptr;
	bool game_over;

    // ---------------
    // Object config
    math::comparision::Key key_compare=math::comparision::number;
    ObjectOption object_option_1, object_option_2;
    bool object_reuse;
	math::resource::Image *object_bg = nullptr;

    // ---------------
    // group config
    math::group::GroupType type_group_1, type_group_2;
    cocos2d::Size ele_group_1_size, ele_group_2_size;
    bool group_reuse;
	math::resource::Image *group_bg = nullptr;

    // ----------------
    // Question config

    
	 
    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> number_list; // object
    std::vector<math::resource::Image*> icons_group_0; // group
    std::vector<math::resource::Image*> icons_group_1; // group

    // list doi tuong
    std::vector<int> status;
    std::vector<cocos2d::Node*> list_obj;
    std::vector<Math_Object *> list_object_1, list_object_2;
    std::vector<Math_Group *> list_group_1, list_group_2;

    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // button
    //Math_Answer* btn_done; 
    bool onlyTouch;
    std::queue<int> queue_obj;
    bool isTouchedOnObject(cocos2d::Vec2 position, cocos2d::Node* object);
    void setSelected(bool selected, cocos2d::Node* object);

    std::vector<cocos2d::DrawNode*> drawer;
    std::vector<cocos2d::Color4F> color;
    int index;
    int pass;
    int obj_id, gr_id;
    std::vector<int> value_list;

    // ham trong game
    bool isCorrect(int i, int j);
    int getValue(int i);
    cocos2d::Vec2 getPositionDraw(int i, Direction direction);
    std::vector<Math_Group*> createGroupList(std::vector<math::resource::Image*>, math::group::GroupType, cocos2d::Size );

    std::vector<Math_Object*> createObjectList(std::vector<math::resource::Image*>, ObjectOption );
    void genObjectGroup();
    void connect(int i, int j);

};

#endif /* Thanh_Matching3C_h */
