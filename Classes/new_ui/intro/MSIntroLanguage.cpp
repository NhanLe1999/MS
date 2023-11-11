//
//  MSIntroLanguage.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#include "MSIntroLanguage.h"
#include "MSIntroLoad.h"

USING_NS_CC;
INITIALIZE_READER(MSIntroLanguage);

std::once_flag ms_intro_language_reader;
MSIntroLanguage * MSIntroLanguage::createView() {
    std::call_once(ms_intro_language_reader, [] {
        REGISTER_CSB_READER(MSIntroLanguage);
    });
    
    auto node = reinterpret_cast<MSIntroLanguage *>(CSLoader::createNode("csb/new_ui/intro/Intro_Language.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void MSIntroLanguage::didLoadFromCSB() {
    auto content = this->getChildByName("content");
    for (auto c : content->getChildren()) {
        auto name = c->getName();
        if (name.find("panel_") != std::string::npos) {
            auto panel = reinterpret_cast<ui::Layout *>(c);
            panel->setOpacity(150);
            _panels.push_back(panel);
            
            if (name == "panel_us") {
                onSelect(panel->getChildByName("button"));
            }
        }
    }
}

void MSIntroLanguage::onEnter() {
    ui::Layout::onEnter();
    auto content = this->getChildByName("content");
    auto delay = 0.f;
    auto counter = 0;
    for (auto c : content->getChildren()) {
        c->setPosition(c->getPosition() + Vec2(1000, 0));
        auto saved_opacity = c->getOpacity();
        c->setOpacity(0);
        c->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseQuinticActionOut::create(MoveBy::create(.5f, Vec2(-1000, 0))), FadeTo::create(.5f, saved_opacity), nullptr), nullptr));
        delay += .1f + counter++ * 0.01;
    }
}

ui::Widget::ccWidgetClickCallback MSIntroLanguage::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSIntroLanguage, onSelect),
        CLICK_MAP(MSIntroLanguage, onNext)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSIntroLanguage::onSelect(Ref * sender) {
    for (auto panel : _panels) {
        panel->setOpacity(150);
    }
    auto bt = reinterpret_cast<ui::Button *>(sender);
    bt->getParent()->setOpacity(255);
}

void MSIntroLanguage::onNext(Ref * sender) {
    auto content = this->getChildByName("content");
    auto delay = 0.f;
    auto counter = 0;
    auto nodes = content->getChildren();
    nodes.pushBack(utils::findChild(this, "button_next"));
    for (auto c : content->getChildren()) {
        c->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseQuinticActionIn::create(MoveBy::create(.5f, Vec2(-1000, 0))), FadeOut::create(.5f), nullptr), nullptr));
        delay += .1f - counter++ * 0.01;
    }
    
    this->scheduleOnce([=](float) {
        this->removeFromParent();
    }, delay + 0.5f, "schedule_to_remove");
    
    Director::getInstance()->getRunningScene()->scheduleOnce([](float) {
        auto intro_load = MSIntroLoad::createView();
        intro_load->setName("loadview");
        Director::getInstance()->getRunningScene()->addChild(intro_load);
    }, delay + .5f, "schedule_next_page");
}
