//
//  MJLesson.cpp
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#include "MJLesson.h"

namespace mj {
    namespace model {
        
        void Lesson::addActivity(mj::model::Activity activity) {
            _activities.push_back(activity);
            std::sort(_activities.begin(), _activities.end(), [](const Activity & a, const Activity & b) {
                return a.getLessonOrder() < b.getLessonOrder();
            });
        }
    }
}
