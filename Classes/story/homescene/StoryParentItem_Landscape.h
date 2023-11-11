//
//  StoryParentItem_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#ifndef StoryParentItem_Landscape_hpp
#define StoryParentItem_Landscape_hpp

#include "CocoStudioMacro.h"

class StoryParentItemDelegate {
public:
    virtual void onSelectItem(const int & tag, const std::string &item) = 0;
};

class StoryParentItem_Landscape : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(StoryParentItem_Landscape);
    static StoryParentItem_Landscape * createItem(const cocos2d::ValueMap & data);
    static StoryParentItem_Landscape * createItemBig(const cocos2d::ValueMap & data);
    void adjustWidth(float width);
    void setSelected(bool isSelected);

private:
    void didLoadFromCSB(const cocos2d::ValueMap & data, bool is_big_item = false);
    bool is_big_item = false;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onClickItem(cocos2d::Ref * sender);

    std::string _item_name;
    int _tag;
    cocos2d::Size _original_size;
    CC_SYNTHESIZE(StoryParentItemDelegate *, _delegate, Delegate);
};

CREATE_READER_CLASS(StoryParentItem_LandscapeReader, StoryParentItem_Landscape);

#endif /* StoryParentItem_Landscape_hpp */
