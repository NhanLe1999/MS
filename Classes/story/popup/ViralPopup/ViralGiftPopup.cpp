//
//  ViralGiftPopup.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/30/18.
//

#include "ViralGiftPopup.h"
#include "StoryLanguageManager.h"

INITIALIZE_READER(ViralGiftPopup);
std::once_flag viral_gift_popup_reader;

ViralGiftPopup* ViralGiftPopup::createPopup(){
    std::call_once(viral_gift_popup_reader, [] {
        REGISTER_CSB_READER(ViralGiftPopup);
    });
    auto p = static_cast<ViralGiftPopup *>(CSLoader::createNodeWithVisibleSize ("csb/ViralGiftPopup.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void ViralGiftPopup::show(){
    auto popup = ViralGiftPopup::createPopup();
    popup->setAnchorPoint(Point::ANCHOR_MIDDLE);
    popup->setPosition(Director::getInstance()->getRunningScene()->getContentSize()/2);
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX - 1);
}

void ViralGiftPopup::didLoadFromCSB(){
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralGiftPopup.csb"));
    runani->setTag(1014);
    this->runAction(runani);
    runani->play("show", false);
    
    utils::findChild<ui::Text*>(this, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.title").c_str());
    utils::findChild<ui::Text*>(this, "content")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.content").c_str());
    utils::findChild<ui::Text*>(this, "titlebtn")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button").c_str());
}

void ViralGiftPopup::onOK(Ref* sender){
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/ViralGiftPopup.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        this->removeFromParent();
    });
}

#pragma mark - Callback Functions
ui::Widget::ccWidgetClickCallback ViralGiftPopup::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(ViralGiftPopup, onOK),
        
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

