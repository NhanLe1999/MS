//
//  DigitalClock.hpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/12/17.
//

#ifndef DigitalClock_h
#define DigitalClock_h

#include <stdio.h>
#include "cocos2d.h"
#include "ClockBase.h"

namespace math {
    
    class DigitalClock : public ClockBase {
    public:
        DigitalClock(){};
        ~DigitalClock(){};
        
        static DigitalClock* createClock(int theme);
        void setMode24(bool mode_24);
        
        void setTime (int, int) override;
        void setClockSize(int width) override;
        void show(float delay = 0, math::animation::ObjectShowType show = math::animation::ScaleUp) override;
        void destroy (float delay = 0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp) override;
        
    private:

        bool init(int theme);
        bool mode24h = true;
        bool is_am = true;
        
        cocos2d::ui::Text* am_pm_text;
        
    };
}
#endif /* DigitalClock_h */

