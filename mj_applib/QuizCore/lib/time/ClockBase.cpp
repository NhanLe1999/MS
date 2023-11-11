//
//  ClockBase.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 11/23/17.
//

#include "ClockBase.h"

using namespace math;
void ClockBase::setClockSize(int width) {

    cocos2d::Size clock_size = cocos2d::Size(width, width);
    this->setContentSize(clock_size);
    _face->setContentSize(clock_size);
    _face->setPosition(clock_size/2);
    
}

int ClockBase::getHour() {
    return hour;
}

int ClockBase::getMinute() {
    return minute;
}

void ClockBase::setTime(int hour, int minute) {
    this->hour = hour;
    this->minute = minute;
    
    //fix hour
    if (hour >= 24 || hour < 0) {
        hour = 0;
    }
    if (minute >=60 || minute < 0) {
        minute = 0;
    }
    
    //time mode 24
    math::resource::Voice voice;
    voice.voice_id = 1;
    voice.paths = getTimeAudios(true);
    math::resource::Text text;
    text.text = getTimeSemiText(true);
    text.voices.push_back(voice);
    math::resource::Property prop_semi24;
    prop_semi24.type = math::resource::Property::PROP;
    prop_semi24.key = "name_1";
    prop_semi24.value = text;
    
    //time mode 12
    math::resource::Voice voice12;
    voice12.voice_id = 1;
    voice12.paths = getTimeAudios(false);
    math::resource::Text text12;
    text12.text = getTimeSemiText(false);
    text12.voices.push_back(voice12);
    math::resource::Property prop_semi12;
    prop_semi12.type = math::resource::Property::PROP;
    prop_semi12.key = "name_n";
    prop_semi12.value = text12;
    
    math::resource::PropertiesSet prop_set;
    prop_set.lang = m_lang->getLanguage();
    prop_set.properties.push_back(prop_semi12);
    prop_set.properties.push_back(prop_semi24);
    
    _img_clock = math::resource::ResHelper::createImage("");
    _img_clock->properties.push_back(prop_set);
}

void ClockBase::show(float delay, math::animation::ObjectShowType show) {
    
    auto position_clock = this->getPosition();
    this->setOpacity(0);
    
    for (Node* child : getChildren()) {
        child->setOpacity(0);
        child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::FadeIn::create(0.25)));
    }
    
    switch (show) {
        case math::animation::ScaleUp :
        {
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
        }
            break;
        case math::animation::SlideUp :
        {
            this->setPosition(cocos2d::Vec2(position_clock.x, position_clock.y - 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
        }
            break;
        case math::animation::SlideDown :
        {
            this->setPosition(cocos2d::Vec2(position_clock.x, position_clock.y + 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
        }
            break;
        case math::animation::SlideLeft :
        {
            this->setPosition(cocos2d::Vec2(position_clock.x + 100, position_clock.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
        }
            break;
        case math::animation::SlideRight :
        {
            this->setPosition(cocos2d::Vec2(position_clock.x - 100, position_clock.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, position_clock)))));
        }
            break;
        default:
        {
            this->setRotation(180);
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)), cocos2d::RotateTo::create(0.4, 0), NULL)));
        }
            break;
    }
}

void ClockBase::destroy (float delay, math::animation::ObjectShowType revert_show) {
    this->stopAllActions();
    //_face->removeFromParentAndCleanup(true);
    
    for (Node* child : getChildren()) {
        child->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay+0.25), cocos2d::FadeOut::create(0.25)));
    }
    
    auto position_clock = this->getPosition();
    switch (revert_show) {
        case math::animation::ScaleUp :
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideUp :
        {
            auto position = cocos2d::Vec2(position_clock.x, position_clock.y - 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideDown :
        {
            auto position = cocos2d::Vec2(position_clock.x, position_clock.y + 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideLeft :
        {
            auto position = cocos2d::Vec2(position_clock.x + 100, position_clock.y);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideRight :
        {
            auto position = cocos2d::Vec2(position_clock.x - 100, position_clock.y);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        default:
        {
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.5, 0)), cocos2d::RotateTo::create(0.4, 180), NULL), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
    }
}

void ClockBase::onTouchClock(bool correct) {
    if (correct) {
        
    } else {
        
    }
}

std::string ClockBase::getTimeNumber(bool mode24) {
    if (mode24) {
        return cocos2d::StringUtils::format("%d:%02d", hour, minute);
    } else {
        std::string am_pm = "a.m.";
        int fix_hour = hour;
        if (fix_hour>12) {
            fix_hour-=12;
        }
        if (hour>=12) {
            am_pm = "p.m.";
        }
        return cocos2d::StringUtils::format("%d:%02d %s", fix_hour, minute, am_pm.c_str());
    }
}

std::string ClockBase::getTimeText(bool mode24) {
    if (mode24) {
        std::string hour_str = math::number::getNumberText(hour);
        hour_str = math::func::upperFirstLetter(hour_str);
        
        if (minute == 0) {
            return (hour_str + " o'clock");
        } else if (minute == 30) {
            return "Half past " + hour_str;
        }
    } else {
        std::string am_pm = " a.m.";
        int fix_hour = hour;
        if (fix_hour>12) {
            fix_hour-=12;
        }
        if (hour>=12) {
            am_pm = " p.m.";
        }

        std::string hour_str = math::number::getNumberText(fix_hour);
        hour_str = math::func::upperFirstLetter(hour_str);
        
        if (minute == 0) {
            return hour_str + " o'clock" + am_pm;
        } else if (minute == 30) {
            return "Half past " + hour_str + am_pm;
        }
    }
    
    return getTimeNumber(mode24);
}

std::string ClockBase::getTimeSemiText(bool mode24) {
    if (mode24) {
        std::string hour_str = cocos2d::StringUtils::format("%d", hour);
        
        if (minute == 0) {
            return (hour_str + " o'clock");
        } else if (minute == 30) {
            return "Half past " + hour_str;
        }
    } else {
        std::string am_pm = " a.m.";
        int fix_hour = hour;
        if (fix_hour>12) {
            fix_hour-=12;
        }
        if (hour>=12) {
            am_pm = " p.m.";
        }
        
        std::string hour_str = cocos2d::StringUtils::format("%d", fix_hour);
        
        if (minute == 0) {
            return hour_str + " o'clock" + am_pm;
        } else if (minute == 30) {
            return "Half past " + hour_str + am_pm;
        }
    }
   
    return getTimeNumber(mode24);
}

std::vector<std::string> ClockBase::getTimeAudios(bool mode24) {
    std::vector<std::string> audios;
    
    if (mode24) {
        auto hour_audios = math::number::getNumberAudio(hour);
        
        if (minute == 0) {
            hour_audios.push_back("oclock.mp3");
        } else if (minute == 30) {
            hour_audios.insert(hour_audios.begin(), "half_past.mp3");
        }
    } else {
        std::string am_pm = "am.mp3";
        int fix_hour = hour;
        if (fix_hour>12) {
            fix_hour-=12;
        }
        if (hour>=12) {
            am_pm = "pm.mp3";
        }
        
        auto hour_audios = math::number::getNumberAudio(fix_hour);
        
        if (minute == 0) {
            hour_audios.push_back("oclock.mp3");
        } else if (minute == 30) {
            hour_audios.insert(hour_audios.begin(), "half_past.mp3");
        }
        hour_audios.push_back(am_pm);
    }
    return audios;
}

math::resource::Image* ClockBase::getImgOfClock() {
    return _img_clock;
}


