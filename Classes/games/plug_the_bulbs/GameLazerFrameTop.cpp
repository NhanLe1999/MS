//
//  GameLazerFrame.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 6/23/17.
//

#include "GameLazerFrameTop.h"
#include "GameLazerFrameBottom.h"
#include "GAFWrapper.h"
#include "GameLazerConstants.h"
#include "GameInstructionOverlay.h"
#include "MJDefault.h"
#include "GameLazer.h"
#include "HSAudioEngine.h"

INITIALIZE_READER(MJGameLazerFrameTop);

USING_NS_CC;

MJGameLazerFrameTop * MJGameLazerFrameTop::createFrame(mj::GameObject object, bool is_image, bool isTextLength) {
    auto p = static_cast<MJGameLazerFrameTop *>(CSLoader::createNode("csb/game/lazer/start_point.csb"));
    if (p) {
        p->didLoadFromCSB(object, is_image, isTextLength);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameLazerFrameTop::didLoadFromCSB(mj::GameObject object, bool is_image, bool isTextLength) {
    _rope = DrawNode::create(10);
    this->addChild(_rope);
    _object = object;
    _object = object;

    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    auto image = static_cast<ui::ImageView *>(ui::Helper::seekWidgetByName(this, "image"));

    if (isTextLength)
    {
        text->setFontSize(text->getFontSize() - 10);
    }

    text->setString(object.text);
    image->loadTexture(object.image_path);

    text->setVisible(!is_image);
    image->setVisible(is_image);
    image->setContentSize(cocos2d::Size(190, 145));
}

std::string MJGameLazerFrameTop::getName() {
    return _object.text;
}

ui::Widget::ccWidgetTouchCallback MJGameLazerFrameTop::onLocateTouchCallback(const std::string &callback_name) {
    if (callback_name == "onTouchPlug") {
        return CC_CALLBACK_2(MJGameLazerFrameTop::onTouchPlug, this);
    }
    return nullptr;
}

void MJGameLazerFrameTop::onTouchPlug(cocos2d::Ref * sender, ui::Widget::TouchEventType event_type) {
    auto obj = static_cast<ui::Widget *>(sender);

    Director::getInstance()->getRunningScene()->removeChildByName("guide_overlay");
    switch (event_type) {
        case ui::Widget::TouchEventType::BEGAN:
            removeSuggetionGame();
            cocos2d::experimental::AudioEngine::play2d(soundBulbDrag);
            playAudioObject(_object.audio_path);
            this->setLocalZOrder(1);
            this->getParent()->setLocalZOrder(1);
            this->schedule(CC_SCHEDULE_SELECTOR(MJGameLazerFrameTop::updateRope));
            break;

        case ui::Widget::TouchEventType::MOVED:
            movePlug(obj->getTouchMovePosition());
            break;

        case ui::Widget::TouchEventType::ENDED:
            resetSuggetionGame();
            if (_delegate) {
                _delegate->onPlugMoved(this, obj->getTouchEndPosition());
            }
            break;
        case ui::Widget::TouchEventType::CANCELED:
            movePlugBack();
            break;
        default:
            break;
    }
}

void MJGameLazerFrameTop::updateRope(float dt) {
    auto plug = this->getChildByName("plug");
    _rope->clear();
    _rope->drawLine(plug->getPosition() + Point(0, 43), Point(115, 83), Color4F(0.949, 0.447, 0.000, 1));
}

void MJGameLazerFrameTop::movePlug(Point position) {
    auto plug = this->getChildByName<ui::ImageView *>("plug");
    plug->setPosition(this->convertToNodeSpace(position));

    if (_delegate) {
        _delegate->onPlugMoving(this, position);
    }
}

void MJGameLazerFrameTop::movePlugBack() {
    //original point: 92.5, 50
    auto plug = this->getChildByName<ui::ImageView *>("plug");
    cocos2d::experimental::AudioEngine::play2d(soundBulbWrong);
    plug->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(0.75, Point(115, 41)), 0.6), CallFunc::create([=] {
//        this->unschedule(CC_SCHEDULE_SELECTOR(MJGameLazerFrameTop::updateRope));
    }), nullptr));
}

void MJGameLazerFrameTop::turnBulbOn() {
    this->getChildByName("light_off")->setVisible(false);
    this->getChildByName("light_on")->setVisible(true);
}

void MJGameLazerFrameTop::connectToBottom(MJGameLazerFrameBottom * bottom) {
    auto plug = this->getChildByName<ui::ImageView *>("plug");
    plug->setTouchEnabled(false);

//    this->unschedule(CC_SCHEDULE_SELECTOR(MJGameLazerFrameTop::updateRope));

    auto destination = bottom->convertToWorldSpace(Point(115, 330));
    destination = this->convertToNodeSpace(destination);

    end1 = Node::create();
    end1->setPosition(Point(115, 83));
    this->addChild(end1);

    end2 = Node::create();
    end2->setPosition(plug->getPosition() + Point(0, 43));
    this->addChild(end2);

    control_point = Node::create();
    control_point->setPosition((end1->getPosition() + end2->getPosition()) / 2);
    this->addChild(control_point);

    end2->runAction(EaseCircleActionIn::create(MoveTo::create(0.5, destination + Point(0, 43))));

    this->schedule([=](float dt) {
        _rope->clear();
        _rope->drawQuadBezier(end1->getPosition(), control_point->getPosition(), end2->getPosition(), 10, Color4F::ORANGE);
    }, "trampoline");

    plug->runAction(ScaleTo::create(0.5, 1, 1.3));
    plug->runAction(Sequence::create(
        EaseCircleActionIn::create(MoveTo::create(0.5, destination)),
        CallFunc::create([=] {
            cocos2d::experimental::AudioEngine::play2d(soundBulbRight);
            control_point->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(1, (end1->getPosition() + destination + Point(0, 43)) / 2), 0.15), CallFunc::create([=] {
                this->unschedule("trampoline");

            }), nullptr));

            this->scheduleOnce([=](float) {
                //cocos2d::experimental::AudioEngine::play2d(_object.audio_path);
            }, 1.5f, "delay_text_audio");
//            MJGameInstructionOverlay::playInstructionAfterDelay(1.5, mj::GameType::PLUG_IN_BULBS_TI, _object.audio_path);

            plug->loadTexture("games/lazer/plug_out_none.png");
            this->turnBulbOn();
            bottom->turnBulbOn();
            auto electric = GAFWrapper::createGAF("gaf/electric/electric.gaf");
            electric->setPosition(destination);
            electric->play(false, [=](gaf::GAFObject * object) {
                electric->runAction(Sequence::create(DelayTime::create(0.01f), CallFunc::create([=]{
                    electric->removeFromParent();
                }), NULL));
            });
            this->addChild(electric);
        }),
        EaseElasticOut::create(ScaleTo::create(1, 1), 0.5),
        CallFunc::create([=] {
//            this->unschedule(CC_SCHEDULE_SELECTOR(MJGameLazerFrameTop::updateRope));
        }),
        nullptr)
    );
}

void MJGameLazerFrameTop::onBgFrame(cocos2d::Ref *sender){
    playAudioObject(_object.audio_path);
}

void MJGameLazerFrameTop::resetSuggetionGame()
{
    if (auto gameLazer = dynamic_cast<MJGameLazer*>(this->getParent()->getParent()->getParent()->getParent()))
    {
        gameLazer->showGuide(8.5);
    }
}

void MJGameLazerFrameTop::playAudioObject(std::string path)
{
    if (MJDEFAULT->getBoolForKey("key_play_audio_gameLazer", true))
    {
        removeSuggetionGame();
        MJDEFAULT->setBoolForKey("key_play_audio_gameLazer", false);
        AUDIO_ENGINE->play(path, false, 1.0f, [this]() {
            resetSuggetionGame();
            MJDEFAULT->setBoolForKey("key_play_audio_gameLazer", true);
            });
    }
}

void MJGameLazerFrameTop::removeSuggetionGame()
{
    if (auto gameLazer = dynamic_cast<MJGameLazer*>(this->getParent()->getParent()->getParent()->getParent()))
    {
        if (auto hand = utils::findChild(gameLazer, "hand_suggetion"))
        {
            hand->removeFromParent();
        }
        gameLazer->unschedule("show_guide_bulbs_1");
        gameLazer->stopSoundAudioSuggetion();
    }
}

#pragma mark - Callback Functions
cocos2d::ui::Widget::ccWidgetClickCallback MJGameLazerFrameTop::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MJGameLazerFrameTop, onBgFrame),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}
