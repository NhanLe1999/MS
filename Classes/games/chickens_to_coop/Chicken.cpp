//
//  Chicken.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/3/17.
//

#include "Chicken.h"
#include "GAFWrapper.h"
#include "AudioEngine.h"
#include "HSAudioEngine.h"
#include "GameChickensConstants.h"

INITIALIZE_READER(MJChicken);

USING_NS_CC;

MJChicken * MJChicken::createChicken(mj::GameObject word) {
    auto chicken = static_cast<MJChicken *>(CSLoader::createNode("csb/game/chickens_to_coop/chicken.csb"));
    if (chicken) {
        chicken->didLoadFromCSB(word);
        return chicken;
    }

    CC_SAFE_DELETE(chicken);
    return nullptr;
}

void MJChicken::didLoadFromCSB(mj::GameObject word) {
    _word = word;

    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    text->setString(word.text);
    text->setLocalZOrder(2);
    if (CONFIG_MANAGER->isGameVMonkey()){
        text->setFontName(PHONIC_MANAGER->getInstance()->getGameFontName());
    }
    _chicken_gaf = GAFWrapper::createGAF(gafChicken, ui::Helper::seekWidgetByName(this, "image"));
    this->scheduleOnce([=](float dt) {
        this->_chicken_gaf->playSequence("run", true);
    }, random(0.f, 0.4f), "chicken_run_delay");
    this->setTouchEnabled(true);
    this->addTouchEventListener(CC_CALLBACK_2(MJChicken::onTouchChicken, this));
}

void MJChicken::flipChicken(bool flip) {
    _chicken_gaf->setFlippedX(flip);
}

void MJChicken::run() {
    this->_chicken_gaf->playSequence("run", true);

    auto speed = random(100.f, 200.f);
    auto flipx = false;
    Point destination;
    if (this->getPositionX() < this->getParent()->getContentSize().width / 2) {
        destination = Point(this->getParent()->getContentSize().width, this->getPositionY());
        flipx = false;
    } else {
        destination = Point(0, this->getPositionY());
        flipx = -true;
    }

    this->flipChicken(flipx);
    auto distance = destination.distance(this->getPosition());
    auto time = distance / speed;
    this->runAction(Sequence::create(MoveTo::create(time, destination), CallFunc::create([=]() {
        this->run();
    }), nullptr));
}

void MJChicken::onTouchChicken(Ref * sender, ui::Widget::TouchEventType event_type) {
    switch (event_type) {
        case ui::Widget::TouchEventType::BEGAN:
            if (_delegate) {
                _delegate->onChickenTouchedTo();
            }
            cocos2d::experimental::AudioEngine::play2d(soundChickenDrag, false, 0.3);
            _origin = this->getPosition();
            this->stopAllActions();
            this->setLocalZOrder(1);
            _chicken_gaf->playSequence("drag", false, [=](gaf::GAFObject * obj, std::string sequence_name) {
                _chicken_gaf->playSequence("dragidle", true);
                });
            if (_isPlayAudioChicken)
            {
                _isPlayAudioChicken = false;
                AUDIO_ENGINE->playSound(_word.audio_path, false, [=] {
                    _isPlayAudioChicken = true;
                    });
            }
            break;

        case ui::Widget::TouchEventType::MOVED:
            this->setPosition(this->getParent()->convertToNodeSpace(this->getTouchMovePosition()));
            break;

        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED: {
            if (_delegate) {
                _delegate->onChickenDraggedTo(this, this->getParent()->convertToWorldSpace(this->getPosition()));
            }
        }
            break;

        default:
            break;
    }
}

void MJChicken::drop(std::function<void()> callback) {
    auto destination = Point(this->getPositionX(), _origin.y);
    auto distance = destination.distance(this->getPosition());
    auto velocity = 700.f;
    auto time = distance / velocity;
    this->runAction(Sequence::create(MoveTo::create(time, destination), CallFunc::create([=]() {
        this->run();
        if(callback){
            callback();
        }
    }), nullptr));
}
