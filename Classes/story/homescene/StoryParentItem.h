//
//  StoryParentItem.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryParentItem__
#define __StoryParentItem__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class StoryParentItemPortraitDelegate {
public:
    virtual void onClickItem(int item_id) {};
};

class StoryParentItem : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryParentItem);
    void setItemWithData(ValueMap map_data);
    void setDelegate(StoryParentItemPortraitDelegate * delegate);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow;
    void onClick(cocos2d::Ref *sender);
    ValueMap data;
    StoryParentItemPortraitDelegate * delegate = nullptr;

public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryParentItemReader, StoryParentItem);

#endif /* defined(__ALTP__StoryParentItem__) */
