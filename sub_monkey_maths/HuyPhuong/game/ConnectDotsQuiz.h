//
//  ConnectDotsQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef ConnectDotsQuiz_h
#define ConnectDotsQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class ConnectDotsQuiz : public Math_Quiz
{
public:
    static ConnectDotsQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;

    struct Dot {
        int index;
        float x;
        float y;
    };
    
    bool connect_number = true;
    int number_start = 1;
    bool is_asc = true;
    int number_skip = 1;
    bool lower_case = true;
    
    std::string color_string;
    
    Math_Object* template_object;
    std::vector<Math_Object*> item_list;
    std::vector<math::resource::Image*> img_list;

    cocos2d::Vec2 image_center;
    cocos2d::Size image_size;
    bool auto_draw_last = true;
    bool complete = false;
    
    std::vector<Dot> points;
    cocos2d::DrawNode* drawer = nullptr;
    std::vector<cocos2d::DrawNode*> drawer_list;

    void update (float dt) override;
    int index_path=0;
    
    //callback Func
    void onTouchObject(cocos2d::Node* node);
    cocos2d::Vec2 getLocation(int point);
    
    bool activeObject(Math_Object* object);
    void drawLine(Math_Object* from, Math_Object* to);
    void drawLineDone();

    cocos2d::Vec2 delta;
    cocos2d::Vec2 current_position;
    cocos2d::Vec2 start_position;
    cocos2d::Vec2 target_position;
    bool drawing = false;

    cocos2d::Node * draw_parent;

};


#endif /* ConnectDotsQuiz_h */
