
#include "GearsForCar_Gear.h"
#include "PhonicInfoModel.h"
#include "HSAudioEngine.h"

GearsForCar_Gear * GearsForCar_Gear::createItem() {
    GearsForCar_Gear * obj = new GearsForCar_Gear();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void GearsForCar_Gear::didLoadFromCSB() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    gear = ImageView::create("games/GearsForCar/slot.png");
    this->addChild(gear);
    this->setContentSize(gear->getContentSize());
    flower = GAFWrapper::createGAF("gaf/GearsForCar/effec/effec.gaf");
    this->addChild(flower,10);
    flower->setVisible(false);
}

void GearsForCar_Gear::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
}

void GearsForCar_Gear::setSlot(bool isEmpty){
    this->isEmpty = isEmpty;
    if (isEmpty){
        gear->loadTexture("games/GearsForCar/empty_slot.png");
    }
    else{
        gear->loadTexture("games/GearsForCar/slot.png");
    }
}

void GearsForCar_Gear::setCorrect(){
    gear->loadTexture("games/GearsForCar/slot.png");
    AUDIO_ENGINE->play("sounds/GearsForCar/drop_correct.mp3");
    flower->setVisible(true);
    flower->play(false);
    isEmpty=false;
    auto seq = Sequence::create(
                ScaleTo::create(0.2f,0.6f),
                ScaleTo::create(0.2f, 1.1f),
                ScaleTo::create(0.1f, 1.0f),
                CallFunc::create([=](){
        if (callBackFunc){
            callBackFunc();
        }
    }), NULL);
    gear->runAction(seq);
}

void GearsForCar_Gear::setCallBack(const std::function<void()> afunc){
    callBackFunc = afunc;
}
