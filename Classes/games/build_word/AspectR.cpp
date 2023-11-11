//
//  AspectR.cpp
//  InfiniteSpell
//
//  Created by Pijush Sarkar on 24/05/16.
//
//

#include "AspectR.h"

float AspectR::getWidth(float width) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return (width/DESIGN_WIDTH) * visibleSize.width;
}

float AspectR::getHeight(float height) {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    return (height/DESIGN_HEIGHT) * visibleSize.height;
}

Size AspectR::getSize(Size size) {
    return Size(getWidth(size.width),getHeight(size.height));
}

Size AspectR::getSize(float w, float h) {
    return Size(getWidth(w),getHeight(h));
}

float AspectR::getX(float x) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return x + origin.x;
}

float AspectR::getY(float y) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return y + origin.y;
}

Vec2 AspectR::getPos(Vec2 pos) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return Vec2(pos.x + origin.x, pos.y + origin.y);
}

Vec2 AspectR::getPos(float x, float y) {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    return Vec2(x + origin.x, y + origin.y);
}

Size AspectR::getScale(Size curSize, Size newSize) {
    float width = (1.0/curSize.width) * newSize.width;
    float height = (1.0/curSize.height) * newSize.height;
    
    return Size(width,height);
}

float AspectR::getMidX() {
	return getWidth(DESIGN_WIDTH / 2);
}

float AspectR::getMidY() {
	return getHeight(DESIGN_HEIGHT / 2);
}

Vec2 AspectR::getMid() {
	return Vec2(getMidX(), getMidY());
}

float AspectR::getFontSize(float val) {
	return (val / DESIGN_WIDTH) * Director::getInstance()->getVisibleSize().width;
}