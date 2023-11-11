//
//  MSCollectionCategoryItem.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef MSCollectionCategoryItem_hpp
#define MSCollectionCategoryItem_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MSCollectionCategoryItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSCollectionCategoryItem);

    static MSCollectionCategoryItem * createItem(mj::model::CategoryInfo info);

	static MSCollectionCategoryItem * createItem(mj::model::AudiobookSeriesInfo info);

private:

	void didLoadFromCSB(mj::model::CategoryInfo info);

	void didLoadFromCSB(mj::model::AudiobookSeriesInfo info);
    
    CC_SYNTHESIZE(mj::model::CategoryInfo, _info, CategoryInfo);

	CC_SYNTHESIZE(mj::model::AudiobookSeriesInfo, _audiobookInfo, AudiobookCategoryInfo);

};

CREATE_READER(MSCollectionCategoryItem);

#endif /* MSCollectionCategoryItem_hpp */
