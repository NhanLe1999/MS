//
//  StoryInfo_Mask.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 2/14/19.
//

#include "StoryInfo_Mask.h"
#include "MJHelper.h"

USING_NS_CC;

INITIALIZE_READER(StoryInfo_Mask);

std::once_flag storyinfo_mask_reader_flag;

StoryInfo_Mask * StoryInfo_Mask::createView() {
    std::call_once(storyinfo_mask_reader_flag, [] {
        REGISTER_CSB_READER(StoryInfo_Mask);
    });
    
    auto node = reinterpret_cast<StoryInfo_Mask *>(CSLoader::createNodeWithVisibleSize("csb/StoryCover_Mask.csb"));
    if (node) {
        node->didLoadFromCSB();
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

void StoryInfo_Mask::didLoadFromCSB() {
    this->setTouchEnabled(true);
    
    auto mask_sprite = Sprite::create("mjstory/new_ui/face_mask.png");
    auto mask_size = mask_sprite->getContentSize();
    mask_sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mask_sprite->setContentSize(Size(500, 500));
    
    auto mask = cocos2d::ClippingNode::create(mask_sprite);
    mask->setName("mask");
    mask->setAlphaThreshold(0);
    mask->setPosition(mask_size / 2);
    
    Director::getInstance()->getTextureCache()->reloadTexture("face.jpg");
    
    _sprite = Sprite::create("face.jpg");
    _sprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mask->addChild(_sprite);
    
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(500, 500));
    layout->addChild(mask);
    layout->setPosition(Point::ZERO);
    mask->setPosition(Point(250, 250));
    mask->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mask->setOpacity(0x44);
    
    auto mask_placeholder = utils::findChild(this, "mask_placeholder");
    mask_placeholder->addChild(layout);
    layout->setName("facemask");
    
    _mask_bounding_box = mask_placeholder->getBoundingBox();
    _mask_bounding_box.origin = mask_placeholder->getParent()->convertToWorldSpace(_mask_bounding_box.origin);
}

void StoryInfo_Mask::onEnter() {
    ui::Layout::onEnter();
}

void StoryInfo_Mask::onExit() {
    ui::Layout::onExit();
}

void StoryInfo_Mask::onBack(Ref * sender) {
    this->removeFromParent();
}

void StoryInfo_Mask::onTouch(Ref * sender, ui::Widget::TouchEventType event) {
    auto wg = reinterpret_cast<ui::Widget *>(sender);
    switch (event) {
        case ui::Widget::TouchEventType::BEGAN:
            _original_position = _sprite->getPosition();
            break;
            
        case ui::Widget::TouchEventType::MOVED: {
            auto new_position = _original_position + wg->getTouchMovePosition() - wg->getTouchBeganPosition();
            _sprite->setPosition(new_position);
            adjustFaceInBox();
        }
            break;
            
        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED: {
            auto pos = _sprite->convertToNodeSpace(this->getContentSize() / 2);
            CCLOG("center point: %f, %f", pos.x, pos.y);
            auto new_anchor = Point(pos.x / _sprite->getContentSize().width, pos.y / _sprite->getContentSize().height);
            CCLOG("new_anchor: %f, %f", new_anchor.x, new_anchor.y);
            _sprite->setAnchorPoint(new_anchor);
            _sprite->setPosition(Point::ZERO);
        }
            break;
            
        default:
            break;
    }
}

void StoryInfo_Mask::onZoomIn(Ref * sender, ui::Widget::TouchEventType event) {
    CCLOG("zooming in...");
    if (event == ui::Widget::TouchEventType::BEGAN) {
        this->schedule([this](float dt) {
            _scale += dt;
            _sprite->setScale(_scale);
            
            auto bb = _sprite->getBoundingBox();
            if (bb.size.width < _mask_bounding_box.size.width || bb.size.height < _mask_bounding_box.size.height) {
                _scale -= dt;
                _sprite->setScale(_scale);
                
                return;
            }
            
            adjustFaceInBox();
        }, "zoom_in");
    }
    
    if (event == ui::Widget::TouchEventType::ENDED) {
        this->unschedule("zoom_in");
    }
}

void StoryInfo_Mask::onZoomOut(Ref * sender, ui::Widget::TouchEventType event) {
    CCLOG("zooming out...");
    if (event == ui::Widget::TouchEventType::BEGAN) {
        this->schedule([this](float dt) {
            _scale -= dt;
            _sprite->setScale(_scale);
            
            auto bb = _sprite->getBoundingBox();
            if (bb.size.width < _mask_bounding_box.size.width || bb.size.height < _mask_bounding_box.size.height) {
                _scale += dt;
                _sprite->setScale(_scale);
                
                return;
            }
            
            adjustFaceInBox();
        }, "zoom_out");
    }
    
    if (event == ui::Widget::TouchEventType::ENDED) {
        this->unschedule("zoom_out");
    }
}

void StoryInfo_Mask::adjustFaceInBox() {
    auto bb = _sprite->getBoundingBox();
    bb.origin = _sprite->getParent()->convertToWorldSpace(bb.origin);
    
    Vec2 vector_translation;
    if (bb.getMinX() > _mask_bounding_box.getMinX()) {
        vector_translation.x = _mask_bounding_box.getMinX() - bb.getMinX();
    }
    
    if (bb.getMinY() > _mask_bounding_box.getMinY()) {
        vector_translation.y = _mask_bounding_box.getMinY() - bb.getMinY();
    }
    
    if (bb.getMaxX() < _mask_bounding_box.getMaxX()) {
        vector_translation.x = _mask_bounding_box.getMaxX() - bb.getMaxX();
    }
    
    if (bb.getMaxY() < _mask_bounding_box.getMaxY()) {
        vector_translation.y = _mask_bounding_box.getMaxY() - bb.getMaxY();
    }
    
    _sprite->setPosition(_sprite->getPosition() + vector_translation);
}

void StoryInfo_Mask::onDone(Ref * sender) {
    auto layout = utils::findChild(this, "facemask");
    auto final_sprite = utils::captureNode(layout, 1);
    final_sprite->saveToFile(FileUtils::getInstance()->getWritablePath() + "face.png", false);
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.edit.face");
    this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback StoryInfo_Mask::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryInfo_Mask, onBack),
        CLICK_MAP(StoryInfo_Mask, onDone)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

ui::Widget::ccWidgetTouchCallback StoryInfo_Mask::onLocateTouchCallback(const std::string & name) {
    if (name == "onTouch") {
        return CC_CALLBACK_2(StoryInfo_Mask::onTouch, this);
    }
    
    if (name == "onZoomIn") {
        return CC_CALLBACK_2(StoryInfo_Mask::onZoomIn, this);
    }
    
    if (name == "onZoomOut") {
        return CC_CALLBACK_2(StoryInfo_Mask::onZoomOut, this);
    }
    
    return nullptr;
}
