//
//  LessonBase.cpp
//  Demo
//
//  Created by Ha Anh Man on 3/9/16.
//
//

#include "OverlayLayer.h"
#include "StoryData.h"
#include "HSLib.h"
USING_NS_CC;
// on "init" you need to initialize your instance
bool OverlayLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if (!LayerColor::initWithColor(Color4B::BLACK)) {
        return false;
    }
    this->setOpacity(255.0f*0.8f);
    sceneSize = XHSLib->getSizeScreen();
    scaleXY = StoryData::getInstance()->contentScaleXY;
    
    panel = cocos2d::ui::Layout::create();
    panel->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    panel->setBackGroundColor(cocos2d::Color3B::WHITE);
    panel->setOpacity(0);
    panel->setContentSize(sceneSize);
    panel->setTouchEnabled(true);
    addChild(panel);
    
    return true;
}


