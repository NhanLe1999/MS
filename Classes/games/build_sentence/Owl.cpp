//
//  MJOwl.cpp
//  Azu
//
//  Created by To Ngoc Linh on 6/28/17.
//
//

#include "Owl.h"
#include "GAFWrapper.h"
#include "AudioEngine.h"
#include "GameBuildSentenceConstants.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
INITIALIZE_READER(MJOwl);
USING_NS_CC;

MJOwl * MJOwl::createOwl(mj::GameObject word, int index, bool is_night) {
    auto owl = static_cast<MJOwl *>(CSLoader::createNode("csb/game/build_sentence/owl.csb"));
    if (owl) {
        owl->didLoadFromCSB(word, index, is_night);
        return owl;
    }

    CC_SAFE_DELETE(owl);
    return nullptr;
}

void MJOwl::didLoadFromCSB(mj::GameObject word, int index, bool is_night) {
    _word = word;
    _index = index;

    std::string gaf_name;

    if (is_night) {
        gaf_name = "gaf/owl_night/owl1.gaf";
        _text_color = Color4B::BLACK;
        _tint_text_color = Color4B::ORANGE;
    } else {
        gaf_name = "gaf/owl/owl1.gaf";
        _text_color = Color4B::WHITE;
        _tint_text_color = Color4B::YELLOW;
    }

    auto placeholder = this->getChildByName("gaf_holder");

    _owl = GAFWrapper::createGAF(gaf_name);//, placeholder);
    _owl->setPosition(placeholder->getPosition());
    _owl->setAnchorPoint(placeholder->getAnchorPoint());
    placeholder->getParent()->addChild(_owl);
    placeholder->setVisible(false);
    
    auto text = ui::Text::create(_word.text, (CONFIG_MANAGER->isGameVMonkey())? PHONIC_MANAGER->getGameFontName():"fonts/monkeystories_ultra.ttf", 36);
    text->setPositionPercent(Point(0.5, 0.3125));
    text->setPositionType(ui::Widget::PositionType::PERCENT);
    text->setColor(Color3B(_text_color.r, _text_color.g, _text_color.b));
    text->setName("text");
    _owl->addChild(text);
}

void MJOwl::onTouch(Ref * sender, ui::Widget::TouchEventType event_type) {
    switch (event_type) {
        case ui::Widget::TouchEventType::BEGAN:
            cocos2d::experimental::AudioEngine::play2d(soundOwlDrag);
            this->setLocalZOrder(1);
            shake();
            break;

        case ui::Widget::TouchEventType::MOVED:
            _owl->setPosition(this->convertToNodeSpace(_owl->getTouchMovePosition()));
            break;

        case ui::Widget::TouchEventType::ENDED:
        case ui::Widget::TouchEventType::CANCELED:
            if (_delegate) {
                _delegate->onOwlDroppedOnPosition(this, _owl->getTouchEndPosition());
            }
            //this->setLocalZOrder(0);
            //flyTo(_origin, false);
            break;

        default:
            break;
    }
}

void MJOwl::shake() {
    _owl->playSequence("drag", true);
}

void MJOwl::fly() {
    _owl->playSequence("fly", true);
}

void MJOwl::read(bool with_sound) {

    auto text = _owl->getChildByName("text");
    auto scale = ScaleBy::create(0.25, 1.5);
    auto reverse_scale = scale->reverse();
    text->runAction(Sequence::create(scale, reverse_scale, NULL));

    auto tint = TintTo::create(0.25, _tint_text_color.r, _tint_text_color.g, _tint_text_color.b);
    auto reverse_tint = TintTo::create(0.25, _text_color.r, _text_color.g, _text_color.b);
    text->runAction(Sequence::create(tint, reverse_tint, NULL));


//    this->scheduleOnce([=](float) {
//        experimental::AudioEngine::play2d(_word.audio_path, false, with_sound ? 1.f : 0.f);
//    }, 0.2f, "read_word");

    _owl->playSequence("read", false, [=](gaf::GAFObject *, const std::string & string) {
        this->idle();
        if (this->_delegate) {
            this->_delegate->onOwlRead(this, _index, with_sound);
        }
    });

}

void MJOwl::readAfterDelay(mj::TextTiming timing) {
    assert(timing.text == _word.text && "Not the timing data for this owl");
    auto total_time = timing.time_end - timing.time_start;

    total_time /= 1000;

    auto total_halftime = total_time / 2;

    auto delay = DelayTime::create(timing.time_start / 1000);

    auto text = _owl->getChildByName("text");
    auto scale = ScaleBy::create(total_halftime, 1.5);
    auto reverse_scale = scale->reverse();
    text->runAction(Sequence::create(delay->clone(), scale, reverse_scale, NULL));

    auto tint = TintTo::create(total_halftime, _tint_text_color.r, _tint_text_color.g, _tint_text_color.b);
    auto reverse_tint = TintTo::create(total_halftime, _text_color.r, _text_color.g, _text_color.b);

    text->runAction(Sequence::create(delay, tint, reverse_tint, NULL));
}

void MJOwl::land(bool is_notify) {
    _owl->playSequence("land", false, [=](gaf::GAFObject * object, const std::string & sequence_name) {
        this->idle();
        if (is_notify) {
            if (_delegate) {
                _delegate->onOwlFliedToStand(this);
            }
        }
    });
}

void MJOwl::idle() {
    auto random_id = random(1, 6);
    _owl->playSequence(StringUtils::format("owl%d", random_id), false, [=](gaf::GAFObject *, const std::string &) {
        this->idle();
    });
}

void MJOwl::flyIn(int index, int total) {
    auto angle = 180.f / (total - 1);
    angle = 180 - angle * index;

    Vec2 v = Vec2(1200, 0).rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(angle));
    _owl->setPosition(v);

    flyBackToStand(true);
}

void MJOwl::flyTo(Point position, bool notify_when_landed) {
    fly();
    auto velocity = random(500.f, 800.f);
    auto time = _owl->getPosition().distance(position) / velocity;
    _owl->runAction(Sequence::create(CallFunc::create([=] {
        _owl->setTouchEnabled(false);
    }), MoveTo::create(time, position), CallFunc::create([=] {
        this->land(notify_when_landed);
    }), CallFunc::create([=] {
        _owl->setTouchEnabled(true);
    }), nullptr));
}

void MJOwl::flyBackToStand(bool is_notify) {
    this->setLocalZOrder(0);
    flyTo(Point(this->getContentSize() / 2), is_notify);
}

void MJOwl::moveOwlToSlot(int slot) {
    fly();
    //calculate slot position

    auto new_position = Point(0, this->getContentSize().height / 2 - 220);
    new_position.x = slot * this->getContentSize().width + this->getContentSize().width / 2 + 20 * slot;

    new_position = this->getParent()->convertToWorldSpace(new_position);
    new_position = this->convertToNodeSpace(new_position);

    _origin = new_position;

    auto velocity = random(300.f, 500.f);
    auto time = _owl->getPosition().distance(new_position) / velocity;
    
    _owl->runAction(Sequence::create(MoveTo::create(time, new_position), CallFunc::create([=] {
        if (_enable_touch) {
            _owl->setTouchEnabled(true);
            _owl->addTouchEventListener(CC_CALLBACK_2(MJOwl::onTouch, this));
        }
        this->land(false);
    }), nullptr));
}

void MJOwl::enableTouch() {
    _enable_touch = true;
}

void MJOwl::onExit() {
    ui::Layout::onExit();
    this->_delegate = nullptr;
}

void MJOwl::flyBack() {
    this->setLocalZOrder(0);
    flyTo(_origin, false);
}

void MJOwl::setOwlSize(float rate)
{
	_owl->getParent()->setContentSize(_owl->getParent()->getContentSize()*rate);
	_owl->setContentSize(_owl->getContentSize()*rate);
	_owl->setScale(rate);
}

void MJOwl::setTouchEnabled(bool is_enabled) {
    ui::Layout::setTouchEnabled(is_enabled);
    _owl->setTouchEnabled(is_enabled);
    _owl->setEnabled(is_enabled);
}


cocos2d::Point MJOwl::getOwlPosition() {
    auto p = _owl->getPosition();
    p = _owl->getParent()->convertToWorldSpace(p);
    return p;
}

cocos2d::Point MJOwl::getOriginalPosition() {
    auto p = this->getContentSize() / 2;
    p = this->convertToWorldSpace(p);
    return p;
}
