//
//  Thanh_LocationInGrid.hpp
//  MJQuiz
//
//  Created by Daniel on 11/23/17.
//
//

#ifndef Thanh_LocationInGrid_hpp
#define Thanh_LocationInGrid_hpp

#include <stdio.h>
#include "Math_Quiz.h"

class LocationInGrid{
public:
    std::string key;
    int number;
    std::string text;
};

class Thanh_LocationInGrid : public Math_Quiz
{
public:
    enum GameMode{
        location_in_grid,
        which_object_in_location,
        which_location_include_object
    };

    static Thanh_LocationInGrid* createQuiz();

    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;

    void onTouchedObject(cocos2d::Node* node);

private:
    bool init() override;
    LocationInGrid _location;
    std::vector<LocationInGrid> _list_location;

    // ---------------
    // game config
    GameMode _mode;

    // ---------------
    // Grid config
    cocos2d::Size _size;
    math::resource::Image* _background = nullptr;
    Math_Group* _grid = nullptr;

    // ----------------
    // Question config
    

    //danh sach image cho moi loai obj
    std::vector<math::resource::Image*> _icons;
    std::vector<math::resource::Image*> _icons_question;

    Math_Object* _object_be_touch = nullptr;
    cocos2d::Node* _correct_answer = nullptr;

    //callback Func
    void onAnswerDone(cocos2d::Node* node); 

    // ----------------
    // debugging mode
    int testing_step=0;
    std::vector<Math_Answer*> answer_tested;
    void autoTestingRun(float dt);

    // button
    Math_Answer* btn_done = nullptr;
    std::vector<Math_Answer*> _list_answer;

    // in game
    bool _disable; // disable touch
    std::vector<std::string> _text_answers;
};
#endif /* Thanh_LocationInGrid_hpp */
