
#include "HatchingEggs_Item.h"

INITIALIZE_READER(HatchingEggs_Item);

#define CSB_NAME "csb/game/HatchingEggs/HatchingEggs_Item.csb"

HatchingEggs_Item * HatchingEggs_Item::createItem(mj::PHONIC_INFO item_info) {
    auto p = static_cast<HatchingEggs_Item *>(cocos2d::CSLoader::createNode(CSB_NAME));
    if (p) {
		p->didLoadFromCSB(item_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void HatchingEggs_Item::didLoadFromCSB(mj::PHONIC_INFO item_info)
{
    if (CONFIG_MANAGER->isGameVMonkey()){
       font_name = PHONIC_MANAGER->getGameFontName();
    }
    _audio = item_info.audio_full;
    auto chick_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "chicken_layout");
    chick_layout->addClickEventListener(CC_CALLBACK_1(HatchingEggs_Item:: onClickChicken, this)) ;

    utils::findChild(chick_layout, "green")->setLocalZOrder(-4);

    GAFWrapper* chicken = GAFWrapper::createGAF("gaf/hatchingeggs/Wrong/Wrong.gaf");
    chicken->setName("chicken_gaf");
    chick_layout->addChild(chicken,-3);
    chicken->setPosition(Vec2(chick_layout->getContentSize().width *0.4, chick_layout->getContentSize().height*0.65));
    
    //create text
    auto text = ui::Text::create("", font_name, font_sz);
    text->setString(item_info.text_full);
    text->setFontSize(font_sz);
    text->setFontName(font_name);
    text->setTextColor(Color4B::BLACK);
    text->setName("chick_text");
    text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    chicken->addChild(text);
    text->setPosition(Vec2(chicken->getContentSize().width*0.55, chicken->getContentSize().height*0.4));
    text->setOpacity(0);
    
    
}

void HatchingEggs_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void HatchingEggs_Item::onClickChicken(cocos2d::Ref *sender){
    if(_delegate){
        _delegate->onClickedDelegate(this);
    }
}

void HatchingEggs_Item::setDelegate(HatchingEggs_ItemDelegate *delegate){
    _delegate = delegate;
}

void HatchingEggs_Item::playEffect(bool correct){
    GAFWrapper* chicken_gaf = cocos2d::utils::findChild<GAFWrapper *>(cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "chicken_layout"), "chicken_gaf");
    Vec2 chick_pos = chicken_gaf->getPosition();
    
    if(correct){
        //right
        chicken_gaf->runAction(Spawn::create(Sequence::create(ScaleTo::create(0.3f, 1.25f, 0.75f),ScaleTo::create(0.2f, 1.0f, 1.0f), DelayTime::create(0.2f), ScaleTo::create(0.1f, 1.25f, 0.75f), ScaleTo::create(0.1, 1.0f, 1.0f), NULL),Sequence::create(DelayTime::create(0.3f), MoveTo::create(0.2f, Vec2(chick_pos.x, chick_pos.y+120)), DelayTime::create(0.3f), MoveTo::create(0.3f, chick_pos), NULL), Sequence::create(DelayTime::create(0.3f), RotateTo::create(0.1, 6.0f),RotateTo::create(0.1, -6.0f), RotateTo::create(0.1, 6.0f),RotateTo::create(0.1, -6.0f), RotateTo::create(0.1, 6.0f), RotateTo::create(0.1f, 0.0f), NULL) , NULL));
        
    }else{
        //wrong
        chicken_gaf->play(false);
        chicken_gaf->runAction(Sequence::create(RotateTo::create(0.1, 6.0f),RotateTo::create(0.2, -6.0f), RotateTo::create(0.2, 6.0f),RotateTo::create(0.2, -6.0f), RotateTo::create(0.2, 6.0f), RotateTo::create(0.1f, 0.0f), NULL));
    }
}

