//
// Created by To Ngoc Linh on 9/7/17.
//

#include "MJGradeInfoPanel.h"
#include "StoryLanguageManager.h"
#include "MJDefault.h"
#include "StoryConfigManager.h"
#include "MJPlatformConfig.h"

USING_NS_CC;
INITIALIZE_READER(MJGradeInfoPanel);

std::once_flag mj_grade_info_panel_reader;

MJGradeInfoPanel *MJGradeInfoPanel::createPanel(mj::model::GradeInfo grade, std::vector<mj::model::LevelInfo> levels) {
    std::call_once(mj_grade_info_panel_reader, [] {
        REGISTER_CSB_READER(MJGradeInfoPanel);
    });

    auto csb = CONFIG_MANAGER->isHomeScreenLandscape() ?
            "csb/reading_level/MJReadingGrade.csb" :
            "csb/reading_level/MJReadingGrade_Portrait.csb";

    auto panel = static_cast<MJGradeInfoPanel *>(CSLoader::createNode(csb));
    if (panel) {
        panel->didLoadFromCSB(grade, levels);
        return panel;
    }

    CC_SAFE_DELETE(panel);
    return nullptr;
}

void MJGradeInfoPanel::didLoadFromCSB(mj::model::GradeInfo grade, std::vector<mj::model::LevelInfo> levels) {
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    auto intro = utils::findChild<ui::Text *>(this, "reading.level.intro");
    intro->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "reading.level.intro"));
    intro->setTextColor(Color4B(EXPLORE_GRADE_COLOR.at(grade.grade_id)));

    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    listview->setTouchEnabled(false);
    listview->setScrollBarEnabled(false);
    auto model = utils::findChild<ui::Layout *>(this, "level_model");

    auto listview_width = 0;
    auto listview_height = 0;
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        std::reverse(std::begin(levels), std::end(levels));
    }

    for (auto level : levels) {
        auto item = model->clone();
		auto itemBg = item->getChildByName("background");
		auto itemLabel = item->getChildByName<ui::Text *>("label");
		itemLabel->setString(level.name);
        item->getChildByName("background")->setColor(EXPLORE_GRADE_COLOR.at(grade.grade_id));
		itemBg->setContentSize({ std::max(itemBg->getContentSize().width,itemLabel->getContentSize().width*1.15f),std::max(itemBg->getContentSize().height,itemLabel->getContentSize().height) });
		((ui::Layout*)item)->setContentSize(itemBg->getContentSize());
		listview->pushBackCustomItem(item);

        listview_width += item->getContentSize().width + listview->getItemsMargin();
        listview_height += item->getContentSize().height + listview->getItemsMargin();
    }
    listview_width -= listview->getItemsMargin();
    listview_height -= listview->getItemsMargin();
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        listview->setContentSize(Size(listview_width, listview->getContentSize().height));
    } else {
        listview->setContentSize(Size(listview->getContentSize().width, listview_height));
    }

    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    scrollview->setScrollBarEnabled(false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        scrollview->setClippingType(cocos2d::ui::Layout::ClippingType::SCISSOR);
        scrollview->setAnchorPoint(Vec2(0, 0));
        scrollview->setPosition(Vec2(0, 0));
    }
    
#endif

    auto description = utils::findChild<ui::Text *>(this, "description");
    description->setString(grade.description);
    description->ignoreContentAdaptWithSize(false);

    ui::Helper::doLayout(this);

    auto richtext = ui::RichText::create();
    richtext->setName("richtext");
    richtext->pushBackElement(ui::RichElementText::create(0, EXPLORE_GRADE_COLOR.at(grade.grade_id), 255, grade.name + " ", "fonts/Montserrat-Bold.ttf", 24));

    if (levels.size() == 1) {
        richtext->pushBackElement(ui::RichElementText::create(0, EXPLORE_GRADE_COLOR.at(grade.grade_id), 255, LANGUAGE_MANAGER->getTextByKey(lang_id, "reading.level.include1"), "fonts/Montserrat-Medium.ttf", 24));
    } else {
        richtext->pushBackElement(ui::RichElementText::create(0, EXPLORE_GRADE_COLOR.at(grade.grade_id), 255, LANGUAGE_MANAGER->getTextByKey(lang_id, "reading.level.include"), "fonts/Montserrat-Medium.ttf", 24));
    }

    richtext->setPosition(Point(this->getContentSize().width / 2, this->getContentSize().height - 40));

    auto title = utils::findChild<ui::Text *>(this, "title");
    richtext->setPosition(Vec2(this->getContentSize().width / 2, title->getPositionY()));
    title->getParent()->addChild(richtext);
    title->setVisible(false);

    if (levels.empty()) {
        utils::findChild(this, "content")->setVisible(false);
        utils::findChild(this, "coming_soon")->setVisible(true);
        auto coming_soon = utils::findChild<ui::Text *>(this, "level_info.coming_soon");
        coming_soon->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, "level_info.coming_soon"));
        coming_soon->setTextColor(Color4B(EXPLORE_GRADE_COLOR.at(grade.grade_id)));
        ui::Helper::doLayout(this);

        return;
    }
}

void MJGradeInfoPanel::onEnter() {
    ui::Layout::onEnter();
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        utils::findChild(this, "richtext")->setPositionX(this->getContentSize().width / 2);
    } else {
        utils::findChild(this, "richtext")->setPosition(Vec2(30, this->getContentSize().height / 2));
        utils::findChild(this, "richtext")->setRotation(-90);
    }

    adjustText();
}

void MJGradeInfoPanel::adjustText() {
    auto description = utils::findChild<ui::Text *>(this, "description");
    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    auto intro = utils::findChild<ui::Text *>(this, "reading.level.intro");

    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        intro->setPositionX(intro->getPositionX() + MJ_PLATFORM_CONFIG.getNotchHeight());
        description->setTextAreaSize(cocos2d::Size(scrollview->getContentSize().width - 100 - MJ_PLATFORM_CONFIG.getNotchHeight() * 2, 0));
        description->setContentSize(description->getVirtualRendererSize());
        scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, description->getContentSize().height + 20));
        description->setPosition(Vec2(scrollview->getInnerContainerSize().width / 2, scrollview->getInnerContainerSize().height));
    } else {
        description->setTextAreaSize(cocos2d::Size(scrollview->getContentSize().height - 100- MJ_PLATFORM_CONFIG.getNotchHeight() * 2, 0));
        description->setContentSize(description->getVirtualRendererSize());
        scrollview->setInnerContainerSize(Size(description->getContentSize().height + 20, scrollview->getContentSize().height));
        description->setPosition(Vec2(0, scrollview->getInnerContainerSize().height / 2));
    }
    
}
