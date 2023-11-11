//
//  CoverStory_Item.cpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#include "CoverStory_Item.h"

INITIALIZE_READER(CoverStory_Item);

#define CSB_NAME "csb/game/CoverStory/CoverStory_Item.csb"

std::once_flag CoverStory_Item_reader;
CoverStory_Item* CoverStory_Item::createItem(std::string word){
    
    std::call_once(CoverStory_Item_reader, []{
        REGISTER_CSB_READER(CoverStory_Item);
    });
    
    auto p = static_cast<CoverStory_Item*>(cocos2d::CSLoader::getInstance()->createNode(CSB_NAME));
    if(p){
        p->didLoadFromCSB(word);
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

void CoverStory_Item::didLoadFromCSB(std::string text){
    auto item_layout = utils::findChild<Layout*>(this, "item_layout");
    item_layout->setTouchEnabled(true);
    item_layout->addClickEventListener(CC_CALLBACK_1(CoverStory_Item::onClickedAnswer, this));
  
    auto answer_bg = utils::findChild<ImageView*>(item_layout, "Image_3");
    auto effect_img = utils::findChild<ImageView*>(item_layout, "effect");
    
    answer_bg->setScaleX(1.3);
    effect_img->setScaleX(1.3);
    
    Text* ans_text = Text::create(text, font_name, font_sz);
    ans_text->setTextColor(Color4B::BLACK);
    item_layout->addChild(ans_text, 10);
    ans_text->setPosition(item_layout->getContentSize()/2);
    ans_text->setName("text");
    ans_text->setContentSize(item_layout->getContentSize()*0.95);
    ans_text->ignoreContentAdaptWithSize(false);
    ans_text->setTextVerticalAlignment(TextVAlignment::CENTER);
    ans_text->setTextHorizontalAlignment(TextHAlignment::CENTER);

//    CCLOG("sz: %f", ans_text->getContentSize().width);
//    if(ans_text->getContentSize().width < 100){
//        answer_bg->setScaleX(0.8);
//        effect_img->setScaleX(0.8);
//    }else if(ans_text->getContentSize().width > 250){
//        answer_bg->setScaleX(1.5);
//        effect_img->setScaleX(1.5);
//    }
    
}

void CoverStory_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void CoverStory_Item::setDelegate(CoverStory_ItemDelegate* delegate){
    _delegate = delegate;
}


void CoverStory_Item::playEffect(int is_right){
    auto effect_img = utils::findChild<ImageView*>(this, "effect");
    if(is_right == 0){
        AUDIO_ENGINE->playEffect("sounds/coverStory/cs_correct.mp3");
        effect_img->loadTexture("games/coverStory/cs_ans_correct.png");
        effect_img->runAction(FadeIn::create(0.2));
        runAction(Sequence::create(ScaleTo::create(0.1, 1.2), EaseBounceOut::create(ScaleTo::create(0.2, 1)), NULL));
    }else{
        AUDIO_ENGINE->playEffect("sounds/coverStory/cs_wrong.mp3");
        effect_img->loadTexture("games/coverStory/cs_ans_wrong.png");
        effect_img->runAction(Sequence::create(FadeIn::create(0.2), FadeOut::create(0.2) ,NULL));
        runAction(Sequence::create(RotateTo::create(0.1, 4, 4), RotateTo::create(0.2, -4, -4), RotateTo::create(0.1, 0, 0), NULL));
    }
}

void CoverStory_Item::onClickedAnswer(cocos2d::Ref *sender){
    //    CCLOG("onClicked");
    if (_delegate) {
        _delegate->onClickedDelegate(sender);
        
    }
}
