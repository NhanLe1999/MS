//
//  MSLessonEnd.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#include "MSLessonEnd.h"
#include "MSLessonManager.h"
#include "StoryLanguageManager.h"
#include "StoryRateAction.h"
#include "MSPreActivity.h"

USING_NS_CC;
INITIALIZE_READER(MSLessonEnd);

std::once_flag ms_lesson_end_reader;
MSLessonEnd * MSLessonEnd::createView(mj::model::LessonInfo info) {
    std::call_once(ms_lesson_end_reader, [] {
        REGISTER_CSB_READER(MSLessonEnd);
    });
    
    auto node = reinterpret_cast<MSLessonEnd *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/lesson/LessonEnd.csb"));
    if (node) {
        node->didLoadFromCSB(info);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "GroupPromotion.h"
void MSLessonEnd::didLoadFromCSB(mj::model::LessonInfo info) {
    _delegate = nullptr;
    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(info.lesson_id);
    
    utils::findChild<ui::Text *>(this, "text_completed_lesson")->setString(StringUtils::format("%s %d", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.end.lesson").c_str(),info.lesson_order));
    utils::findChild<ui::Text *>(this, "text_completed_activities")->setString(StringUtils::format("%d/%d %s", (int) activities.size(), (int) activities.size(), LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.lessonpreview.actistatus").c_str()));
    
    utils::findChild<ui::Button *>(this, "btback")->setTitleText(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.preactivity.btback"));
    utils::findChild<ui::Button *>(this, "btcontinue")->setTitleText(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.preactivity.btcontinue"));
    
    mjrate::StoryRateAction::updateActionCompleted();
    if (mjrate::StoryRateAction::shouldShowRate()) {
        auto rate_action = mjrate::StoryRateAction::createPage(mjrate::StoryRateAction::ALL_ACTION);
        this->addChild(rate_action, INT_MAX - 1);
    } else {
        GroupPromotion::attempToShow();
    }
}



ui::Widget::ccWidgetClickCallback MSLessonEnd::onLocateClickCallback(const std::string &callBackName) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSLessonEnd, onNextLesson),
        CLICK_MAP(MSLessonEnd, onBackToLesson)
    };
    
    if (function_map.count(callBackName) > 0) {
        return function_map.at(callBackName);
    }
    
    return nullptr;
}

void MSLessonEnd::onBackToLesson(cocos2d::Ref *sender) {
    if (_delegate) {
        _delegate->onBackToLesson();
    }
}

void MSLessonEnd::onNextLesson(cocos2d::Ref *sender) {
//    if (_delegate) {
//        _delegate->onContinueLearn();
//    }
    this->removeFromParent();
}

