//
//  MJ_GingerBreadMan_Item.h
//

#include "GingerBreadMan_Item.h"
#include "PhonicConfig.h"
using namespace cocos2d;
GingerBreadMan_Item * GingerBreadMan_Item::createItem(GBM_CHARACTER_INFO item_info) {
    GingerBreadMan_Item * obj = new GingerBreadMan_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void GingerBreadMan_Item::didLoadFromCSB(GBM_CHARACTER_INFO item_info){
    this->item_info = item_info;
    
}

void GingerBreadMan_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
    pie = cocos2d::ui::ImageView::create("games/gingerbreadman/gbm_pie.png");
    this->addChild(pie);
    pie->setPosition(pie->getContentSize()/2);
    this->setContentSize(pie->getContentSize());
    pie->setTouchEnabled(true);
    pie->addClickEventListener(CC_CALLBACK_1(GingerBreadMan_Item::onClicked, this));
    
    text_lb = cocos2d::ui::Text::create(item_info.character, PHONIC_MANAGER->getGameFontName(), 100);
    this->addChild(text_lb, 2);
    text_lb->setPosition(pie->getPosition());
    text_lb->setTextColor(Color4B::BLACK);
}

void GingerBreadMan_Item::onClicked(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onItemClicked(this);
    }
}

void GingerBreadMan_Item::setChooseRight(){
    auto star_effect = GAFWrapper::createGAF("gaf/gingerbreadman/effec/effec.gaf");
    this->addChild(star_effect);
    star_effect->setPosition(pie->getContentSize()/2);
    star_effect->play(false);
    pie->removeFromParent();
}

void GingerBreadMan_Item::onChooseWrong(){
    pie->setTouchEnabled(false);
    auto move1 = MoveBy::create(0.1f, Vec2(-5, 0));
    auto move2 = MoveBy::create(0.1f, Vec2(5, 0));
    pie->setColor(Color3B::ORANGE);
    this->runAction(Sequence::create(move1, move2, move2, move1, move1, move2, CallFunc::create([=]{
        pie->setTouchEnabled(true);
        pie->setColor(Color3B::WHITE);
    }), NULL));
}

void GingerBreadMan_Item::setDelegate(GingerBreadMan_ItemDelegate *delegate){
    this->delegate = delegate;
}
