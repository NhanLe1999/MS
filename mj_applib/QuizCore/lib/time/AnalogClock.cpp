//
//  AnalogClock.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "AnalogClock.h"
#include "RomanNumeralsUtils.h"
using namespace math;

#define total_time 720.0
AnalogClock* AnalogClock::createClock(int theme) {
    auto p = new AnalogClock();
    if (p && p->init(theme)) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    return p;
}

bool AnalogClock::init(int theme) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    clock_theme = theme;
    clock_path = cocos2d::StringUtils::format("%sclock/analog/theme_%02d/", m_theme->getThemePath().c_str(), theme);
    
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    this->setContentSize(cocos2d::Size(200, 200));
    
    cocos2d::Size clock_size = getContentSize();
    radius = clock_size.width*0.35;
    
    _face = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sface.png", clock_path.c_str()));
    _face->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    cocos2d::Size real_size = _face->getVirtualRendererSize();
    _face->setPosition(clock_size/2);
    _face->setScale9Enabled(true);
    _face->setCapInsets(cocos2d::Rect(1, 1, real_size.width-2, real_size.height-2));
    _face->setContentSize(clock_size);
    this->addChild(_face, 1);

    _minute_hand = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sminute.png", clock_path.c_str()));
    _minute_hand->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _minute_hand->setPosition(clock_size/2);
    _minute_hand->setScale9Enabled(true);
    _minute_hand->setCapInsets(cocos2d::Rect(1, 1, real_size.width-2, real_size.height-2));
    _minute_hand->setContentSize(clock_size);
    this->addChild(_minute_hand, 20);

    _hour_hand = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%shour.png", clock_path.c_str()));
    _hour_hand->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _hour_hand->setPosition(clock_size/2);
    _hour_hand->setScale9Enabled(true);
    _hour_hand->setCapInsets(cocos2d::Rect(1, 1, real_size.width-2, real_size.height-2));
    _hour_hand->setContentSize(clock_size);
    this->addChild(_hour_hand, 21);
    
    for (int i=1; i<=12; i++) {
        auto image = math::number::getNumber(i);
        image->path = cocos2d::StringUtils::format("%snumber_%d.png", clock_path.c_str(), i);
        _img_numbers.push_back(image);
    }
    
    for (int i=0; i<12; i++) {
        auto number = mpool->getObject();
        number->setEnable(i, math::common::deny, _img_numbers[i], nullptr);
        _numbers.push_back(number);
    }
    
    float delta_1 = 0.5;
    float delta_2 = sqrt(3)*0.5;
    _locations.push_back(cocos2d::Vec2(delta_1, delta_2));
    _locations.push_back(cocos2d::Vec2(delta_2, delta_1));
    _locations.push_back(cocos2d::Vec2(1, 0));
    _locations.push_back(cocos2d::Vec2(delta_2, -delta_1));
    _locations.push_back(cocos2d::Vec2(delta_1, -delta_2));
    _locations.push_back(cocos2d::Vec2(0, -1));
    _locations.push_back(cocos2d::Vec2(-delta_1, -delta_2));
    _locations.push_back(cocos2d::Vec2(-delta_2, -delta_1));
    _locations.push_back(cocos2d::Vec2(-1, 0));
    _locations.push_back(cocos2d::Vec2(-delta_2, delta_1));
    _locations.push_back(cocos2d::Vec2(-delta_1, delta_2));
    _locations.push_back(cocos2d::Vec2(0, 1));

    return true;
}

void AnalogClock::setTime(int _hour, int _minute) {
    math::ClockBase::setTime(_hour, _minute);
    
    if (_minute_hand != NULL) {
        float angle = minute*6.0;
        _minute_hand->setRotation(angle);
    }

    if (_hour_hand != NULL) {
        int fix_hour = hour % 12;
        float time = fix_hour*60+minute;
        float angle = time * 360.0 / total_time;
        _hour_hand->setRotation(angle);
    }
}

void AnalogClock::setClockSize(int width) {
    radius = width*0.325;

    cocos2d::Size clock_size = cocos2d::Size(width, width);
    this->setContentSize(clock_size);
    _face->setContentSize(clock_size);
    _face->setPosition(clock_size/2);

    _minute_hand->setContentSize(clock_size);
    _minute_hand->setPosition(clock_size/2);
    
    _hour_hand->setContentSize(clock_size);
    _hour_hand->setPosition(clock_size/2);
    
    cocos2d::Vec2 center = cocos2d::Vec2(width/2, width/2);
    
    for (int i=0; i<_numbers.size(); i++) {
        Math_Object* number = _numbers[i];
        number->setObjectSize(clock_size*0.3);
        number->setPosition(_locations[i] * radius + center);
    }
}

void AnalogClock::show(float delay, math::animation::ObjectShowType show) {
    auto number_size = getContentSize()*0.15;
    
    for (int i=0; i<12; i++) {
        auto number = _numbers[i];
        auto point = _locations[i] * radius;
        number->setObjectSize(number_size);
        number->setValueText("");
        number->setImageVisible(false);
        if (i%4 == 3 || show_full) {
            if (show_text) {
                number->getLabelValue()->setFontSize(number_size.height);
                if (show_roman) {
                    number->setValueText(RomanNumeralsUtils::int_2_roman(number->getComparisionValue().number));
                } else {
                    number->setValueTextByKey("number");
                }
                number->showValueTextWithAnimation(false);
            } else {
                number->setImageVisible(true);
            }
        }
        //number->onShow(0, math::animation::No_Effect);
        addChild(number, 5);
    }
    
    math::ClockBase::show(delay, show);    
}

void AnalogClock::destroy(float delay, math::animation::ObjectShowType revert_show) {
//    this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create([=](){
//        _hour_hand->removeFromParentAndCleanup(true);
//        _minute_hand->removeFromParentAndCleanup(true);
//    })));
    ClockBase::destroy(delay, revert_show);
}

void AnalogClock::setNumberAsText(bool use_text) {
    show_text = use_text;
}

void AnalogClock::setNumberRoman(bool use_roman) {
    show_roman = use_roman;
}

void AnalogClock::setMissingNumber(int number) {
    int fix_number = number;
    if (number <= 0 || number > 12) {
        fix_number = 12;
    }
    if (show_text) {
        _numbers[fix_number-1]->setValueText("?");
    } else {
        _numbers[fix_number-1]->setImage(math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%snumber_miss.png", clock_path.c_str())));
    }
}

Math_Object* AnalogClock::setMissingNumberAsSlot(int number) {
    int fix_number = number;
    if (number <= 0 || number > 12) {
        fix_number = 12;
    }
    
    _numbers[fix_number-1]->setValueText("");
    return _numbers[fix_number-1];
}

void AnalogClock::showMissingNumber(int number) {
    if (number <= 0 || number > 12) {
        for (Math_Object* number : _numbers) {
            if (show_text) {
                number->setValueTextByKey("number");
            } else {
                int value = number->getComparisionValue().number;
                number->setImage(math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%snumber_%d.png", clock_path.c_str(), value)));
            }
        }
    } else {
        _numbers[number-1]->onTouchObject(true);
        if (show_text) {
            _numbers[number-1]->setValueTextByKey("number");
        } else {
            _numbers[number-1]->setImage(math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%snumber_%d.png", clock_path.c_str(), number)));
        }
    }
}


#pragma mark - Function
std::vector<cocos2d::Vec2> AnalogClock::getLocationNumbers() {
    return _locations;
}

cocos2d::Vec2 AnalogClock::getLocationNumber(int number) {
    return _locations[number-1];
}

void AnalogClock::hideAllNumbers() {
    for (Math_Object* obj : _numbers) {
        obj->setVisible(false);
    }
}

void AnalogClock::hideNumber(int number) {
    for (Math_Object* obj : _numbers) {
        if (obj->getComparisionValue().number == number) {
            obj->setVisible(false);
            break;
        }
    }
}

void AnalogClock::showAllNumbers() {
    for (Math_Object* obj : _numbers) {
        obj->setVisible(true);
    }
}

void AnalogClock::showNumber(int number) {
    for (Math_Object* obj : _numbers) {
        if (obj->getComparisionValue().number == number) {
            obj->setVisible(true);
            obj->onShow(0);
            break;
        }
    }
}

void AnalogClock::showHand(bool hour) {
    if (hour) {
        _hour_hand->setVisible(true);
        _hour_hand->setOpacity(0);
        _hour_hand->runAction(cocos2d::FadeIn::create(0.25));
    } else {
        _minute_hand->setVisible(true);
        _minute_hand->setOpacity(0);
        _minute_hand->runAction(cocos2d::FadeIn::create(0.25));
    }
}

void AnalogClock::hideHand(bool hour) {
    if (hour) {
        _hour_hand->setVisible(false);
        //_hour_hand->runAction(cocos2d::FadeOut::create(0.25));
    } else {
        _minute_hand->setVisible(false);
        //_minute_hand->runAction(cocos2d::FadeOut::create(0.25));
    }
}




