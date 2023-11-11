//
//  MSCollectionAudiobookItem.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#ifndef MSCollectionAudiobookItem_hpp
#define MSCollectionAudiobookItem_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MSCollectionAudiobookItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSCollectionAudiobookItem);
    static MSCollectionAudiobookItem * createThumb(mj::model::AudiobookInfo info);

    void enableDeleteMode();
    void disableDeleteMode();
    void disableNewRead();
    
private:
    void onExit() override;
    void didLoadFromCSB(mj::model::AudiobookInfo info);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onSelect(Ref * sender);
    
    CC_SYNTHESIZE_READONLY(mj::model::AudiobookInfo, _info, Info);
    
    virtual cocos2d::ui::Widget* createCloneInstance() override;
    virtual void copySpecialProperties(cocos2d::ui::Widget* model) override;
    cocos2d::ui::ImageView *icon_read, *icon_new;
};

CREATE_READER(MSCollectionAudiobookItem);
#endif /* MSCollectionAudiobookItem_hpp */
