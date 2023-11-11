
#include "WordTv_Led.h"
#include "PhonicInfoModel.h"
#include "PhonicConfig.h"

WordTv_Led * WordTv_Led::createItem() {
    WordTv_Led * obj = new WordTv_Led();
    if(obj->init())
    {
        obj->autorelease();
        obj->didLoadFromCSB();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return nullptr;
}

void WordTv_Led::didLoadFromCSB() {
    screen_size = cocos2d::Director::getInstance()->getWinSize();
    
    for (int i=0; i<3; i++){
        auto led = GAFWrapper::createGAF("gaf/WordTv/wtv_led/wtv_led.gaf");
        this->addChild(led);
        leds.push_back(led);
        
        led->setPositionX(i*led->getContentSize().width*0.3f);
    }
    //tv->setVisible(false);
    //this->setContentSize(tv->getContentSize());
    leds.at(0)->setVisible(false);
    this->playTv(WordTv_Led::LED_STATE::IDLE);
}

void WordTv_Led::onEnter(){
    cocos2d::ui::Layout::onEnter();
    
}

std::string WordTv_Led::getColorChar(int index){
    switch (index) {
        case 0:
            return "Y";
        case 1:
            return "R";
        case 2:
            return "G";
    }
    return "R";
}

void WordTv_Led::playTv(LED_STATE state){
    _state = state;
//    for (int i=0; i< 3; i++){
//        this->leds.at(i)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(i).c_str()),true);
//    }
    if (state==LED_STATE::BLINK) {
//        this->leds.at(0)->playSequence(StringUtils::format("led%s_action",this->getColorChar(0).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//            this->leds.at(0)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(0).c_str()),true);
//            this->leds.at(1)->playSequence(StringUtils::format("led%s_action",this->getColorChar(1).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//                this->leds.at(1)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(1).c_str()),true);
//                this->leds.at(2)->playSequence(StringUtils::format("led%s_action",this->getColorChar(2).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//                    this->leds.at(2)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(2).c_str()),true);
//                    this->leds.at(0)->playSequence(StringUtils::format("led%s_action",this->getColorChar(0).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//                        this->leds.at(0)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(0).c_str()),true);
//                        this->leds.at(1)->playSequence(StringUtils::format("led%s_action",this->getColorChar(1).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//                            this->leds.at(1)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(1).c_str()),true);
//                            this->leds.at(2)->playSequence(StringUtils::format("led%s_action",this->getColorChar(2).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
//                                this->leds.at(2)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(2).c_str()),true);
//                                this->playTv(WordTv_Led::LED_STATE::IDLE);
//                            });
//                        });
//                    });
//                });
//            });
//        });
        for (int i=0; i< 3; i++){
            this->runAction(Sequence::create(DelayTime::create(i*0.6f)
                                             ,CallFunc::create([=](){
                this->leds.at(1)->playSequence(StringUtils::format("led%s_action",this->getColorChar(1).c_str()));
                this->leds.at(2)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(2).c_str()),true);
            })
                                             ,DelayTime::create(0.28f)
                                             ,CallFunc::create([=](){
                this->leds.at(2)->playSequence(StringUtils::format("led%s_action",this->getColorChar(2).c_str()));
                this->leds.at(1)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(1).c_str()),true);
            })
                                             ,DelayTime::create(0.28f)
                                             ,CallFunc::create([=](){
                this->leds.at(2)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(2).c_str()),true);
                this->leds.at(1)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(1).c_str()),true);
            })
                                             , NULL));
        }
    }
    else if (state == LED_STATE::WRONG){
        this->leds.at(0)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(0).c_str()),true);
        this->leds.at(1)->playSequence(StringUtils::format("led%s_action",this->getColorChar(1).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
            this->playTv(WordTv_Led::LED_STATE::IDLE);
        });
    }
    else if (state ==LED_STATE::ACTION){
        this->leds.at(0)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(0).c_str()),true);
        this->leds.at(2)->playSequence(StringUtils::format("led%s_action",this->getColorChar(2).c_str()),false,[=](gaf::GAFObject *obj, const std::string seq_name){
            this->playTv(WordTv_Led::LED_STATE::IDLE);
        });
    }
    else if (state == LED_STATE::IDLE){
        this->leds.at(0)->playSequence(StringUtils::format("led%s_loop",this->getColorChar(0).c_str()),true);
        this->leds.at(1)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(1).c_str()),true);
        this->leds.at(2)->playSequence(StringUtils::format("led%s_idle",this->getColorChar(2).c_str()),true);
    }
    
}

