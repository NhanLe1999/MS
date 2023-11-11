#include "NhanVat.h"
#include "HSLib.h"
#include "NativeBridge.h"
#include "TruyenTranhPage.h"
#include "audio/include/AudioEngine.h"
#include "MJ_AnimationCache.h"

USING_NS_CC;
//gaf::GAFObject *gaf_object = nullptr;
bool NhanVat::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    return true;
}

#pragma mark - PLAY ANI OBJECT - One layer
bool NhanVat::initOne(std::string fileName, bool loop, NhanVatObj nhanvatobj) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    
    nvObj = nhanvatobj;
    if (nvObj.isGaf) {
        CCLOG("gaf_fileName = %s", fileName.c_str());
        
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        
#endif
        if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isTestStory()) {
            if (!cocos2d::FileUtils::getInstance()->isFileExist(fileName)) {
                init_gaf_done = false;
                std::string alert;
                auto list_pos = NativeBridge::findStringOnString(fileName, "/", false);
                if ((int)list_pos.size() > 0) {
                    auto pos_end = list_pos[(int)list_pos.size()-1];
                    auto file_gaf = NativeBridge::hsStringSub(fileName, pos_end+1, NativeBridge::utf8LengthOfString(fileName)-pos_end-1);
                    alert = std::string("LỖI! cần up lại file: ") + file_gaf;
                }else {
                    alert = "config không có file -> config lại";
                }
                
                auto layout = cocos2d::ui::Layout::create();
                layout->setContentSize(Director::getInstance()->getVisibleSize());
                layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                layout->setPosition(Vec2(layout->getContentSize().width/2, random(100, 700)));
                layout->setTouchEnabled(true);
                Director::getInstance()->getRunningScene()->addChild(layout, 1999999999);
                auto text = cocos2d::ui::Text::create(alert, "fonts/monkeystories_ultra.ttf", 30);
                layout->addChild(text);
                text->setPosition(layout->getContentSize()/2);
                text->setTextColor(Color4B::RED);
                text->enableOutline(Color4B::BLACK, 3);
                return true;
            }
        }
        
        auto gaf_asset = gaf::GAFAsset::create(XHSLib->getFilePathStory(fileName), nullptr);

//        auto gaf_asset = mj::AnimationCache::create(XHSLib->getFilePathStory(fileName));
        CCLOG("gaf_init_done");
        gaf_object = gaf_asset->createObject();
        gaf_object->setLocator(true);
        this->addChild(gaf_object, 10);
        const gaf::GAFHeader& gaf_header_info = gaf_asset->getHeader();
        auto centered_position = cocos2d::Point(-gaf_header_info.frameSize.getMinX(), gaf_header_info.frameSize.getMinY()/2);
        gaf_object->setPosition(Vec2(2,0));
        gaf_object->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
        if (nvObj.istouch) {
            gaf_object->setLooped(loop, false);
            gaf_object->start();
//            gaf_object->pauseAnimation();
            gaf_object->playSequence("idle", loop);
        }else{
            gaf_object->setLooped(loop, true);
            gaf_object->start();
            gaf_object->playSequence("action", true);
        }
    }
    else{
        if (fileName.length() > 4) {
            auto img = ImageView::create(XHSLib->getFilePathStory(fileName));
            this->addChild(img,11);
        }
    }
    
    for (int i = 0; i < (int) nvObj.listTextNV.size(); i++) {
        _textNVObj = TextNVObj(nvObj.listTextNV[i].asValueMap());
        auto panel = cocos2d::ui::Layout::create();
        
        panel->setLocalZOrder(1000);
        if (CONFIG_MANAGER->isTestStory()) {
            panel->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            panel->setBackGroundColor(cocos2d::Color3B(random(0, 255), random(0, 255), random(0, 255)));
            panel->setOpacity(80);
        }
        
        if (nvObj.isGaf) {
            auto posO = Vec2(nvObj.contensize.width/2, nvObj.contensize.height/2);
            panel->setContentSize(_textNVObj.touchboundingbox.size);
            panel->setAnchorPoint(Vec2(0,0));
            //        CCLOG("posO = %f x %f", posO.x, posO.y);
            posO = _textNVObj.touchboundingbox.origin - posO;
            posO = posO + Vec2(_textNVObj.touchboundingbox.size.width/2, _textNVObj.touchboundingbox.size.height/2);
            panel->setPosition(posO);
            
        }else{
            if (nvObj.istouch) {
                if (nvObj.contensize.width != _textNVObj.touchboundingbox.size.width || nvObj.contensize.height != _textNVObj.touchboundingbox.size.height) {
                    auto posO = Vec2(nvObj.contensize.width/2, nvObj.contensize.height/2);
                    panel->setContentSize(_textNVObj.touchboundingbox.size);
                    panel->setAnchorPoint(Vec2(0,0));
                    posO = _textNVObj.touchboundingbox.origin - posO;
                    posO = posO + Vec2(_textNVObj.touchboundingbox.size.width/2, _textNVObj.touchboundingbox.size.height/2);
                    panel->setPosition(posO);
                    //CCLOG("posNV = %f x %f", nvObj.posNV.x, nvObj.posNV.y);
                    //CCLOG("%s posO = %f x %f", nvObj.fileName.c_str(),posO.x, posO.y);
                    
                }else{
                    auto posO = _textNVObj.touchboundingbox.origin - nvObj.posNV;
                    posO = Vec2(posO.x + nvObj.contensize.width/2, posO.y + nvObj.contensize.height/2);
                    panel->setContentSize(_textNVObj.touchboundingbox.size);
                    panel->setPosition(posO);
                    //CCLOG("posNV = %f x %f", nvObj.posNV.x, nvObj.posNV.y);
                    //CCLOG("%s posO = %f x %f", nvObj.fileName.c_str(),posO.x, posO.y);
                }
            }else{
                panel->setContentSize(Size(1,1));
            }
        }
        if (nvObj.istouch) {
            this->addChild(panel);
            panel->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
            _listTouchPanel.push_back(panel);
        }
        
        //Effect
        auto pos_star_check = _textNVObj.star_postion;
        pos_star_check = this->convertToWorldSpace(pos_star_check);
        auto screen_size = cocos2d::Director::getInstance()->getWinSize();
        if (nvObj.istouch && pos_star_check.x > -100 && pos_star_check.y > -100 && pos_star_check.x < (screen_size.width + 100)) {
//            auto gaf_asset = gaf::GAFAsset::create(cocos2d::StringUtils::format("%s.gaf","khung/sao"), nullptr);
            auto gaf_asset = mj::AnimationCache::create("khung/sao.gaf");
            
            auto guide_touch = gaf_asset->createObject();
            guide_touch->setLocator(true);
            this->addChild(guide_touch, 15555);
            guide_touch->setLooped(loop, true);
            guide_touch->setVisible(false);
            const gaf::GAFHeader& gaf_header_info = gaf_asset->getHeader();
            auto centered_position = cocos2d::Point(-gaf_header_info.frameSize.getMinX(), gaf_header_info.frameSize.getMinY()/2);
            auto posGuide = Vec2(nvObj.contensize.width/2, nvObj.contensize.height/2);
            //CCLOG("posO = %f x %f", posGuide.x, posGuide.y);
            posGuide = _textNVObj.star_postion - posGuide;
            posGuide = posGuide - centered_position;
            guide_touch->setPosition(posGuide);
            guide_touch->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
            
            auto guide_size = Size(140,140);
            float _scale = 1;
            if (nvObj.contensize.width < guide_size.width) {
                _scale = (nvObj.contensize.width)/(guide_size.width);
            }else if (nvObj.contensize.height < guide_size.height){
                _scale = (nvObj.contensize.height)/(guide_size.height);
            }
            
            auto tyle = 1;
            if (_scale < 0.25) {
                tyle = 2;
            }
             guide_touch->setScale(tyle*_scale);
//            CCLOG("_scale = %f", _scale);
            
            
            _listGuideTouch.push_back(guide_touch);
        }
    }
    
    normal_color = Color3B::WHITE;
    highlight_color = Color3B::RED;
    
    return true;
}

NhanVat * NhanVat::createOne(std::string fileName, bool loop, NhanVatObj nhanvatobj) {
    auto p_food = new NhanVat();
    if (p_food && p_food->initOne(fileName, loop, nhanvatobj)) {
        p_food->autorelease();
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}

void NhanVat::displayTextNhanvat(cocos2d::Vec2 _posText, int touch_index){
    if (isRunning) return;
    isDisplayText = true;
    _textNVObj = TextNVObj(nvObj.listTextNV[touch_index].asValueMap());
    if (_textNVObj.text == "") {
        if (nvObj.isGaf && nvObj.istouch) {
            this->playGAF();
            if (nvObj.isEffectEnable && !nvObj.isGaf) {
                auto parent = (TruyenTranhPage*)this->getParent()->getParent()->getParent();
                if (parent) {
                    parent->onNextEffectNV();
                    parent = nullptr;
                }
            }
            isDisplayText = false;
        }
        return;
    }
    auto label = Label::createWithTTF(_textNVObj.text, "fonts/monkeystories_ultra.ttf", 60);
    this->addChild(label, 10010);
    label->setScale(0);
    auto parent = dynamic_cast<TruyenTranhPage*>(this->getParent()->getParent()->getParent());
    if (parent) {
        parent->displayTextNV(_textNVObj.text, _textNVObj.widthText, this->convertToWorldSpace(_posText), _textNVObj.timeText);
        parent = nullptr;
    }
    
    auto _duration = 0.25f;
    if (_textNVObj.timeText == 0) {
        _textNVObj.timeText = 1;
    }
    auto startHighlightText = CallFunc::create([=]{
        //label->setTextColor(Color4B(highlight_color));
        if (_textNVObj.soundName != "" && XHSLib->story_play_type != XHSLib->READITMYSELF) {
            AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(_textNVObj.soundName).c_str());
            CCLOG("abc abc %s", XHSLib->getFilePathStory(_textNVObj.soundName).c_str());
        }
    });
    
    auto endHighlightText = CallFunc::create([=]{
        //label->setTextColor(Color4B(normal_color));
        this->playGAF();
        if (nvObj.isEffectEnable && !nvObj.isGaf) {
            auto parent = (TruyenTranhPage*)this->getParent()->getParent()->getParent();
            if (parent) {
                parent->onNextEffectNV();
                parent = nullptr;
            }
        }
        isDisplayText = false;
        label->removeFromParent();
    });
    
    label->runAction(Sequence::create(DelayTime::create(0), startHighlightText, DelayTime::create(_textNVObj.timeText), DelayTime::create(0.2f), FadeOut::create(0.5f), endHighlightText, NULL));
    
    auto parent_2 = (TruyenTranhPage*)this->getParent()->getParent()->getParent();
    if (parent_2) {
        parent_2->displayEffectTextMC(_textNVObj.text, _duration+_textNVObj.timeText);
        parent_2 = nullptr;
    }
    
}

void NhanVat::testText(Vec2 _posText){
}

void NhanVat::startAction(bool isLoop, Vec2 _posText, int touch_index){
    isFirstTouch = true;
    //if (isRunning | isDisplayText) return;
    if (isRunning) return;
    if (nvObj.isEffectEnable) {
        for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
            if (_listGuideTouch[i]) {
                _listGuideTouch[i]->pauseAnimation();
                _listGuideTouch[i]->removeFromParent();
            }
        }
        _listGuideTouch.clear();
    }
    
    //displayTextNhanvat(_posText);
    CCLOG("file name = %s", nvObj.fileName.c_str());
    displayTextNhanvat(_posText, touch_index);
    if (nvObj.isGaf) {
        isRunning = true;
        auto parent = dynamic_cast<TruyenTranhPage*>(this->getParent()->getParent()->getParent());
        if (parent) {
            parent->count_gaf_play++;
            parent->pauseStarGuide();
            parent = nullptr;
            //        parent->_enable_touch = false;
        }
        
    }
    isDisplayText = true;
}

void NhanVat::playGAF(){
    if (nvObj.isGaf) {
        gaf_object->start();
        gaf_object->resumeAnimation();
        gaf_object->playSequence("action", false);
        gaf_object->setLooped(false, true);
        gaf_object->setAnimationFinishedPlayDelegate(CC_CALLBACK_1(NhanVat::onFinishAction, this));
        
    }
    
    AUDIO_ENGINE->playEffect(XHSLib->getFilePathStory(nvObj.sound_effect));
    CCLOG("sound_path = %s", XHSLib->getFilePathStory(nvObj.sound_effect).c_str());
    
}

void NhanVat::startEffect(){
    nvObj.isEffectEnable = true;
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i]){
            _listGuideTouch[i]->setVisible(true);
            _listGuideTouch[i]->start();
        }
    }
}

void NhanVat::pauseStarEffect(){
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i] && _listGuideTouch[i]->isVisible()) {
            _listGuideTouch[i]->setOpacity(0);
            _listGuideTouch[i]->setVisible(false);
        }
    }
}

void NhanVat::resumStarEffect(){
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i] && _listGuideTouch[i]->getOpacity() == 0) {
            _listGuideTouch[i]->setOpacity(255);
            _listGuideTouch[i]->setVisible(true);
        }
    }
}

#pragma mark - GAF_CALLBACK_DONE
void NhanVat::onFinishAction(gaf::GAFObject *obj){
    if (is_click_pause_ani_gaf) return;
    if (nvObj.is_reset_ani) {
        isRunning = false;
        gaf_object->playSequence("idle", true);
    }
    
    auto parent = dynamic_cast<TruyenTranhPage*>(this->getParent()->getParent()->getParent());
    if (parent) {
        if (parent->_enable_first == false && nvObj.is_run_with_mc) {
            parent->playGAFOrderCallback();
//            parent->count_gaf_runfirst++;
//            if (parent->count_gaf_runfirst == parent->num_gaf_object) {
//                parent->_enable_first = true;
//                if (XHSLib->story_play_type != XHSLib->AUTOPLAY) {
//                    parent->onStartEffectNV();
//                }else{
//                    parent->onNextPage(nullptr);
//                }
//            }
        }else{
            if (nvObj.isEffectEnable && nvObj.isGaf) {
                    parent->onNextEffectNV();
            }
        }
        parent->_enable_touch = true;
        parent->count_gaf_done++;
        if (parent->count_gaf_done >= parent->count_gaf_play) {
            parent->count_gaf_play = parent->count_gaf_done = 0;
            parent->resumStarGuide();
        }
        parent = nullptr;
    }
}

void NhanVat::highlightTextAction(){
    this->unscheduleAllCallbacks();
    auto bgText = this->getChildByName("bgtextNhanvat");
    if (!bgText) return;
    auto label = bgText->getChildByName<Label *>("textNhanvat");
    if (!label) return;

    if((nvObj.timeline.size()) == highlight_index) {
        label->setColor(normal_color);
        highlight_index = 0;
        return;
    }
    
    ValueMap map = nvObj.timeline[highlight_index++].asValueMap();
    iStart = map["ts"].asInt();
    iEnd = map["te"].asInt();
    label->setColor(normal_color);
    for(int i = iStart; i<= iEnd; i++) {
        auto letter = label->getLetter(i);
        if(letter != NULL) {
            letter->setColor(highlight_color);
        }
    }
    
    auto duration = (map["e"].asFloat() - map["s"].asFloat())/1000.0f;
    this->scheduleOnce([=](float dt){
        highlightTextAction();
    }, duration, StringUtils::format("highlight_index_%i", highlight_index));
}

void NhanVat::stopAllActionAndGaf(){
    isRunning = true;
    if (gaf_object) {
        is_click_pause_ani_gaf = true;
        gaf_object->pauseAnimation();
    }
    pauseStarEffect();
    this->stopAllActions();
}



