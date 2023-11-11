//
//  Math_Slot.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/13/17.
//
//

#include "Math_Slot.h"

INITIALIZE_READER_INSTANCE(Math_SlotReader);

Math_Slot* Math_Slot::createSlot()
{
    auto p = (Math_Slot *)(cocos2d::CSLoader::createNode("math_slot.csb"));
    if (p && p->init()) {
        //p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    return p;
}

bool Math_Slot::init()
{
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    this->setContentSize(cocos2d::Size(200, 200));
    
    _background=this->getChildByName<cocos2d::ui::ImageView*>("bg");
    _lbl_value=this->getChildByName<cocos2d::ui::Text*>("label");
    _enable=false;
    
    drawer = cocos2d::DrawNode::create();
    addChild(drawer);
    
    this->setTag(2005);
    this->setName("Math_Slot");
    
    return true;
}


void Math_Slot::setEndble(int id, math::resource::Image* bg)
{
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _background->setColor(cocos2d::Color3B::WHITE);
    _background->setVisible(true);
    _lbl_value->setVisible(true);
    
    _enable=true;
    
    _used=false;
    _id=id;
    setBackground(bg);
}

void Math_Slot::setDisable()
{
    _enable=false;
    onHide(0);
}

bool Math_Slot::isEnable()
{
    return _enable;
}

int Math_Slot::getId()
{
    return _id;
}

bool Math_Slot::isUsed()
{
    return _used;
}

bool Math_Slot::isAvailable(cocos2d::Vec2 point, int padding)
{
    cocos2d::Rect slot_bound=this->getBoundingBox();
    slot_bound.origin=cocos2d::Vec2(slot_bound.origin.x-padding, slot_bound.origin.y-padding);
    slot_bound.size=cocos2d::Size(slot_bound.size.width+padding*2, slot_bound.size.height+padding*2);

    if (slot_bound.containsPoint(point)) {
        return true;
    }
    return false;
}

void Math_Slot::setComparisionValue(math::comparision::Value comparision)
{
    _comparision=comparision;
}

bool Math_Slot::isCorrect(math::comparision::Key key)
{
    if (_used) {
        return _linker->isEqual(_comparision, key);
    }
    return false;
}

void Math_Slot::setSlotSize(cocos2d::Size size)
{
    //size = size * 0.9;
    this->setContentSize(size);
    _background->setContentSize(size);
    _background->setPosition(size/2);
    
    _lbl_value->setPosition(cocos2d::Vec2(size.width/2, 0));
    _lbl_value->setFontSize(size.height*.35);    
}

void Math_Slot::fitSlotSize() {
    setSlotSize (getContentSize());
}

#pragma mark - background
void Math_Slot::setBackground(math::resource::Image *image, float padding)
{
    if (image!=nullptr) {
        _background->setVisible(true);
        _bg_src=image;
        _bg_src->used=true;
        _background->loadTexture(image->path);
        _background->setScale9Enabled(true);
        auto render_size = _background->getVirtualRendererSize();
        cocos2d::Rect cap=cocos2d::Rect(1, 1, render_size.width-2, render_size.height-2);
        if (padding > 0) {
            padding = padding>0.45?0.45:padding;
            cap=cocos2d::Rect(render_size.width*padding, render_size.height*padding, render_size.width*(1-2*padding), render_size.height*(1-2*padding));
        }
        _background->setCapInsets(cap);
        return;
    }
    _background->setVisible(false);
}

void Math_Slot::setBackgroundVisible(bool visible)
{
    _background->setVisible(visible);
}

void Math_Slot::setBackgroundColor(cocos2d::Color3B color)
{
    _background->setColor(color);
}

cocos2d::ui::ImageView* Math_Slot::getBackground()
{
    return _background;
}

math::resource::Image* Math_Slot::getBackgroundSource()
{
    return _bg_src;
}


#pragma mark - Text
void Math_Slot::setValueText(std::string text)
{
    _lbl_value->setString(text);
}

void Math_Slot::setValueTextVisible(bool visible)
{
    _lbl_value->setVisible(visible);
}

std::string Math_Slot::getValueText()
{
    return _lbl_value->getString();
}

cocos2d::ui::Text* Math_Slot::getValueLabel()
{
    return _lbl_value;
}


#pragma mark - Linker
void Math_Slot::setLinker(Math_Object *object)
{
    _used=true;
    _linker=object;
}

void Math_Slot::removeLinker()
{
    _linker=nullptr;
    _used=false;
}

Math_Object* Math_Slot::getLinker()
{
    return _linker;
}


#pragma mark - Animation
void Math_Slot::onShow(float delay, math::animation::ObjectShowType show)
{
    this->stopAllActions();
    auto position = this->getPosition();
    this->setOpacity(0);
    
    switch (show) {
        case math::animation::No_Effect :
        {
            this->setOpacity(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
        }
            break;
        case math::animation::ScaleUp :
        {
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
        }
            break;
        case math::animation::SlideUp :
        {
            this->setPosition(cocos2d::Vec2(position.x, position.y - 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideDown :
        {
            this->setPosition(cocos2d::Vec2(position.x, position.y + 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideLeft :
        {
            this->setPosition(cocos2d::Vec2(position.x + 100, position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        case math::animation::SlideRight :
        {
            this->setPosition(cocos2d::Vec2(position.x - 100, position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position)))));
        }
            break;
        default:
        {
            this->setRotation(180);
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)), cocos2d::RotateTo::create(0.4, 0), NULL)));
        }
            break;
    }
}

void Math_Slot::onHide(float delay, math::animation::ObjectShowType revert_show)
{
    if (getLinker()!=nullptr) {
        getLinker()->onHide(delay, revert_show);
        removeLinker();
    }
    
    this->stopAllActions();
    auto current_position = this->getPosition();

    switch (revert_show) {
        case math::animation::No_Effect :
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::ScaleUp :
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideUp :
        {
            auto position = cocos2d::Vec2(current_position.x, current_position.y - 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideDown :
        {
            auto position = cocos2d::Vec2(current_position.x, current_position.y + 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideLeft :
        {
            auto position = cocos2d::Vec2(current_position.x + 100, current_position.y);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideRight :
        {
            auto position = cocos2d::Vec2(current_position.x - 100, current_position.y);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        default:
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)), cocos2d::RotateTo::create(0.4, 180), NULL), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
    }
}

