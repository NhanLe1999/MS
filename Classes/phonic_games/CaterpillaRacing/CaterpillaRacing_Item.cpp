//
//  CaterpillaRacing_Item.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 7/16/18.
//

#include "CaterpillaRacing_Item.h"

INITIALIZE_READER (CaterpillaRacing_Item);

#define CSB_NAME "csb/game/CaterpillaRacing/CaterpillaRacing_Item.csb"
#define img_path "sample_game/caterpillaracing/"
#define audio_p "sample_game/caterpillaracing/"

CaterpillaRacing_Item* CaterpillaRacing_Item::createItem(CaterpillaData::Game game, int showImage){
    auto p = static_cast<CaterpillaRacing_Item*>(cocos2d::CSLoader::getInstance()->createNode(CSB_NAME));
    if(p){
        p->didLoadFromCSB(game, showImage);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void CaterpillaRacing_Item::didLoadFromCSB(CaterpillaData::Game game, int showImage){
    if (CONFIG_MANAGER->isGameVMonkey()){
        font_name = PHONIC_MANAGER->getInstance()->getGameFontName();
    }
    auto item_layout = utils::findChild<Layout*>(this, "item_panel");
    item_layout->setTouchEnabled(true);
    item_layout->addClickEventListener(CC_CALLBACK_1(CaterpillaRacing_Item::onClickItem, this));
    auto size = item_layout->getContentSize();
    int index_p = (game.images.size() > 0)? random(0, (int) game.images.size()-1) : 0;
    
    if(game.images.size() <= 0){
        showImage = 0;
    }
    
    if(random(0, 1) && showImage == 1){
        std::string img =  game.images[index_p].first;
        ImageView* image = utils::findChild<ImageView*>(item_layout, "Image_2");
        image->loadTexture(img);
        image->setScale(0.8);
        image->setName("child");
        image->ignoreContentAdaptWithSize(false);
//        _audio = audio_p + game.images[index].second;
         _audio = game.images[index_p].second;
    }else{
        Text* text = Text::create();
		auto string = game.text;//random(0, 1) ? game.text : mj::helper::to_upper(game.text);
        text->setString(string);
        text->setFontName(font_name);
        text->setFontSize(font_sz);
        while (text->getContentSize().width > item_layout->getContentSize().width-150) {
            font_sz--;
            text->setFontSize(font_sz);
        }
        text->enableOutline(Color4B::WHITE, 2);
        text->setTextColor(text_color[random(0, 3)]);
        item_layout->addChild(text);
        text->setPosition(item_layout->getContentSize()/2);
        text->setName("child");
        item_layout->setScale(1);
//        _audio = audio_p + game.audio_path;
         _audio = game.audio_path;
    }
}

void CaterpillaRacing_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void CaterpillaRacing_Item::setDelegate(CaterpillaRacing_ItemDelegate* delegate){
    _delegate = delegate;
}

void CaterpillaRacing_Item::onClickItem(Ref* sender){
//    CCLOG("onClicked");
    if (_delegate) {
        _delegate->onClickedDelegate(this);
    }
}

void CaterpillaRacing_Item::bubbleEffect(int delta, int i){
    if( i%2 == 0){
         this->runAction(RepeatForever::create(Sequence::create(MoveTo::create(0.5f, Vec2(this->getPositionX() - delta , this->getPositionY())), MoveTo::create(0.5f, Vec2(this->getPositionX() + delta, this->getPositionY() -delta)), MoveTo::create(0.5f, Vec2(this->getPositionX(), this->getPositionY() + delta)), NULL)));
    }else{
         this->runAction(RepeatForever::create(Sequence::create( MoveTo::create(0.5f, Vec2(this->getPositionX() + delta, this->getPositionY() +delta)), MoveTo::create(0.5f, Vec2(this->getPositionX(), this->getPositionY() + delta)), MoveTo::create(0.5f, Vec2(this->getPositionX() - delta , this->getPositionY())), NULL)));
    }
  
}

std::string CaterpillaRacing_Item::getAudioPath(){
    return _audio;
}
