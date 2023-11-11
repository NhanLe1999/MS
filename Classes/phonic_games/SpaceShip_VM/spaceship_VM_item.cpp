//
//  MJ_SpaceShip_VM_Item.h
//

#include "SpaceShip_VM_Item.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
using namespace cocos2d;
SpaceShip_VM_Item * SpaceShip_VM_Item::createMeteorite(METEORITE_INFO item_info) {
    SpaceShip_VM_Item * obj = new SpaceShip_VM_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoad_Meteorite(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void SpaceShip_VM_Item::didLoad_Meteorite(METEORITE_INFO item_info){
    this->item_info = item_info;
    
    item_gaf = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at("meteorite"));
    this->addChild(item_gaf, 5);
    item_gaf->setPosition(Vec2(180, 10));
    
    click_layout = cocos2d::ui::Layout::create();
    click_layout->setContentSize(Size(170, 150));
    this->addChild(click_layout, 6);
    click_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    click_layout->setTouchEnabled(true);
//    click_layout->setBackGroundColor(Color3B::YELLOW);
//    click_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    click_layout->setOpacity(200);
    click_layout->addClickEventListener(CC_CALLBACK_1(SpaceShip_VM_Item::onClickItem, this));
    
    text = cocos2d::ui::Text::create(item_info.character, PHONIC_MANAGER->getGameFontName(), 100);
    click_layout->addChild(text, 6,"lbl_info");
    text->setTextColor(Color4B::BLACK);
	text->enableOutline(cocos2d::Color4B::WHITE, 2);
    text->setPosition(click_layout->getContentSize()/2);
    
}

SpaceShip_VM_Item * SpaceShip_VM_Item::createShip() {
    SpaceShip_VM_Item * obj = new SpaceShip_VM_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoad_Ship();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void SpaceShip_VM_Item::didLoad_Ship(){
    item_gaf = GAFWrapper::createGAF(PHONIC_MANAGER->gaf_cache.at("ship"));
    this->addChild(item_gaf, 5);
    item_gaf->setPosition(Vec2(0, -170));
}

void SpaceShip_VM_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void SpaceShip_VM_Item::playSequence(std::string sequence, bool looped, std::function<void()> callback){
    item_gaf->playSequence(sequence, looped, [=] (gaf::GAFObject *obj, std::string name_sequence){
        if (callback) {
            callback();
        }
    });
}

void SpaceShip_VM_Item::shoot(Vec2 pos_shoot, std::function<void()> callback){
    auto bullet = cocos2d::ui::ImageView::create("games/SpaceShip_VM/SpaceShip_VM_bullet.png");
    this->addChild(bullet);
    auto duration = 0.4f/1024*pos_shoot.distance(Vec2(0, 0));
    item_gaf->playSequence("shot", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        item_gaf->playSequence("fly", true);
    });
    bullet->runAction(Sequence::create(DelayTime::create(0.1f), MoveTo::create(duration, pos_shoot), CallFunc::create([=]{
        if (callback){
            callback();
        }
        bullet->removeFromParent();
    }), NULL));
    
}
void SpaceShip_VM_Item::explosive(std::function<void()> callback){
    text->setVisible(false);
    item_gaf->playSequence("no", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        if (callback) {
            callback();
        }
    });
}

void SpaceShip_VM_Item::onChooseWrong(){
    text->setVisible(false);
}

void SpaceShip_VM_Item::onClickItem(cocos2d::Ref *sender){
    if (delegate) {
        delegate->onItemClicked(this);
    }
}

void SpaceShip_VM_Item::setDelegate(SpaceShip_VM_ItemDelegate *delegate){
    this->delegate = delegate;
}
