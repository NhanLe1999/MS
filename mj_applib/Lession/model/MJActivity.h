//
//  MJActivity.h
//  MonkeyMath-mobile
//
//  Created by HuyPhuong on 4/2/18.
//

#ifndef MJActivity_h
#define MJActivity_h

#include <stdio.h>
#include "MJBaseModel.h"
#include "cocos2d.h"
namespace mj {
    namespace model {
        class Activity : public BaseModel {
        public:
            Activity() {};
            ~Activity() {};
            using BaseModel::BaseModel;
            Activity(int, int, std::string, std::string);
            
            CC_SYNTHESIZE_READONLY(std::string, _link, Link);
            CC_SYNTHESIZE_READONLY(int, _game_id, GameId);
            
            CC_SYNTHESIZE(int, _lesson_id, LessonId);
            CC_SYNTHESIZE(int, _unit_id, UnitId);
            
            CC_SYNTHESIZE(int, _play_time, PlayTime);
            CC_SYNTHESIZE(int, _difficult_level, DifficultLevel);
            CC_SYNTHESIZE(int, _lesson_order, LessonOrder);
        };
    }
}
#endif /* MJActivity_h */
