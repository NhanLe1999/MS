//
//  MJLesson.h
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#ifndef MJLesson_h
#define MJLesson_h

#include <stdio.h>
#include "cocos2d.h"
#include "MJBaseModel.h"
#include "MJActivity.h"

namespace mj {
    namespace model {
        class Lesson : public BaseModel{
        public:
            using BaseModel::BaseModel;

            CC_SYNTHESIZE_READONLY(int, _id, Id);
            CC_SYNTHESIZE_READONLY(std::vector<Activity>, _activities, Activities);

            CC_SYNTHESIZE(int, _unit_id, UnitId);
            
            void addActivity(Activity activity);
        };
    }
}

#endif /* MJLesson_h */
