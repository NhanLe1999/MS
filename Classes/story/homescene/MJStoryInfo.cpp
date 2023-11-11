//
// Created by To Ngoc Linh on 9/7/17.
//

#include "MJStoryInfo.h"
#include "MJGradeInfoContainer.h"
#include "StoryDataModel.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(MJStoryInfo);

std::once_flag mj_story_info_reader;

MJStoryInfo * MJStoryInfo::createPage(mj::model::StoryInfo info) {
    std::call_once(mj_story_info_reader, [] {
        REGISTER_CSB_READER(MJStoryInfo);
    });

    auto csb = CONFIG_MANAGER->isHomeScreenLandscape() ?
            "csb/reading_level/MJStoryInfo.csb" :
            "csb/reading_level/MJStoryInfo_Portrait.csb";

    auto page = static_cast<MJStoryInfo *>(CSLoader::createNodeWithVisibleSize(csb));
    if (page) {
        page->didLoadFromCSB(info);
        return page;
    }

    CC_SAFE_DELETE(page);
    return nullptr;
}

void MJStoryInfo::didLoadFromCSB(mj::model::StoryInfo info) {
    const auto ui_color = Color3B(30, 183, 235);
    const auto transparent_level = 255;
    const auto bold_font = "fonts/monkeystories_ultra.ttf";
    const auto regular_font = "fonts/monkeystories_medium.ttf";

    _info = info;

    auto thumb_name = _info.thumbnail_url.substr(_info.thumbnail_url.find_last_of('/'));
    thumb_name =  FileUtils::getInstance()->getWritablePath() + image_cache_path + thumb_name;

    if (FileUtils::getInstance()->isFileExist(thumb_name))
        utils::findChild<ui::ImageView *>(this, "thumb")->loadTexture(thumb_name);

    auto placeholder = utils::findChild(this, "grade_info");
    auto grade_info = MJGradeInfoContainer::createView();
    grade_info->setContentSize(placeholder->getContentSize());
    grade_info->setPosition(placeholder->getContentSize() / 2);
    grade_info->setAnchorPoint(Point::ANCHOR_MIDDLE);
    placeholder->addChild(grade_info);

    utils::findChild<ui::Text *>(this, "title")->setString(_info.name);

    auto detail = ui::RichText::create();
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
    detail->pushBackElement(ui::RichElementText::create(0, ui_color, transparent_level, LANGUAGE_MANAGER->getTextByKey(lang_id, "story.info.lexile") + ": ", regular_font, 24));
    detail->pushBackElement(ui::RichElementText::create(1, ui_color, transparent_level, _info.lexile, bold_font, 24));
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        detail->pushBackElement(ui::RichElementNewLine::create(10, ui_color, transparent_level));
        detail->pushBackElement(ui::RichElementText::create(2, ui_color, transparent_level, LANGUAGE_MANAGER->getTextByKey(lang_id, "story.info.word_count") + ": ", regular_font, 24));
        detail->pushBackElement(ui::RichElementText::create(3, ui_color, transparent_level, StringUtils::toString(_info.total_words), bold_font, 24));
        detail->pushBackElement(ui::RichElementText::create(4, ui_color, transparent_level, "      " + LANGUAGE_MANAGER->getTextByKey(lang_id, "story.info.page_count") + ": ", regular_font, 24));
        detail->pushBackElement(ui::RichElementText::create(5, ui_color, transparent_level, StringUtils::toString(_info.total_page), bold_font, 24));
    } else {
        detail->pushBackElement(ui::RichElementText::create(2, ui_color, transparent_level, "      " + LANGUAGE_MANAGER->getTextByKey(lang_id, "story.info.word_count") + ": ", regular_font, 24));
        detail->pushBackElement(ui::RichElementText::create(3, ui_color, transparent_level, StringUtils::toString(_info.total_words), bold_font, 24));
        detail->pushBackElement(ui::RichElementText::create(4, ui_color, transparent_level, "      " + LANGUAGE_MANAGER->getTextByKey(lang_id, "story.info.page_count") + ": ", regular_font, 24));
        detail->pushBackElement(ui::RichElementText::create(5, ui_color, transparent_level, StringUtils::toString(_info.total_page), bold_font, 24));
    }

    auto detail_placeholder = utils::findChild(this, "detail_sample");
    detail->setAnchorPoint(detail_placeholder->getAnchorPoint());
    detail->setPosition(detail_placeholder->getPosition());
    detail_placeholder->getParent()->addChild(detail);
    detail_placeholder->removeFromParent();

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        detail->setPosition(detail->getPosition() + Point(0, 30));
    }

    auto display_lang = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    auto level = StoryDataModel::getInstance()->getLevelInfoById(_info.levels[0], display_lang);
    auto grade = StoryDataModel::getInstance()->getGradeInfoById(level.grade_id, display_lang);

    auto label_grade = utils::findChild<ui::Text *>(this, "grade_name");
    label_grade->setString(grade.name);
    auto background_grade = utils::findChild(this, "grade_background");
    background_grade->setColor(EXPLORE_GRADE_COLOR.at(grade.grade_id));
    background_grade->getParent()->setContentSize(Size(std::max(50.f, label_grade->getContentSize().width + 20), background_grade->getContentSize().height));
    ui::Helper::doLayout(background_grade->getParent());

    auto label_level = utils::findChild<ui::Text *>(this, "level_name");
    label_level->setString(level.name);
    auto background_level = utils::findChild(this, "level_background");
    background_level->setColor(EXPLORE_GRADE_COLOR.at(grade.grade_id));
    background_level->getParent()->setContentSize(Size(std::max(50.f, label_level->getContentSize().width + 20), background_level->getContentSize().height));
    ui::Helper::doLayout(background_level->getParent());

    ui::Helper::doLayout(this);
    grade_info->selectGrade(grade.grade_id);

//    if (!MJDEFAULT->getBoolForKey(key_display_levelguide)) {
//        MJDEFAULT->setBoolForKey(key_display_levelguide, true);
//    }
}

cocos2d::ui::Widget::ccWidgetClickCallback MJStoryInfo::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onBack") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onBack(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
#endif
        

        return CC_CALLBACK_1(MJStoryInfo::onBack, this);
    }

    return nullptr;
//    return WidgetCallBackHandlerProtocol::onLocateClickCallback(callback_name);
}

void MJStoryInfo::onBack(cocos2d::Ref *sender) {
    this->removeFromParent();
}
