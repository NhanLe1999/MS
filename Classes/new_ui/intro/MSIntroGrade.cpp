//
//  MSIntroGrade.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#include "MSIntroGrade.h"
#include "MJHelper.h"
#include "MSIntroLanguage.h"

#include "MSIntroLoad.h"
#include "StoryLanguageManager.h"
#include "MSGradeManager.h"
USING_NS_CC;
INITIALIZE_READER(MSIntroGrade);

std::once_flag ms_intro_grade_reader;
MSIntroGrade * MSIntroGrade::createView() {
    std::call_once(ms_intro_grade_reader, [] {
        REGISTER_CSB_READER(MSIntroGrade);
    });
    
    Application::getInstance()->getCurrentLanguageCode();
    
    auto node = reinterpret_cast<MSIntroGrade *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/intro/Intro_Grade.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "StoryConfigManager.h"
Scene * MSIntroGrade::createScene() {
    auto scene = Scene::create();
    if (scene) {
        auto bg_img = "bg.jpg";
        if(CONFIG_MANAGER->isVietnameseStories()){
            bg_img = "mjstory/new_ui_vn/bg_loading.png";
        }
        auto bg = ui::ImageView::create(bg_img);
        bg->setPosition(Director::getInstance()->getVisibleSize() / 2);
        bg->setContentSize(Director::getInstance()->getVisibleSize());
        bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
        scene->addChild(bg);
//
//        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
//        auto grade_id = ms::GradeManager::getInstance().getCurrentGradeId();
//        if (grade_id == 0) {
//            auto view = MSIntroGrade::createView();
//            scene->addChild(view);
//            return scene;
//        }
        
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        auto grade_id = ms::GradeManager::getInstance().getCurrentGradeId();
        if (grade_id == 0) {
            ms::GradeManager::getInstance().setInitialGrade("All Grades");
        }
        auto view = MSIntroLoad::createView();
        view->setName("loadview");
        scene->addChild(view);
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

void MSIntroGrade::didLoadFromCSB() {
    CCLOG("Lang: %s", Application::getInstance()->getCurrentLanguageCode());
    
    auto content = this->getChildByName("content");
    for (auto c : content->getChildren()) {
        if (dynamic_cast<ui::Button *>(c)) {
            auto bt = reinterpret_cast<ui::Button *>(c);
            _buttons.push_back(bt);
            
            bt->setPressedActionEnabled(false);
            uncheckButton(bt);
        }
    }
    
    utils::findChild(this, "button_next")->setVisible(false);
}

void MSIntroGrade::onEnter() {
    ui::Layout::onEnter();
    
//    experimental::AudioEngine::lazyInit();
//    auto sample_id = experimental::AudioEngine::play2d("sounds/Snowman.mp3");
//    experimental::AudioEngine::getState(sample_id);
//    this->scheduleOnce([=](float) {
//        auto duration = experimental::AudioEngine::getDuration(sample_id);
//        this->scheduleOnce([](float) {
//            experimental::AudioEngine::play2d("sounds/sample.mp3");
//        }, duration  - 0.1, "actual_schedule");
//    }, 0.05, "schedule_next_audio");
//    experimental::AudioEngine::setFinishCallback(sample_id, [](int, std::string) {
//        CCLOG("Stop playing.");
//    });
    
//    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/sample.mp3");
    
    auto title = utils::findChild<ui::Text *>(this, "question");
    title->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "intro.grade.title"));
    auto length = StringUtils::getCharacterCountInUTF8String(title->getString());
    auto delay = 0.f;
    for (auto i = 0; i < length; ++i) {
        auto letter = title->getLetter(i);
        if (!letter) {
            continue;
        }
        
        letter->setOpacity(0);
        letter->setScale(1.5);
        letter->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(FadeIn::create(0.25), ScaleTo::create(0.25, 1), nullptr), nullptr));
        delay += 0.05;
    }
    
    auto saved_delay = delay;
    
    auto details = utils::findChild<ui::Text *>(this, "details");
    details->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "intro.grade.detail"));
    length = StringUtils::getCharacterCountInUTF8String(details->getString());
    for (auto i = 0; i < length; ++i) {
        auto letter = details->getLetter(i);
        if (!letter) {
            continue;
        }
        
        letter->setOpacity(0);
        letter->runAction(Sequence::create(DelayTime::create(delay), FadeTo::create(0.25, 200), nullptr));
        delay += 0.015;
    }
    
    auto notice = utils::findChild<ui::Text *>(this, "notice");
    notice->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "intro.grade.notice"));
    notice->setPosition(notice->getPosition() + Vec2(0, -200));
    notice->setOpacity(0);
    notice->runAction(Sequence::create(DelayTime::create(1), Spawn::create(EaseQuarticActionOut::create(MoveBy::create(0.5, Vec2(0, 200))), FadeIn::create(0.5), NULL), NULL));
    
    mj::helper::shuffle(_buttons);
    delay = saved_delay + 0.25;
    
    for (auto button : _buttons) {
        button->setOpacity(0);
        button->setScale(0);
        button->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(FadeIn::create(0.25), EaseBackOut::create(ScaleTo::create(0.5, 1)), nullptr), nullptr));
        button->setCascadeOpacityEnabled(false);
        delay += 0.05;
    }
    
    utils::findChild<ui::Text *>(this, "text_next")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "intro.grade.btnext"));
}

ui::Widget::ccWidgetClickCallback MSIntroGrade::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSIntroGrade, onSelect),
        CLICK_MAP(MSIntroGrade, onNext),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSIntroGrade::onSelect(Ref * sender) {
    for (auto bt : _buttons) {
        if (bt == sender) {
            checkButton(bt);
        } else {
            uncheckButton(bt);
        }
    }
}

void MSIntroGrade::onNext(Ref * sender) {
    auto content = this->getChildByName("content");
    auto delay = 0.f;
    auto counter = 0;
    for (auto c : content->getChildren()) {
        c->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseQuinticActionIn::create(MoveBy::create(.5f, Vec2(-1000, 0))), FadeOut::create(.5f), nullptr), nullptr));
        delay += .1f - counter++ * 0.01;
    }
    
    this->scheduleOnce([=](float) {
        this->removeFromParent();
    }, delay + 0.5f, "schedule_to_remove");
    
    Director::getInstance()->getRunningScene()->scheduleOnce([](float) {
        auto intro_lang = MSIntroLoad::createView();
        intro_lang->setName("loadview");
         Director::getInstance()->getRunningScene()->addChild(intro_lang);
    }, delay + .5f, "schedule_next_page");
}

void MSIntroGrade::checkButton(ui::Button * bt) {
    auto bg = bt->getChildByName("bg");
    bg ? bg->setVisible(true) : (void) nullptr;
    
    auto text = bt->getChildByName<ui::Text *>("text");
    text ? text->setTextColor(Color4B(Color3B(38, 176, 230))) : (void) nullptr;
    
    auto grade = text->getString();
    ms::GradeManager::getInstance().setInitialGrade(grade);
    
    showNextButton();
}


void MSIntroGrade::uncheckButton(ui::Button * bt) {
    auto bg = bt->getChildByName("bg");
    bg ? bg->setVisible(false) : (void) nullptr;
    
    auto text = bt->getChildByName<ui::Text *>("text");
    text ? text->setTextColor(Color4B::WHITE) : (void) nullptr;
    
    auto bt_name = bt->getName();
    if (bt_name == "button_grade_2" || bt_name == "button_grade_3" || bt_name == "button_grade_4" || bt_name == "button_grade_5") {
        bt->setBright(false);
        bt->setEnabled(false);
        text->setTextColor(Color4B::WHITE);
        text->setOpacity(150);
        bt->runAction(Sequence::create(DelayTime::create(3), FadeTo::create(0.25, 150), NULL));
    }
}

void MSIntroGrade::showNextButton() {
    auto next_button = utils::findChild(this, "button_next");
    if (next_button->isVisible()) {
        return;
    }
    next_button->setOpacity(0);
    next_button->setPosition(next_button->getPosition() + Vec2(-100, 0));
    next_button->setVisible(true);
    next_button->runAction(EaseQuinticActionOut::create(MoveBy::create(0.5, Point(100, 0))));
    next_button->runAction(FadeIn::create(0.25));
}
