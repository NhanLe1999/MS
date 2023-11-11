//
//  DigitalClock.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 10/11/17.
//

#include "DigitalClock.h"
//#include "RomanNumeralsUtils.h"

using namespace math;

#define total_time 720.0
#define number_size 0.13

DigitalClock* DigitalClock::createClock(int theme) {
    auto p = new DigitalClock();
    if (p && p->init(theme)) {
        p->autorelease();
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool DigitalClock::init(int theme) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    clock_theme = theme;
    
    std::string clock_path = cocos2d::StringUtils::format("%sclock/digital/theme_%02d/", m_theme->getThemePath().c_str(), theme);

    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    this->setContentSize(cocos2d::Size(200, 200));
    
    cocos2d::Size clock_size = getContentSize();
    
    _face = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("%sface.png", clock_path.c_str()));
    _face->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    cocos2d::Size real_size = _face->getVirtualRendererSize();
    _face->setPosition(clock_size/2);
    _face->setScale9Enabled(true);
    _face->setCapInsets(cocos2d::Rect(1, 1, real_size.width-2, real_size.height-2));
    _face->setContentSize(clock_size);
    this->addChild(_face, 1);
    
    for (int i=0; i<10; i++) {
        auto image = math::resource::ResHelper::createImage(cocos2d::StringUtils::format("%snumber_%d.png", clock_path.c_str(), i));
        _img_numbers.push_back(image);
    }
    
    for (int i=0; i<4; i++) {
        auto number = mpool->getObject();
        number->setEnable(i, math::common::deny, _img_numbers.front(), nullptr);
        number->setObjectSize(clock_size*number_size);
        number->fixHeightObject();
        //number->onShow(0, math::animation::No_Effect);
        _numbers.push_back(number);
        addChild(number, 4);
    }
    float number_width = _numbers.front()->getContentSize().width / clock_size.width;
    for (int i=0; i<4; i++) {
        if (i<2) {
            cocos2d::Vec2 point = cocos2d::Vec2(0.5 - number_width*(1.7 - i) , 0.5);
            _locations.push_back(point);
            _numbers[i]->setPosition(point * clock_size.width);
        } else {
            cocos2d::Vec2 point = cocos2d::Vec2(0.5 + number_width*(i - 1.3) , 0.5);
            _locations.push_back(point);
            _numbers[i]->setPosition(point * clock_size.width);
        }
    }
    
    am_pm_text = cocos2d::ui::Text::create("AM", _numbers.front()->getLabelValue()->getFontName(), clock_size.width*0.1);
    am_pm_text->setTextColor(_numbers.front()->getLabelValue()->getTextColor());
    addChild(am_pm_text, 5);
    
    return true;
}

void DigitalClock::setMode24(bool mode_24) {
    mode24h = mode_24;
}

void DigitalClock::setTime(int _hour, int _minute) {
    math::ClockBase::setTime(_hour, _minute);
    
    is_am = hour < 12;
    
    int fix_hour = hour;
    if (!mode24h) {
        if (fix_hour > 12) {
            fix_hour = hour % 12;
        }
    }
    
    _numbers[0]->setImage(_img_numbers[fix_hour / 10]);
    _numbers[1]->setImage(_img_numbers[fix_hour % 10]);

    _numbers[2]->setImage(_img_numbers[minute / 10]);
    _numbers[3]->setImage(_img_numbers[minute % 10]);
    
    am_pm_text->setString(is_am?"A.M.":"P.M.");
}

void DigitalClock::setClockSize(int width) {
    cocos2d::Size clock_size = cocos2d::Size(width, width);
    this->setContentSize(clock_size);
    _face->setContentSize(clock_size);
    _face->setPosition(clock_size/2);
    
    for (int i=0; i<4; i++) {
        _numbers[i]->setObjectSize(clock_size*number_size);
        _numbers[i]->fixHeightObject();
        _numbers[i]->setPosition(_locations[i]*width);
    }
    
    float number_width = _numbers.front()->getContentSize().width / clock_size.width;
    for (int i=0; i<4; i++) {
        if (i<2) {
            cocos2d::Vec2 point = cocos2d::Vec2(0.5 - number_width*(1.7 - i) , 0.5);
            _locations.push_back(point);
            _numbers[i]->setPosition(point * clock_size.width);
        } else {
            cocos2d::Vec2 point = cocos2d::Vec2(0.5 + number_width*(i - 1.3) , 0.5);
            _locations.push_back(point);
            _numbers[i]->setPosition(point * clock_size.width);
        }
    }
    
    am_pm_text->setFontSize(width * 0.1);
    am_pm_text->setPosition(cocos2d::Vec2(0.5, 0.35) * width);
}

void DigitalClock::show(float delay, math::animation::ObjectShowType show) {
    math::ClockBase::show(delay, show);
}

void DigitalClock::destroy(float delay, math::animation::ObjectShowType revert_show) {
//    this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::CallFunc::create([=](){
//        am_pm_text->removeFromParentAndCleanup(true);
//    })));
    ClockBase::destroy(delay, revert_show);
}


