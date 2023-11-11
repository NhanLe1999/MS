//
//  MSCollectionAISpeakingLessonItem.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//

#ifndef MSCollectionAISpeakingLessonItemDelegate_h
#define MSCollectionAISpeakingLessonItemDelegate_h

#include "StoryDataModel.h"

namespace ms { namespace delegate {
    class CollectionAISpeakingLessonItem {
    public:
        virtual void onLearnLesson(mj::model::AISpeakingLessonInfo lesson) = 0;
        virtual void onTryToLearnLockedLessons(mj::model::AISpeakingLessonInfo lesson) {};
        virtual void showPermissionPopup() = 0;
        virtual void onSavelessonLearnAfterLearned(mj::model::AISpeakingLessonInfo lessonInfo) {};
        virtual void onUpdateTopicProccess(){};
        virtual void showSubscribePopup(){};
        virtual void onVerticalScrollView(int indexOffset) {};
    };
}}

#endif /* MSCollectionAISpeakingLessonItem */
