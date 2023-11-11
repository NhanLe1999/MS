
#include "MSBookObject.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "NativeBridge.h"
#include "StoryConfigManager.h"
#include "MSEventLogger.h"
#include "HSLib.h"

INITIALIZE_READER(MSBookObject);
#define speed_opacity 15
USING_NS_CC;
std::once_flag MSBookObject_reader;
#define CSB_NAME "csb/book_page/MSBookObject.csb"

MSBookObject * MSBookObject::createObject(mj::BOOK_IMAGE_INFO book_image_info) {
    std::call_once(MSBookObject_reader, [] {
        REGISTER_CSB_READER(MSBookObject);
    });
    auto p = static_cast<MSBookObject *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
		p->setContentSize(Size::ZERO);
        p->didLoadFromCSB(book_image_info);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MSBookObject::didLoadFromCSB(mj::BOOK_IMAGE_INFO book_image_info) {
    _book_image_info = book_image_info;
//    CCLOG("object - type = %s\npath= %s", _book_image_info.type.c_str(), _book_image_info.path.c_str());
    addObject();
}

void MSBookObject::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MSBookObject::startPlaying() {

    MS_LOGGER.logEvent(StringUtils::format("%s", __FUNCTION__),  {
            {"book_image_info_type", cocos2d::Value(_book_image_info.type)},
            {"book_image_info_animation", cocos2d::Value(_book_image_info.animation_type)}
    });

    if (_book_image_info.type == OBJECT_TYPE_ANIMATION) {
        if (XHSLib->story_play_type == XHSLib->READITMYSELF) {
            if (_book_image_info.animation_type == ANI_TYPE_SEQUENCE){
                if (!_book_image_info.list_sequence.empty()) {
                    if(animation)  animation->goToEndFrame();
                }
            }
        }else {
            this->runAction(Sequence::create(DelayTime::create(.05f), CallFunc::create([=]{
                if (_book_image_info.animation_type == ANI_TYPE_BEGIN) {
                    runAniWithFirst();
                }else if (_book_image_info.animation_type == ANI_TYPE_SEQUENCE){
                    runAniWithSequence();
                }
            }), NULL));
        }
    }
}

void MSBookObject::addObject(){
    if (_book_image_info.type == OBJECT_TYPE_ANIMATION) {
        addAnimation();
    }else addImage();
    
//    auto layout = cocos2d::ui::Layout::create();
//    layout->setContentSize(cocos2d::SizeFromString(_book_image_info.contentsize));
//    layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    layout->setBackGroundColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
//    layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    this->addChild(layout, 1900000000);
    
    addTouchBox();
}

void MSBookObject::addImage(){
    if (_book_image_info.path != "") {
        auto img = ImageView::create(_book_image_info.path);
        this->addChild(img);
    }
}

void MSBookObject::addTouchBox(){
    
    CRASH_AUTO_LOG;

    if (!_book_image_info.touchable || _book_image_info.list_touch.empty()) return;

    auto size_object = cocos2d::SizeFromString(_book_image_info.contentsize);
    
    for (int i = 0; i < (int) _book_image_info.list_touch.size(); i++) {
        if (_book_image_info.list_touch[i].boundingbox == "") continue;
        auto touch_layout = LayoutPolygon::create();
        touch_layout->setVertices(_book_image_info.list_touch[i].vertices);
        auto rect = cocos2d::RectFromString(_book_image_info.list_touch[i].boundingbox);
        touch_layout->setContentSize(rect.size);
        // touch_layout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(touch_layout);
        
        Vec2 pos_layout;
        if (_book_image_info.type == OBJECT_TYPE_ANIMATION) {
            pos_layout = Vec2(size_object/2);
            pos_layout = rect.origin - pos_layout;
            // pos_layout = pos_layout + rect.size/2;
        } else {
            if (size_object.width != rect.size.width || size_object.height != rect.size.height) {
                pos_layout = Vec2(size_object/2);
                pos_layout = rect.origin - pos_layout;
                // pos_layout = pos_layout + rect.size/2;
            }else {
                pos_layout = rect.origin - cocos2d::PointFromString(_book_image_info.position);
                // pos_layout = pos_layout + size_object/2;
            }
        }
        touch_layout->setPosition(pos_layout);
        list_touch_box.push_back(touch_layout);
		auto bb = touch_layout->getBoundingBox();
		CCLOG("touch bounding box convert from %s to [%.0f, %.0f] [%.0f, %.0f]", _book_image_info.list_touch[i].boundingbox.c_str(), bb.origin.x, bb.origin.y, bb.size.width, bb.size.height);
        
        if (CONFIG_MANAGER->isTestStory() || CONFIG_MANAGER->isDevEnvironment()) {
            touch_layout->setBackGroundColor(Color3B(random(0, 255), random(0, 255), random(0, 255)));
            touch_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
            touch_layout->setOpacity(100);
            
            auto vertices = _book_image_info.list_touch[i].vertices;
            if (!vertices.empty()) {
                auto drawnode = DrawNode::create();
                drawnode->setLineWidth(6);
                auto color = Color4F(random(0.f, 1.f) / 2, random(0.f, 1.f) / 2, random(0.f, 1.f) / 2, 0.7);
                drawnode->drawPolygon(vertices.data(), (int)vertices.size(), color, 1, color);
                touch_layout->addChild(drawnode, 19283487);
            }
        }
        
        //guide_touch
        if (_book_image_info.list_touch[i].star_position == "") continue;
        auto pos_guide_touch = cocos2d::PointFromString(_book_image_info.list_touch[i].star_position);
        auto pos_guide_check = this->convertToWorldSpace(pos_guide_touch);
        if (pos_guide_touch.x >= -100 && pos_guide_touch.y >= -100) {
            auto guide_touch = GAFWrapper::createGAF("khung/sao.gaf");
            pos_guide_touch = pos_guide_touch - size_object/2;
            guide_touch->setPosition(pos_guide_touch);
            this->addChild(guide_touch, 555555);
            auto guide_size = cocos2d::Size(140,140);
            float _scale = 1;
            if (size_object.width < guide_size.width) {
                _scale = (size_object.width)/(guide_size.width);
            }else if (size_object.height < guide_size.height){
                _scale = (size_object.height)/(guide_size.height);
            }
            
            auto tyle = 1;
            if (_scale < 0.25) {
                tyle = 2;
            }
            guide_touch->setScale(tyle*_scale);
//            CCLOG("_scale = %f", _scale);
            _listGuideTouch.push_back(guide_touch);
            guide_touch->setVisible(false);
            check_have_star = true;
        }
    }

    MS_LOGGER.logEvent("COMPLETE_MSBookObject_addTouchBox", {});
}

void MSBookObject::addAnimation(){
    if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isTestStory()) {
        auto fileName = _book_image_info.path;
        CCLOG("gaf_file = %s", fileName.c_str());
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
            return;
        }
    }
    
    bool isAnimationFileExist = cocos2d::FileUtils::getInstance()->isFileExist(_book_image_info.path);
    
    if(isAnimationFileExist) {
        animation = GAFWrapper::createGAF(_book_image_info.path);
        if (CONFIG_MANAGER->isTestStory()) {
            animation->enableDebug();
        }
        this->addChild(animation);
        if (!_book_image_info.touchable) {
            animation->play(true);
        }
        
    }

    MS_LOGGER.logEvent(StringUtils::format("COMPLETE_MSBookObject::addAnimation"),
        {
            {"book_image_info_path", cocos2d::Value(_book_image_info.path)},
            {"is_file_exist", cocos2d::Value(isAnimationFileExist ? "true" : "false")}
        });
}

void MSBookObject::playAudioObject(cocos2d::Vec2 pos_touch){
    if (!touch_enable) return;
    isFirstTouch = true;
    if (_book_image_info.type == OBJECT_TYPE_ANIMATION ) {
        if (is_running_ani) return;
        is_running_ani = true;
    }
    
    if (isEffectEnable) {
        for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
            if (_listGuideTouch[i]) {
                _listGuideTouch[i]->pauseAnimation();
                _listGuideTouch[i]->removeFromParent();
            }
        }
        _listGuideTouch.clear();
    }
    
    if (_delegate){
        _delegate->onTouchObject(_book_image_info.audio_info.text, _book_image_info.audio_info.w_text, pos_touch, _book_image_info.audio_info.duration,_book_image_info);
        _delegate->onActionGuideEffectObject("pause");

    }
    
    auto duration_delay = 0.25f;
    this->runAction(Sequence::create(DelayTime::create(duration_delay), CallFunc::create([=]{
        if (XHSLib->story_play_type != XHSLib->READITMYSELF) {
            AUDIO_ENGINE->playEffect(_book_image_info.audio_info.path);
        }
    }), DelayTime::create(_book_image_info.audio_info.duration + 0.25f), CallFunc::create([=]{
        if (_book_image_info.type == OBJECT_TYPE_ANIMATION ) {
            runAniWithTouch();
        }else {
            if (_delegate) {
                if (isEffectEnable) {
                    _delegate->onActionGuideEffectObject("next");
                }
                _delegate->onActionGuideEffectObject("resum");
            }
        }
    }), NULL));
}

void MSBookObject::runAniWithTouch(){
    CRASH_AUTO_LOG;
    if (!_book_image_info.animation_reset){
        touch_enable = false;
    }
    
    if (_book_image_info.animation_type == ANI_TYPE_SEQUENCE) {
        auto sequence_last = _book_image_info.list_sequence[(int)_book_image_info.list_sequence.size()-1];
        if(animation)  animation->playSequence(sequence_last.name, false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            if (_book_image_info.animation_reset){//animation reset thì mới set về idle, không thì cứ để nguyên hiện trạng
                animation->playSequence(sequence_last.name, false);
                animation->pauseAnimation();
            }
            is_running_ani = false;
            touch_enable = true;
            if (_delegate) {
                if (isEffectEnable) {
                    _delegate->onActionGuideEffectObject("next");
                }
                _delegate->onActionGuideEffectObject("resum");
            }
        });
        AUDIO_ENGINE->playEffect(sequence_last.effect);
        CCLOG("Audio - play path= %s", sequence_last.effect.c_str());

    }else{
        if(animation)  animation->playSequence("action", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
            if (_book_image_info.animation_reset){//animation reset thì mới set về idle, không thì cứ để nguyên hiện trạng
                animation->playSequence("idle", true);
                is_running_ani = false;
                
            }
            if (_delegate) {
                if (isEffectEnable) {
                    _delegate->onActionGuideEffectObject("next");
                }
                _delegate->onActionGuideEffectObject("resum");
            }
        });
        AUDIO_ENGINE->playEffect(_book_image_info.effect);
        CCLOG("Audio - play path= %s", _book_image_info.effect.c_str());
    }
    MS_LOGGER.logEvent("COMPLETE_MSBookObject::runAniWithTouch", {});
}

void MSBookObject::runAniWithMC() {
    if (_book_image_info.animation_type != ANI_TYPE_END) return;
    if(animation) animation->playSequence("action", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
        animation->playSequence("idle", true);
        if (_delegate){
            _delegate->onRunAniObjectWithMCDone();
        }
    });
    AUDIO_ENGINE->playEffect(_book_image_info.effect);
    CCLOG("Audio - play path= %s", _book_image_info.effect.c_str());
}


void MSBookObject::runAniWithSequence(){
   
    CRASH_AUTO_LOG;
    
	if(animation) {
        animation->play(false);
        animation->pauseAnimation();
    }

    sort( _book_image_info.list_sequence.begin( ), _book_image_info.list_sequence.end( ), [](mj::BOOK_IMAGE_SEQUENCE lhs, mj::BOOK_IMAGE_SEQUENCE rhs )
         {
             return lhs.end < rhs.end;
         });
    
    if (_book_image_info.list_sequence.empty() || _book_image_info.list_sequence.size() == 0)
    {
        return;
    }
	
    for (int i = 0; i < (int) _book_image_info.list_sequence.size(); i++) {
        auto sequence_ani = _book_image_info.list_sequence[i];
        auto duration = (sequence_ani.end - sequence_ani.start)/1000.0f;
        if (duration <= 0) {
            duration = 0;
        }
        
        this->runAction(Sequence::create(DelayTime::create(sequence_ani.start/1000.0f), CallFunc::create([=]{
           
            if(animation) {
                
                animation->playSequence(sequence_ani.name, sequence_ani.loop, [=] (gaf::GAFObject *obj, std::string name_sequence){
                    if (i == (int) _book_image_info.list_sequence.size()-1){
                        if (_delegate){
                            _delegate->onRunAniWithSequenceDone();
                        }
                    }
                });
            }

            AUDIO_ENGINE->playEffect(sequence_ani.effect);
            CCLOG("Audio - play path = %s", sequence_ani.effect.c_str());
        }), DelayTime::create(duration), CallFunc::create([=]{
            if (duration > 0){
                if(animation) {
                    animation->pauseAnimation();
                }
            }
        }), NULL));
    }
	
    MS_LOGGER.logEvent("COMPLETE_MSBookObject_runAniWithSequence", {});
}

void MSBookObject::runAniWithFirst(){
    if(!animation) return;
    animation->playSequence("idle", true);
    animation->pauseAnimation();
//    is_running_ani = true;
//    animation->playSequence("action", false, [=] (gaf::GAFObject *obj, std::string name_sequence){
//        if (_book_image_info.animation_reset){
//            animation->playSequence("idle", true);
//            is_running_ani = false;
//        }
//    });
//    AUDIO_ENGINE->playEffect(_book_image_info.effect);
}

#pragma mark - GUIDE_STAR
void MSBookObject::startEffect(){
    isEffectEnable = true;
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i]){
            _listGuideTouch[i]->setVisible(true);
            _listGuideTouch[i]->play(true);
        }
    }
}

void MSBookObject::pauseStarEffect(){
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i] && _listGuideTouch[i]->isVisible()) {
            _listGuideTouch[i]->pauseAnimation();
            _listGuideTouch[i]->setOpacity(0);
            _listGuideTouch[i]->setVisible(false);
        }
    }
}

void MSBookObject::resumStarEffect(){
    for (int i = 0; i < (int)_listGuideTouch.size(); i++) {
        if (_listGuideTouch[i] && _listGuideTouch[i]->getOpacity() == 0) {
            _listGuideTouch[i]->resumeAnimation();
            _listGuideTouch[i]->setOpacity(255);
            _listGuideTouch[i]->setVisible(true);
        }
    }
}

void MSBookObject::stopAllActionAndEffect(){
    is_running_ani = true;
    touch_enable = false;
    pauseStarEffect();
    if (_book_image_info.type == OBJECT_TYPE_ANIMATION) {
        if(animation) animation->pauseAnimation();
    }
    this->stopAllActions();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
}

#pragma mark -- TOUCH
bool MSBookObject::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (!touch_enable) return false;
    touch_pos = this->convertToNodeSpace(touch->getLocation());
    return true;
}
void MSBookObject::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event){
}
void MSBookObject::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (!touch_enable) return;
//    touch_end = this->convertToNodeSpace(touch->getLocation());
//    for (int i = 0; i < (int) list_touch_box.size(); i++) {
//        if (list_touch_box[i]->getBoundingBox().containsPoint(touch_pos) && list_touch_box[i]->getBoundingBox().containsPoint(touch_end) && touch_end.distance(touch_pos) < 10) {
//            playAudioObject();
//            return;
//        }
//    }
}

void MSBookObject::setDelegate(MSBookObjectDelegate *delegate){
    _delegate = delegate;
}

#pragma callback
ui::Widget::ccWidgetClickCallback MSBookObject::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onRefresh") {
        return CC_CALLBACK_1(MSBookObject::onRefresh, this);
    }
    return nullptr;
}

void MSBookObject::onRefresh(cocos2d::Ref *sender) {
    this->removeFromParent();
}

void MSBookObject::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
//    AUDIO_ENGINE->stopAllEffects();
}

