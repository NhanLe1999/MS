//
//  Math_Answer.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#include "Math_Answer.h"
#include "AudioHelper.h"

INITIALIZE_READER_INSTANCE(Math_AnswerReader);

Math_Answer* Math_Answer::createAnswer() {
    auto p = (Math_Answer *)(cocos2d::CSLoader::createNode("math_answer.csb"));
    if (p && p->init()) {
    } else {
        CC_SAFE_DELETE(p);
    }
    return p;
}

bool Math_Answer::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    _panel=math::uihelper::getChild<cocos2d::ui::Layout*>(this, "panel");
    _bg=math::uihelper::getChild<cocos2d::ui::ImageView*>(this, "bg");
    _text=math::uihelper::getChild<cocos2d::ui::Text*>(this, "text");
    _icon=math::uihelper::getChild<cocos2d::ui::ImageView*>(this, "icon");
    _button=math::uihelper::getChild<cocos2d::ui::Button*>(this, "button");

    this->setTag(2001);
    this->setName("Math_Answer");
    return true;
}

void Math_Answer::setEnable(std::string title, math::common::AnswerType type, math::callback::onAnswerClicked callback, cocos2d::Size size) {
    
    _type = type;
    _enable = true;
    _text->setString(title);
    _click_callback = callback;
    _clickable = false;
    _correct = false;
    _text->setFontName(m_text_font);
    _text->disableEffect();
    
    _icon_src = nullptr;
    this->setScale(1);
    this->setRotation(0);
    this->setColor(cocos2d::Color3B::WHITE);
    
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
    _icon->setVisible(false);
    
    auto text = _text->getString();
    text = math::func::upperFirstLetter(text);
    _text->setString(text);

    setAnswerSize(size);
}

void Math_Answer::setAnswerSize(cocos2d::Size size) {
    if (_type == math::common::done) {
        size = cocos2d::Size(size.width, 100);
    }

    this->setContentSize(size);
    _bg->setContentSize(size);
    _bg->setPosition(cocos2d::Vec2(0, size.height/2));
    _text->setPosition(size/2);
    _text->setTextColor(m_text_color);
    _button->setContentSize(size);
    
    _icon->setContentSize(cocos2d::Size(size.height, size.height) * 0.65);
    
    switch (_type) {
        case math::common::done :
        {
            _bg->loadTexture(cocos2d::StringUtils::format("%scommon/done.png", m_theme->getThemePath().c_str()));
            auto render_size = _bg->getVirtualRendererSize();
            _bg->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
            _text->setTextColor(cocos2d::Color4B::WHITE);
            _text->setFontSize(size.height*0.5);
        }
            break;
        case math::common::icon_name :
        {
            _bg->loadTexture(cocos2d::StringUtils::format("%scommon/answer.png", m_theme->getThemePath().c_str()));
            auto render_size = _bg->getVirtualRendererSize();
            _bg->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.45, render_size.width*0.2, render_size.height*0.1));
            _text->setFontSize(size.height*0.4);
            
            auto icon_w = _icon->getContentSize().width;
            auto text_w = _text->getContentSize().width;
            auto start_x = (size.width - text_w - icon_w - 5) / 2;
            _icon->setPosition(cocos2d::Vec2(start_x + icon_w*0.5, getContentSize().height*0.5));
            _text->setPosition(cocos2d::Vec2(start_x + icon_w + 5 + text_w*0.5, getContentSize().height*0.5));
        }
            break;
        case math::common::answer :
        {
            _bg->loadTexture(cocos2d::StringUtils::format("%scommon/answer.png", m_theme->getThemePath().c_str()));
            auto render_size = _bg->getVirtualRendererSize();
            _bg->setCapInsets(cocos2d::Rect(render_size.width*0.4, render_size.height*0.45, render_size.width*0.2, render_size.height*0.1));
            _text->setFontSize(size.height*0.4);
        }
            break;
        default:
        {
            _bg->setVisible(true);
            _text->setVisible(true);
        }
            break;
    }
}

void Math_Answer::fitAnswerSize() {
    setAnswerSize(getContentSize());
}

void Math_Answer::setCustomAnswer(math::resource::Image* img, cocos2d::Size size, float font_size, cocos2d::Color4B text_color, float padding) {
    _type = math::common::custom_bg;
    _bg->setVisible(true);
    _text->setVisible(true);
    
    this->setContentSize(size);
    _bg->loadTexture(img->path);
    auto render_size = _bg->getVirtualRendererSize();
    if (padding == 0) {
        _bg->setCapInsets(cocos2d::Rect(1, 1, render_size.width-2, render_size.height-2));
    } else {
        _bg->setCapInsets(cocos2d::Rect(render_size.width*padding, render_size.height*padding, render_size.width*(1-padding*2), render_size.height*(1-padding*2)));
    }
    _text->setTextColor(text_color);
    _text->setFontSize(font_size);
    _text->setPosition(size/2);
    _text->enableOutline(cocos2d::Color4B::WHITE, 4);
    
//    if (text_color != cocos2d::Color4B::WHITE) {
//    } else {
//        _text->enableOutline(cocos2d::Color4B::BLACK, 5);
//    }
    
    _bg->setContentSize(size);
    _bg->setPosition(cocos2d::Vec2(0, size.height/2));

    _button->setContentSize(size);    
}

void Math_Answer::setDisable(float delay, math::animation::ObjectShowType revert_show) {
    disableClickAnswer();
    _enable=false;
    onHide(revert_show, delay);
}

bool Math_Answer::isEnable() {
    return _enable;
}

math::common::AnswerType Math_Answer::getType() {
    return _type;
}

void Math_Answer::setCorrect(bool correct) {
    _correct=correct;
}

bool Math_Answer::isCorrect() {
    return _correct;
}

void Math_Answer::setIcon(math::resource::Image* image) {
    _icon_src = image;
    _icon_src->used=true;
    
    if (_type==math::common::icon_name) {
        _icon->setVisible(true);
        _icon->loadTexture(image->path);
        
        auto size = getContentSize();
        auto icon_w = _icon->getContentSize().width;
        auto text_w = _text->getContentSize().width;
        auto start_x = (size.width - text_w - icon_w - 5) / 2;
        _icon->setPosition(cocos2d::Vec2(start_x + icon_w*0.5, getContentSize().height*0.5));
        _text->setPosition(cocos2d::Vec2(start_x + icon_w + 5 + text_w*0.5, getContentSize().height*0.5));
    }
}

math::resource::Image* Math_Answer::getIcon() {
    return _icon_src;
}

void Math_Answer::setTitleWithIcon(std::string key) {
    if (_icon_src!=nullptr) {
        _text->setString(_icon_src->getText(key));
        auto text = _text->getString();
        text = math::func::upperFirstLetter(text);
        _text->setString(text);
    }
}

void Math_Answer::setTitleWithIconProp(std::string key) {
    if (_icon_src!=nullptr) {
        _text->setString(cocos2d::StringUtils::toString(_icon_src->getComparisionValue(key)));
        auto text = _text->getString();
        text = math::func::upperFirstLetter(text);
        _text->setString(text);
    }
}

void Math_Answer::setAnswerVisiable(bool visiable) {
    _text->setVisible(visiable);
}

void Math_Answer::onShowInParent(cocos2d::Node *parent, int order, float delay, math::animation::ObjectShowType show) {
    parent->addChild(this, order);
    this->onShow(show, delay);
}

void Math_Answer::onShow(math::animation::ObjectShowType show, float delay) {
    this->stopAllActions();
    this->captureState();
    this->setOpacity(0);

    switch (show) {
        case math::animation::ScaleUp :
        {
            this->setScale(0);
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.5, 1)))));
        }
            break;
        case math::animation::SlideUp :
        {
            this->setPosition(cocos2d::Vec2(_capture_node.position.x, _capture_node.position.y - 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _capture_node.position)))));
        }
            break;
        case math::animation::SlideDown :
        {
            this->setPosition(cocos2d::Vec2(_capture_node.position.x, _capture_node.position.y + 100));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _capture_node.position)))));
        }
            break;
        case math::animation::SlideLeft :
        {
            this->setPosition(cocos2d::Vec2(_capture_node.position.x + 100, _capture_node.position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _capture_node.position)))));
        }
            break;
        case math::animation::SlideRight :
        {
            this->setPosition(cocos2d::Vec2(_capture_node.position.x - 100, _capture_node.position.y));
            this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::FadeIn::create(0.25), cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, _capture_node.position)))));
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
    this->scheduleOnce([=](float){
        _clickable = true;
    }, delay + 0.5, "button can click");
}

void Math_Answer::onHide(math::animation::ObjectShowType revert_show, float delay) {
    this->stopAllActions();
    this->restoreCaptuedState(false);
    _bg->setColor(cocos2d::Color3B::WHITE);

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
            auto position = cocos2d::Vec2(_capture_node.position.x, _capture_node.position.y - 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideDown :
        {
            auto position = cocos2d::Vec2(_capture_node.position.x, _capture_node.position.y + 100);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideLeft :
        {
            auto position = cocos2d::Vec2(_capture_node.position.x + 100, _capture_node.position.y);
            this->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::createWithTwoActions(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.25), cocos2d::FadeOut::create(0.25)), cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, position))), cocos2d::CallFunc::create([=](){
                this->removeFromParentAndCleanup(true);
            }), NULL));
        }
            break;
        case math::animation::SlideRight :
        {
            auto position = cocos2d::Vec2(_capture_node.position.x - 100, _capture_node.position.y);
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

void Math_Answer::onAnimationClick(bool correct) {
    if (correct) {
        audio_helper->play("answer_correct.mp3");

        std::string gaf = "click1";
        auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%sgaf/%s/%s.gaf", m_theme->getThemePath().c_str(), gaf.c_str(), gaf.c_str()));
        auto anim = gaf_asset->createObjectAndRun(false);
        anim->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
        anim->setPosition(this->getPosition());
        this->getParent()->addChild(anim, 20);
        scheduleOnce([=](float){
            anim->removeFromParentAndCleanup(true);
        }, 0.5, cocos2d::StringUtils::format("remove anim ans %s", _text->getString().c_str()));

        this->setColor(cocos2d::Color3B(191, 227, 149));
        this->runAction(cocos2d::Sequence::create(cocos2d::EaseBackIn::create(cocos2d::ScaleTo::create(0.25, 1.2)), cocos2d::EaseBackOut::create(cocos2d::ScaleTo::create(0.25, 1)), cocos2d::CallFunc::create([=](){
            this->setColor(cocos2d::Color3B::WHITE);
        }), NULL));
    } else {
        audio_helper->play("answer_wrong.mp3");
        
        this->setColor(cocos2d::Color3B(253, 192, 193));
        auto shake=cocos2d::Sequence::create(cocos2d::MoveBy::create(0.15, cocos2d::Vec2(8, 0)), cocos2d::MoveBy::create(0.15, cocos2d::Vec2(-8, 0)), NULL);
        this->runAction(cocos2d::Sequence::create(cocos2d::Repeat::create(shake, 2), cocos2d::CallFunc::create([=](){
            this->setColor(cocos2d::Color3B::WHITE);
        }), NULL));
    }
}

void Math_Answer::onAutoClick() {
    if (_click_callback && _clickable) {
        _click_callback(this);
    }
}

#pragma mark - Callback Functions
cocos2d::ui::Widget::ccWidgetClickCallback Math_Answer::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(Math_Answer, onClicked),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}

void Math_Answer::onClicked(cocos2d::Ref *pSender) {
    if (_click_callback && _clickable) {
        _click_callback(this);
    }
}

#pragma mark - Code ver 2.1
void Math_Answer::disableClickAnswer() {
    _clickable = false;
}

void Math_Answer::enableClickAnswer() {
    _clickable = true;
}

#pragma mark - Code ver 2.2 - Animation
void Math_Answer::setFlipBG(bool value) {
    
}

void Math_Answer::captureState() {
    _capture_node.position = this->getPosition();
    _capture_node.scale = this->getScale();
    _capture_node.rotation = this->getRotation();
}

void Math_Answer::restoreCaptuedState(bool animated) {
    if (animated) {
        this->runAction(cocos2d::MoveTo::create(0.1, _capture_node.position));
        this->runAction(cocos2d::ScaleTo::create(0.1, _capture_node.scale));
        this->runAction(cocos2d::RotateTo::create(0.1, _capture_node.rotation));
        this->runAction(cocos2d::Sequence::createWithTwoActions(cocos2d::DelayTime::create(0.15), cocos2d::CallFunc::create([=](){
            _clickable = true;
        })));
    } else {
        _clickable = true;
        this->setPosition(_capture_node.position);
        this->setScale(_capture_node.scale);
        this->setRotation(_capture_node.rotation);
    }
}

void Math_Answer::playAnswerAudio(std::string key, float delay) {
    if (_icon_src != nullptr) {
        scheduleOnce([=](float){
            auto audios = _icon_src->getAudios(key);
            audio_helper->play_sequence(audios, nullptr, "");
        }, delay, cocos2d::StringUtils::format("play_%s", _icon_src->getAudios(key).front().c_str()));
    }
}

cocos2d::ui::Text* Math_Answer::getValueText() {
    return _text;
}

float Math_Answer::getRenderSize() {
    if (_type == math::common::icon_name) {
        return _icon->getContentSize().width + _text->getContentSize().width + 25;
    } else {
        return _text->getContentSize().width + 20;
    }
}

