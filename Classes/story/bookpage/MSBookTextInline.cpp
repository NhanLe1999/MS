//
//  MSBookTextInline.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/27/18.
//

#include "MSBookTextInline.h"
#include "MJHelper.h"
#include "AudioEngine.h"
#include "StoryConfigManager.h"

USING_NS_CC;

#define TAG_READ_TRANSLATION 0x4793

MSBookTextInline * MSBookTextInline::create(mj::BOOK_TEXT_INFO info) {
    MSBookTextInline *pRet = new(std::nothrow) MSBookTextInline();
    if (pRet && pRet->init(info)) {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool MSBookTextInline::init(mj::BOOK_TEXT_INFO info) {
    if (!ui::Layout::init()) {
        return false;
    }
    
    _info = info;
    while (_info.text.find("<br>") != std::string::npos) {
        _info.text = mj::helper::replace_string(_info.text, "<br>", "\n");
    }
    
    auto bb = RectFromString(_info.boundingbox);
    this->setContentSize(bb.size);
    this->setPosition(bb.origin);
    
    auto new_text = _info.text;
    
    for (auto i = 0; i < (int)_info.inline_images.size(); ++i) {
        auto inline_image = _info.inline_images[i];
        auto replace_text = StringUtils::format("~%d", i);
        while (replace_text.length() < inline_image.text.length()) {
            replace_text += "~";
        }
        
        for (auto u = 0; u < inline_image.position.size(); ++u) {
            auto pos = inline_image.position[u];
            pos = mj::helper::positionInString(_info.text, inline_image.text, pos);
            new_text.replace(pos, replace_text.length(), replace_text);
        }
    }
    
    for (auto i = 0; i < (int)_info.inline_translations.size(); ++i) {
        auto inline_image = _info.inline_translations[i];
        auto replace_text = StringUtils::format("^%d", i);
        while (replace_text.length() < inline_image.text.length()) {
            replace_text += "^";
        }
        
        for (auto u = 0; u < inline_image.position.size(); ++u) {
            auto pos = inline_image.position[u];
            pos = mj::helper::positionInString(_info.text, inline_image.text, pos);
            new_text.replace(pos, replace_text.length(), replace_text);
        }
    }
    
    CCLOG("replaced: %s", new_text.c_str());
    while (new_text.find("\n") != std::string::npos) {
        new_text = mj::helper::replace_string(new_text, "\n", " {new_line} ");
    }
    
    while (new_text.find("<br>") != std::string::npos) {
        new_text = mj::helper::replace_string(new_text, "<br>", " {new_line} ");
    }
    
    auto split = mj::helper::split(new_text, ' ');
    
    auto last_pos = 0;
    
    for (auto s : split) {
        if (s.find("~") != std::string::npos) {
            auto ss = mj::helper::split(s, '~');
            ss.erase(std::remove_if(ss.begin(), ss.end(), [](std::string e) {
                return e.empty();
            }), ss.end());
            for (auto c : ss) {
                if (c.front() >= 0 && std::isdigit(c.front())) {
                    std::string new_c = "";
                    while (!c.empty() && std::isdigit(c.front())) {
                        new_c += c.front();
                        c.erase(c.begin());
                    }
                    auto image_idx = std::atoi(new_c.c_str());
                    auto current_info = _info.inline_images[image_idx];
                    
                    ElementInfo e;
                    e.type = ElementType::IMAGE;
                    e.text = current_info.text;
                    e.image = current_info.image_path;
                    e.audio = current_info.audio_path;
                    auto sync = _info.text.find(e.text, last_pos);
                    if (sync != std::string::npos) {
                        auto temp_str = _info.text.substr(0, sync);
                        CCLOG("start pos of element %s: %ld", e.text.c_str(), StringUtils::getCharacterCountInUTF8String(temp_str));
                        last_pos = (int)sync + (int)e.text.length();
                        e.start_sync_index = (int)StringUtils::getCharacterCountInUTF8String(temp_str);
                    }
                    e.image_size = Size(current_info.image_width, current_info.image_height);
                    std::copy(current_info.position.begin(), current_info.position.end(), std::back_inserter(e.position));
                    _elements.push_back(e);
                    
                    if (!c.empty()) {
                        ElementInfo e;
                        e.type = ElementType::NORMAL;
                        e.text = c;
                        auto sync = _info.text.find(e.text, last_pos);
                        if (sync != std::string::npos) {
                            auto temp_str = _info.text.substr(0, sync);
                            CCLOG("start pos of element %s: %ld", e.text.c_str(), StringUtils::getCharacterCountInUTF8String(temp_str));
                            last_pos = (int)sync + (int)e.text.length();
                            e.start_sync_index = (int)StringUtils::getCharacterCountInUTF8String(temp_str);
                        }
                        _elements.push_back(e);
                    }
                } else {
                    ElementInfo e;
                    e.type = ElementType::NORMAL;
                    e.text = c;
                    auto sync = _info.text.find(e.text, last_pos);
                    if (sync != std::string::npos) {
                        auto temp_str = _info.text.substr(0, sync);
                        CCLOG("start pos of element %s: %ld", e.text.c_str(), StringUtils::getCharacterCountInUTF8String(temp_str));
                        last_pos = (int)sync + (int)e.text.length();
                        e.start_sync_index = (int)StringUtils::getCharacterCountInUTF8String(temp_str);
                    }
                    _elements.push_back(e);
                }
            }
        } else if (s.find("^") != std::string::npos) {
            auto ss = mj::helper::split(s, '^');
            ss.erase(std::remove_if(ss.begin(), ss.end(), [](std::string e) {
                return e.empty();
            }), ss.end());
            auto translation_id = -1;
            for (auto c : ss) {
                if (c.front() > 0 && std::isdigit(c.front())) {
                    std::string new_c = "";
                    while (!c.empty() && std::isdigit(c.front())) {
                        new_c += c.front();
                        c.erase(c.begin());
                    }
                    
                    translation_id = std::atoi(new_c.c_str());
                    break;
                }
            }
            
            auto current_info = _info.inline_translations[translation_id];
            s.replace(s.find("^"), current_info.text.length(), current_info.text);
            ElementInfo e;
            e.text = s;
            e.type = ElementType::TRANSLATED;
            e.translation = current_info.translation;
            e.translation_audio = current_info.translation_path;
            e.audio = current_info.audio_path;
            std::copy(current_info.position.begin(), current_info.position.end(), std::back_inserter(e.position));
            
            auto sync = _info.text.find(e.text, last_pos);
            if (sync != std::string::npos) {
                auto temp_str = _info.text.substr(0, sync);
                CCLOG("start pos of element %s: %ld", e.text.c_str(), StringUtils::getCharacterCountInUTF8String(temp_str));
                last_pos = (int)sync + (int)e.text.length();
                e.start_sync_index = (int)StringUtils::getCharacterCountInUTF8String(temp_str);
            }
            
            _elements.push_back(e);
        } else {
            ElementInfo e;
            e.type = ElementType::NORMAL;
            e.text = s;
            auto sync = _info.text.find(e.text, last_pos);
            if (sync != std::string::npos) {
                auto temp_str = _info.text.substr(0, sync);
                CCLOG("start pos of element %s: %ld", e.text.c_str(), StringUtils::getCharacterCountInUTF8String(temp_str));
                last_pos = (int)sync + (int)e.text.length();
                e.start_sync_index = (int)StringUtils::getCharacterCountInUTF8String(temp_str);
            }
            _elements.push_back(e);
            
        }
    }
    
    _render = ui::RichText::create();
    _render->setWrapMode(ui::RichText::WrapMode::WRAP_PER_WORD);
    _render->setContentSize(this->getContentSize());
    _render->setPosition(this->getContentSize() / 2);
    _render->ignoreContentAdaptWithSize(false);
//    _render->setVerticalSpace(_info.line_space / Director::getInstance()->getContentScaleFactor());
    _render->setVerticalSpace(_info.line_space);
    _render->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _render->setPosition(this->getContentSize() / 2);
    
    this->addChild(_render);
    
    for (auto e : _elements) {
        ui::RichElement * rich_element;
        int img_space = 0;
        if (e.type == ElementType::NORMAL || e.type == ElementType::TRANSLATED) {
            if (e.text == "{new_line}") {
                _render->pushBackElement(ui::RichElementNewLine::create(-1, Color3B::BLACK, 0x00));
                continue;
            }
            auto t = ui::Text::create(e.text, _info.fontname, _info.fontsize);
            t->setTextColor(Color4B(_info.normal_color_3b));
//            t->setTouchEnabled(true);
            rich_element = ui::RichElementCustomNode::create(-1, Color3B::YELLOW, 0xff, t);
            _element_renders.push_back(std::make_pair(t, e));
        } else {
            auto n = ui::Layout::create();
            auto p = ui::ImageView::create(e.image);
            p->ignoreContentAdaptWithSize(false);
            
            auto original_size = p->getContentSize();
            auto original_ratio = original_size.width / original_size.height;
            
            auto adapt_size = e.image_size;
            auto adapt_ratio = adapt_size.width / adapt_size.height;
            
            if (adapt_ratio > original_ratio) {
                //nếu ảnh dọc
                adapt_size.width = adapt_size.height * original_ratio;
            } else {
                //nếu ảnh ngang
                CCLOG("anh ngang: %s, ratio: %f", e.text.c_str(), original_ratio);
                img_space = 1;
                if(original_ratio > 1.7){
                    img_space = 2;
                }
                adapt_size.height = adapt_size.width / original_ratio;
                _render->pushBackElement(ui::RichElementText::create(-1, Color3B::BLACK, 0xff, " ", _info.fontname, _info.fontsize));
            }
            
            p->setContentSize(adapt_size);
            p->setPosition(Point(e.image_size.width / 2, _info.fontsize / 2));
            n->setContentSize(Size(e.image_size.width, 0));
            n->addChild(p);
            
            if (CONFIG_MANAGER->isTestStory()) {
                auto layout_color = ui::Layout::create();
                layout_color->setBackGroundColorType(BackGroundColorType::SOLID);
                layout_color->setBackGroundColorOpacity(0x55);
                layout_color->setBackGroundColor(Color3B(random(0, 0xff), random(0, 0xff), random(0, 0xff)));
                layout_color->setContentSize(p->getContentSize());
                layout_color->setPosition(p->getPosition());
                layout_color->setAnchorPoint(p->getAnchorPoint());
                n->addChild(layout_color, -1);
            }
            
            rich_element = ui::RichElementCustomNode::create(-1, Color3B::YELLOW, 0xff, n);
            _element_renders.push_back(std::make_pair(p, e));
        }
        
        _render->pushBackElement(rich_element);
        
        for(int i = 0; i< img_space; i++){
             _render->pushBackElement(ui::RichElementText::create(-1, Color3B::BLACK, 0xff, " ", _info.fontname, _info.fontsize));
        }
        
        if (e.type == ElementType::NORMAL || e.type == ElementType::TRANSLATED) {
            //space
            _render->pushBackElement(ui::RichElementText::create(-1, Color3B::BLACK, 0xff, " ", _info.fontname, _info.fontsize));
        }
    }
    
    _render->formatText();
    
    return true;
}

void MSBookTextInline::repositioning() {
    auto bb_size = _element_renders.front().first->getBoundingBox();
    bb_size.origin = _element_renders.front().first->getParent()->convertToWorldSpace(bb_size.origin);
    for (auto e : _element_renders) {
        auto e_bb = e.first->getBoundingBox();
        e_bb.origin = e.first->getParent()->convertToWorldSpace(e_bb.origin);
        bb_size = bb_size.unionWithRect(e_bb);
    }
    
    this->setContentSize(Size(this->getContentSize().width, bb_size.size.height));
    
    auto origin = this->getBoundingBox().origin;
    origin = this->getParent()->convertToWorldSpace(origin);
    
    _render->setPosition(Vec2(this->getContentSize() / 2) + origin - bb_size.origin);
    _render->setPositionX(_render->getPositionX() + (this->getContentSize().width - bb_size.size.width) / 2);
}

void MSBookTextInline::enableInteraction() {
    showTranslatedIndicator();
    this->setTouchEnabled(true);
    this->setSwallowTouches(false);
    this->setPropagateTouchEvents(true);
    this->addClickEventListener(CC_CALLBACK_1(MSBookTextInline::onClick, this));
}

void MSBookTextInline::showTranslatedIndicator() {
    auto counter = 0;
    auto last_index = 0;
    for (auto info : _elements) {
        if (info.type == ElementType::TRANSLATED) {
            auto bb = Rect::ZERO;
            Node * anchor_node = nullptr;
            for (auto i = last_index; i < _element_renders.size(); ++i) {
                auto r = _element_renders[i];
                if (info == r.second) {
                    if (!anchor_node) {
                        anchor_node = r.first;
                        last_index = i;
                    } else {
                        if (i == last_index + 1) {
                            last_index++;
                        } else {
                            break;
                        }
                    }
                    if (bb.equals(Rect::ZERO)) {
                        bb = r.first->getBoundingBox();
                    } else {
                        bb = bb.unionWithRect(r.first->getBoundingBox());
                    }
                    
//                    reinterpret_cast<ui::Text *>(r.first)->setFontName("fonts/Linotte Semi Bold.ttf");
                }
            }
            last_index++;
            
            bb.origin = anchor_node->getParent()->convertToWorldSpace(bb.origin);
            bb.origin = this->convertToNodeSpace(bb.origin);
            
            //underline
            auto layout = ui::Layout::create();
            layout->setContentSize(Size(bb.size.width, 2));
            layout->setPosition(bb.origin);
//                layout->setBackGroundColor(Color3B::BLACK);
//                layout->setBackGroundColorType(BackGroundColorType::SOLID);
            this->addChild(layout, INT_MAX / 2);
//            layout->setVisible(false);
            
            auto dot_number = int(bb.size.width / 15);
            for (auto i = 0; i < dot_number; ++i) {
                auto dot = ui::Layout::create();
                dot->setContentSize(Size(10, 2));
                dot->setPosition(Point(i * 15, 0));
                dot->setBackGroundColor(_info.normal_color_3b);
//                dot->setBackGroundColorOpacity(0x44);
                dot->setBackGroundColorType(BackGroundColorType::SOLID);
                dot->setOpacity(0);
                layout->addChild(dot);
                
                dot->runAction(Sequence::create(DelayTime::create(0.1 * counter++), FadeIn::create(0.25), FadeTo::create(0.25, 0x77), NULL));
            }
            
            auto last_dot = ui::Layout::create();
            last_dot->setContentSize(Size(bb.size.width - 15 * dot_number, 2));
            last_dot->setPosition(Point(dot_number * 15, 0));
            last_dot->setBackGroundColor(_info.normal_color_3b);
            last_dot->setBackGroundColorType(BackGroundColorType::SOLID);
            last_dot->setOpacity(0);
            layout->addChild(last_dot);
            last_dot->runAction(Sequence::create(DelayTime::create(0.1 * counter++), FadeIn::create(0.25), FadeTo::create(0.25, 0x77), NULL));
            if (last_dot->getContentSize().width < 2) {
                last_dot->setVisible(false);
            }
        }
    }
}

void MSBookTextInline::onClick(Ref * sender) {
    if (this->getChildByName("translation")) {
        clearTranslation();
    }
    
    auto touch_position = this->getTouchEndPosition();
    //        CCLOG("rich text click at %f %f", touch_position.x, touch_position.y);
    for (auto i = 0; i < (int) _element_renders.size(); ++i) {
        auto bb = _element_renders[i].first->getBoundingBox();
        bb.origin = _element_renders[i].first->getParent()->convertToWorldSpace(bb.origin);
        if (bb.containsPoint(touch_position)) {
            auto idx = _element_renders[i].second;
            CCLOG("click element: %s", idx.text.c_str());
            
            if (idx.type == ElementType::TRANSLATED) {
                showTranslated(_element_renders[i].first, _element_renders[i].second);
            }
            if (idx.type == ElementType::IMAGE) {
                showImage(_element_renders[i].first, _element_renders[i].second);
            }
        }
    }
}

void MSBookTextInline::clearTranslation() {
    this->removeChildByName("translation");
    this->removeChildByName("translation_overlay");
    cocos2d::experimental::AudioEngine::stopAll();
    this->stopActionsByFlags(TAG_READ_TRANSLATION);
}

void MSBookTextInline::showTranslated(Node * n, ElementInfo info) {
    auto node = Node::create();
    this->addChild(node, INT_MAX / 2 + 1);
    node->setName("translation");
    
    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize() * 2);
    overlay->setAnchorPoint(Point::ANCHOR_MIDDLE);
    overlay->setTouchEnabled(true);
    overlay->setSwallowTouches(false);
    overlay->setPropagateTouchEvents(true);
    overlay->addClickEventListener([this](Ref * sender) {
        this->clearTranslation();
    });
    this->addChild(overlay, -1);
    overlay->setName("translation_overlay");
    
    auto bb = Rect::ZERO;
    for (auto r : _element_renders) {
        if (info == r.second) {
            if (bb.equals(Rect::ZERO)) {
                bb = r.first->getBoundingBox();
            } else {
                bb = bb.unionWithRect(r.first->getBoundingBox());
            }
        }
    }
    
    bb.origin = n->getParent()->convertToWorldSpace(bb.origin);
    bb.origin = this->convertToNodeSpace(bb.origin);
//    CCLOG("bounding box of translated text: (%f, %f), (%f, %f)", bb.origin.x, bb.origin.y, bb.size.width, bb.size.height);
    
    //underline
    auto layout = ui::Layout::create();
    layout->setContentSize(Size(bb.size.width, 2));
    layout->setPosition(bb.origin);
//    layout->setBackGroundColor(Color3B::BLACK);
//    layout->setBackGroundColorType(BackGroundColorType::SOLID);
    node->addChild(layout, INT_MAX / 2);
    layout->setVisible(false);
    
    auto dot_number = int(bb.size.width / 10);
    for (auto i = 1; i < dot_number; ++i) {
        auto dot = ui::Layout::create();
        dot->setContentSize(Size(4, 2));
        dot->setPosition(Point(i * 10, 0));
        dot->setBackGroundColor(Color3B::BLACK);
        dot->setBackGroundColorType(BackGroundColorType::SOLID);
        layout->addChild(dot);
    }
    
    //translate text
    auto translate_text = ui::Text::create(info.translation, "Vollkorn-Regular.ttf", 25);
    translate_text->setTextColor(Color4B(_info.normal_color_3b));
    translate_text->setAnchorPoint(Point::ANCHOR_MIDDLE);
    mj::helper::setTextWidth(translate_text, 350);
    
    auto tt_bb = Rect::ZERO;
    for (auto i = 0; i < StringUtils::getCharacterCountInUTF8String(info.translation); i++) {
        auto letter = translate_text->getLetter(i);
        if (letter) {
            if (tt_bb.equals(Rect::ZERO)) {
                tt_bb = letter->getBoundingBox();
            } else {
                tt_bb = tt_bb.unionWithRect(letter->getBoundingBox());
            }
        }
    }
    
    //background
    auto background = ui::ImageView::create("mjstory/new_ui/inline_text_background.png");
    background->setScale9Enabled(true);
    background->setCapInsets(Rect(30, 30, 20, 20));
    background->setContentSize(tt_bb.size + Size(60, 60));
    background->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);;
    background->setPosition(Point(bb.getMidX(), bb.getMaxY()));
    translate_text->setContentSize(Size(tt_bb.size.width + 1, translate_text->getContentSize().height));
    translate_text->setPosition(background->getContentSize() / 2);
    background->addChild(translate_text);
    node->addChild(background);
    
    background->setTouchEnabled(true);
    background->addClickEventListener([](Ref * sender) {
        
    });
    
    auto backgroud_arrow = ui::ImageView::create("mjstory/new_ui/inline_text_background_arrow.png");
    backgroud_arrow->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
    backgroud_arrow->setPosition(background->getPosition());
    node->addChild(backgroud_arrow);
    
    auto bg_bb = background->getBoundingBox();
    bg_bb.origin = this->convertToWorldSpace(bg_bb.origin);
    if (bg_bb.origin.x < 0) {
        background->setPosition(background->getPosition() + Point(- bg_bb.origin.x, 0));
    }
    if (bg_bb.getMaxX() > Director::getInstance()->getVisibleSize().width) {
        background->setPosition(background->getPosition() + Point(- bg_bb.getMaxX() + Director::getInstance()->getVisibleSize().width, 0));
    }
    
    auto audio_id = experimental::AudioEngine::play2d(info.audio);
    if (!info.translation_audio.empty()) {
        experimental::AudioEngine::setFinishCallback(audio_id, [this, info](int, std::string) {
            auto action_read_translation = Sequence::create(DelayTime::create(1), CallFunc::create([this, info] {
                experimental::AudioEngine::play2d(info.translation_audio);
            }), NULL);
            action_read_translation->setFlags(TAG_READ_TRANSLATION);
            this->runAction(action_read_translation);
        });
    }
}

void MSBookTextInline::showImage(Node * n, ElementInfo e) {
    if(CONFIG_MANAGER->isVietnameseStories()){
        n->runAction(Sequence::create( Spawn::create(ScaleTo::create(0.2, 1.2), CallFunc::create([=]{
            experimental::AudioEngine::play2d(e.audio);
        }), nullptr), DelayTime::create(0.2), ScaleTo::create(0.2, 1.0), NULL));
    }else{
        if (!n->getChildByName("text")) {
            auto text = ui::Text::create(e.text, _info.fontname, _info.fontsize * 0.95);
            text->setAnchorPoint(Point::ANCHOR_MIDDLE);
            text->setPosition(Point(n->getContentSize().width / 2, -25));
            text->setTextColor(Color4B(_info.normal_color_3b));
            text->setOpacity(0);
            text->setName("text");
            n->addChild(text);
            n->runAction(
                         Sequence::create(
                                          Spawn::create(
                                                        ScaleTo::create(0.25, 0.75),
                                                        MoveBy::create(0.25, Vec2(0, n->getContentSize().height * 0.25 / 2)),
                                                        nullptr),
                                          CallFunc::create([text] {
                text->runAction(FadeIn::create(0.25));
            }),
                                          NULL));
            
            experimental::AudioEngine::play2d(e.audio);
        } else {
            auto text = n->getChildByName("text");
            text->runAction(Sequence::create(FadeOut::create(0.25), RemoveSelf::create(), CallFunc::create([n] {
                n->runAction(Spawn::create(
                                           ScaleTo::create(0.25, 1),
                                           MoveBy::create(0.25, Vec2(0, -n->getContentSize().height * 0.25 / 2)),
                                           nullptr));
            }), NULL));
        }
    }

}

static bool intersect(int a, int b, int m, int n) {
    return !((a > n) || (m > b));
}

void MSBookTextInline::highlight(int start_idx, int stop_idx) {
    CCLOG("attemp to highlight %d", start_idx);
    resetHighlight();
    for (auto p : _element_renders) {
        auto e_start = p.second.start_sync_index;
        auto e_stop = e_start + (int)StringUtils::getCharacterCountInUTF8String(p.second.text);
        
        if (intersect(e_start, e_stop, start_idx, stop_idx)) {
            CCLOG("highlight element %s at index %d", p.second.text.c_str(), p.second.start_sync_index);
            if (p.second.type != ElementType::IMAGE) {
                if (mj::helper::isContainAlnum(p.second.text)) {
                    auto node = reinterpret_cast<ui::Text *>(p.first);
                    node->setTextColor(Color4B(_info.highlight_color_3b));
                }
            } else {
                auto image_node = reinterpret_cast<ui::ImageView *>(p.first);
                image_node->setScale(1.2);
            }
            
            if (stop_idx <= e_stop) {
                break;
            }
        }
    }
}

void MSBookTextInline::resetHighlight() {
    for (auto p : _element_renders) {
        if (p.second.type == ElementType::IMAGE) {
            p.first->setScale(1);
        } else {
            reinterpret_cast<ui::Text *>(p.first)->setTextColor(Color4B(_info.normal_color_3b));
        }
    }
}
