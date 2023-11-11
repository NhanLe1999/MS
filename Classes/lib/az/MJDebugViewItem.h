//
// Created by To Ngoc Linh on 9/18/17.
//

#ifndef MJSTORY_MJDEBUGVIEWITEM_H
#define MJSTORY_MJDEBUGVIEWITEM_H

#include "cocostudiomacro2.h"

struct DebugInfo {
    std::string name;
    std::string title;
    std::string description;
    cocos2d::Value value;
};

class MJDebugViewItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJDebugViewItem);
    static MJDebugViewItem * createItem(DebugInfo info);
    void applyValue();

private:
    void didLoadFromCSB(DebugInfo info);
    DebugInfo _info;
};

CREATE_READER(MJDebugViewItem);

#endif //MJSTORY_MJDEBUGVIEWITEM_H
