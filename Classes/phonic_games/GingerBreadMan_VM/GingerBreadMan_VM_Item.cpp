//
//  MJ_GingerBreadMan_VM_Item.h
//

#include "GingerBreadMan_VM_Item.h"
#include "PhonicConfig.h"
using namespace cocos2d;
GingerBreadMan_VM_Item * GingerBreadMan_VM_Item::createItem(GBM_CHARACTER_INFO item_info) {
    GingerBreadMan_VM_Item * obj = new GingerBreadMan_VM_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void GingerBreadMan_VM_Item::didLoadFromCSB(GBM_CHARACTER_INFO item_info){
    this->item_info = item_info;
    
}

void GingerBreadMan_VM_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
    pie = cocos2d::ui::ImageView::create("games/GingerBreadMan_VM/gbm_pie.png");
    this->addChild(pie);
    pie->setPosition(pie->getContentSize()/2);
    this->setContentSize(pie->getContentSize());
    pie->setTouchEnabled(true);
    pie->addClickEventListener(CC_CALLBACK_1(GingerBreadMan_VM_Item::onClicked, this));
    auto font_name = "fonts/danhvan.ttf";
    if (item_info.character == "/" || item_info.character == "`" || item_info.character == "~" || item_info.character == "?" || item_info.character == "." || item_info.character == "´"){
        font_name = "fonts/vmonkey_punctuation_custom_font.ttf";
    }
    text_lb = cocos2d::ui::Text::create(item_info.character, font_name, 100);
    //if (item_info.character == "´") text_lb->setString("/");
    this->addChild(text_lb, 2);
    text_lb->setPosition(pie->getPosition());
    text_lb->setTextColor(Color4B::BLACK);
}

void GingerBreadMan_VM_Item::onClicked(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onItemClicked(this);
    }
}

void GingerBreadMan_VM_Item::setChooseRight(){
    auto star_effect = GAFWrapper::createGAF("gaf/GingerBreadMan_VM/effec/effec.gaf");
    this->addChild(star_effect);
    star_effect->setPosition(pie->getContentSize()/2);
    star_effect->play(false);
    pie->removeFromParent();
}

void GingerBreadMan_VM_Item::onChooseWrong(){
    pie->setTouchEnabled(false);
    auto move1 = MoveBy::create(0.1f, Vec2(-5, 0));
    auto move2 = MoveBy::create(0.1f, Vec2(5, 0));
    pie->setColor(Color3B::ORANGE);
    this->runAction(Sequence::create(move1, move2, move2, move1, move1, move2, CallFunc::create([=]{
        pie->setTouchEnabled(true);
        pie->setColor(Color3B::WHITE);
    }), NULL));
}

void GingerBreadMan_VM_Item::setDelegate(GingerBreadMan_VM_ItemDelegate *delegate){
    this->delegate = delegate;
}
