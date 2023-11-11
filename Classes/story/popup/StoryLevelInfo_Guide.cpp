//
//  StoryLevelInfo_Guide.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryLevelInfo_Guide.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryFreeManager.h"
#include "StoryLevelInfo.h"
#include "MJStoryInfo.h"
#include "StoryDataModel.h"

INITIALIZE_READER_INSTANCE(StoryLevelInfo_GuideReader);

USING_NS_CC;
using namespace ui;

StoryLevelInfo_Guide * StoryLevelInfo_Guide::createPage() {
    auto p = static_cast<StoryLevelInfo_Guide *>(CSLoader::createNodeWithVisibleSize("csb/StoryLevelInfo_Guide.csb"));
    if (p) {
        p->didLoadFromCSB();
    }
    return p;
}

void StoryLevelInfo_Guide::didLoadFromCSB() {
    
//    content->setString("5-Star ratings from you will help us\nprovide awesome updates!");
}
#pragma mark - onEnter
void StoryLevelInfo_Guide::onEnter() {
    Layout::onEnter();
    Node *bg;
    if (_isPortrait) {
        auto layout_doc = this->getChildByName("layout_doc");
        layout_doc->setVisible(true);
        bg = layout_doc->getChildByName("bg_doc");
    }else bg = this->getChildByName("bg_ngang");
    bg->setVisible(true);
    title = bg->getChildByName<Text *>("title");
    content = bg->getChildByName<Text *>("content");
    
    title->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("levelinfo", "guide_title"));
    content->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("levelinfo", "guide_content", true));

}

void StoryLevelInfo_Guide::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryLevelInfo_Guide::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}
void StoryLevelInfo_Guide::initWithData(StoryInfoObj storyInfo, bool isPortrait){
    _isPortrait = isPortrait;
    _storyInfo = storyInfo;
}

void StoryLevelInfo_Guide::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    
    auto background = utils::findChild(this, "panel_background");
    background->runAction(Sequence::create(FadeOut::create(0.5), CallFunc::create([=] {
        isRunning = false;
        if (!isShow) {
            if (isClickLevelInfo){
                auto info = StoryDataModel::getInstance()->getStoryById(_storyInfo._id);
                auto level_info = MJStoryInfo::createPage(info);
                //                auto level_info = StoryLevelInfo::createPage();
                //                level_info->initWithData(_storyInfo, _isPortrait);
                DIRECTOR->getRunningScene()->addChild(level_info, 39999999);
                //                level_info->onShowHidePopUp(true);
                
            }
            this->removeFromParent();
            
        }
    }), nullptr));
//    auto runani = CSLoader::createTimeline(StringUtils::format("csb/StoryLevelInfo_Guide.csb"));
//    runani->setTag(1011);
//    this->runAction(runani);
//    if (isShow) {
//        runani->play("show", false);
//    }else runani->play("hide", false);
//    runani->setLastFrameCallFunc([=]{
//
//    });
}

#pragma mark - BUTTON
void StoryLevelInfo_Guide::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

void StoryLevelInfo_Guide::onLevelInfo(cocos2d::Ref *sender){
    isClickLevelInfo = true;
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryLevelInfo_Guide::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryLevelInfo_Guide, onClose),
        CLICK_MAP(StoryLevelInfo_Guide, onLevelInfo),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


