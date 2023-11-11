//
//  Domino.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "Domino.h"
#include "Math_Libs.h"
    
Domino* Domino::createDomino() {
    auto p = new Domino();
    if (p && p->init()) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Domino::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    
    drawer = cocos2d::DrawNode::create();
    this->addChild(drawer, 2);
    
    _background = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sgameui/domino/domino.png", m_theme->getThemePath().c_str()));
    _background->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    _background->setPosition(cocos2d::Vec2::ZERO);
    _background->setScale9Enabled(true);
    _background->setCapInsets(cocos2d::Rect(5, 5, _background->getVirtualRendererSize().width-10, _background->getVirtualRendererSize().height-10));
    this->addChild(_background, 1);
    
    return true;
}

void Domino::setDomino(int a, int b, int width, bool vertical) {
    drawer->clear();
    for (cocos2d::ui::ImageView* dot : dots_list) {
        dot->removeFromParentAndCleanup(true);
    }
    dots_list.clear();
    
    first = a;
    second = b;
    
    this->setContentSize(cocos2d::Size(width, width*2));
    _background->setContentSize(getContentSize());
    
    if (!vertical) {
        this->setRotation(90);
    } else {
        this->setRotation(0);
    }
    
    //Draw dot first
    int delta = width/4;
    cocos2d::Vec2 center_first = cocos2d::Vec2(width/2, width/2);
    cocos2d::Vec2 center_second = cocos2d::Vec2(width/2, width*3/2);
    drawDots(first, delta, center_first);
    drawDots(second, delta, center_second);
}

void Domino::drawDots(int number, int delta, cocos2d::Vec2 center) {
    int dot_size = delta*0.4;
    switch (number) {
        case 0:
            // no dot
            break;
        case 1:
        {
            this->drawDot(center, dot_size, cocos2d::Color4F::BLACK);
        }
            break;
        case 2:
        {
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
        }
            break;
        case 3:
        {
            this->drawDot(center, dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
        }
            break;
        case 4:
        {
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
            
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
        }
            break;
        case 5:
        {
            this->drawDot(center, dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
            
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
        }
            break;
        default:
        {
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x-delta, center.y), dot_size, cocos2d::Color4F::BLACK);

            this->drawDot(cocos2d::Vec2(center.x+delta, center.y + delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y - delta), dot_size, cocos2d::Color4F::BLACK);
            this->drawDot(cocos2d::Vec2(center.x+delta, center.y), dot_size, cocos2d::Color4F::BLACK);
        }
            break;
    }
}

void Domino::drawDot(cocos2d::Vec2 center, float radius, cocos2d::Color4F color) {
    //drawer->drawDot(center, radius, color);
    
    cocos2d::ui::ImageView* dot = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sgameui/domino/domino_dot.png", m_theme->getThemePath().c_str()));
    dot->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    dot->setPosition(center);
    dot->setScale9Enabled(true);
    dot->setCapInsets(cocos2d::Rect(1,1, dot->getVirtualRendererSize().width-2, dot->getVirtualRendererSize().height-2));
    dot->setContentSize(cocos2d::Size(radius, radius)*2.5);
    this->addChild(dot, 3);
    dots_list.push_back(dot);
}
