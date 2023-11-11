//
//  MSCollectionStoryItem.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#ifndef MSCollectionStoryItem_hpp
#define MSCollectionStoryItem_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MSCollectionStoryItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSCollectionStoryItem);
    static MSCollectionStoryItem * createItem(mj::model::StoryInfo info);
//    void load(mj::model::StoryInfo info);
    void enableDeleteMode();
    void disableDeleteMode();
    void disableNewRead();
    
private:
    void onEnter() override;
    void didLoadFromCSB(mj::model::StoryInfo info);
    
//    mj::model::StoryInfo _info;
    CC_SYNTHESIZE(mj::model::StoryInfo, _info, StoryInfo);
    
    virtual cocos2d::ui::Widget* createCloneInstance() override;
    virtual void copySpecialProperties(cocos2d::ui::Widget* model) override;
    
    static MSCollectionStoryItem * _model;
    cocos2d::ui::ImageView *icon_new, *icon_read;
};

CREATE_READER(MSCollectionStoryItem);
#endif /* MSCollectionStoryItem_hpp */
