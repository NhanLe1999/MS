//
//  ClockBase.h
//  MJQuiz
//
//  Created by HuyPhuong on 11/23/17.
//

#ifndef ClockBase_h
#define ClockBase_h

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Math_Libs.h"
#include "PoolManager.h"

#include "NumberUtils.h"

namespace math {
    class ClockBase : public cocos2d::ui::Layout {
    public:
        ClockBase(){};
        ~ClockBase(){};
        
        int getHour();
        int getMinute();
        
        std::string getTimeNumber(bool mode24 = true);
        std::string getTimeText(bool mode24 = true);
        std::string getTimeSemiText(bool mode24 = true);
        
        std::vector<std::string> getTimeAudios(bool mode24 = true);
        
        virtual void setClockSize(int width);
        virtual void setTime (int hour, int minute);
        virtual void show(float delay = 0, math::animation::ObjectShowType show = math::animation::ScaleUp);
        virtual void destroy(float delay = 0, math::animation::ObjectShowType revert_show = math::animation::ScaleUp);
        
        void onTouchClock(bool correct);
        
        math::resource::Image* getImgOfClock();
        
    protected:        
        int hour;
        int minute;
        bool clock_theme;
        
        cocos2d::ui::ImageView* _face;
        math::resource::Image* _img_clock = nullptr;
        
        std::vector<math::resource::Image*> _img_numbers;
        std::vector<Math_Object*> _numbers;
        std::vector<cocos2d::Vec2> _locations;
        
    };
}


#endif /* ClockBase_h */
