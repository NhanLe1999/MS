//
//  MSStoryGuide.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/3/18.
//

#include "MSStoryGuide.h"
#include "StoryLanguageManager.h"
#include "HSLib.h"
#include "MJDefinitions.h"

USING_NS_CC;
INITIALIZE_READER(MSStoryGuide);

std::once_flag ms_story_guide_reader;

MSStoryGuide * MSStoryGuide::createGuide() {
    std::call_once(ms_story_guide_reader, [] {
        REGISTER_CSB_READER(MSStoryGuide);
    });
    
    auto guide = reinterpret_cast<MSStoryGuide *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/Guide.csb"));
    if (guide) {
        guide->didLoadFromCSB();
    } else {
        CC_SAFE_DELETE(guide);
    }
    
    return guide;
}

void MSStoryGuide::didLoadFromCSB() {
    auto guide = this->getChildByName<ui::Layout *>("guide");
    guide->setTouchEnabled(true);
    guide->setSwallowTouches(false);
    guide->setPropagateTouchEvents(true);

    auto swipeDown = utils::findChild<ui::Text*>(this, "swipe.down");
    auto swipeLeft = utils::findChild<ui::Text*>(this, "swipe.left");
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
    {
        swipeDown->setFontName("fonts/leelawdb.ttf");
        swipeLeft->setFontName("fonts/leelawdb.ttf");
    }
    swipeDown->setString(LANGUAGE_MANAGER->getDisplayTextByKey("swipe.down"));
    swipeLeft->setString(LANGUAGE_MANAGER->getDisplayTextByKey("swipe.left"));
    
    guide->addClickEventListener([guide](Ref * sender) {
        guide->setVisible(false);
    });
}

void MSStoryGuide::startTimer() {
    if (MJDEFAULT->getIntegerForKey(key_count_guide_story, 0) > 5) {
        return;
    }
    
    if (MJDEFAULT->getBoolForKey(key_current_guide_showed, false)) {
        return;
    }
    this->stopAllActions();
    this->runAction(Sequence::create(ActionFloat::create(5, 0, 5, [=](float v) {
        this->getChildByName<ui::Text *>("timer")->setString(StringUtils::format("%0.2f", v));
    }), CallFunc::create([=] {
        if (XHSLib->show_popup_subscribe) {
            return;
        }
        this->getChildByName("guide")->runAction(FadeIn::create(0.25));
    }), nullptr));
}
