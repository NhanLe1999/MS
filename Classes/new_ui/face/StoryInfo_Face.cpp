//
//  StoryInfo_Face.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/11/19.
//

#include "StoryInfo_Face.h"
#include "StoryCameraHelper.h"
USING_NS_CC;

INITIALIZE_READER(StoryInfo_Face);

std::once_flag story_info_face_reader;

StoryInfo_Face * StoryInfo_Face::createView() {
    std::call_once(story_info_face_reader, [] {
        REGISTER_CSB_READER(StoryInfo_Face);
    });
    
    auto node = reinterpret_cast<StoryInfo_Face *>(CSLoader::createNodeWithVisibleSize("csb/StoryCover_NewFace.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "StoryInfo_Mask.h"
void StoryInfo_Face::didLoadFromCSB() {
    addFaceMask();
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.change.face", [this](EventCustom *) {
        this->getParent()->addChild(StoryInfo_Mask::createView());
        this->getParent()->setPosition(Point::ZERO);
        this->removeFromParent();
    }), this);
}

void StoryInfo_Face::addFaceMask() {
    auto face_mask = utils::findChild<ui::ImageView *>(this, "face_mask");
    if (!face_mask) {
        return;
    }
    
    FileUtils::getInstance()->isFileExist("face.png", [this,face_mask](bool is_exist) {
        if (is_exist) {
            Director::getInstance()->getTextureCache()->reloadTexture("face.png");
            face_mask->loadTexture("face.png");
            face_mask->setOpacity(0xff);
        }
    });
}

void StoryInfo_Face::onEnter() {
    ui::Layout::onEnter();
}

void StoryInfo_Face::onExit() {
    ui::Layout::onExit();
}

void StoryInfo_Face::onBack(Ref * sender) {
    this->getParent()->runAction(Sequence::create(
                                                  EaseQuarticActionIn::create(
                                                                              MoveBy::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width, 0))),
                                 CallFunc::create([this] {
        this->removeFromParent();
    }), nullptr));
}

void StoryInfo_Face::onCamera(Ref * sender) {
    ms::helper::showCamera();
}

void StoryInfo_Face::onPhoto(Ref * sender) {
    ms::helper::showPhotoLibrary();
//    this->getParent()->runAction(Sequence::create(DelayTime::create(0.5),
//                                                  EaseQuarticActionIn::create(
//                                                                              MoveBy::create(0.5, Vec2(Director::getInstance()->getVisibleSize().width, 0))),
//                                                  CallFunc::create([this] {
//        this->removeFromParent();
//    }), nullptr));
}

ui::Widget::ccWidgetClickCallback StoryInfo_Face::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryInfo_Face, onBack),
        CLICK_MAP(StoryInfo_Face, onCamera),
        CLICK_MAP(StoryInfo_Face, onPhoto)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

