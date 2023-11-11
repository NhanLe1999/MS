//
//  MJ_GearsForCar_Item.h
//

#include "GearsForCar_Item.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#include "GearsForCar.h"
using namespace cocos2d;
GearsForCar_Item * GearsForCar_Item::createItem(GearsForCar_Item::GEAR_ITEM_WITH_TEXT item_info) {
    GearsForCar_Item * obj = new GearsForCar_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void GearsForCar_Item::didLoadFromCSB(GearsForCar_Item::GEAR_ITEM_WITH_TEXT item_info){
    this->_item_info = item_info;
    
    bgGear= cocos2d::ui::ImageView::create("games/GearsForCar/bg_item.png");
    this->addChild(bgGear);
    bgGear->setPosition(bgGear->getContentSize()/2);
    this->setContentSize(bgGear->getContentSize());
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    item = cocos2d::ui::ImageView::create(_item_info.image_path);
    bgGear->addChild(item);
    item->setPosition(bgGear->getContentSize()/2);
    if(item->getContentSize().width >=item->getContentSize().height && item->getContentSize().height>150){
        item->setScale(150/item->getContentSize().width);
    }
    else if(item->getContentSize().height >=item->getContentSize().width && item->getContentSize().width>150){
         item->setScale(150/item->getContentSize().height);
    }
    this->setCascadeOpacityEnabled(true);
    bgGear->setCascadeOpacityEnabled(true);

	text_item = cocos2d::ui::Text::create(_item_info.text, "fonts/KGWHATTHETEACHERWANTS.TTF", 70);
	text_item->setTextColor(Color4B::BLACK);
	text_item->setPosition(item->getPosition());
	text_item->setVisible(false);
	bgGear->addChild(text_item);
	
}

void GearsForCar_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();

}

void GearsForCar_Item::onClicked(){
    auto seq = Sequence::create(ScaleTo::create(0.1f, 1.1f), CallFunc::create([=](){
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(_item_info.audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
        });
    }), nullptr);
    this->runAction(seq);
}

void GearsForCar_Item::setClickedCallback(const std::function<void()> afunc){
    clickedFinish = afunc;
}

void GearsForCar_Item::showItem(){
    AUDIO_ENGINE->play("sounds/GearsForCar/pop.mp3");
    bgGear->runAction(ScaleTo::create(0.3f, 1.0f));
}

void GearsForCar_Item::setStart(){
    bgGear->setScale(0);
}

void GearsForCar_Item::showText() {
	this->setTouchEnabled(false);
	bgGear->runAction(Sequence::create(ScaleTo::create(0.15, 0, 1), CallFunc::create([=]() {
		item->setVisible(false);
		text_item->setVisible(true);
	}), ScaleTo::create(0.15, 1, 1),DelayTime::create(1.f), ScaleTo::create(0.15, 0, 1), CallFunc::create([=]() {
		item->setVisible(true);
		text_item->setVisible(false);
	}), ScaleTo::create(0.15, 1, 1), CallFunc::create([=]() {
		this->setTouchEnabled(true);
	}), NULL));
}


void GearsForCar_Item::onFinishClick()
{
	this->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f), CallFunc::create([=](){
		if (clickedFinish){
			clickedFinish();
		}
	}), nullptr));
}

