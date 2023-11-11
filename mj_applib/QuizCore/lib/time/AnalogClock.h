//
//  AnalogClock.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef AnalogClock_h
#define AnalogClock_h

#include <stdio.h>
#include "cocos2d.h"
#include "ClockBase.h"


namespace math {
    class AnalogClock : public ClockBase {
    public:
        AnalogClock(){};
        ~AnalogClock(){};
        
        static AnalogClock* createClock(int theme);
        void setTime (int hour, int minute) override;
        void setClockSize(int width) override;
        
        void setMissingNumber(int number);
        Math_Object* setMissingNumberAsSlot(int number);
        void showMissingNumber(int number = 0);
        
        void setNumberAsText(bool use_text);
        void setNumberRoman(bool use_roman);
        
        void show(float delay = 0, math::animation::ObjectShowType show = math::animation::ScaleUp) override;
        void destroy(float delay = 0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp) override;
        
        std::vector<cocos2d::Vec2> getLocationNumbers();
        cocos2d::Vec2 getLocationNumber(int number);
        void hideAllNumbers();
        void hideNumber(int number);
        void showAllNumbers();
        void showNumber(int number);
        
        void hideHand(bool hour);
        void showHand(bool hour);
        
    private:

        bool init(int theme);
        int radius;
        
        bool show_full = true;
        bool show_text = false;
        bool show_roman = false;
        
        std::string clock_path;
        
        cocos2d::ui::ImageView* _hour_hand;
        cocos2d::ui::ImageView* _minute_hand;
        
    };
}
#endif /* AnalogClock_h */
