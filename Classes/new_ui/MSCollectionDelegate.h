//
//  MSCollectionDelegate.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/17/17.
//

#ifndef MSCollectionDelegate_h
#define MSCollectionDelegate_h

#include "ui/CocosGUI.h"

class MSSimpleCollection;

namespace ms {
    namespace delegate {
        class Collection {
        public:
            virtual void onSelectCollectionItem(cocos2d::ui::Widget * item) = 0;
            virtual void onSelectMore(MSSimpleCollection * sender) {};
            virtual void onPlayAll(MSSimpleCollection * sender) {};
        };
    }
}

#endif /* MSCollectionDelegate_h */
