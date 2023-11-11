//
//  MSPreActivity.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#include "MSPreActivity.h"
#include "StoryDataModel.h"
#include "MSLessonManager.h"
#include "StoryLanguageManager.h"
USING_NS_CC;
INITIALIZE_READER(MSPreActivity);

std::once_flag ms_pre_activity_reader;
MSPreActivity * MSPreActivity::createView(mj::model::ActInfo info) {
    std::call_once(ms_pre_activity_reader, [] {
        REGISTER_CSB_READER(MSPreActivity);
    });
    
    auto node = reinterpret_cast<MSPreActivity *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/lesson/PreActivity.csb"));

    if (node) {
        node->didLoadFromCSB(info);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSPreActivity::didLoadFromCSB(mj::model::ActInfo info) {
    _delegate = nullptr;
    
    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
    auto lesson_info = ms::LessonManager::getInstance().getLessonById(info.lesson_id);
    
    auto index = ms::LessonManager::getInstance().getActivityIndexOfLesson(info);
    
    utils::findChild<ui::Text *>(this, "text_lesson_name")->setString(lesson_info.name);
    utils::findChild<ui::Text *>(this, "text_completed")->setString(StringUtils::format("%d/%d %s", index + 1, (int)activities.size(), LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.lessonpreview.actistatus").c_str()));
    
    utils::findChild<ui::LoadingBar *>(this, "progress_bar")->setPercent((index + 1) * 100.f / activities.size());
    
    utils::findChild<ui::Button *>(this, "button_back")->setTitleText(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.preactivity.btback"));
    utils::findChild<ui::Button *>(this, "button_continue")->setTitleText(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.preactivity.btcontinue"));
    
}

ui::Widget::ccWidgetClickCallback MSPreActivity::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSPreActivity, onBackToLesson),
        CLICK_MAP(MSPreActivity, onContinue)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSPreActivity::onBackToLesson(cocos2d::Ref *sender) {
	_delegate ? _delegate->onBackToLesson() : (void) nullptr;
    this->removeFromParent();
}

void MSPreActivity::onContinue(cocos2d::Ref *sender) {

    this->removeFromParent();
}
