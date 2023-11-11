//
// Created by To Ngoc Linh on 9/7/17.
//

#ifndef MJSTORY_MJSTORYINFO_H
#define MJSTORY_MJSTORYINFO_H

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MJStoryInfo : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJStoryInfo);
    static MJStoryInfo * createPage(mj::model::StoryInfo info);

private:
    void didLoadFromCSB(mj::model::StoryInfo info);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onBack(cocos2d::Ref * sender);

    mj::model::StoryInfo _info;
};

CREATE_READER(MJStoryInfo);

#endif //MJSTORY_MJSTORYINFO_H
