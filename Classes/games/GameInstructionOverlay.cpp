//
//  GameInstructionOverlay.cpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 7/12/17.
//

#include "GameInstructionOverlay.h"
#include "AudioEngine.h"
#include "MJDefault.h"
#include "cocos2d.h"

using namespace cocos2d;

USING_NS_CC;

void MJGameInstructionOverlay::playInstructionAfterDelay(float delay, mj::GameType type, const std::string & audio_path, const std::string & audio_path_extra, MJGameInstructionDelegate * delegate, std::function<void()> callback) {
    auto overlay = MJGameInstructionOverlay::createOverlay(type, audio_path, audio_path_extra, callback);
    overlay->setDelegate(delegate);
    overlay->setDelay(delay);
    overlay->setName("MJGameInstructionOverlay");
    Director::getInstance()->getRunningScene()->addChild(overlay, 1000);
}

void MJGameInstructionOverlay::playInstruction(mj::GameType type, const std::string & audio_path, const std::string &audio_path_extra, MJGameInstructionDelegate * delegate, std::function<void()> callback ) {
    MJGameInstructionOverlay::playInstructionAfterDelay(0, type, audio_path, audio_path_extra, delegate);
}

MJGameInstructionOverlay * MJGameInstructionOverlay::createOverlay(mj::GameType type, const std::string & audio_path, const std::string & audio_path_extra, std::function<void()> callback) {
    auto overlay = new MJGameInstructionOverlay();
    if (overlay && overlay->init(type, audio_path, audio_path_extra, callback)) {
        overlay->autorelease();
        return overlay;
    }

    CC_SAFE_DELETE(overlay);
    return nullptr;
}

bool MJGameInstructionOverlay::init(mj::GameType type, const std::string & audio_path, const std::string & audio_path_extra, std::function<void()> callback ) {
    if (!ui::Layout::init()) {
        return false;
    }

    _type = type;
    _delegate = nullptr;
    _delay_time = 0;
    _instruction_audio_path = audio_path;
    _instruction_audio_path_extra = audio_path_extra;
    _callback = callback;
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
    this->setPosition(Point::ZERO);
    this->setTouchEnabled(false);
    this->setSwallowTouches(false);
//    this->setPropagateTouchEvents(false);

    return true;
}

void MJGameInstructionOverlay::onEnter() {
    ui::Layout::onEnter();
    this->scheduleOnce([=](float) {
        _instruction_audio_id = cocos2d::experimental::AudioEngine::play2d(_instruction_audio_path);
        if (_instruction_audio_id == -1) {

            if (_callback) {
                _callback();
            }

            this->removeFromParent();
            return;
        }
        cocos2d::experimental::AudioEngine::setFinishCallback(_instruction_audio_id, [=](int, const std::string &) {
            if (_instruction_audio_path_extra.empty()) {
                if (_callback) {
                    _callback();
                }
                this->removeFromParent();
            }
            else {
                _instruction_audio_id = cocos2d::experimental::AudioEngine::play2d(_instruction_audio_path_extra);
                if (_instruction_audio_id == -1) {
                    if (_callback) {
                        _callback();
                    }
                    this->removeFromParent();
                    return;
                }
                cocos2d::experimental::AudioEngine::setFinishCallback(_instruction_audio_id, [=](int, const std::string &) {

                    if (_callback) {
                        _callback();
                    }

                    this->removeFromParent();
                });
            }
        });
    }, _delay_time, "play_instruction");
}

void MJGameInstructionOverlay::onDeleteCallback()
{
    CCLOG("cmmmmmmmmmmmmmmmmm");
    _callback = nullptr;
}

void MJGameInstructionOverlay::onExit() {
    MJDEFAULT->deleteValueForKey("key.stop.audio.intro");
    ui::Layout::onExit();
}
