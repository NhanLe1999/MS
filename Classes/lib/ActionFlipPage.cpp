//
//  ActionFlipPage.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 8/15/17.
//

#include "ActionFlipPage.h"

USING_NS_CC;

ActionFlipPage * ActionFlipPage::create(float duration, bool is_reverse) {
    ActionFlipPage * action = new (std::nothrow) ActionFlipPage();
    if (action && action->initWithDuration(duration, is_reverse)) {
        action->autorelease();
        return action;
    }

    delete action;
    return nullptr;
}

bool ActionFlipPage::initWithDuration(float duration, bool is_reverse) {
    if (ActionInterval::initWithDuration(duration)) {
        _is_reverse = is_reverse;
        return true;
    }
    return false;
}

ActionFlipPage * ActionFlipPage::clone() const {
    return ActionFlipPage::create(_duration, _is_reverse);
}

ActionFlipPage * ActionFlipPage::reverse() const {
    auto action = ActionFlipPage::create(_duration, !_is_reverse);
    return action;
}

void ActionFlipPage::startWithTarget(cocos2d::Node *target) {
    ActionInterval::startWithTarget(target);
    _original_transform = target->getNodeToParentTransform();
}

void ActionFlipPage::update(float time) {
    auto ratio = time;
    if (_is_reverse) {
        ratio = 1 - ratio;
    }
    auto angle = - M_PI / 3 * ratio;

    auto transform = _original_transform;
    transform.rotateY(angle);
    auto scale_x = (M_PI / 3 + angle) / (M_PI / 3);
    transform.scale(scale_x, 1, 1);

    _target->setNodeToParentTransform(transform);
}
