
#include "MSBookText.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "NativeBridge.h"
#include "HSLib.h"
#include "MJPlatformConfig.h"
#include "MSScreenRecorder.h"
#include "MSAudioDelay.h"
#include "MSEventLogger.h"
#include "MSBookTextInline.h"

INITIALIZE_READER(MSBookText);

USING_NS_CC;

#define CSB_NAME "csb/book_page/MSBookText.csb"

std::once_flag MSBookText_reader;

MSBookText* MSBookText::createBookText(std::vector<mj::BOOK_TEXT_INFO> list_text, std::vector<mj::BOOK_AUDIO_INFO> list_audio, std::string box_type) {
	std::call_once(MSBookText_reader, [] {
		REGISTER_CSB_READER(MSBookText);
	});
	auto p = static_cast<MSBookText*>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
	if (p) {
		p->didLoadFromCSB(list_text, list_audio, box_type);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void MSBookText::didLoadFromCSB(std::vector<mj::BOOK_TEXT_INFO> list_text, std::vector<mj::BOOK_AUDIO_INFO> list_audio, std::string box_type) {
//    this->setClippingEnabled(true);
    _list_text_info = list_text;
    _list_audio_info = list_audio;
    _box_type = box_type;
}

void MSBookText::onEnter() {
    cocos2d::ui::Layout::onEnter();
    addBoxText();
}

void MSBookText::addBoxText() {

    int size_boxtext = 0;
    
    if (_box_type == TYPE_BOX_0) {
        auto layout = Layout::create();
        layout->setBackGroundColor(Color3B::WHITE);
        layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layout->setContentSize(cocos2d::Size(XHSLib->getSizeScreen().width/2, XHSLib->getSizeScreen().height));
        layout->setAnchorPoint(Vec2(0, 0));
        layout->setPosition(Vec2(0,0));
        this->addChild(layout);
    }
    
    for (int i = 0; i < (int)_list_text_info.size(); i++) {
        auto text_info = _list_text_info[i];
//        CCLOG("font = %s", XHSLib->fontNameStory.c_str());
        auto box_label = Text::create(StringUtils::format("%s", trimContentText(text_info.text).c_str()), XHSLib->fontNameStory, text_info.fontsize);
        this->addChild(box_label, 10+i);
        box_label->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        box_label->setTextHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        cocos2d::Rect rect;
        Vec2 _pos_label;
        if (_box_type == TYPE_BOX_0) {
            auto size_box = XHSLib->getSizeScreen().width/2;
            rect = cocos2d::Rect(size_box*0.065, 0, size_box*0.9, XHSLib->getSizeScreen().height);
            _pos_label= Vec2(rect.origin.x + rect.size.width*0.5, rect.origin.y + rect.size.height*0.5);
            
            auto notch_height = mj::PlatformConfig::getInstance().getNotchHeight();
            if (notch_height > 0) {
                _pos_label.x += notch_height/2;
                rect = cocos2d::Rect(size_box*0.065, 0, size_box*0.9  - notch_height/2, XHSLib->getSizeScreen().height);
            }

        }else {
            rect = cocos2d::RectFromString(text_info.boundingbox);
            _pos_label= Vec2(rect.origin.x + rect.size.width*0.5 + (XHSLib->getSizeScreen().width - 1024)/2, rect.origin.y + rect.size.height*0.5);
        }
        
        box_label->setTextAreaSize(rect.size);
        box_label->setPosition(_pos_label);
        auto _color = RectFromString(text_info.normal_color);
        box_label->setColor(Color3B(_color.origin.x, _color.origin.y, _color.size.width));
        box_label->setOpacity(0);
        box_label->setVisible(false);
        
        list_normal_color.push_back(Color3B(_color.origin.x, _color.origin.y, _color.size.width));
        _color = RectFromString(text_info.highlight_color);
        list_highlight_color.push_back(Color3B(_color.origin.x, _color.origin.y, _color.size.width));
        
        auto layout = Layout::create();
        layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        layout->setBackGroundColor(Color3B::ORANGE);
        layout->setContentSize(box_label->getContentSize());
        layout->setOpacity(100);
//        box_label->addChild(layout);
        
        list_box_text.push_back(box_label);
        size_boxtext += NativeBridge::utf8LengthOfString(trimContentText(text_info.text));
        list_size_boxtext.push_back(size_boxtext);
        
        if (!_list_text_info[i].inline_images.empty() || !_list_text_info[i].inline_translations.empty()) {
            auto info = _list_text_info[i];
            info.fontname = XHSLib->fontNameStory;
            _inline_text = MSBookTextInline::create(info);
            _inline_text->setPosition(_pos_label);
            _inline_text->setAnchorPoint(Point::ANCHOR_MIDDLE);
            this->addChild(_inline_text);
            _inline_text->repositioning();
        }
    }
    showText();
//    this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
//        this->startHighlightText();
//    }), NULL));
}

void MSBookText::resetBox() {
    
    MS_LOGGER.logEvent("MSBookText_resetBox", {});

    for (auto bt : list_box_text) {
        bt->setVisible(false);
    }
    if (!list_box_text.empty()) {
        list_box_text.front()->setVisible(true);
        list_box_text.front()->setOpacity(0xff);
    }
}

void MSBookText::startHighlightText() {

    MS_LOGGER.logEvent("MSBookText_startHighlightText", {});
    
    if (list_box_text.empty()) {
        onDoneHighlightTextBox();
        return;
    }
    
    highlight_index = 0; boxtext_index = 0; letter_index = 0; index_text_show = 0;
    
    for (auto idx = 0; idx < (int)list_box_text.size(); ++idx) {
        list_box_text[idx]->setTextColor(Color4B::WHITE);
        list_box_text[idx]->setColor(list_normal_color[idx]);
    }

    if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
        if (!_list_audio_info[boxtext_index].list_sync.empty()) {
            list_sync = _list_audio_info[boxtext_index].list_sync;
        }
        ms::recorder::disableMicro();
//        audio_mc_id = AUDIO_ENGINE->playEffect(_list_audio_info[boxtext_index].path);
        if (!_list_audio_info[boxtext_index].path.empty()) {
		
            audio_mc_id = mj::helper::AudioDelay::play(_list_audio_info[boxtext_index].path, _list_audio_info[boxtext_index].delay_config);
        }
//        experimental::AudioEngine::setFinishCallback(audio_mc_id, [](int, std::string) {
//            ms::recorder::enableMicro();
//        });
        if (!list_sync.empty()) {
            if (_box_type == TYPE_BOX_2) {
                this->scheduleOnce([this](float dt) {
                    this->highlightActionType2();
                    }, list_sync.front().s / 1000.f, "delay_start_highlight");
            }
            else {
                this->scheduleOnce([this](float dt) {
                    this->highlightActionType1();
                    }, list_sync.front().s / 1000.f, "delay_start_highlight");
            }
        }
        
    } else {
        onDoneHighlightTextBox();
    }
	
    MS_LOGGER.logEvent("COMPLETE_MSBookText_startHighlightText", {
        {"story_play_type",cocos2d::Value(XHSLib->story_play_type)},
        {"box_type",cocos2d::Value(_box_type)},
        {"audio_info_path",cocos2d::Value(_list_audio_info[boxtext_index].path)},
    });
}

void MSBookText::highlightActionType1(){

    CRASH_AUTO_LOG;
	
    if (list_box_text.empty() || _list_audio_info.empty())
    {
        MS_LOGGER.logEvent("Fail_highlightActionType1", {});
    }
	
    list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
    if (highlight_index >= (int)list_sync.size()) {
        list_sync.clear();
        highlight_index = 0;
        if (boxtext_index >= (int) list_box_text.size()-1) {//done
            onDoneHighlightTextBox();
        }else {
            if (XHSLib->story_play_type == XHSLib->READTOME && ms::recorder::isRecording()) return;
            list_box_text[boxtext_index]->runAction(FadeOut::create(0.2f));
            boxtext_index++;
            list_sync = _list_audio_info[boxtext_index].list_sync;
            list_box_text[boxtext_index]->setVisible(true);
            list_box_text[boxtext_index]->runAction(Sequence::create(DelayTime::create(0.2f), FadeIn::create(0.2f), DelayTime::create(0.35f), CallFunc::create([=]{
                ms::recorder::disableMicro();
//                audio_mc_id = AUDIO_ENGINE->playEffect(_list_audio_info[boxtext_index].path);
                audio_mc_id = mj::helper::AudioDelay::play(_list_audio_info[boxtext_index].path, _list_audio_info[boxtext_index].delay_config);
//                experimental::AudioEngine::setFinishCallback(audio_mc_id, [](int, std::string) {
//                    ms::recorder::enableMicro();
//                });
                highlightActionType1();
            }), NULL));
        }
        return;
    }
    
    auto delay_highlight = 0.f;
    if (highlight_index == 0 && boxtext_index != 0) {
        delay_highlight = list_sync[0].s / 1000.f;
    }
    
    std::vector<cocos2d::Sprite *> list_letter;
    auto sync_info = list_sync[highlight_index];
    letter_index = sync_info.ts;
    
    CCLOG("--- old highlight element %s", sync_info.w.c_str());
    
    if (_inline_text) {

        this->_inline_text->highlight(sync_info.ts, sync_info.te);
    }
    
    for (int i = sync_info.ts; i <= sync_info.te; i++) {
        auto letter = list_box_text[boxtext_index]->getLetter(i);
        if (letter) {
            letter->setColor(list_normal_color[boxtext_index]);
            letter->runAction(Sequence::create(DelayTime::create(delay_highlight), CallFuncN::create([=](Node * n) {
                n->setColor(list_highlight_color[boxtext_index]);
            }), nullptr));
            list_letter.push_back(letter);
        }
        letter_index++;
    }

    highlight_index++;
    auto duration = (sync_info.e - sync_info.s)/1000.0f;
    
    this->runAction(Sequence::create(DelayTime::create(delay_highlight + duration), CallFunc::create([=]{
        for (int i = 0; i < (int) list_letter.size(); i++) {
            list_letter[i]->setColor(list_normal_color[boxtext_index]);
        }
        if (_inline_text) {
            this->_inline_text->resetHighlight();
        }
    }), NULL));
    
    if (highlight_index < (int)list_sync.size()) {
        duration = (list_sync[highlight_index].s - sync_info.s)/1000.0f;
    }
    this->runAction(Sequence::create(DelayTime::create(delay_highlight + duration), CallFunc::create([=]{
        highlightActionType1();
    }), NULL));
}

void MSBookText::highlightActionType2(){

    MS_LOGGER.logEvent("MSBookText_highlightActionType2", {});

    if (highlight_index >= (int)list_sync.size() || list_sync.empty() || list_box_text.empty()) {
        onDoneHighlightTextBox();
        return;
    }
    std::vector<cocos2d::Sprite *> list_letter;
    auto sync_info = list_sync[highlight_index];
    letter_index = sync_info.ts;
    for (int i = sync_info.ts; i <= sync_info.te; i++) {
        auto index_letter_convert = getIndexLetterConvert();
        if (boxtext_index >= (int)list_box_text.size()) continue;
        auto letter = list_box_text[boxtext_index]->getLetter(index_letter_convert);
        if (letter) {
            letter->setColor(list_highlight_color[boxtext_index]);
            list_letter.push_back(letter);
        }
        letter_index++;
    }
    
    
    if (_inline_text) {
        this->_inline_text->highlight(sync_info.ts, sync_info.te);
    }
    
    highlight_index++;
    auto duration = (sync_info.e - sync_info.s)/1000.0f;
    this->runAction(Sequence::create(DelayTime::create(duration), CallFunc::create([=]{
        if(boxtext_index < list_box_text.size() && boxtext_index < list_normal_color.size())
        {
            list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
        }

        if (boxtext_index > 0 && (boxtext_index-1) < list_box_text.size() && (boxtext_index-1) < list_normal_color.size()) {
            list_box_text[boxtext_index-1]->setColor(list_normal_color[boxtext_index-1]);
        }

        for (int i = 0; i < (int) list_letter.size(); i++) {
            if(boxtext_index < list_normal_color.size())
            {
                list_letter[i]->setColor(list_normal_color[boxtext_index]);
            }
        }
        if (_inline_text) {
            _inline_text->resetHighlight();
        }
    }), NULL));
    
    if (highlight_index < (int)list_sync.size()) {
        if(highlight_index < list_sync.size())
        {
            duration = (list_sync[highlight_index].s - sync_info.s)/1000.0f;
        }
    }
    this->runAction(Sequence::create(DelayTime::create(duration), CallFunc::create([=]{
        highlightActionType2();
    }), NULL));
    MS_LOGGER.logEvent("COMPLETE_highlightActionType2", {});
}

int MSBookText::getIndexLetterConvert(){
    
    CRASH_AUTO_LOG;

    int index_letter_convert = letter_index;

    if (boxtext_index < list_size_boxtext.size())
    {
        if (letter_index > list_size_boxtext[boxtext_index] + boxtext_index) {
            boxtext_index++;
        }
    }
    
    if (boxtext_index > 0 && (boxtext_index-1) < list_size_boxtext.size()) {
        index_letter_convert = letter_index - list_size_boxtext[boxtext_index-1] - boxtext_index;
    }

    return index_letter_convert;
}

bool MSBookText::shouldBoxTextVisible(int idx) {

    CRASH_AUTO_LOG;

    if (idx >= _list_text_info.size()) { // out of index
        return false;
    }
    else {
        if (!_list_text_info.empty() && idx >= 0)
        {
            return !_list_text_info.empty() && _list_text_info[idx].inline_translations.empty() && _list_text_info[idx].inline_images.empty();
        }
        else {
            return false;
        }
    }
}

void MSBookText::showText() {
    if (list_box_text.empty()) {
        return;
    }
    auto duration = 0.2f;
    if (_box_type == TYPE_BOX_2) {
        for (int i = 0; i < (int)_list_text_info.size(); i++) {
            if (_list_text_info[i].end > 0.0f) {
                auto duration_show = (_list_text_info[i].end - _list_text_info[i].start)/1000.0f;
                list_box_text[i]->runAction(Sequence::create(DelayTime::create(_list_text_info[i].start/1000.0f), CallFunc::create([=]{
                    list_box_text[i]->setVisible(shouldBoxTextVisible(i));
                }), FadeIn::create(duration), DelayTime::create(duration_show), FadeOut::create(0.2f), NULL));
            }else{
                list_box_text[i]->setVisible(shouldBoxTextVisible(i));
                list_box_text[i]->runAction(Sequence::create(DelayTime::create(_list_text_info[i].start/1000.0f), FadeIn::create(duration), NULL));
            }
        }
    }else {
        list_box_text[0]->setVisible(shouldBoxTextVisible(0));
        list_box_text[0]->runAction(FadeIn::create(duration));
    }
}

void MSBookText::onDoneHighlightTextBox(){
   
    CRASH_AUTO_LOG;

    if (is_done_highlight) return;
    is_done_highlight = true;
    if (_delegate) {
        _delegate->onDoneHighlightTextMC();
    }
    if (_inline_text) {
        _inline_text->enableInteraction();
    }
}

bool MSBookText::hasNextBoxText() {
    if (_box_type == TYPE_BOX_2 || (int)_list_audio_info.size() <= 1 || boxtext_index >= ((int)list_box_text.size()-1)) return false;
    return true;
}

bool MSBookText::hasPrevBoxText() {
    if (_box_type == TYPE_BOX_2 || (int)_list_audio_info.size() <= 1 || boxtext_index < 1) return false;
    return true;
}

bool MSBookText::onNextBoxText(){

    CRASH_AUTO_LOG;

    stopAllActionAndSound();
    if (_box_type == TYPE_BOX_2 || (int)_list_audio_info.size() <= 1 || boxtext_index >= ((int)list_box_text.size()-1)) return false;
    list_box_text[boxtext_index]->setVisible(false);
    list_box_text[boxtext_index]->setOpacity(0);
    list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
    this->stopAllActions();
    this->resetTextColor();
    boxtext_index++;
    list_box_text[boxtext_index]->setVisible(true);
    list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
    list_box_text[boxtext_index]->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(0.2f), CallFunc::create([=]{
        if (XHSLib->story_play_type != XHSLib->READITMYSELF && !is_done_highlight) {
            highlight_index = 0;
            list_sync = _list_audio_info[boxtext_index].list_sync;
            ms::recorder::disableMicro();
//            audio_mc_id = AUDIO_ENGINE->playEffect(_list_audio_info[boxtext_index].path);
            audio_mc_id = mj::helper::AudioDelay::play(_list_audio_info[boxtext_index].path, _list_audio_info[boxtext_index].delay_config);
//            experimental::AudioEngine::setFinishCallback(audio_mc_id, [](int, std::string) {
//                ms::recorder::enableMicro();
//            });
            this->highlightActionType1();
        }
    }), NULL));
    return true;
    
}
bool MSBookText::onBackBoxText() {

    CRASH_AUTO_LOG;

    if (_box_type == TYPE_BOX_2 || (int)_list_audio_info.size() <= 1 || boxtext_index < 1) return false;
    stopAllActionAndSound();
    list_box_text[boxtext_index]->setVisible(false);
    list_box_text[boxtext_index]->setOpacity(0);
    list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
    this->stopAllActions();
    this->resetTextColor();
    boxtext_index--;
    list_box_text[boxtext_index]->setVisible(true);
    list_box_text[boxtext_index]->setColor(list_normal_color[boxtext_index]);
    list_box_text[boxtext_index]->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(0.2f), CallFunc::create([=]{
        if (XHSLib->story_play_type != XHSLib->READITMYSELF && !is_done_highlight) {
            highlight_index = 0;
            list_sync = _list_audio_info[boxtext_index].list_sync;
            ms::recorder::disableMicro();
//            audio_mc_id = AUDIO_ENGINE->playEffect(_list_audio_info[boxtext_index].path);
            audio_mc_id = mj::helper::AudioDelay::play(_list_audio_info[boxtext_index].path, _list_audio_info[boxtext_index].delay_config);
//            experimental::AudioEngine::setFinishCallback(audio_mc_id, [](int, std::string) {
//                ms::recorder::enableMicro();
//            });
            this->highlightActionType1();
        }
    }), NULL));
    return true;
}

std::string MSBookText::trimContentText(std::string content){
    auto re_string = std::string("");
    //    CCLOG("before trim = %s", content.c_str());
    re_string = replaceString(content, " <br> ", "\n");
    re_string = replaceString(re_string, "<br>", "\n");
    re_string = replaceString(re_string, " <br>", "\n");
    re_string = replaceString(re_string, " <br> ", "\n");
    //    CCLOG("after trim = %s", re_string.c_str());
    return re_string;
}

std::string MSBookText::replaceString(std::string subject, const std::string& search,
                                     const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

void MSBookText::stopAllActionAndSound() {

    CRASH_AUTO_LOG;

    removeHSLabelEffect();
    AUDIO_ENGINE->stopEffect(audio_mc_id);
    this->stopAllActions();
    this->unscheduleUpdate();
    this->unscheduleAllCallbacks();
}

#pragma mark - TEXT_OBJECT_EFFECT
void MSBookText::playTextMCEffect(std::string _str_find, float duration){

    CRASH_AUTO_LOG;

    if (_box_type == TYPE_BOX_1) {//có nhiều text trong 1 box =>chỉ có 1 box.
        //lấy label text cuối cùng để search.
//        displayEffectTextMC(list_box_text[(int)list_box_text.size()-1], _str_find, duration);
        for (int i = 0; i < (int) list_box_text.size(); i ++) {
            if (list_box_text[i]->isVisible() && list_box_text[i]->getOpacity() != 0) {
                displayEffectTextMC(list_box_text[i], _str_find, duration);
            }
        }
        
    }else{
        //có 2 box trở lên => phải search trong tất cả các box.
        for (int i = 0; i < (int) list_box_text.size(); i ++) {
            if (list_box_text[i]->isVisible()) {
                displayEffectTextMC(list_box_text[i], _str_find, duration);
            }
        }
    }
}

void MSBookText::displayEffectTextMC(Text *_label, std::string _str_find, float duration){
   
    CRASH_AUTO_LOG;

    auto arr_post = NativeBridge::findStringOnString(NativeBridge::string_towlower(_label->getString()), NativeBridge::string_towlower(_str_find));
    if ((int)arr_post.size() == 0) return;
    //    _label->setOpacity(60);
    for (int i = 0; i < (int) arr_post.size(); i ++) {
        //        CCLOG("pos: %d | %s", arr_post[i], _str_find.c_str());
        auto _start = arr_post[i];
        auto _end = _start + NativeBridge::utf8LengthOfString(_str_find);
        std::vector<Sprite *> _letters;
        for(int j = _start; j < _end ; j++) {
            auto letter = _label->getLetter(j);
            if(letter != NULL) {
//                letter->stopAllActions();
                letter->setOpacity(0);
                //CCLOG("letter pos = %f", letter->getPositionX());
                _letters.push_back(letter);
            }
            letter = nullptr;
        }
        
        if ((int)_letters.size() < 1) continue;
        auto rect = getRectOfLetters(_letters, _str_find);
        CCLOG("bounding box: %f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);

        //        CCLOG("x = %f| y = %f| w = %f| H = %f | LineH = %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, _label->getLineHeight());
        auto posX = rect.origin.x + rect.size.width/2;
        float posY = rect.origin.y + rect.size.height/2;
        
        auto tag_delay = _start * 1000000 + _end;
        auto check_label = _label->getChildByTag(tag_delay);
        
        addTagHSLabelToArr(tag_delay);
        if (!check_label) {
            auto hoisung = ui::Text::create(NativeBridge::hsStringSub(_label->getString(), arr_post[i], NativeBridge::utf8LengthOfString(_str_find)), _label->getFontName(), _label->getFontSize());
            _label->addChild(hoisung);
            hoisung->setTag(tag_delay);
            
            hoisung->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            hoisung->setPosition(rect.origin);
            
            CCLOG("bounding box: %f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
            rect = mj::helper::getTextBoundingBox(hoisung);
            hoisung->setPosition(hoisung->getPosition() - rect.origin);
            CCLOG("bounding box: %f %f %f %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
            
            if (boxtext_index < (int)list_highlight_color.size()){
                hoisung->setTextColor(Color4B(list_highlight_color[boxtext_index]));
            }else hoisung->setTextColor(Color4B(list_highlight_color[0]));
            
            check_label = hoisung;
            
            auto movefirst = MoveBy::create(0.05, Vec2(0, -5));
            auto move1 = MoveBy::create(0.25f, Vec2(0,10));
            auto move2 = MoveBy::create(0.25f, Vec2(0,-10));
            
            hoisung->runAction(Sequence::create(movefirst, Repeat::create(Sequence::create(move1, move2, NULL), 10), nullptr));
            
            hoisung->scheduleOnce([=](float) {
                auto check = false;
                auto count_check = 0;
                auto check_index = 0;
                for (int check_id = 0; check_id < (int)_listEffectID.size(); check_id++) {
                    if (tag_delay == _listEffectID[check_id]) {
                        if (count_check == 0) {
                            check_index = check_id;
                        }
                        count_check++;
                    }
                }
                if (count_check == 1) {
                    auto check_label = _label->getChildByTag(_start * 1000000 + _end);
                    if (check_label) {
                        check_label->removeFromParent();
                    }
                }
                
                if (count_check > 1) {
                    check = true;
                }
                if (check_index < _listEffectID.size())
                    _listEffectID.erase(_listEffectID.begin() + check_index);
                
                if (!check) {
                    
                    for(int j = _start; j < _end; j++) {
                        auto letter = _label->getLetter(j);
                        if(letter != NULL) {
                            letter->setColor(list_normal_color[0]);
                            letter->setOpacity(255);
                            letter = nullptr;
                        }
                    }
                    if ((int)_listEffectID.size() == 0) {
                        _label->setOpacity(255);
                    }
                }
                
            }, duration, StringUtils::format("effect_textmc_%d", tag_delay));
            
            for (auto tag_fx : _listEffectID) {
                auto start_fx = tag_fx / 1000000;
                auto end_fx = tag_fx % 1000000;
                
                auto mini = std::max(_start, start_fx);
                auto maxi = std::min(_end, end_fx);
                
                if (start_fx == _start && end_fx == _end) {
                    continue;
                }
                
                if (mini < maxi) {
                    //conflict with other fx, stop other fx
                    this->unschedule(StringUtils::format("effect_textmc_%d", tag_fx));
                    _listEffectID.erase(std::find(_listEffectID.begin(), _listEffectID.end(), tag_fx));
                    _label->removeChildByTag(tag_fx);
                    for (auto c = start_fx; c < end_fx; ++c) {
                        if (_start <= c && c < _end) {
                            continue;
                        }
                        
                        auto letter = _label->getLetter(c);
                        if(letter != NULL) {
                            letter->setColor(list_normal_color[0]);
                            letter->setOpacity(255);
                            letter = nullptr;
                        }
                    }
                }
            }
        }
        
        //runing text fx
        if (std::find(_listEffectID.begin(), _listEffectID.end(), tag_delay) == _listEffectID.end()) {
            _listEffectID.push_back(tag_delay);
        } else {
            auto fx = _label->getChildByTag(tag_delay);
            if (fx) {
                fx->scheduleOnce([](float){}, duration, StringUtils::format("effect_textmc_%d", tag_delay));
            }
        }
    }
}


int MSBookText::getNumLineIndexOfString(float numLineH, float _height){
   
    CRASH_AUTO_LOG;

    auto maxNumLine = 6;
    for (int i = 1; i < maxNumLine; i++){
        if (_height > i*numLineH && _height < (i+1)*numLineH) {
            //            CCLOG("numLineH = %f, _height = %f | i = %d", numLineH, _height, i);
            return i;
        }
    }
    
    return 0;
}

cocos2d::Rect MSBookText::getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find){

    CRASH_AUTO_LOG;

    if (_letter_arr.empty()) {
        return Rect();
    }
    
    auto rect = (_letter_arr[0] != nullptr ? _letter_arr[0]->getBoundingBox(): Rect());
    for (int i = 1; i < (int) _letter_arr.size(); i++) {
        rect = rect.unionWithRect(_letter_arr[i]->getBoundingBox());
    }
    return rect;
}

void MSBookText::addTagHSLabelToArr(int tag){
    _listTagHSLabel.push_back(tag);
}

void MSBookText::removeHSLabelEffect(){

    CRASH_AUTO_LOG;

    if (_box_type != TYPE_BOX_1) return;
    if ((int) _listTagHSLabel.size() <= 0) return;
    auto text_box = list_box_text[(int)list_box_text.size()-1];
    list_box_text[(int)list_box_text.size()-1]->removeAllChildren();

    for(int j = 0; j < NativeBridge::utf8LengthOfString(text_box->getString()); j++) {
        auto letter = text_box->getLetter(j);
        if(letter != NULL) {
            letter->setOpacity(255);
            letter = nullptr;
        }
    }
    text_box->setOpacity(255);
    text_box->setColor(list_normal_color[0]);
    _listTagHSLabel.clear();
    _listEffectID.clear();
}

void MSBookText::setDelegate(MSBookTextDelegate *delegate){
    _delegate = delegate;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MSBookText::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MSBookText::onRefresh, this);
    }
    return nullptr;
}

void MSBookText::onRefresh(cocos2d::Ref *sender) {
    this->removeFromParent();
}

void MSBookText::onExitTransitionDidStart(){
    
    CRASH_AUTO_LOG;

    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();

}

void MSBookText::resetTextColor(){

    CRASH_AUTO_LOG;

    auto lenght = NativeBridge::utf8LengthOfString(list_box_text[boxtext_index]->getString());
    
    for (int i = 0; i < lenght; i++){
        auto letter = list_box_text[boxtext_index]->getLetter(i);
        auto normal_color = list_normal_color[boxtext_index];
        if (letter){
            letter->setColor(normal_color);
        }
    }
}

