//
// Created by To Ngoc Linh on 9/7/17.
//

#ifndef MJSTORY_MJGRADEINFOPANEL_H
#define MJSTORY_MJGRADEINFOPANEL_H

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MJGradeInfoPanel : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGradeInfoPanel);
    static MJGradeInfoPanel * createPanel(mj::model::GradeInfo grade, std::vector<mj::model::LevelInfo> levels);
    void adjustText();

private:
    void onEnter() override;
    void didLoadFromCSB(mj::model::GradeInfo grade, std::vector<mj::model::LevelInfo> levels);
};

CREATE_READER(MJGradeInfoPanel);

#endif //MJSTORY_MJGRADEINFOPANEL_H
