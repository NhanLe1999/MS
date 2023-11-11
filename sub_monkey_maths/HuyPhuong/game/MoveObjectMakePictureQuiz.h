//
//  MoveObjectMakePictureQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef MoveObjectMakePictureQuiz_h
#define MoveObjectMakePictureQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class MoveObjectMakePictureQuiz : public Math_Quiz
{
public:
    static MoveObjectMakePictureQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    struct PiecePoint {
        int index;
        int x;
        int y;
    };
    std::vector<PiecePoint> points;
    std::vector<PiecePoint> start_points;
    
    struct PieceFamily {
        std::vector<int> points;
    };
    std::vector<PieceFamily> families;
    
    bool suggess_index = 0;

    cocos2d::Size image_size;
    cocos2d::Vec2 image_center;
    
    cocos2d::Size start_size;
    cocos2d::Vec2 start_center;

    Math_Object* comp_object = nullptr;
    Math_Object* sugg_object = nullptr;

    Math_Object* selected_object;
    int move_count=0;
    
    math::resource::Image* img_complete = nullptr;
    math::resource::Image* img_suggess = nullptr;
    math::resource::Image* img_target = nullptr;
    math::resource::Image* img_start = nullptr;

    std::vector<math::resource::Image*> img_list;
    
    //callback Func
    void onMoveObjectBegan(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    
    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
    
    cocos2d::Vec2 getLocation(int index);
    cocos2d::Vec2 getStartLocation(int index);

    
};


#endif /* MoveObjectMakePictureQuiz_h */
