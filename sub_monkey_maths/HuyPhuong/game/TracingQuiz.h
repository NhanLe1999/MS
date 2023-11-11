//
//  TracingQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef TracingQuiz_h
#define TracingQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


class TracingQuiz : public Math_Quiz
{
public:
    static TracingQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
    void onGuideStarted() override;
    void onGuideDone() override;
    
private:
    bool init() override;
    
    bool moveable = false;

    math::resource::Image*  img_tracing = nullptr;
    math::resource::Image*  img_item = nullptr;
    cocos2d::Size           tracing_size;
    cocos2d::Vec2           tracing_center;
    std::string             color_tracing;

    struct TracingPoint {
        int index;
        int x;
        int y;
    };
    
    struct TracingPath {
        int index;
        std::vector<int> points;
    };
    
    std::vector<TracingPoint> points;
    std::vector<TracingPath> paths;
    int index_path=0;

    Math_Object* game_object = nullptr;
    Math_Object* path_object = nullptr;

    std::vector<Math_Object*> point_objects;
    std::vector<Math_Object*> path_objects;
    std::vector<Math_Object*> item_list;

    std::vector<Node*> gaf_list;
    void GAFSequenceCallback(gaf::GAFObject* object, const std::string& sequenceName);
    
    cocos2d::EventListenerTouchOneByOne* touch_listener;
    void addEvents();
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchCancelled(cocos2d::Touch * touch, cocos2d::Event * event) override;
    
    Math_Object* touchOnObject(cocos2d::Vec2 point);
    bool isTraceDone();
    void doCheckPath ();
    void completeTrace();
    bool isInPath(cocos2d::Vec2 point);
    
    void startTracing(float dt = 0);
    void activeItem(Math_Object* object);
    Math_Object* nextItem();
    cocos2d::Vec2 getLocation(int point);
};


#endif /* TracingQuiz_h */
