//
//  Thanh_WhichPictureShow.hpp
//  MJQuiz
//
//  Created by Daniel on 08/06/17.
//
//

#ifndef Thanh_WhichPictureShow_h
#define Thanh_WhichPictureShow_h

#include <stdio.h>
#include "Math_Quiz.h"

class Thanh_WhichPictureShow : public Math_Quiz
{
public:
    enum GameMode{
        add,
        sub
    };

    static Thanh_WhichPictureShow* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedGroup(cocos2d::Node* node);
private:
    bool init() override;

    // ---------------
    // game config
    int number_group;
    int number_operand;
    bool first_number;
    GameMode game_mode;
    cocos2d::Size object_size;

    // ---------------
    // group config
    math::resource::Image *background = nullptr;
    math::group::GroupType group_type;
    int maximum_cap;

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    std::vector<std::vector<std::pair<math::resource::Image*, int> > > icons_group;

    // img in question
    std::vector<math::resource::Image*> img_list;

    // result
    std::vector<Math_Group*> list_group;
    Math_Group *group_be_touch, *correct_group;

    //callback Func
    void onAnswerDone(bool correct);

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    Math_Object* last_object_touch = nullptr;
    void autoTestingRun(float dt);

    // in game a + b = result
    int result, a, b, c;
};

#endif /* Thanh_WhichPictureShow_h */
