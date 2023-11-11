//
//  MSCollectionDataSource.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/10/17.
//

#ifndef MSCollectionDataSource_h
#define MSCollectionDataSource_h

#include "ui/CocosGUI.h"

namespace ms {
    namespace delegate {
        class CollectionDataSource {
        public:
            virtual int numberOfColumns() { return 0; }
            virtual int numberOfCells() { return 0; }
            virtual cocos2d::ui::Widget* cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned = {}) { return nullptr; }
            virtual cocos2d::Size modelSize() { return cocos2d::Size(); }
            virtual cocos2d::Size collectionSize() = 0;
            
            virtual bool shouldScrollWhenReload() { return false; }
            virtual double percentScrollWhenReload() { return 0; }
            
            virtual void offLoadCellAt(int index) = 0;
            
            virtual void removeCellAtPosition(int row, int col) {};
        };
    }
}

#endif /* MSCollectionDataSource_h */
