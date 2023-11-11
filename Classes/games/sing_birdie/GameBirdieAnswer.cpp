//
//  GameBirdieAnswer.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/5/17.
//

#include "GameBirdieAnswer.h"

INITIALIZE_READER(MJGameBirdieAnswer);

USING_NS_CC;

MJGameBirdieAnswer * MJGameBirdieAnswer::createAnswer(mj::GameObject data) {
    auto answer = static_cast<MJGameBirdieAnswer *>(CSLoader::createNode("csb/game/birdie/answer.csb"));
    if (answer) {
        answer->didLoadFromCSB(data);
        return answer;
    }

    CC_SAFE_DELETE(answer);
    return nullptr;
}

void MJGameBirdieAnswer::didLoadFromCSB(mj::GameObject data) {
    _data = data;
    auto text = static_cast<ui::Text *>(ui::Helper::seekWidgetByName(this, "text"));
    text->setString(data.text);
    text->setFontName("monkeystories_ultra.ttf");
    auto event_listener_one_set_done = EventListenerCustom::create(kEventBirdieOneSetDone, [=](EventCustom *) {
        this->setTouchEnabled(false);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener_one_set_done, this);
}

void MJGameBirdieAnswer::moveBackToOrigin() {
    this->runAction(Sequence::create(EaseElasticOut::create(MoveTo::create(0.5, _origin), 0.7), CallFunc::create([=] {
        this->setTouchEnabled(true);
    }), nullptr));
    this->setLocalZOrder(0);
    this->getParent()->setLocalZOrder(0);
}

void MJGameBirdieAnswer::onExit() {
    ui::Layout::onExit();
    this->getEventDispatcher()->removeEventListenersForTarget(this);
}
