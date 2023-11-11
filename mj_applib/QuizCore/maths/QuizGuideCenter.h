//
//  QuizGuideCenter.h
//  MJStory
//
//  Created by HuyPhuong on 3/23/18.
//

#ifndef QuizGuideCenter_h
#define QuizGuideCenter_h

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "CocoStudioMacro.h"
#include "PoolManager.h"

#include "MJGame.h"
#include "AudioHelper.h"

#define timing_step 0.1
#define timing_delay 0.5
#define timing_fade 0.25

#define swipe_velocity 500.0

#define GuideCenter QuizGuideCenter::getGuideCenter()

class QuizGuideCenter : public cocos2d::ui::Layout
{
public:
    
    QuizGuideCenter(){};
    ~QuizGuideCenter(){};
    
    static QuizGuideCenter * getGuideCenter();
    
    void startGuide(std::function<void(void)> onstart, std::function<void(void)> ondone);
    void stopGuide();
    bool isGuideEnable();
    bool isGuiding();
    
    void skippedGuide();
    
    void setQuizGuide(std::string quiz, int activity);
    void setInactiveTime(float time);
    void setIncorrectTime(int time);
    
    void resetGuideCount();
    int guideCount();
    
    void resetIncorrectCount();
    void increamentIncorrect();

    void setGuideEnabled();
    void setGuideDisabled();
    
    //Guide action
    void guideNone();
    void guideByTouch(cocos2d::Vec2 point);
    void guideByDragDrop(cocos2d::Vec2 point1, cocos2d::Vec2 point2);

    void guideBySwipe(std::vector<cocos2d::Vec2> points);
    void guideBySwipe(cocos2d::Sequence* swipe, cocos2d::Vec2 point);
    template<typename ...T>
    void guideBySwipe(cocos2d::Sequence* swipe, cocos2d::Vec2 point, T... args) {
        cocos2d::Sequence* action = nullptr;
        if (swipe == nullptr) {
            action = cocos2d::Sequence::create(cocos2d::Place::create(point), cocos2d::FadeIn::create(timing_fade), cocos2d::DelayTime::create(timing_delay), NULL);
        } else {
            float time = point.distance(_lastPoint)/swipe_velocity;
            action = cocos2d::Sequence::create(swipe, cocos2d::MoveTo::create(time, point), NULL);
        }
        _lastPoint = point;
        
        guideBySwipe(action, args...);
    }
    

    void guideByColor(std::vector<cocos2d::Vec2> points);
    void guideByColor(cocos2d::Sequence* swipe, cocos2d::Vec2 point);
    template<typename ...T>
    void guideByColor(cocos2d::Sequence* swipe, cocos2d::Vec2 point, T... args) {
        cocos2d::Sequence* action = nullptr;
        if (swipe == nullptr) {
            action = cocos2d::Sequence::create(cocos2d::Place::create(point), cocos2d::FadeIn::create(timing_fade), cocos2d::CallFunc::create([=](){
                _hand->playSequence("tap", false);
            }), cocos2d::DelayTime::create(timing_delay), cocos2d::CallFunc::create([=](){
                _hand->playSequence("swipe", false);
            }), cocos2d::DelayTime::create(timing_delay), NULL);
        } else {
            float time = point.distance(_lastPoint)/swipe_velocity;
            action = cocos2d::Sequence::create(swipe, cocos2d::MoveTo::create(time, point), cocos2d::CallFunc::create([=](){
                _hand->playSequence("tap", false);
            }), cocos2d::DelayTime::create(timing_delay), cocos2d::CallFunc::create([=](){
                _hand->playSequence("swipe", false);
            }), cocos2d::DelayTime::create(timing_delay), NULL);
        }
        _lastPoint = point;
        guideByColor(action, args...);
    }
    
private:
#pragma mark -
    bool _isGuide;
    bool _isGuiding;
    std::function<void(void)> _onGuideStart = nullptr;
    std::function<void(void)> _onGuideDone = nullptr;
    
    bool guide_on_start = false;
    
    gaf::GAFObject* _hand;
    cocos2d::Vec2 _lastPoint = cocos2d::Vec2::ZERO;

    float inactive_time = 10.0;
    int incorrect_time = 5;
    float countdown;
    int incorrect_count;
    int guide_count;
    
    bool initGC();
    void onGuideStarted();
    void onGuideDone();
    void countdownInactive(float);
    
};
#endif /* QuizGuideCenter_h */
