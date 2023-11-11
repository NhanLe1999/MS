#include "TextStory.h"
#include "ui/UIHelper.h"
#include "HSLib.h"
#include "TruyenTranhPage.h"
#include "NativeBridge.h"
#include "StoryData.h"
#include "MJFirebaseListener.h"
#include "MJPlatformConfig.h"
#include "MSScreenRecorder.h"
#include "MJHelper.h"
INITIALIZE_READER_INSTANCE(TextStoryReader);

USING_NS_CC;
using namespace ui;

void TextStory::didLoadFromCSB() {
    sizeTextStoryLayer = DIRECTOR->getVisibleSize().width*displayNVSize;
    bg = this->getChildByName<ImageView *>("bg");
    this->setContentSize(cocos2d::Size(XHSLib->wbackground, XHSLib->hbackground));
    ui::Helper::doLayout(this);
    this->setPosition(Vec2(0, 0));
    
    auto panel = Layout::create();
    if (_textMCObj.type_story == XHSLib->SIMPLE_TYPE) {
//        CCLOG("simple size = %f", sizeTextStoryLayer);
        panel->setContentSize(cocos2d::Size(sizeTextStoryLayer, this->getContentSize().height));
        panel->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        panel->setBackGroundColor(Color3B::WHITE);
        this->addChild(panel, 0);
    }
    
    Rect rectBox;
    if ((int)_textMCObj.textMC.size() == 0) {
        return;
    }
//    CCLOG("textMC = %s", JsonDump::getInstance()->VectorToString(_textMCObj.textMC).c_str());

    for (int i = 0; i < (int)_textMCObj.textMC.size(); i ++) {
        if (_textMCObj.textMC[i].isNull() || _textMCObj.textMC[i].getType() != Value::Type::VECTOR) {
            continue;
        }
        mapText = _textMCObj.textMC[i].asValueVector()[textIndex].asValueMap();
//        CCLOG("textMC = %s", JsonDump::getInstance()->MapToString(mapText).c_str());

        auto boxlabel = Label::createWithTTF("", "monkeystories_medium.ttf", 30);
        boxlabel->setHorizontalAlignment(cocos2d::TextHAlignment::LEFT);
        boxlabel->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
        if (_textMCObj.type_story == XHSLib->SIMPLE_TYPE) {
            boxlabel->setAnchorPoint(Vec2(0, 0.5));
            boxlabel->setPosition(Vec2(sizeTextStoryLayer*0.065, this->getContentSize().height*0.5));
            boxlabel->setWidth(sizeTextStoryLayer*0.9);
            
            auto notch_height = mj::PlatformConfig::getInstance().getNotchHeight();
            if (notch_height > 0) {
                boxlabel->setPositionX(boxlabel->getPositionX() + notch_height/2);
                boxlabel->setWidth(sizeTextStoryLayer*0.9 - notch_height/2);
            }
            
            
        } else {
//            CCLOG("chandoi: %s", mapText["boundingbox"].asString().c_str());
            rectBox = RectFromString(mapText["boundingbox"].asString().c_str());
            //rectBox = convertRectBox(rectBox);
            Vec2 _anchor, _pos;
            _anchor = Vec2(0.5, 0.5);
            _pos = Vec2(rectBox.origin.x + rectBox.size.width*0.5 + (XHSLib->getSizeScreen().width - 1024)/2, rectBox.origin.y + rectBox.size.height*0.5);
            boxlabel->setWidth(rectBox.size.width);
            boxlabel->setAnchorPoint(_anchor);
            boxlabel->setPosition(_pos);
        }
        
        this->addChild(boxlabel, 10003);
        
        boxlabel->setString(StringUtils::format("%s", trimContentText(mapText["text"].asString()).c_str()));
//        CCLOG("boxlabel = %s", boxlabel->getString().c_str());
//        CCLOG("chandoi = %f | %f | w = %f", boxlabel->getPositionX(), boxlabel->getPositionY(), boxlabel->getContentSize().width);
        setFontNameAndFontSize(boxlabel, mapText);
        _labelArr.push_back(boxlabel);
        
//        CCLOG("hstext %d = %s",i+1, _labelArr[i]->getString().c_str());
        AUDIO_ENGINE->preloadEffect(XHSLib->getFilePathStory(StringUtils::format("%s", mapText["bgsoundname"].asString().c_str())).c_str());
    }
    
    if ((int)_labelArr.size() > 0) {
        label = _labelArr[0];
        if (label->getStringNumLines() == 1) {
            label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
            isOneLine = true;
        }
    }
    
//    CCLOG("hstext check 1 = %s", label->getString().c_str());
}

Rect TextStory::convertRectBox(cocos2d::Rect _rectbox){
    _rectbox.origin.x = _rectbox.origin.x+ (XHSLib->getSizeScreen().width - 1024)/2;
    return _rectbox;
}

std::string TextStory::trimContentText(std::string content){
    auto re_string = std::string("");
//    CCLOG("before trim = %s", content.c_str());
    re_string = replaceString(content, " <br> ", "\n");
    re_string = replaceString(re_string, "<br>", "\n");
    re_string = replaceString(re_string, " <br>", "\n");
    re_string = replaceString(re_string, " <br> ", "\n");
//    CCLOG("after trim = %s", re_string.c_str());
    return re_string;
}

std::string TextStory::replaceString(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

#pragma mark - onEnter
void TextStory::onEnter() {
    Layout::onEnter();
}

void TextStory::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
//    auto letter = label->getLetter(0);
//    Vec2 v2 = label->convertToWorldSpace(letter->getPosition());
//    CCLOG("v2: %f, %f", v2.x, v2.y);
//    LayerColor *lcolor = LayerColor::create(Color4B(100, 100, 100, 100));
//    lcolor->setContentSize(Size(100, 30));
//    lcolor->setPosition(v2);
//    this->addChild(lcolor, 10000);

}
#pragma mark - onExit
void TextStory::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
    auto parent = (TruyenTranhPage *) this->getParent();
    if (!parent->isEndStory && isStartMCRead) {
        AUDIO_ENGINE->stopAllEffects();
    }
    parent = nullptr;
}

void TextStory::animShow(){
    
    CCLOG("fuck = %d", XHSLib->story_play_type);
        this->scheduleOnce([=](float dt){
            isStartMCRead = true;
            if (XHSLib->story_play_type != XHSLib->READITMYSELF && !_is_done_highlight) {
                runCauAction();
            }
        }, 1.0, StringUtils::format("runCauActionBox%d", boxIndex+1000));
    
//    if (isAnimRunning) return;
//    isAnimRunning = true;
//    this->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(0,0)),CallFunc::create([=]{
//        isAnimRunning = false;
//    }), NULL));
}

void TextStory::startPlaying() {
    boxIndex = 0;
    textIndex = 0;
    soundMCIndex = 0;

    if ((int)_textMCObj.textMC.size() > 0) {
        animShow();
    }
    if (XHSLib->story_play_type == XHSLib->READITMYSELF) {
        auto parent = (TruyenTranhPage*)this->getParent();
        parent->playGAFWhenMCDone();
        parent = nullptr;
    }
}

bool TextStory::hasNextBoxText() {
    return (_textMCObj.countBox1 > 1) && (textIndex < _textMCObj.countBox1 - 1);
}

bool TextStory::hasPrevBoxText() {
    return (_textMCObj.countBox1 > 1) && (textIndex > 0);
}

void TextStory::nextPage(){
    if ((_textMCObj.countBox1 > 1) && (textIndex < _textMCObj.countBox1 - 1)) {
//        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "nextboxtext", "true");
        textIndex++;
        AUDIO_ENGINE->stopEffect(soundMCIndex);
        this->unscheduleAllCallbacks();
        removeHSLabelEffect();
        mapText = _textMCObj.textMC[0].asValueVector()[textIndex].asValueMap();
        label->setColor(normal_color);
        label->setString(StringUtils::format("%s", trimContentText(mapText["text"].asString()).c_str()));
        setFontNameAndFontSize(label, mapText);
//        CCLOG("hstext %d = %s",textIndex, mapText["text"].asString().c_str());
        if (label->getStringNumLines() == 1 && isOneLine) {
            label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
        }
        removeHSLabelEffect();
        animShow();
        XHSLib->enable_keyboard = true;
        
    }else{
//        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "nextpage", "true");
        auto parent = (TruyenTranhPage*)this->getParent();
        parent->isreplacescene_enable = false;
        this->stopAllActions();
        this->unscheduleAllCallbacks();
        this->unscheduleUpdate();
        
        auto index_endpage = XHSLib->pageNumAll;
        if (CONFIG_MANAGER->isTestGame()) {
            index_endpage = 1;
        }
        if (XHSLib->pageIndex >= index_endpage) {//XHSLib->pageNumAll
            parent->isEndStory = true;
            parent->stopAllActionAndEffect();
        }
        XHSLib->pageIndex++;
        
        EventCustom e("mjstory.replace_page");
        int * edata = new int;
        *edata = 1;
        e.setUserData(edata);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
        CCLOG("Event 1");
//        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.replace_page");
//        auto home_scene = static_cast<Scene *>(CSLoader::createNode("csb/TruyenTranhPage.csb"));
//        DIRECTOR->replaceScene(TransitionSlideInR::create(0.20f, TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex)));//TransitionPageTurn
         parent = nullptr;
    }
}

void TextStory::backPage(){
    if ((_textMCObj.countBox1 > 1) && (textIndex > 0)) {
//        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "backboxtext", "true");
        textIndex--;
        removeHSLabelEffect();
        AUDIO_ENGINE->stopEffect(soundMCIndex);
        this->unscheduleAllCallbacks();
        mapText = _textMCObj.textMC[0].asValueVector()[textIndex].asValueMap();
        label->setString(StringUtils::format("%s", trimContentText(mapText["text"].asString()).c_str()));
        setFontNameAndFontSize(label, mapText);
        removeHSLabelEffect();
//        CCLOG("hstext %d = %s",textIndex, mapText["text"].asString().c_str());
        animShow();
        XHSLib->enable_keyboard = true;
    }else{
        if (XHSLib->pageIndex == 1) {
            return;
        }
//        mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "backpage", "true");
        this->stopAllActions();
        this->unscheduleAllCallbacks();
        this->unscheduleUpdate();
        
//        if (XHSLib->pageIndex == 1) {
//            return;
//        }
        auto parent = (TruyenTranhPage*)this->getParent();
        parent->isreplacescene_enable = false;
        parent = nullptr;
        XHSLib->pageIndex--;
        EventCustom e("mjstory.replace_page");
        int * edata = new int;
        *edata = 2;
        e.setUserData(edata);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
        CCLOG("Event 1");
//        auto home_scene = static_cast<Scene *>(CSLoader::createNode("csb/TruyenTranhPage.csb"));
//        DIRECTOR->replaceScene(TransitionSlideInL::create(0.25f, home_scene));
//        DIRECTOR->replaceScene(TransitionSlideInL::create(0.20f, TruyenTranhPage::createPage(XHSLib->story_id, XHSLib->lang_id, XHSLib->pageIndex)));//TransitionPageTurn

    }
}

void TextStory::runCauAction(){
    iStart = 0;
    iEnd = 0;
    sync_index = 0;
    label = _labelArr[boxIndex];
    mapText = _textMCObj.textMC[boxIndex].asValueVector()[textIndex].asValueMap();
    timeline.clear();
    if (mapText["timeline"].isNull() || mapText["timeline"].getType() != Value::Type::VECTOR) {
        playGAFWhenTextMCNull();
        return;
    }
    timeline = mapText["timeline"].asValueVector();
//    //hoitest:
//    timeline = FileUtils::getInstance()->getValueVectorFromFile("mjstory/hoideptrai.plist");
    
    if ((int)timeline.size() <=0) {
        playGAFWhenTextMCNull();
        return;
    }
//    CCLOG("timeline: %i | %f | %s", (int)timeline.size(), timeline[0].asValueMap()["s"].asFloat()/1000.0, StringUtils::format("sync_cau%d", cau_index+1).c_str());

//    CCLOG("audio = %s", cocos2d::FileUtils::getInstance()->getWritablePath().c_str());
    if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
        ms::recorder::disableMicro();
        soundMCIndex = AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(StringUtils::format("%s", mapText["bgsoundname"].asString().c_str())).c_str());
        experimental::AudioEngine::setFinishCallback(soundMCIndex, [](int, std::string) {
            ms::recorder::enableMicro();
        });
    }
    ValueMap map = timeline[0].asValueMap();
    auto delay0 = map["ts"].asInt()/1000.0f;
    this->scheduleOnce([=](float dt){
        highlightTextAction();
    }, delay0, StringUtils::format("start_highlight"));
}

void TextStory::highlightTextAction() {
//    CCLOG("sync_index: %i", sync_index);
    this->unscheduleAllCallbacks();
    if((timeline.size()) == sync_index) {
        label->setColor(normal_color);
        sync_index = 0;
        timeline.clear();
        if ((int)_textMCObj.textMC.size() > 1 && boxIndex < (int) _textMCObj.textMC.size()-1) {
            boxIndex++;
            setColorWhenDoneHighlightCharacter();
            this->scheduleOnce([=](float dt){
                runCauAction();
            }, 0.5, StringUtils::format("runCauActionBox%d", boxIndex+1000));
        }else{
            if (textIndex < _textMCObj.countBox1 - 1) {
                if (XHSLib->story_play_type == XHSLib->READTOME && ms::recorder::isRecording()) {
                    setColorWhenDoneHighlightCharacter();
                    return;
                }
                nextPage();
            }else{
                setColorWhenDoneHighlightCharacter();
                auto parent = (TruyenTranhPage*)this->getParent();
                //                parent->onStartEffectNV();
                _is_done_highlight = true;
                parent->playGAFWhenMCDone();
                parent = nullptr;
                //            if (XHSLib->story_play_type != XHSLib->AUTOPLAY) {
                //                auto parent = (TruyenTranhPage*)this->getParent();
                //                parent->onStartEffectNV();
                //                parent->playGAFWhenMCDone();
                //                parent = nullptr;
                //            }else{
                //                this->nextPage();
                //            }
            }
        }
        return;
    }
    setColorWhenDoneHighlightCharacter();
    ValueMap map = timeline[sync_index++].asValueMap();
    iStart = map["ts"].asInt();
    iEnd = map["te"].asInt();

    for(int i = iStart; i<= iEnd; i++) {
        auto letter = label->getLetter(i);
        if(letter != NULL) {
            letter->setColor(highlight_color);
        }
    }
//    CCLOG("iStart = %d | iEnd = %d", iStart, iEnd);
    duration = (map["e"].asFloat() - map["s"].asFloat())/1000.0f;
    this->runAction(Sequence::create(DelayTime::create(duration), CallFunc::create([=]{
        highlightTextAction();
    }), NULL));
//    this->scheduleOnce([=](float dt){
//        highlightTextAction();
//    }, duration, StringUtils::format("sync_index_%i", sync_index));
}

void TextStory::setColorWhenDoneHighlightCharacter(){
    for(int i = 0; i< NativeBridge::utf8LengthOfString(label->getString()); i++) {
        auto letter = label->getLetter(i);
        if(letter != NULL) {
            letter->setColor(normal_color);
        }
    }
}

void TextStory::animHide(){
    if (isAnimRunning) return;
    isAnimRunning = true;
    this->runAction(Sequence::create(MoveTo::create(0.5f, Vec2(-bg->getContentSize().width,0)),CallFunc::create([=]{
        isAnimRunning = false;
    }), NULL));
}

void TextStory::setFontNameAndFontSize(Label *hsLabel, cocos2d::ValueMap mapFont){
    auto fontName = mapFont["fontname"].asString();
    if (fontName == string("")) {
        fontName = XHSLib->fontNameStory;
    }
//    CCLOG("XHSLib->fontNameStory = %s", XHSLib->fontNameStory.c_str());
    //check file?
    
    if (fontSize < 20) {
        fontSize = 33;
    }
//    if (mapFont["normal_color"].asString() == string("")) {
//        normal_color = Color3B::BLACK;
//    }else{
//        Rect rect_normal = RectFromString(mapFont["normal_color"].asString());
//        normal_color = Color3B((int)rect_normal.origin.x, (int)rect_normal.origin.y , (int)rect_normal.size.width);
//    }
//    
//    auto rect_highlight_str = mapFont["highlight_color"].asString();
//    if (rect_highlight_str == "") {
//        highlight_color = Color3B::RED;
//    }else{
//        auto rect_highlight = RectFromString(rect_highlight_str);
//        highlight_color = Color3B((int)rect_highlight.origin.x, (int)rect_highlight.origin.y , (int)rect_highlight.size.width);
//    }

//    CCLOG("fontconfig: fontName = %s | fontSize = %d | r = %d", XHSLib->fontNameStory.c_str(), fontSize, normal_color.r);
    
    //    normal_color = XHSLib->normal_color;
    //    highlight_color = XHSLib->highlight_color;

    
    if (_textMCObj.type_story == XHSLib->SIMPLE_TYPE) {
        switch (hsLabel->getStringNumLines()) {
            case 1:
                fontSize = 57;
                break;
            case 2:
                fontSize = 50;
                break;
            case 3:
                fontSize = 40;
                break;
            default:
                fontSize = 35;
                break;
        }

    }
    hsLabel->setColor(normal_color);
    hsLabel->setTTFConfig(TTFConfig(fontName, fontSize));
}

void TextStory::onHide(cocos2d::Ref *sender){
    animHide();
}

void TextStory::playTextMCEffect(std::string _str_find, float duration){
    if (_textMCObj.textMC.size() <= 1) {//có nhiều text trong 1 box =>chỉ có 1 box.
        //lấy label text cuối cùng để search.
        displayEffectTextMC(label, _str_find, duration);
        
    }else{
        //có 2 box trở lên => phải search trong tất cả các box.
        for (int i = 0; i < (int) _labelArr.size(); i ++) {
            displayEffectTextMC(_labelArr[i], _str_find, duration);
        }
    }
}

void TextStory::displayEffectTextMC(Label *_label, std::string _str_find, float duration){
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
                letter->setOpacity(0);
                //CCLOG("letter pos = %f", letter->getPositionX());
                _letters.push_back(letter);
            }
            letter = nullptr;
        }
        auto rect = getRectOfLetters(_letters, _str_find);
//        CCLOG("x = %f| y = %f| w = %f| H = %f | LineH = %f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, _label->getLineHeight());
        auto posX = rect.origin.x + rect.size.width/2;
        float posY = rect.origin.y + rect.size.height/2;
//        if (_label->getStringNumLines() > 1) {
//            posY = rect.origin.y + rect.size.height/2;
//            auto lineH = _label->getLineHeight();
//            if (StoryData::getInstance()->isHdr) {
//                lineH = lineH/2;
//            }
//            auto line_index = getNumLineIndexOfString(lineH, posY);
//            posY = lineH*line_index + lineH/2;
//        }else{
//            posY = _label->getContentSize().height/2;
//        }
        auto tag_delay = _start * 1000000 + _end;
        auto check_label = _label->getChildByTag(tag_delay);
        
        addTagHSLabelToArr(tag_delay);
        if (!check_label) {
            auto hoisung = ui::Text::create(NativeBridge::hsStringSub(_label->getString(), arr_post[i], NativeBridge::utf8LengthOfString(_str_find)), _label->getTTFConfig().fontFilePath, _label->getTTFConfig().fontSize);
            _label->addChild(hoisung);
            hoisung->setTag(tag_delay);
            hoisung->setTextColor(Color4B(highlight_color));
            hoisung->setPosition(rect.origin);
            hoisung->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
            
            rect = mj::helper::getTextBoundingBox(hoisung);
            hoisung->setPosition(hoisung->getPosition() - rect.origin);
            
            check_label = hoisung;
//            CCLOG("hoisung = %f",hoisung->getPositionX());
            
            auto movefirst = MoveBy::create(0.05, Vec2(0, -5));
            auto move1 = MoveBy::create(0.25f, Vec2(0,10));
            auto move2 = MoveBy::create(0.25f, Vec2(0,-10));

//            hoisung->runAction(_scale1);
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
                        //                    check_label->runAction(Sequence::create(ScaleTo::create(0.09f, 1.0f), CallFunc::create([=]{
                        //
                        //                    }), NULL));
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
                            letter->setColor(normal_color);
                            letter->setOpacity(255);
                            letter = nullptr;
                        }
                    }
                    if ((int)_listEffectID.size() == 0) {
                        _label->setOpacity(255);
                    }
                    
                    //                this->scheduleOnce([=](float dt){
                    //                }, 0.0f, StringUtils::format("effect_textmc_%d_%d_1", tag_delay, count_delay_effect++));
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
                            letter->setColor(normal_color);
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
                //chỉ có tác dụng re-init lại timer để kết thúc highlight text
                fx->scheduleOnce([](float){}, duration, StringUtils::format("effect_textmc_%d", tag_delay));
            }
        }
    }
}


int TextStory::getNumLineIndexOfString(float numLineH, float _height){
    auto maxNumLine = 6;
    for (int i = 1; i < maxNumLine; i++){
        if (_height > i*numLineH && _height < (i+1)*numLineH) {
//            CCLOG("numLineH = %f, _height = %f | i = %d", numLineH, _height, i);
            return i;
        }
    }

    return 0;
}

cocos2d::Rect TextStory::getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find){
    auto rect = _letter_arr[0]->getBoundingBox();
    for (int i = 1; i < (int) _letter_arr.size(); i++) {
        rect = rect.unionWithRect(_letter_arr[i]->getBoundingBox());
    }
    return rect;
    
//    auto minX = 0, maxX = 0, minY = 0, maxY = 0;
//    minX = _letter_arr[0]->getPositionX() - _letter_arr[0]->getContentSize().width/2;
//    minY = _letter_arr[0]->getPositionY() - _letter_arr[0]->getContentSize().height/2;
//
//    maxX = _letter_arr[0]->getPositionX() + _letter_arr[0]->getContentSize().width/2;
//    maxY = _letter_arr[0]->getPositionY() + _letter_arr[0]->getContentSize().height/2;
//
//
//    for (int i = 1; i < (int)_letter_arr.size(); i++) {
//        if (NativeBridge::hsStringSub(str_find, i, 1) == std::string(" ")) {
//            continue;
//        }
//        auto _posX_min = _letter_arr[i]->getPositionX() - _letter_arr[i]->getContentSize().width/2;
//        if (minX > _posX_min) {
//            minX = _posX_min;
////            CCLOG("letter = %d | posletter = %f | sizeletter = %f", i, _letter_arr[i]->getPositionX(), _letter_arr[i]->getContentSize().width);
//        }
//
//        auto _posX_max = _letter_arr[i]->getPositionX() + _letter_arr[i]->getContentSize().width/2;
//
//        if (maxX < _posX_max) {
//            maxX = _posX_max;
//        }
//
//        auto _posY_min = _letter_arr[i]->getPositionY() - _letter_arr[i]->getContentSize().height/2;
//        if (minY > _posY_min) {
//            minY = _posY_min;
//        }
//        auto _posY_max = _letter_arr[i]->getPositionY() + _letter_arr[i]->getContentSize().height/2;
//        if (maxY < _posY_max) {
//            maxY = _posY_max;
//        }
//
////        CCLOG("content = %f", _letter_arr[i]->getContentSize().height);
//    }
//    _letter_arr.clear();
//    auto rect = Rect(minX, minY, maxX - minX, maxY - minY);
//    return rect;
}

void TextStory::addTagHSLabelToArr(int tag){
    auto check = false;
    for (int i = 0; i < (int) _listTagHSLabel.size(); i++) {
        if (tag == _listTagHSLabel[i]) {
            check = true;
            break;
        }
    }
    if (!check) {
        _listTagHSLabel.push_back(tag);
    }
}

void TextStory::removeHSLabelEffect(){
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();

    for (int i = 0; i < (int) _listTagHSLabel.size(); i ++) {
        auto hschecklabel = label->getChildByTag(_listTagHSLabel[i]);
        if (hschecklabel) {
            hschecklabel->removeFromParent();
        }
    }
    _listTagHSLabel.clear();
    label->setOpacity(255);
//    CCLOG("fontconfig: r = %d", normal_color.r);

    label->setColor(normal_color);
    for(int j = 0; j < NativeBridge::utf8LengthOfString(label->getString()); j++) {
        auto letter = label->getLetter(j);
        if(letter != NULL) {
            letter->setColor(normal_color);
            letter->setOpacity(255);
            letter = nullptr;
        }
    }
    _listEffectID.clear();
}

void TextStory::playGAFWhenTextMCNull(){
    auto parent = (TruyenTranhPage*)this->getParent();
    parent->playGAFWhenMCDone();
    parent = nullptr;
}

void TextStory::stopAllActionAndMC(){
    this->unscheduleAllCallbacks();
    removeHSLabelEffect();
}

#pragma mark - Setting
#pragma mark - Callback Functions
Widget::ccWidgetClickCallback TextStory::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(TextStory, onHide),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

