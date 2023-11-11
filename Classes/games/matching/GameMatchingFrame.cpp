//
//  GameMatchingFrame.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#include "GameMatchingFrame.h"
#include "GameMatchingAnswer.h"
#include "GAFWrapper.h"
#include "GameMatchingConstants.h"
#include "GameInstructionOverlay.h"

INITIALIZE_READER(MJGameMatchingFrame);

USING_NS_CC;

auto fish_cc = 1;

MJGameMatchingFrame* MJGameMatchingFrame::createFrame(mj::GameObject object, bool is_undersea) {
    auto csb_name = is_undersea ? "csb/game/matching/Frame_2.csb" : "csb/game/matching/Frame.csb";

    auto p = static_cast<MJGameMatchingFrame*>(cocos2d::CSLoader::createNode(csb_name));
    if (p) {
        p->_is_undersea = is_undersea;
        p->didLoadFromCSB(object);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameMatchingFrame::didLoadFromCSB(mj::GameObject object) {
    _data = object;
    _is_answered = false;
    static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "image"))->loadTexture(_data.image_path);

    if (_is_undersea) {
        fish_cc = (fish_cc - 1) % 6 + 1;

        static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "fish"))->loadTexture(StringUtils::format("games/matching_2/fish_%d.png", fish_cc));
        static_cast<ui::ImageView*>(ui::Helper::seekWidgetByName(this, "fin"))->loadTexture(StringUtils::format("games/matching_2/fin_%d.png", fish_cc));
        fish_cc++;
    }
}

void MJGameMatchingFrame::setCallbackName(std::function<void()> _callback) {
    
    if (auto panel01 = utils::findChild<ui::Layout*>(this, "Panel_1")) {
        panel01->addClickEventListener([=](cocos2d::Ref*) {
            if (_callback) {
                _callback();
            }
        });
    }
    else {
        this->addClickEventListener([=](Ref*) {
            if (_callback) {
                _callback();
            }
            });
    }
}


std::string MJGameMatchingFrame::getObjectName() {
    return _data.text;
}



void MJGameMatchingFrame::addRightAnswer(MJGameMatchingAnswer* answer, cocos2d::Point position) {

    //    auto text_audio_id = experimental::AudioEngine::play2d(_data.audio_path);
    this->scheduleOnce([=](float) {
        auto duration = 0;//experimental::AudioEngine::getDuration(text_audio_id);
        this->scheduleOnce([=](float dt) {
            experimental::AudioEngine::play2d(_is_undersea ? soundSeaRight : soundLightRight);
            }, duration, "delay_fish_effect");
        //        MJGameInstructionOverlay::playInstructionAfterDelay(duration, mj::GameType::LIGHT_THEM_UP, _is_undersea ? soundSeaRight :soundLightRight);
        }, 0.1, "delay_duration");

    //    experimental::AudioEngine::setFinishCallback(text_audio_id, [=](int, std::string) {
    auto csb_name = _is_undersea ? "csb/game/matching/Frame_2.csb" : "csb/game/matching/Frame.csb";
    auto action_timeline = CSLoader::createTimeline(csb_name);
    action_timeline->play("right_anim", false);
    this->runAction(action_timeline);

    if (!_is_undersea) {
        this->getChildByName("light_off")->setVisible(false);
        this->getChildByName("light_on")->setVisible(true);
        this->getChildByName("light_none")->setVisible(false);

        auto light = GAFWrapper::createGAF("gaf/light/Nhap_nhay.gaf", this->getChildByName("light_on"));
        light->setLocalZOrder(-1);
        light->play(true);
    }
    else {
        auto eye_open = GAFWrapper::createGAF("gaf/fish_eye_open/eye_open.gaf", ui::Helper::seekWidgetByName(this, "eye_anim"));
        //        light->setLocalZOrder(-1);
        eye_open->play(true);
    }
    //    });

    _is_answered = true;
    position = this->convertToNodeSpace(position);

    auto answer_in_frame = MJGameMatchingAnswer::clone(answer);
    answer_in_frame->setAnchorPoint(Point::ANCHOR_MIDDLE);
    answer_in_frame->setPosition(position);
    answer_in_frame->getChildByName<ui::Widget*>("background")->setTouchEnabled(false);

    this->addChild(answer_in_frame);

    auto start_point = answer_in_frame->getPosition();
    auto end_point = Point(125, 48);

    auto distance = start_point.distance(end_point);
    const float velocity = 1000;
    auto time = distance / velocity;

    answer_in_frame->runAction(Sequence::create(EaseExponentialOut::create(MoveTo::create(time, end_point)), CallFunc::create([=] {
        this->setLocalZOrder(0);
        this->getParent()->setLocalZOrder(0);
        answer_in_frame->setScale(0.9);
        }), EaseElasticOut::create(ScaleTo::create(0.5, 1), 0.3), nullptr));

}

void MJGameMatchingFrame::showWrongAnswer() {

    experimental::AudioEngine::play2d(_is_undersea ? soundSeaWrong : soundLightWrong);

    auto csb_name = _is_undersea ? "csb/game/matching/Frame_2.csb" : "csb/game/matching/Frame.csb";

    auto action_timeline = CSLoader::createTimeline(csb_name);
    action_timeline->play("wrong_anim", false);

    if (_is_undersea) {
        action_timeline->setLastFrameCallFunc([=] {
            auto eye_confuse = static_cast<GAFWrapper*>(ui::Helper::seekWidgetByName(this, "eye_confuse"));
            eye_confuse->removeFromParent();
            });
    }
    this->runAction(action_timeline);

    if (!_is_undersea) {
        this->getChildByName("light_off")->setVisible(true);
        this->getChildByName("light_on")->setVisible(false);
        this->getChildByName("light_none")->setVisible(false);
    }
    else {
        auto eye_confuse = GAFWrapper::createGAF("gaf/fish_eye_confuse/eye_confuse.gaf", ui::Helper::seekWidgetByName(this, "eye_anim"));
        eye_confuse->setName("eye_confuse");
        //        light->setLocalZOrder(-1);
        eye_confuse->play(true);
    }
}

void MJGameMatchingFrame::shake() {
    if (_is_undersea) {
        auto action_timeline = CSLoader::createTimeline("csb/game/matching/Frame_2.csb");
        action_timeline->play("shake", false);
        this->runAction(action_timeline);
    }
}
