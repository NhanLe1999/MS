//
//  Math_Question.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/14/17.
//
//

#include "Math_Question.h"

#define time_in_out 0.5
#define time_change_text 0.25

INITIALIZE_READER_INSTANCE(Math_QuestionReader);

#define qposition cocos2d::Vec2(quiz_size.width/2, quiz_size.height-20)

#pragma mark - Contructor
Math_Question * Math_Question::createQuestion() {
    auto p = (Math_Question *)(cocos2d::CSLoader::createNode("math_question.csb"));
    if (p && p->init()) {
    } else {
        CC_SAFE_DELETE(p);
    }
    
    return p;
}

bool Math_Question::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    }
    
    this->setContentSize(cocos2d::Size(850, 100));
    this->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
    
    this->setTag(2004);
    this->setName("Math_Question");
    
    return true;
}

void Math_Question::setEnable(std::function<void()> callback)
{
    _background = math::uihelper::getChild<cocos2d::ui::ImageView*>(this, "background");
    _text = math::uihelper::getChild<cocos2d::ui::Text*>(this, "background", "text");
    _button = math::uihelper::getChild<cocos2d::ui::Button*>(this, "background", "btn");
    _button_speaker = math::uihelper::getChild<cocos2d::ui::Button*>(this, "btn_speaker");

    this->setContentSize(cocos2d::Size(question_width, 100));
    _enable=true;
    _callback=callback;
    data_set.clear();
    
    //set image of theme
    _background->loadTexture(cocos2d::StringUtils::format("%sthemeui/question.png", m_theme->getThemePath().c_str()));
    auto render_size = _background->getVirtualRendererSize();
    _background->setScale9Enabled(true);
    _background->setCapInsets(cocos2d::Rect(render_size.width *0.4, render_size.height*0.4, render_size.width*0.2, render_size.height*0.2));
    _background->setPosition(cocos2d::Vec2(0, 400));
    
    //text configed
    _text->setString("");
    _text->setTextColor(cocos2d::Color4B::WHITE);
    _text->setFontName(m_text_font);
    _text->setFontSize(32);
    _text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    _text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    _text->setTextAreaSize(cocos2d::Size(question_width-50, 0));
//    auto label = static_cast<cocos2d::Label*>(_text->getVirtualRenderer());
//    label->setLineHeight(50);
    
    //btn touch question
    _button->setOpacity(0);
    _button->setTouchEnabled(false);

    //btn speaker
    _button_speaker->loadTextureNormal(cocos2d::StringUtils::format("%sthemeui/speaker2.png", m_theme->getThemePath().c_str()));
    _button_speaker->setOpacity(0);
    _button_speaker->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_TOP);
    auto brsize = _button_speaker->getVirtualRendererSize();
    _button_speaker->setContentSize(brsize);
}

void Math_Question::setDisable()
{
    //remvove all child of data
    this->removeFromParentAndCleanup(true);
}

bool Math_Question::isEnable()
{
    return _enable;
}

#pragma mark - Animation
void Math_Question::onStart(float delay) {
    _background->setVisible(true);
    _button->setVisible(true);
    _text->setVisible(true);
    
    _showing = true;
    index_question = 0;

    this->adjustQuestionSize();
    auto qsize = getContentSize();
    
    _button_speaker->setPosition(cocos2d::Vec2((question_width - quiz_size.width)/2 + 60, qsize.height));
    _button_speaker->setOpacity(0);
    _button_speaker->setTouchEnabled(false);
    
    _background->setOpacity(0);
    _background->setPosition(cocos2d::Vec2(qsize.width/2, qsize.height/2 + 200));
    _background->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(delay), cocos2d::Spawn::create(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(qsize.width/2, qsize.height/2))), cocos2d::FadeIn::create(0.5),  NULL), cocos2d::CallFunc::create([=]() {
        if (_callback) {
            _callback();
        }
        if (_audio_callback) {
            _audio_callback(true);
        }
    }), NULL));
}

void Math_Question::onHide(float delay)
{
    scheduleOnce([=](float){
        if (_showing) {
            _button_speaker->setPosition(cocos2d::Vec2(0, 1000));
            hideQuestion();
        } else {
            _button_speaker->runAction(cocos2d::FadeOut::create(0.3));
        }
    }, delay, "hide Q");
    
    scheduleOnce([=](float){
        this->setDisable();
    }, delay + 0.5, "disable Q");
}

void Math_Question::showQuestion() {
    if (_showing) {
        if (_audio_callback) {
            _audio_callback(true);
        }
    } else {
        _showing = true;
        _background->stopAllActions();
        
        auto qsize = getContentSize();
        _background->runAction(cocos2d::Spawn::createWithTwoActions(cocos2d::EaseBackOut::create(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(qsize.width/2, qsize.height/2))), cocos2d::FadeIn::create(0.5)));
        
        _button_speaker->stopAllActions();
        _button_speaker->setTouchEnabled(false);
        _button_speaker->runAction(cocos2d::FadeOut::create(0.3));
        
        if (_audio_callback) {
            scheduleOnce([=](float){
                _audio_callback(true);
            }, 0.5, "show question callback");
        }
    }
}

void Math_Question::hideQuestion() {
    if (_showing) {
        _showing = false;
        //question
        auto qsize = getContentSize();
        _background->stopAllActions();
        _background->runAction(cocos2d::Spawn::createWithTwoActions(cocos2d::EaseBackIn::create(cocos2d::MoveTo::create(0.5, cocos2d::Vec2(qsize.width/2, qsize.height/2 + 200))), cocos2d::FadeOut::create(0.5)));
        
        //button speaker
        _button_speaker->stopAllActions();
        _button_speaker->runAction(cocos2d::Sequence::create(cocos2d::DelayTime::create(0.2), cocos2d::FadeIn::create(0.3), cocos2d::CallFunc::create([=](){
            _button_speaker->setTouchEnabled(true);
        }), NULL));
    }
}

bool Math_Question::goNext() {
    index_question++;
    if (index_question < data_set.size()) {
        if (_showing) {
            _text->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.2), cocos2d::CallFunc::create([=](){
                this->adjustQuestionSize();
                if (_callback) {
                    _callback();
                }
                if (_audio_callback) {
                    _audio_callback(true);
                }
            }), cocos2d::FadeIn::create(0.2), NULL));
        } else {
            this->adjustQuestionSize();
            showQuestion();
            scheduleOnce([=](float){
                if (_callback) {
                    _callback();
                }
            }, 0.5, "next Q callback");
        }
        return true;
    }
    return false;
}

bool Math_Question::goToQuestionAtIndex(int index) {
    //find index
    index_question = index;
    if (index_question < data_set.size()) {
        if (_showing) {
            _text->runAction(cocos2d::Sequence::create(cocos2d::FadeOut::create(0.2), cocos2d::CallFunc::create([=](){
                this->adjustQuestionSize();
                if (_callback) {
                    _callback();
                }
                if (_audio_callback) {
                    _audio_callback(true);
                }
            }), cocos2d::FadeIn::create(0.2), NULL));
        } else {
            this->adjustQuestionSize();
            showQuestion();
            scheduleOnce([=](float){
                if (_callback) {
                    _callback();
                }
            }, 0.5, "next Q callback");
        }
        return true;
    }
    return false;
}

#pragma mark - Data
std::vector<std::string> Math_Question::getOptionalKeys() {
    std::vector<std::string> keys;
    index_question = 0;
    while (index_question < data_set.size()) {
        for (int index=0; index < data_set[index_question].structures.size(); index++) {
            math::question::Structure structure = data_set[index_question].structures[index];
            if (structure.type == math::question::OPTIONAL) {
                auto key = structure.optional.key;
                bool add = true;
                for (std::string _key : keys) {
                    if (key == _key) {
                        add = false;
                        break;
                    }
                }
                if (add) {
                    keys.push_back(key);
                }
            }
        }
        index_question++;
    }
    return keys;
}

void Math_Question::setOptionalTextsComp(std::string key, std::vector<math::resource::Image *> images) {
    index_question = 0;
    while (index_question < data_set.size()) {
        for (int index=0; index < data_set[index_question].structures.size(); index++) {
            math::question::Structure structure = data_set[index_question].structures[index];
            if (structure.type == math::question::OPTIONAL) {
                if (structure.optional.key == key) {
                    math::question::Structure _struc;
                    _struc.index = structure.index;
                    _struc.type = structure.type;
                    _struc.keyword_index = structure.keyword_index;
                    _struc.keywords = structure.keywords;
                    
                    math::question::OptionalWord _opt;
                    _opt.index = structure.optional.index;
                    _opt.key = structure.optional.key;
                    
                    for (math::resource::Image* img : images) {
                        math::resource::Text txt;
                        txt.text = cocos2d::StringUtils::toString(img->getComparisionValue(key));
                        auto text = img->getTextValue("name_1");
                        txt.voices = text.voices;
                        _opt.texts.push_back(txt);
                    }
                    _struc.optional = _opt;
                    
                    //change optional by _opt
                    data_set[index_question].structures.erase(data_set[index_question].structures.begin()+index);
                    data_set[index_question].structures.insert(data_set[index_question].structures.begin()+index, _struc);
                }
            }
        }
        index_question++;
    }
}

void Math_Question::setOptionalTexts(std::string key, std::vector<math::resource::Image *> images) {
    index_question = 0;
    while (index_question < data_set.size()) {
        for (int index=0; index < data_set[index_question].structures.size(); index++) {
            math::question::Structure structure = data_set[index_question].structures[index];
            if (structure.type == math::question::OPTIONAL) {
                if (structure.optional.key == key) {
                    math::question::Structure _struc;
                    _struc.index = structure.index;
                    _struc.type = structure.type;
                    _struc.keyword_index = structure.keyword_index;
                    _struc.keywords = structure.keywords;
                    
                    math::question::OptionalWord _opt;
                    _opt.index = structure.optional.index;
                    _opt.key = structure.optional.key;
                    _struc.showicon = structure.showicon;
                    _struc.highlight = structure.highlight;
                    CCLOG("question option index: %d", _struc.index);
                    if (_struc.showicon) {
                        _struc.icon = images[_opt.index];
                    }
                    for (math::resource::Image* img : images) {
                        auto text = img->getTextValue(key);
                        _opt.texts.push_back(text);
                    }
                    _struc.optional = _opt;
                    
                    //change optional by _opt
                    data_set[index_question].structures.erase(data_set[index_question].structures.begin()+index);
                    data_set[index_question].structures.insert(data_set[index_question].structures.begin()+index, _struc);
                }
            }
        }
        index_question++;
    }
}

void Math_Question::setOptionalTexts(std::string key, std::vector<math::resource::Text> texts) {
    index_question = 0;
    while (index_question < data_set.size()) {
        for (int index=0; index < data_set[index_question].structures.size(); index++) {
            math::question::Structure structure = data_set[index_question].structures[index];
            if (structure.type == math::question::OPTIONAL) {
                if (structure.optional.key == key) {
                    math::question::Structure _struc;
                    _struc.index = structure.index;
                    _struc.type = structure.type;
                    _struc.keyword_index = structure.keyword_index;
                    _struc.keywords = structure.keywords;
                    
                    math::question::OptionalWord _opt;
                    _opt.index = structure.optional.index;
                    _opt.key = structure.optional.key;
                    
                    for (math::resource::Text txt : texts) {
                        _opt.texts.push_back(txt);
                    }
                    _struc.optional = _opt;
                    
                    //change optional by _opt
                    data_set[index_question].structures.erase(data_set[index_question].structures.begin()+index);
                    data_set[index_question].structures.insert(data_set[index_question].structures.begin()+index, _struc);
                }
            }
        }
        index_question++;
    }
}

void Math_Question::setKeywordValue(int value) {
    index_question = 0;
    while (index_question < data_set.size()) {
        for (int index=0; index < data_set[index_question].structures.size(); index++) {
            math::question::Structure structure = data_set[index_question].structures[index];
            if (structure.type == math::question::KEYWORD) {
                for (int i = 0; i < structure.keywords.size(); i++) {
                    math::question::Keyword kw = structure.keywords[i];
                    if (kw.value == value) {
                        math::question::Structure _struc;
                        _struc.index = structure.index;
                        _struc.type = structure.type;
                        _struc.keyword_index = i;
                        _struc.keywords = structure.keywords;
                        
                        data_set[index_question].structures.erase(data_set[index_question].structures.begin()+index);
                        data_set[index_question].structures.insert(data_set[index_question].structures.begin()+index, _struc);
                        break;
                    }
                }
            }
        }
        index_question++;
    }
}

void Math_Question::pushData(math::question::Question data) {
    data_set.push_back(data);
}

std::vector<math::resource::Text> Math_Question::getQuestionTexts() {
    std::vector<math::resource::Text> texts;
    for (math::question::Structure structure : data_set[index_question].structures) {
        math::resource::Text text;
        switch (structure.type) {
            case math::question::TEMPLATE :
            {
                text = structure.template_text;
                text.highlight = structure.highlight;
                text.showicon = false;
            }
                break;
            case math::question::KEYWORD :
            {
                text = structure.keywords[structure.keyword_index].text;
                text.highlight = structure.highlight;
                text.showicon = false;
            }
                break;
            default:
            {
                text = structure.optional.getText();
                text.highlight = structure.highlight;
                if (structure.showicon && structure.icon) {
                    text.showicon = true;
                    text.icon = structure.icon->path;
                } else {
                    text.showicon = false;
                }
            }
                break;
        }
        texts.push_back(text);
    }
    return texts;
}

std::string Math_Question::getQuestionTextValue() {
    std::string question_txt = "";
    highlight_set.clear();
    auto texts = getQuestionTexts();
    for (math::resource::Text txt : texts) {
        std::string text = math::func::trim(txt.text);
        std::string first_txt = text.substr(0, 1);
        if (!(first_txt == "." || first_txt == "?" || first_txt == "!" || first_txt == ":" || first_txt == "," || question_txt == "")) {
            question_txt = cocos2d::StringUtils::format("%s ", question_txt.c_str());
        }
        
        if (txt.highlight || txt.showicon) {
            math::question::Highlight highlight;
            highlight.start = (int)question_txt.length();
            highlight.number = (int)text.length();
            highlight.show_icon = false;
            highlight_set.push_back(highlight);
            
            if (txt.showicon) {
                math::question::Highlight highlight;
                highlight.start = (int)question_txt.length()+(int)text.length();
                highlight.number = 3;
                highlight.show_icon = true;
                highlight.icon = txt.icon;
                highlight_set.push_back(highlight);
            }
        }
        if (txt.showicon) {
            text = cocos2d::StringUtils::format("%s###", text.c_str());
        }
        question_txt = cocos2d::StringUtils::format("%s%s", question_txt.c_str(), text.c_str());
    }
    question_txt = cocos2d::StringUtils::format("%s%s", question_txt.c_str(), math::func::trim(data_set[index_question].ending).c_str());
    return question_txt;
}

std::vector<std::string> Math_Question::getQuestionVoices(int voice_id) {
    std::vector<std::string> audios;
    auto texts = getQuestionTexts();
    for (math::resource::Text txt : texts) {
        auto txt_voices = txt.getVoices(voice_id);
        for (std::string voice : txt_voices) {
            audios.push_back(voice);
        }
    }
    return audios;
}

math::question::Question Math_Question::getData() {
    return data_set[index_question];
}

#pragma mark - Callback Functions
cocos2d::ui::Widget::ccWidgetClickCallback Math_Question::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(Math_Question, onSpeak),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}

void Math_Question::onSpeak(cocos2d::Ref *) {
    showQuestion();
}

void Math_Question::setAudioCallback(std::function<void(bool)> callback) {
    _audio_callback = callback;
}

int Math_Question::getNumberQuestions() {
    return (int)data_set.size();
}

void Math_Question::enableTouchQuestion() {
    _button->setTouchEnabled(true);
}

#pragma mark - Text + Highlight
void Math_Question::setQuestionWidth(float width) {
    question_width = width;
    setContentSize(cocos2d::Size(width, 50));
    _text->setTextAreaSize(cocos2d::Size(width, 0));
    _text->setContentSize(cocos2d::Size(width - 50, 50));
}

cocos2d::Size Math_Question::adjustQuestionSize() {
    if (index_question >= data_set.size()) {
        index_question = 0;
    }
    _text->setString(getQuestionTextValue());
    auto label = static_cast<cocos2d::Label*>(_text->getVirtualRenderer());
    auto size = label->getContentSize();
    auto question_size = cocos2d::Size(question_width, size.height + 40);
    auto center = cocos2d::Vec2(question_size.width/2, question_size.height/2);
    
    this->setContentSize(question_size);
    _background->setContentSize(question_size);
    if (_showing) {
        _background->setPosition(center);        
    }
    _text->setContentSize(cocos2d::Size(question_width-50, size.height));
    _text->setPosition(cocos2d::Vec2(center.x, center.y));
    _button->setContentSize(question_size);
    _button->setPosition(center);
    
    doHighlight();
    return question_size;
}

void Math_Question::doHighlight () {
    for (cocos2d::ui::ImageView* ivicon : highlight_icons) {
        ivicon->removeFromParentAndCleanup(true);
    }
    highlight_icons.clear();
    
    for (int i=0; i<_text->getStringLength(); i++) {
        auto letter = _text->getLetter(i);
        if (letter) {
            letter->setOpacity(255);
            letter->setColor(cocos2d::Color3B::WHITE);
        }
    }
    if (highlight_set.empty()) {
        _text->setTextColor(m_text_color);
    } else {
        _text->setTextColor(cocos2d::Color4B::WHITE);
        for (math::question::Highlight highlight : highlight_set) {
            if (highlight.show_icon) {
                cocos2d::Vec2 point = cocos2d::Vec2::ZERO;
                for (int i=highlight.start; i<highlight.start+highlight.number; i++) {
                    auto letter = _text->getLetter(i);
                    if (letter) {
                        if (point == cocos2d::Vec2::ZERO) {
                            point = letter->getPosition();
                        }
                        letter->setColor(cocos2d::Color3B::GRAY);
                        letter->setOpacity(0);
                    }
                }
                auto icon = cocos2d::ui::ImageView::create(highlight.icon);
                icon->setScale9Enabled(true);
                auto srender = icon->getVirtualRendererSize();
                icon->setCapInsets(cocos2d::Rect(1, 1, srender.width-2, srender.height-2));
                icon->setContentSize(cocos2d::Size(50, 50));
                icon->setAnchorPoint(cocos2d::Vec2(0.0, 0.4));
                icon->setPosition(point);
                _text->addChild(icon);
                highlight_icons.push_back(icon);
            } else {
                for (int i=highlight.start; i<highlight.start+highlight.number; i++) {
                    auto letter = _text->getLetter(i);
                    if (letter) {
                        letter->setColor(cocos2d::Color3B::RED);
                    }
                }
            }
        }
        for (int i=0; i<_text->getStringLength(); i++) {
            auto letter = _text->getLetter(i);
            if (letter && letter->getColor() == cocos2d::Color3B::WHITE) {
                letter->setColor(m_text_color_3b);
            }
        }
    }
}

