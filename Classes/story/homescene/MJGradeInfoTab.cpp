//
// Created by To Ngoc Linh on 9/6/17.
//

#include "MJGradeInfoTab.h"
#include "StoryConfigManager.h"

USING_NS_CC;
INITIALIZE_READER(MJGradeInfoTab);

std::once_flag mj_grade_info_tab_reader;

MJGradeInfoTab * MJGradeInfoTab::createTab(mj::model::GradeInfo info) {
    std::call_once(mj_grade_info_tab_reader, [] {
        REGISTER_CSB_READER(MJGradeInfoTab);
    });

    auto csb_name = CONFIG_MANAGER->isHomeScreenLandscape() ?
            "csb/reading_level/MJGradeTab.csb" :
            "csb/reading_level/MJGradeTab_Portrait.csb";

    auto tab = static_cast<MJGradeInfoTab *>(CSLoader::createNode(csb_name));
    if (tab) {
        tab->didLoadFromCSB(info);
        return tab;
    }

    CC_SAFE_DELETE(tab);
    return nullptr;
}

void MJGradeInfoTab::didLoadFromCSB(mj::model::GradeInfo info) {
    _info = info;
    _listener = nullptr;

    auto label_name = utils::findChild<ui::Text *>(this, "name");
    label_name->setString(_info.name);
    label_name->setColor(EXPLORE_GRADE_COLOR.at(_info.grade_id));
}

void MJGradeInfoTab::setSelected(bool is_selected) {

    auto label_tint_color = Color3B();
    auto background_tint_color = Color3B();

    if (is_selected) {
        label_tint_color = Color3B::WHITE;
        background_tint_color = EXPLORE_GRADE_COLOR.at(_info.grade_id);
    } else {
        label_tint_color = EXPLORE_GRADE_COLOR.at(_info.grade_id);
        background_tint_color = Color3B::WHITE;
    }
    auto background = utils::findChild(this, "background");
    background->runAction(TintTo::create(.25f, background_tint_color));
    auto label_name = utils::findChild(this, "name");
    label_name->runAction(TintTo::create(.25f, label_tint_color));
}

cocos2d::ui::Widget::ccWidgetClickCallback MJGradeInfoTab::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onSelectTab") {
        return CC_CALLBACK_1(MJGradeInfoTab::onSelectTab, this);
    }

    return nullptr;
}

void MJGradeInfoTab::onSelectTab(cocos2d::Ref *sender) {
    this->setSelected(true);
    if (_listener) {
        _listener->onSelectTab(this, _info);
    }
}
