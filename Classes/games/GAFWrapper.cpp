//
//  GAFWrapper.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/22/17.
//
//

#include "GAFWrapper.h"

USING_NS_CC;

GAFWrapper * GAFWrapper::createGAF(const std::string &gaf_name) {
    auto wrapper = new GAFWrapper();
    if (wrapper && wrapper->init(gaf_name)) {
        wrapper->autorelease();
    } else {
        CC_SAFE_DELETE(wrapper);
        wrapper = nullptr;
    }

    return wrapper;
}

GAFWrapper * GAFWrapper::createGAF(const std::string &gaf_name, cocos2d::Node * placeholder) {
    auto wrapper = new GAFWrapper();
    if (wrapper && wrapper->init(gaf_name, placeholder)) {
        wrapper->autorelease();
    } else {
        CC_SAFE_DELETE(wrapper);
        wrapper = nullptr;
    }

    return wrapper;
}

GAFWrapper * GAFWrapper::createGAF(gaf::GAFAsset *asset){
    auto wrapper = new GAFWrapper();
    if (wrapper && wrapper->init(asset)) {
        wrapper->autorelease();
    } else {
        CC_SAFE_DELETE(wrapper);
        wrapper = nullptr;
    }
    
    return wrapper;
}

bool GAFWrapper::init(const std::string & gaf_name) {
    if (!ui::Layout::init()) {
        return false;
    }

    auto asset = gaf::GAFAsset::create(gaf_name);
    _object = asset->createObject();
    _object->setLocator(true);
    _object->setAnchorPoint(Point::ANCHOR_MIDDLE);

    asset->setDesiredAtlasScale(cocos2d::Director::getInstance()->getContentScaleFactor());
    this->addChild(_object);

    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setContentSize(_object->getContentSize());
    _object->setPosition(this->getContentSize() / 2);
    _object->gotoAndStop(0);
    this->setCascadeOpacityEnabled(true);
    return true;
}

bool GAFWrapper::init(const std::string & gaf_name, cocos2d::Node * placeholder) {
    if (!ui::Layout::init()) {
        return false;
    }

    auto asset = gaf::GAFAsset::create(gaf_name);
    _object = asset->createObject();
    _object->setLocator(true);
    _object->setAnchorPoint(Point::ANCHOR_MIDDLE);

    asset->setDesiredAtlasScale(cocos2d::Director::getInstance()->getContentScaleFactor());
    this->addChild(_object);

    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setContentSize(_object->getContentSize());
    _object->setPosition(this->getContentSize() / 2);
    _object->gotoAndStop(0);

    this->setPosition(placeholder->getPosition());
    this->setAnchorPoint(placeholder->getAnchorPoint());
    placeholder->getParent()->addChild(this);
    placeholder->setVisible(false);

    return true;
}

bool GAFWrapper::init(gaf::GAFAsset *asset) {
    if (!ui::Layout::init()) {
        return false;
    }
    _object = asset->createObject();
    _object->setLocator(true);
    _object->setAnchorPoint(Point::ANCHOR_MIDDLE);
    
    asset->setDesiredAtlasScale(cocos2d::Director::getInstance()->getContentScaleFactor());
    this->addChild(_object);
    
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setContentSize(_object->getContentSize());
    _object->setPosition(this->getContentSize() / 2);
    _object->gotoAndStop(0);
    this->setCascadeOpacityEnabled(true);
    return true;
}

void GAFWrapper::setFps(int fps) {
    _object->setFps(fps);
}

void GAFWrapper::play(bool looped, std::function<void(gaf::GAFObject* obj)> callback) {
    _object->pauseAnimation();
    _object->stop();
    reset();
    _object->setLooped(looped);
    this->unschedule("gaf_play_callback");
//    _object->setAnimationFinishedPlayDelegate(callback);
    auto anim_length = _object->getTotalFrameCount();
    auto fps = _object->getFps();
    _object->start();
    this->scheduleOnce([=](float dt) {
        if (callback) {
            callback(_object);
        }
    }, anim_length * 1.f / fps, "gaf_play_callback");
}

void GAFWrapper::playSequence(std::string sequence, bool looped, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback) {
    _object->pauseAnimation();
    _object->clearSequence();
    _object->stop();
    this->unschedule(schedule_name);
    reset();
    auto anim_length = _object->getSequences().at(sequence).length();
    auto fps = _object->getFps();
    _object->start();
    _object->playSequence(sequence, looped, true);
    schedule_name = std::string("gaf_sequence_callback") + sequence + StringUtils::toString(random(0, 10000));
    this->scheduleOnce([=](float dt) {
        if (callback) {
            callback(_object, sequence);
        }
    }, anim_length * 1.f / fps, schedule_name);
}

void GAFWrapper::playSequenceWithPause(std::string sequence, bool looped, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback){
    _object->pauseAnimation();
    _object->clearSequence();
    _object->stop();
    this->unschedule("gaf_sequence_callback");
    reset();
    _object->start();
    _object->playSequence(sequence, looped, true);
    _object->setSequenceDelegate(callback);
}

void GAFWrapper::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    if (_object) _object->setPosition(size / 2);
}

void GAFWrapper::reset() {
    _object->setSequenceDelegate(nullptr);
    _object->setAnimationFinishedPlayDelegate(nullptr);
    _object->gotoAndStop(0);
}

void GAFWrapper::onExit() {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    reset();
#endif
    this->unschedule("gaf_play_callback");
    this->unschedule("gaf_sequence_callback");
    ui::Layout::onExit();
}

void GAFWrapper::forceLoop(bool is_loop, std::function<void(gaf::GAFObject * obj)> callback) {
    _object->setLooped(is_loop);
    _object->setAnimationFinishedPlayDelegate(callback);
}

void GAFWrapper::pauseAnimation(){
    _object->pauseAnimation();
}

void GAFWrapper::resumeAnimation(){
    _object->resumeAnimation();
}

void GAFWrapper::goToEndFrame(){
    _object->setFrame(_object->getTotalFrameCount()-1);
}

void GAFWrapper::enableDebug() {
    auto sequences = _object->getSequences();
    auto debug_layout = ui::Layout::create();
    auto counter = 0;
    auto max_width = 0.f;
    for (auto s : sequences) {
        auto info = s.second;
        if (info.endFrameNo - info.startFrameNo <= 1) {
            continue;
        }
        
        auto button = ui::Layout::create();
        button->setContentSize(Size(30, 30));
        button->setPosition(Point(0, counter * 30));
        button->setBackGroundColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
        button->setBackGroundColorOpacity(0xaa);
        button->setBackGroundColorType(BackGroundColorType::SOLID);
        debug_layout->addChild(button);
        
        auto seq_name = ui::Text::create(info.name, "fonts/Montserrat-SemiBold.ttf", 20);
        button->setContentSize(Size(seq_name->getVirtualRendererSize().width + 20, 30));
        max_width = std::max(max_width, button->getContentSize().width);
        seq_name->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        seq_name->setPosition(Point(10, 15));
        button->addChild(seq_name);
        
        button->setName(info.name);
        button->setTouchEnabled(true);
        button->addClickEventListener([this](Ref * sender) {
            auto name = reinterpret_cast<Node *>(sender)->getName();
            this->playSequence(name);
        });
        
        counter++;
    }
    debug_layout->setContentSize(Size(max_width, counter * 30));
    debug_layout->setAnchorPoint(Point::ANCHOR_MIDDLE);
    debug_layout->setPosition(Point(this->getContentSize() / 2));
    this->addChild(debug_layout);
}
