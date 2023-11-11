//
//  MSCollectionFreeItemDelegate.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//

#ifndef MSCollectionFreeItemDelegate_h
#define MSCollectionFreeItemDelegate_h

#include "StoryDataModel.h"

namespace ms { namespace delegate {
    class MSCollectionFreeItemDelegate {
    public:
        virtual bool onItemsLearn(int day) = 0;
        virtual void onLearnStory (mj::model::StoryInfo storyItemInfo) = 0;
        virtual void onLearnLessonFree(mj::model::LessonInfo lessonItemInfo) = 0;
        virtual std::vector<int> getAllStoriesLearn() = 0;
        virtual bool checkItemLearned(mj::model::FreeItemInfo freeItemInfo) = 0;
        virtual void onsetAvataUrl(std::string url) = 0;
    };

}}

#endif /* MSCollectionFreeItemDelegate */
