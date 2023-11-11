//
//  Firefly_Item.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/3/18.
//

#include "Firefly_Item.h"

INITIALIZE_READER(Firefly_Item);

#define CSB_NAME "csb/game/FireFly/FireFly_Item.csb"

std::once_flag Firefly_Item_reader;
Firefly_Item* Firefly_Item::createItem(std::string word, int fly_id, int word_id, gaf::GAFAsset* asset){
    
    std::call_once(Firefly_Item_reader, []{
        REGISTER_CSB_READER(Firefly_Item);
    });
    
    auto p = static_cast<Firefly_Item*>(cocos2d::CSLoader::getInstance()->createNode(CSB_NAME));
    if(p){
        p->didLoadFromCSB(word, fly_id, word_id, asset);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void Firefly_Item::didLoadFromCSB(std::string word, int fly_id, int word_id, gaf::GAFAsset* asset){
    auto item_layout = utils::findChild<Layout*>(this, "item_layout");
    item_layout->setTouchEnabled(true);
    item_layout->addTouchEventListener(CC_CALLBACK_2(Firefly_Item::onTouchedItem, this));
    item_layout->setTag(fly_id);
    item_layout->setBackGroundColorType(BackGroundColorType::NONE);
    
    GAFWrapper* fly = GAFWrapper::createGAF(asset);
    item_layout->addChild(fly);
    fly->setPosition(item_layout->getContentSize()/2);
    fly->playSequence("idle", true);
    int flipped = random(0, 1);
    fly->setFlippedX(flipped);
    fly->setTag(flipped);
    fly->setName("gaf");
    
    Text* text = Text::create(word, font_name, font_sz);
    text->setTextColor(Color4B::BLACK);
    text->setFlippedX(flipped);
    text->setTag(word_id);
    text->setName("text");
    fly->addChild(text);
    text->setPosition(Vec2(fly->getContentSize().width*0.55, fly->getContentSize().height*0.45));
  
    //TODO: fix text font size
    while (text->getVirtualRendererSize().width > getContentSize().width * 0.55) {
        auto font_size = text->getFontSize();
        text->setFontSize(font_size - 2);
        CCLOG("Scale down font: %f", font_size);
    }
}

void Firefly_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void Firefly_Item::setDelegate(Firefly_ItemDelegate* delegate){
    _delegate = delegate;
}

void Firefly_Item::setMoving(bool isMove){
    _moving = isMove;
}

bool Firefly_Item::isMoving(){
    return _moving;
}

void Firefly_Item::onTouchedItem(Ref* sender, ui::Widget::TouchEventType type){
    //    CCLOG("onClicked");
    if (_delegate) {
        _delegate->onTouchedDelegate(sender, type);
    }
}
