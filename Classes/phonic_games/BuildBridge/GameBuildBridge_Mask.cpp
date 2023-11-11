//
//  GameBuildBridge_Mask.cpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 7/6/18.
//

#include "GameBuildBridge_Mask.h"

USING_NS_CC;

GameBuildBridge_Mask * GameBuildBridge_Mask::createMask() {
    auto p = new GameBuildBridge_Mask();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool GameBuildBridge_Mask::init() {
    if (!ui::Layout::init()) {
        return false;
    }
    
    this->setCascadeOpacityEnabled(true);
    
    auto visible_size = Director::getInstance()->getVisibleSize();
    
    _mask_size = Vec2(visible_size * 2).length();
    _mask = LayerRadialGradient::create(Color4B(Color3B::BLACK, 0xff), Color4B(Color3B::BLACK, 0x00), _mask_size, visible_size, 0);
    
    _mask->setBlendFunc({GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA});
    _mask->setContentSize(visible_size * 2);
    _mask->setColor(Color3B::WHITE);
    _mask->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _mask->setPosition(-Point(visible_size));
    this->addChild(_mask);
    
    this->getVirtualRenderer();
    
    return true;
}

void GameBuildBridge_Mask::focus(double duration) {
    this->runAction(ActionFloat::create(duration, _mask_size, 300, [this](float v) {
        _mask->setRadius(v);
    }));
}

void GameBuildBridge_Mask::unFocus(double duration) {
    this->runAction(ActionFloat::create(duration, _mask->getRadius(), _mask_size, [this](float v) {
        _mask->setRadius(v);
    }));

    this->runAction(ActionFloat::create(duration, 0x00, 0xff, [this](float v) {
        _mask->setEndColor(Color4B(0x00, 0x00, 0x00, v));
    }));
}
