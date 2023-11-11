//
//  MJSearchbar.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#include "MSSearchbar.h"
#include "MSGradeBox.h"
#include "MSSearchbarDelegate.h"
#include "MJDefinitions.h"

USING_NS_CC;
INITIALIZE_READER(MSSearchbar);

std::once_flag ms_searchbar_reader;

MSSearchbar * MSSearchbar::createBar(int i_gradeID) {
    std::call_once(ms_searchbar_reader, [] {
        REGISTER_CSB_READER(MSSearchbar);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Searchbar_new.csb");
    auto bar = reinterpret_cast<MSSearchbar *>(CSLoader::createNode(csb_name));
    if (bar) {
        bar->didLoadFromCSB(i_gradeID);
        return bar;
    }
    
    CC_SAFE_DELETE(bar);
    return nullptr;
}

void MSSearchbar::didLoadFromCSB(int i_gradeID) {
    _delegate = nullptr;
    auto gradebox_layout = utils::findChild<ui::Layout*>(this, "gradebox_layout");//"gradebox_placeholder");
    gradebox_layout->setAnchorPoint(Vec2(1, 0.5));
    gradebox_layout->setPositionX(utils::findChild<ui::Layout*>(this, "Panel_3")->getContentSize().width - 15);
    auto gradebox = MSGradeBox::createBox(false,i_gradeID);
    gradebox->setDelegate(this);
    gradebox->setName("gradebox");
    gradebox->setAnchorPoint(Vec2(1, 0.5));
    gradebox->setPosition(Vec2(gradebox_layout->getContentSize().width, gradebox_layout->getContentSize().height/2));
    gradebox_layout->addChild(gradebox);

    utils::findChild(this, "gradebox_placeholder")->removeFromParentAndCleanup(true);
    //placeholder->removeFromParent();
}

ui::Widget::ccWidgetClickCallback MSSearchbar::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSSearchbar, onSearch)
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void MSSearchbar::onSearch(cocos2d::Ref *sender) {
    mj::helper::playButtonFX();
    _delegate ? _delegate->onSelectSearch() : (void) nullptr;
}

void MSSearchbar::onSelectGradeBox() {
    _delegate ? _delegate->onSelectGrade() : (void) nullptr;
}

void MSSearchbar::onDeselectGradeBox(int grade_id) {
    _delegate ? _delegate->onDeselectGrade(grade_id) : (void) nullptr;
}
