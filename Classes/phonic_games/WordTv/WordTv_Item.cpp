//
//  MJ_WordTv_Item.h
//

#include "WordTv_Item.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
using namespace cocos2d;
WordTv_Item * WordTv_Item::createItem(mj::GEAR_ITEM item_info) {
    WordTv_Item * obj = new WordTv_Item();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB(item_info);
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void WordTv_Item::didLoadFromCSB(mj::GEAR_ITEM item_info){
    this->_item_info = item_info;
    
    bgGear= cocos2d::ui::ImageView::create("games/WordTv/photobg1.png");
    this->addChild(bgGear);
    bgGear->setPosition(bgGear->getContentSize()/2);
    this->setContentSize(bgGear->getContentSize());
    this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    auto bgGear2= cocos2d::ui::ImageView::create("games/WordTv/photobg2.png");
    bgGear->addChild(bgGear2);
    bgGear2->setPosition(bgGear->getContentSize()/2);
    bgGear2->setScale(0.96);
    
    item = cocos2d::ui::ImageView::create(_item_info.image_path);
    bgGear->addChild(item);
    item->setPosition(bgGear->getContentSize()/2);
    item->ignoreContentAdaptWithSize(false);
    item->setContentSize(Size(250, 250));
    //if(item->getContentSize().width >=item->getContentSize().height && item->getContentSize().height>150){
    //    item->setScale(220/item->getContentSize().width);
    //}
    //else if(item->getContentSize().height >=item->getContentSize().width && item->getContentSize().width>150){
    //     item->setScale(220/item->getContentSize().height);
    //}

	item->setScale(220.0f / item->getContentSize().height);
    this->setCascadeOpacityEnabled(true);
    bgGear->setCascadeOpacityEnabled(true);
    
    filter = GAFWrapper::createGAF("gaf/WordTv/wtv_photofilter/wtv_photofilter.gaf");
    this->addChild(filter);
    filter->setPosition(bgGear->getContentSize()/2);
    filter->setVisible(false);
    
    this->setTouchEnabled(true);
}

void WordTv_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();

}

void WordTv_Item::onClicked(){
    auto seq = Sequence::create(ScaleTo::create(0.1f, 1.1f), CallFunc::create([=](){
        auto audio_id = cocos2d::experimental::AudioEngine::play2d(_item_info.audio_path);
        cocos2d::experimental::AudioEngine::setFinishCallback(audio_id, [=](int au_id, std::string path){
           /* this->runAction(Sequence::create(ScaleTo::create(0.2f, 1.0f), CallFunc::create([=](){
                if(clickedFinish){
                    clickedFinish();
                }
            }), nullptr));*/
        });
    }), nullptr);
    this->runAction(seq);
}

void WordTv_Item::setCallbackW(const std::function<void()> afunc){
    clickedFinishW = afunc;
}

void WordTv_Item::setCallbackC(const std::function<void()> afunc){
    clickedFinishC = afunc;
}

void WordTv_Item::setWrong(){
    filter->setVisible(true);
    filter->playSequence("filter_red");
    AUDIO_ENGINE->play("sounds/WordTv/wtv_wrong.mp3");
    this->runAction(Sequence::create(RotateTo::create(0.1f, -5)
                                     , RotateTo::create(0.2f, 5)
                                     , RotateTo::create(0.1f, 0)
                                     ,CallFunc::create([=](){
        filter->setVisible(false);
        if (clickedFinishW)
            clickedFinishW();
    }), NULL));
}

void WordTv_Item::setCorrect(){
    bgGear->runAction(FadeOut::create(0.5f));
    bgGear->setCascadeOpacityEnabled(false);
    AUDIO_ENGINE->play("sounds/WordTv/wtv_correct_50%.mp3");
    this->runAction(MoveBy::create(0.1f, Vec2(0,15)));
    filter->setVisible(true);
    filter->playSequence("filter_green", false,[=](gaf::GAFObject *obj, const std::string seq_name){
        if (clickedFinishC)
            clickedFinishC();
    });
    
}


