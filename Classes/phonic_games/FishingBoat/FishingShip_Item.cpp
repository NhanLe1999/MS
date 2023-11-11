
#include "FishingShip_Item.h"
#include "HSAudioEngine.h"
#include "GAFWrapper.h"

INITIALIZE_READER(FishingShip_Item);

#define CSB_NAME "csb/game/FishingShip/FishingShip_Item.csb"

FishingShip_Item * FishingShip_Item::createItem(mj::PHONIC_INFO item_info) {
    auto p = static_cast<FishingShip_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void FishingShip_Item::didLoadFromCSB(mj::PHONIC_INFO item_info)
{
	this->item_info = item_info;
    Layout* item_layout = utils::findChild<Layout*>(this, "fish");
    
    item_layout->setTouchEnabled(true);
    item_layout->addTouchEventListener(CC_CALLBACK_2(FishingShip_Item::onClickItem, this));
   
    Text* text = Text::create();
    text->setString(item_info.text_full);
    text->setFontName(font_name);
    text->setFontSize(font_sz);
    text->setTextColor(Color4B::BLACK);
    text->setName("text_" + item_info.text_full);
    text->setTag(227);
    
    item_layout->addChild(text);
    text->setPosition(Vec2( item_layout->getContentSize().width*0.4, item_layout->getContentSize().height*0.5));
    
}

void FishingShip_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void FishingShip_Item::onClickItem(cocos2d::Ref *sender, ui::Widget::TouchEventType type){
    if(_delegate){
        _delegate->onClickedDelegate(sender, type);
    }
}

void FishingShip_Item::setDelegate(FishingShip_ItemDelegate *delegate){
    _delegate = delegate;
}

void FishingShip_Item::setSplash(bool is_set){
    _water_splash = is_set;
}

bool FishingShip_Item::getSplash(){
    return _water_splash;
}

void FishingShip_Item::setHint(bool is_hint){
    _hint = is_hint;

}

bool FishingShip_Item::getHint(){
    return _hint;
}

mj::PHONIC_INFO FishingShip_Item::getInfo()
{
	return item_info;
}


void FishingShip_Item::playEffect(bool correct){
   
    if(correct){
        //right
        
    }else{
        //wrong
        
    }
}

