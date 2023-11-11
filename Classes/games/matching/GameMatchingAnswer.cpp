//
//  GameMatchingAnswer.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/21/17.
//
//

#include "GameMatchingAnswer.h"
#include "GameMatchingConstants.h"
#include "StoryConfigManager.h"
#include "HSAudioEngine.h"
#include "GameMatching.h"
INITIALIZE_READER(MJGameMatchingAnswer);

USING_NS_CC;

MJGameMatchingAnswer* MJGameMatchingAnswer::createAnswer(mj::GameObject object, bool is_undersea) {
    auto csb_name = is_undersea ? "csb/game/matching/Answer_2.csb" : "csb/game/matching/Answer.csb";

    auto p = static_cast<MJGameMatchingAnswer*>(cocos2d::CSLoader::createNode(csb_name));
    if (p) {
        p->_is_undersea = is_undersea;
        p->didLoadFromCSB(object);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

MJGameMatchingAnswer* MJGameMatchingAnswer::clone(MJGameMatchingAnswer* other) {
    auto p = MJGameMatchingAnswer::createAnswer(other->_data, other->_is_undersea);
    return p;
}

void MJGameMatchingAnswer::didLoadFromCSB(mj::GameObject object) {
    _delegate = nullptr;
    _data = object;
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);

    static_cast<ui::Text*>(ui::Helper::seekWidgetByName(this, "text"))->setString(_data.text);
    utils::findChild<ui::Text*>(this, "text")->setFontSize(30);
    if (CONFIG_MANAGER->isVietnameseStories()) {
        utils::findChild<ui::Text*>(this, "text")->setFontName("fonts/danhvan.ttf");
        utils::findChild<ui::Text*>(this, "text")->setPositionY(utils::findChild<ui::Text*>(this, "text")->getPositionY() - 5);
    }
}

ui::Widget::ccWidgetTouchCallback MJGameMatchingAnswer::onLocateTouchCallback(const std::string& callback_name) {
    if (callback_name == "onTouch") {
        return CC_CALLBACK_2(MJGameMatchingAnswer::onTouch, this);
    }
    return nullptr;
}

void MJGameMatchingAnswer::onTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType event_type) {
    if (_gameMatching && !_gameMatching->getSpeaking()) {
        return;
    }
    auto obj = static_cast<ui::Widget*>(sender);
    switch (event_type) {
    case ui::Widget::TouchEventType::BEGAN:
        if (_delegate) {
            _delegate->onAnswerBegan(this, obj->getTouchEndPosition());
        }
        cocos2d::experimental::AudioEngine::play2d(_is_undersea ? soundSeaAnswerDrag : soundLightAnswerDrag);
        cocos2d::experimental::AudioEngine::play2d(_data.audio_path);
        this->setLocalZOrder(100);
        this->getParent()->setLocalZOrder(100);
        _original_point = this->getPosition();
        break;

    case ui::Widget::TouchEventType::MOVED:
        onTouchMovedToPosition(obj->getTouchMovePosition());
        break;

    case ui::Widget::TouchEventType::ENDED:
        if (_delegate) {
            _delegate->onAnswerMoved(this, obj->getTouchEndPosition());
        }
        // moveBackToOrigin();
        break;

    case ui::Widget::TouchEventType::CANCELED:
        if (_delegate) {
            _delegate->onAnswerMoved(this, obj->getTouchEndPosition());
        }
        break;

    default:
        break;
    }
}

void MJGameMatchingAnswer::onTouchMovedToPosition(cocos2d::Point position) {
    CCLOG("Touch Moved: %f %f", position.x, position.y);
    auto node_position = this->convertToNodeSpace(position);
    this->getChildByName("background")->setPosition(node_position);

    if (_delegate) {
        _delegate->onAnswerMoving(this, position);
    }
}

void MJGameMatchingAnswer::moveToPosition(cocos2d::Point position) {
    auto background = static_cast<ui::ImageView*>(this->getChildByName("background"));

    auto start_point = background->getPosition();
    auto end_point = position;

    auto distance = start_point.distance(end_point);
    const float velocity = 1000;
    auto time = distance / velocity;

    auto easing = EaseElasticOut::create(MoveTo::create(time, end_point), 0.5);

    background->runAction(Sequence::create(easing, CallFunc::create([=] {
        this->setLocalZOrder(0);
        this->getParent()->setLocalZOrder(0);
        }), nullptr));
}

void MJGameMatchingAnswer::moveBackToOrigin() {
    moveToPosition(this->getContentSize() / 2);
}

std::string MJGameMatchingAnswer::getAnswer() {
    return _data.text;
}
