//
//  MSCollectionLessonItemDelegate.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//

#ifndef MSCollectionLessonItemDelegate_h
#define MSCollectionLessonItemDelegate_h

#include "StoryDataModel.h"

namespace ms { namespace delegate {
    class CollectionLessonItem {
    public:
        virtual void onLearnLesson(mj::model::LessonInfo lesson) = 0;
        virtual void onTryToLearnLockedLessons(mj::model::LessonInfo lesson) {};
    };
}}

#endif /* MSCollectionLessonItemDelegate_h */
