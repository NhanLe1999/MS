//
//  MSActivityWait.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/5/17.
//

#include "MSActivityWait.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
USING_NS_CC;
INITIALIZE_READER(MSActivityWait);

std::once_flag ms_activity_wait_reader;

MSActivityWait * MSActivityWait::createView() {
    std::call_once(ms_activity_wait_reader, [] {
        REGISTER_CSB_READER(MSActivityWait);
    });
    
    auto view = reinterpret_cast<MSActivityWait *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/lesson/ActivityWaiting.csb"));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MSActivityWait::didLoadFromCSB() {
    utils::findChild<ui::Text *>(this, "text_waiting")->setString(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.waiting").c_str()));
}

void MSActivityWait::onEnter() {
    ui::Layout::onEnter();
    auto timeline = CSLoader::createTimeline("csb/new_ui/lesson/ActivityWaiting.csb");
    this->runAction(timeline);
    timeline->gotoFrameAndPlay(0, 60, 0, true);
    
    if (CONFIG_MANAGER->isVietnameseStories()) {
        auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        this->addChild(indicator);
        indicator->setPosition(this->getContentSize()/2);
        indicator->setVisible(true);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
        
    }
}
