//
// Created by To Ngoc Linh on 9/6/17.
//

#ifndef MJSTORY_MJGRADEINFOTAB_H
#define MJSTORY_MJGRADEINFOTAB_H

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MJGradeInfoTab;

class MJGradeInfoTabListener {
public:
    virtual void onSelectTab(MJGradeInfoTab * tab, mj::model::GradeInfo info) = 0;
};

class MJGradeInfoTab : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGradeInfoTab);
    static MJGradeInfoTab * createTab(mj::model::GradeInfo info);
    void setSelected(bool is_selected);
    void onSelectTab(cocos2d::Ref * sender);
    inline int getGradeId() { return _info.grade_id; }
    inline mj::model::GradeInfo getGradeInfo() { return _info; }

private:
    void didLoadFromCSB(mj::model::GradeInfo info);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;

    CC_SYNTHESIZE(MJGradeInfoTabListener *, _listener, Listener);

    mj::model::GradeInfo _info;
};

CREATE_READER(MJGradeInfoTab);

#endif //MJSTORY_MJGRADEINFOTAB_H
