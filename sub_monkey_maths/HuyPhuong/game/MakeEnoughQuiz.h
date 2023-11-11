//
//  MakeEnoughQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef MakeEnoughQuiz_h
#define MakeEnoughQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"

class MakeEnoughQuiz : public Math_Quiz
{
public:
    static MakeEnoughQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    //Game config
    std::string game_mode;
    cocos2d::Size object_size;
    std::vector<math::resource::Image*> img_list;
    bool make_copy;
    bool align_pair = true;

    math::resource::Image* bg_img;
    
    //Color game
    std::string current_color = "";
    math::resource::Image* white_img = nullptr;
    math::resource::Image* color_img = nullptr;
    std::vector<math::resource::Image*> color_imgs;
    std::vector<Math_Object*> color_objects;

    //drag
    std::vector<Math_Object*> select_list;
    std::vector<Math_Object*> _dragers;

    //callback Func
    //color
    void onTouchedObject(Node* node);
    void onAnswerDone(Node* node);
    //match
    void onTouchedGroup(Node* node);
    //drag
    void onMoveBegan(Node* node);
    void onMovedObject(Node* node, cocos2d::Vec2 point);

    //debugging mode
    int testing_step=0;
    //Smart autotest
    void autoTestingRun(float dt);
    
};


#endif /* MakeEnoughQuiz_h */
