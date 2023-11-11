//
//  MSCollectionProfileItemDelegate.h
//  MJStory
//
//

#ifndef MSCollectionProfileItemDelegate_h
#define MSCollectionProfileItemDelegate_h

#include "StoryDataModel.h"

namespace ms {
    namespace delegate {
        class MSCollectionProfileItemDelegate {
        public:
            virtual void onSelectedProfile(int profileId) = 0;
            virtual void onDeleteSelectedProfile(int profileId) = 0;
            
        };

    }
}

#endif /* MSCollectionProfileItemDelegate_h */
