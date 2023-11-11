//
// Created by To Ngoc Linh on 9/6/17.
//

#include "MJGradeInfoContainer.h"
#include "StoryDataModel.h"
#include "MJGradeInfoPanel.h"
#include "StoryConfigManager.h"
#include "MJPlatformConfig.h"
#include "StoryParentReadingLevel_Landscape.h"

USING_NS_CC;
INITIALIZE_READER(MJGradeInfoContainer);

std::once_flag mj_grade_info_container;

MJGradeInfoContainer *MJGradeInfoContainer::createView() {
    std::call_once(mj_grade_info_container, [] {
        REGISTER_CSB_READER(MJGradeInfoContainer);
    });

    auto csb_name = CONFIG_MANAGER->isHomeScreenLandscape() ?
            "csb/reading_level/MJReadingGradeContainer.csb" :
            "csb/reading_level/MJReadingGradeContainer_Portrait.csb";
    auto view = static_cast<MJGradeInfoContainer *>(CSLoader::createNode(csb_name));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void MJGradeInfoContainer::didLoadFromCSB() 
{
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        auto content = utils::findChild<ui::Layout *>(this, "content");
        content->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
        content->setAnchorPoint(Vec2(0, 0));
        content->setPosition(Vec2(0, 0));
    }    
#endif

    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    listview->setScrollBarEnabled(false);
    auto grades = StoryDataModel::getInstance()->getAllGradesInfo(lang_id);
    _levels = StoryDataModel::getInstance()->getAllLevelsInfo(lang_id);
    auto listview_width = 0;
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        std::reverse(std::begin(grades), std::end(grades));
    }
    
	for (auto grade : grades) 
	{
        auto tab = MJGradeInfoTab::createTab(grade);
        tab->setListener(this);
        listview->pushBackCustomItem(tab);
        listview_width += tab->getContentSize().width;
    }

	if (!listview->getItems().empty())
	{
		m_currentTab = dynamic_cast<MJGradeInfoTab*>(listview->getItems().front());
	}

	if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
		listview->jumpToBottom();
	}
}

void MJGradeInfoContainer::onSelectTab(MJGradeInfoTab *selected_tab, mj::model::GradeInfo info) 
{
	m_currentTab = selected_tab;

	if (auto  parent = dynamic_cast<StoryParentReadingLevel_Landscape*>(m_parent))
	{
		parent->onSelectTab(selected_tab, info);
	}

    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    for (auto c : listview->getItems()) {
        auto tab = dynamic_cast<MJGradeInfoTab *>(c);
        if (tab && tab != selected_tab) {
            tab->setSelected(false);
        }
    }

    auto grade_level = _levels;
    grade_level.erase(std::remove_if(std::begin(grade_level), std::end(grade_level), [info](mj::model::LevelInfo l) {
        return l.grade_id != info.grade_id;
    }), std::end(grade_level));

    auto panel = MJGradeInfoPanel::createPanel(info, grade_level);
    panel->setName("info");

    auto placeholder = utils::findChild(this, "placeholder");

    panel->setContentSize(placeholder->getContentSize());
    panel->setPosition(placeholder->getContentSize() / 2);
    panel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    placeholder->removeChildByName("info");
    placeholder->addChild(panel);
    ui::Helper::doLayout(this);
    panel->adjustText();
    
    auto listview_width = 200 * listview->getItems().size();
    
    if (listview_width < this->getContentSize().width) {
        listview->setContentSize(Size(listview_width, listview->getContentSize().height));
        listview->setPositionX(this->getContentSize().width / 2);
    }
}

void MJGradeInfoContainer::selectGrade(int grade_id) {
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    for (auto i = 0; i < listview->getItems().size(); ++i) {
        auto c = listview->getItems().at(i);
        auto tab = dynamic_cast<MJGradeInfoTab *>(c);
        if (tab && tab->getGradeId() == grade_id) {
            tab->onSelectTab(nullptr);
            this->onSelectTab(tab, tab->getGradeInfo());
            listview->jumpToItem(i, Point::ANCHOR_MIDDLE, Point::ANCHOR_MIDDLE);
            break;
        }
    }

	if (auto  parent = dynamic_cast<StoryParentReadingLevel_Landscape*>(m_parent))
	{
		parent->setPushEventEnable(true);
	}
}

void MJGradeInfoContainer::onEnter() {
    Layout::onEnter();
    ui::Helper::doLayout(this);

    auto panel = utils::findChild<MJGradeInfoPanel *>(this, "info");
    if (panel)
        panel->adjustText();

        auto listview = utils::findChild<ui::ListView *>(this, "listview");
        auto listview_width = 200 * listview->getItems().size();

        if (listview_width < this->getContentSize().width) {
            listview->setContentSize(Size(listview_width, listview->getContentSize().height));
            listview->setPositionX(this->getContentSize().width / 2);
        }
//    if (listview_width < this->getContentSize().width) {
//        listview->setContentSize(Size(listview_width, listview->getContentSize().height));
//        listview->setPositionX(this->getContentSize().width / 2);
//        CCLOG("listview size: %f, %f", listview->getContentSize().width, listview->getContentSize().height);
//    }

}
