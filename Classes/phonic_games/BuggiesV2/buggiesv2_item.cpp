//
//  MJ_BuggiesV2_Item.h
//

#include "BuggiesV2_Item.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
using namespace cocos2d;
BuggiesV2_Item * BuggiesV2_Item::createItem(CHARACTER_INFO item_info) {
    BuggiesV2_Item * obj = new BuggiesV2_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}
#include "MJHelper.h"
void BuggiesV2_Item::didLoadFromCSB(CHARACTER_INFO item_info){
    this->item_info = item_info;
    
    text = cocos2d::ui::Text::create(item_info.character, PHONIC_MANAGER->getGameFontName(), 200);
    this->addChild(text);
    text->setPosition(text->getContentSize()/2);
    this->setContentSize(text->getContentSize());
    text->enableOutline(Color4B::BLACK, 3);
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    text->runAction(Repeat::create(Sequence::create(ScaleTo::create(1.0f, 1.1f), ScaleTo::create(1.0f, 1.0f), NULL), 10000000));
    
}

void BuggiesV2_Item::runText(){
    auto random = cocos2d::random(0, (int)list_pos.size()-1);
    auto pos_goal = list_pos[random];
//    CCLOG("%s = %f x %f", text->getString().c_str(), pos_goal.x, pos_goal.y);
	
    auto duration = 8 * 1.0f/1024*pos_goal.distance(this->getPosition());
    this->runAction(Sequence::create(EaseSineIn::create(MoveTo::create(duration, pos_goal)), DelayTime::create(cocos2d::random(5, 10)*0.1f), CallFunc::create([=]{
        this->runText();
    }), NULL));
}

void BuggiesV2_Item::onMergeCharacter(){
    this->removeChildByName("particle");
}

void BuggiesV2_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
    this->addStar();
	auto screen_size = this->getParent()->getContentSize();
	auto width_box = 100;
	auto min_h = 200;
	auto row = (screen_size.height - min_h) / width_box - 1;
	auto columne = screen_size.width / width_box - 1;

	for (int i = 0; i < columne; i++) {
		for (int j = 0; j < row; j++) {
			list_pos.push_back(Vec2(width_box / 2 + i * width_box, min_h + width_box / 2 + j * width_box));
		}
	}	
}
void BuggiesV2_Item::setDelegate(BuggiesV2_ItemDelegate *delegate){
    this->delegate = delegate;
}

void BuggiesV2_Item::addStar(){
    ParticleSystemQuad *exploisive = ParticleSystemQuad::create("plist/buggiesv2/colorbuggies_1.plist");
    this->addChild(exploisive, 100001);
    exploisive->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    //    exploisive->setDisplayFrame(cocos2d::SpriteFrame::create("particle/9.png", cocos2d::Rect(0,0,100,100)));
    exploisive->start();
    exploisive->setName("particle");
}
