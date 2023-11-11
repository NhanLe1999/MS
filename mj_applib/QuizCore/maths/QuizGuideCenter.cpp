//
//  QuizGuideCenter.cpp
//  MJStory
//
//  Created by HuyPhuong on 3/23/18.
//

#include "QuizGuideCenter.h"

#pragma mark - Contructor

static QuizGuideCenter * _instance = nullptr;
QuizGuideCenter * QuizGuideCenter::getGuideCenter() {
    
    if (!_instance) {
        _instance = new QuizGuideCenter();
        if (_instance && _instance->initGC()) {
        } else {
            CC_SAFE_DELETE(_instance);
        }
    }
    return _instance;
}

bool QuizGuideCenter::initGC() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    /*---- Layout chính: chứa phần nội dung quiz ----*/
    this->setContentSize(quiz_size);
    this->setLayoutType(Layout::Type::ABSOLUTE);
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_BOTTOM_LEFT);
    this->setPosition(cocos2d::Vec2::ZERO);
    this->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
    this->setBackGroundColor(cocos2d::Color3B::BLACK);
    this->setOpacity(0);
    this->setSwallowTouches(true);
    
    auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/hand_tap/hand_tap.gaf", m_theme->getThemePath().c_str()));
    _hand = gaf_asset->createObjectAndRun(false);
    _hand->retain();
    _hand->setAnchorPoint(cocos2d::Point(0.3, 0.225));
    _hand->setPosition(cocos2d::Vec2::ZERO);
    _hand->setScale(1.3);
    _hand->setOpacity(0);
    addChild(_hand);
    
    return true;
}

void QuizGuideCenter::startGuide(std::function<void(void)> onstart, std::function<void(void)> ondone) {
    cocos2d::Director::getInstance()->getRunningScene()->addChild(this, INT_MAX-1);
    
    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
    touch_listener->onTouchBegan = [=](cocos2d::Touch*, cocos2d::Event *)->bool {
        if (_isGuiding) {
            this->skippedGuide();
            return false;
        } else {
            countdown = inactive_time;
            unschedule(schedule_selector(QuizGuideCenter::countdownInactive));
            return true;
        }
    };
    touch_listener->onTouchEnded = [=](cocos2d::Touch*, cocos2d::Event *){
        countdown = inactive_time;
        this->schedule(schedule_selector(QuizGuideCenter::countdownInactive), timing_step);
    };
    touch_listener->onTouchCancelled = [=](cocos2d::Touch*, cocos2d::Event *){
        countdown = inactive_time;
        this->schedule(schedule_selector(QuizGuideCenter::countdownInactive), timing_step);
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);

    _onGuideStart = onstart;
    _onGuideDone = ondone;
    setGuideEnabled();
}

void QuizGuideCenter::stopGuide() {
    this->getEventDispatcher()->removeEventListenersForTarget(this);
    
    _onGuideStart = nullptr;
    _onGuideDone = nullptr;
    setGuideDisabled();
    _hand->stopAllActions();
    _hand->setOpacity(0);
    _hand->setPosition(cocos2d::Vec2::ZERO);
    
    this->removeFromParentAndCleanup(false);
}

bool QuizGuideCenter::isGuideEnable() {
    return _isGuide;
}

bool QuizGuideCenter::isGuiding() {
    return _isGuiding;
}

void QuizGuideCenter::setQuizGuide(std::string quiz, int activity) {
    
}

void QuizGuideCenter::setInactiveTime(float time) {
    inactive_time = time;
}

void QuizGuideCenter::setIncorrectTime(int time) {
    incorrect_time = time;
}

void QuizGuideCenter::setGuideEnabled() {
    _isGuide = true;
    countdown = inactive_time;
    incorrect_count = 0;
    this->schedule(schedule_selector(QuizGuideCenter::countdownInactive), timing_step);
}

void QuizGuideCenter::setGuideDisabled() {
    _isGuide = false;
    unschedule(schedule_selector(QuizGuideCenter::countdownInactive));
}

void QuizGuideCenter::countdownInactive(float) {
    if (countdown > 0) {
        countdown -= timing_step;
    } else {
        setGuideDisabled();
        onGuideStarted();
    }
}

void QuizGuideCenter::resetGuideCount () {
    guide_count = 0;
}

int QuizGuideCenter::guideCount () {
    return guide_count;
}

void QuizGuideCenter::resetIncorrectCount() {
    incorrect_count = 0;
}

void QuizGuideCenter::increamentIncorrect() {
    incorrect_count++;
    if (incorrect_count == incorrect_time) {
        scheduleOnce([=](float){
            setGuideDisabled();
            onGuideStarted();
        }, 0.35, "guide in incorrect");
    }
}

#pragma mark -
void QuizGuideCenter::onGuideStarted() {
    guide_count++;
    if (_onGuideStart) {
        _onGuideStart();
    }
}

void QuizGuideCenter::onGuideDone() {
    _isGuiding = false;
    setGuideEnabled();
    if (_onGuideDone) {
        _onGuideDone();
    }
}

void QuizGuideCenter::skippedGuide() {
    if (_isGuiding) {
        _hand->stopAllActions();
        _hand->setOpacity(0);
        _hand->setPosition(cocos2d::Vec2::ZERO);
        onGuideDone();
    }
}

void QuizGuideCenter::guideNone () {
    onGuideDone();
}

#pragma mark - Guide TOUCH
void QuizGuideCenter::guideByTouch(cocos2d::Vec2 point) {
    _isGuiding = true;
    cocos2d::Sequence* action = cocos2d::Sequence::create(cocos2d::Place::create(point), cocos2d::FadeIn::create(timing_fade), cocos2d::CallFunc::create([=](){
        _hand->playSequence("tap", false);
    }), cocos2d::DelayTime::create(timing_delay*2), cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);
    _hand->runAction(action);
}

#pragma mark - Guide DRAG-DROP
void QuizGuideCenter::guideByDragDrop(cocos2d::Vec2 point1, cocos2d::Vec2 point2) {
    _isGuiding = true;
    float time = point1.distance(point2)/swipe_velocity;
    cocos2d::Sequence* action = cocos2d::Sequence::create(cocos2d::Place::create(point1), cocos2d::FadeIn::create(timing_fade), cocos2d::CallFunc::create([=](){
        _hand->playSequence("tap", false);
    }), cocos2d::DelayTime::create(timing_delay), cocos2d::MoveTo::create(time, point2), cocos2d::DelayTime::create(timing_delay), cocos2d::CallFunc::create([=](){
        _hand->playSequence("swipe", false);
    }), cocos2d::DelayTime::create(timing_delay), cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);
    _hand->runAction(action);
}

#pragma mark - Guide SWIPE
void QuizGuideCenter::guideBySwipe(std::vector<cocos2d::Vec2> points) {
    _isGuiding = true;
    auto point = points.front();
    _lastPoint = point;
    points.erase(points.begin());
    cocos2d::Sequence* swipe = cocos2d::Sequence::create(cocos2d::Place::create(point), cocos2d::FadeIn::create(timing_fade), NULL);
    
    for (cocos2d::Vec2 point : points) {
        float time = point.distance(_lastPoint)/swipe_velocity;
        _lastPoint = point;
        auto swipe2 = swipe->clone();
        swipe = cocos2d::Sequence::create(swipe2, cocos2d::MoveTo::create(time, point), NULL);
    }
    auto swipe2 = swipe->clone();
    swipe = cocos2d::Sequence::create(swipe2, cocos2d::DelayTime::create(timing_delay), cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);

    _hand->playSequence("swipe", false);
    _hand->runAction(swipe);
}

void QuizGuideCenter::guideBySwipe(cocos2d::Sequence* swipe, cocos2d::Vec2 point) {
    _isGuiding = true;
    float time = point.distance(_lastPoint)/swipe_velocity;
    cocos2d::Sequence* action = cocos2d::Sequence::create(swipe, cocos2d::MoveTo::create(time, point), cocos2d::DelayTime::create(timing_delay), cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);

    _hand->playSequence("swipe", false);
    _hand->runAction(action);
}

#pragma mark - Guide COLOR
void QuizGuideCenter::guideByColor(std::vector<cocos2d::Vec2> points) {
    _isGuiding = true;
    auto point = points.front();
    _lastPoint = point;
    points.erase(points.begin());
    
    cocos2d::Sequence* swipe = cocos2d::Sequence::create(cocos2d::Place::create(point), cocos2d::FadeIn::create(timing_fade), cocos2d::CallFunc::create([=](){
        _hand->playSequence("tap", false);
    }), cocos2d::DelayTime::create(timing_delay), cocos2d::CallFunc::create([=](){
        _hand->playSequence("swipe", false);
    }), cocos2d::DelayTime::create(timing_delay), NULL);
    
    for (cocos2d::Vec2 point : points) {
        float time = point.distance(_lastPoint)/swipe_velocity;
        _lastPoint = point;
        auto swipe2 = swipe->clone();
        swipe = cocos2d::Sequence::create(swipe2, cocos2d::MoveTo::create(time, point), cocos2d::CallFunc::create([=](){
            _hand->playSequence("tap", false);
        }), cocos2d::DelayTime::create(timing_delay), cocos2d::CallFunc::create([=](){
            _hand->playSequence("swipe", false);
        }), cocos2d::DelayTime::create(timing_delay), NULL);
    }
    auto swipe2 = swipe->clone();
    swipe = cocos2d::Sequence::create(swipe2, cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);
    _hand->runAction(swipe);
}

void QuizGuideCenter::guideByColor(cocos2d::Sequence* swipe, cocos2d::Vec2 point) {
    _isGuiding = true;
    float time = point.distance(_lastPoint)/swipe_velocity;
    cocos2d::Sequence* action = cocos2d::Sequence::create(swipe, cocos2d::MoveTo::create(time, point), cocos2d::CallFunc::create([=](){
        _hand->playSequence("tap", false);
    }), cocos2d::DelayTime::create(timing_delay), cocos2d::FadeOut::create(timing_fade), cocos2d::CallFunc::create(CC_CALLBACK_0(QuizGuideCenter::onGuideDone, this)), NULL);
    _hand->runAction(action);
}

