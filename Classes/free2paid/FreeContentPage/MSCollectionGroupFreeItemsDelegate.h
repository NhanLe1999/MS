//
//  MSCollectionAISpeakingLessonItem.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//

#ifndef MSCollectionGroupFreeItemsDelegate_h
#define MSCollectionGroupFreeItemsDelegate_h

#include "StoryDataModel.h"

namespace ms { namespace delegate {
    class MSCollectionGroupFreeItemsDelegate {
    public:
        virtual void onTouchToGroupItem(bool isClickToUnlock, int index) = 0;
        virtual bool onItemsLearn(int day) = 0;
        virtual void onResetLayoutListView(int index) = 0;
        virtual void onLearnStory(mj::model::StoryInfo storyItemInfo) = 0;
        virtual void onLearnLessonFree(mj::model::LessonInfo lessonItemInfo) = 0;
        virtual std::vector<int> getAllStoriesLearn() = 0;
        virtual bool checkItemLearned(mj::model::FreeItemInfo freeItemInfo) = 0;
        virtual void showNotifyLessonNotReadyAlertWhenLearned() = 0;
        virtual void showNotifyLessonNotReadyAlertWhenHaveNotLearned() = 0;
    };
}}

#endif /* MSCollectionAISpeakingLessonItem */
