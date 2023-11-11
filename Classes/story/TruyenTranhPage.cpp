#include "TruyenTranhPage.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include <algorithm>
#include "StoryEndingPage.h"
#include "MJ_AnimationCache.h"
#include "MJHelper.h"
#include "MJFirebaseListener.h"

#include "MSStoryGuide.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../proj.win32/Win32DeviceInfo.h"
#endif
INITIALIZE_READER_INSTANCE(TruyenTranhPageReader);

USING_NS_CC;
using namespace ui;
TruyenTranhPage * TruyenTranhPage::createPage(int story_id, int lang_id, int page_id) {
    auto temp = utils::gettime();
    auto p = static_cast<TruyenTranhPage *>(CSLoader::createNodeWithVisibleSize("csb/TruyenTranhPage.csb"));
    if (p) {
        p->preload(story_id, lang_id, page_id);
    }

    CCLOG("Story Page Loading Time: %lf", utils::gettime() - temp);
    return p;
}
void TruyenTranhPage::preload(int story_id, int lang_id, int page_id) {
    _storyid = story_id;
    _langid = lang_id;
    _pageid = page_id;
    didLoadFromCSB();
}

void TruyenTranhPage::didLoadFromCSB() {
    _guide = MSStoryGuide::createGuide();
    this->addChild(_guide, 1000000);
    
    this->setContentSize(DIRECTOR->getVisibleSize());
    ui::Helper::doLayout(this);
    this->setClippingEnabled(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    if (CONFIG_MANAGER->isFixOpenglWin32()){
        this->setClippingEnabled(false);
    }
#endif
    //    this->getChildByName<cocos2d::ui::Layout *>("bg")->setBackGroundColor(Color3B::WHITE);
//    auto touch_listener = cocos2d::EventListenerTouchOneByOne::create();
//    touch_listener->onTouchBegan = CC_CALLBACK_2(TruyenTranhPage::onTouchBegan, this);
//    touch_listener->onTouchEnded = CC_CALLBACK_2(TruyenTranhPage::onTouchEnded, this);
//    touch_listener->onTouchMoved = CC_CALLBACK_2(TruyenTranhPage::onTouchMoved, this);
//    touch_listener->onTouchCancelled = CC_CALLBACK_2(TruyenTranhPage::onTouchEnded, this);
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch_listener, this);
    
    this->setTouchEnabled(true);
    this->addTouchEventListener([this](Ref * sender, ui::Widget::TouchEventType event) {
        switch (event) {
            case ui::Widget::TouchEventType::BEGAN:
                onTouchBegan();
                break;
                
            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED:
                this->onTouchEnded();
                break;
                
            default:
                break;
        }
    });
    
    mapPage = FileUtils::getInstance()->getValueMapFromFile(XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.plist", _storyid, _langid, _pageid)));
    
    CCLOG("load_plist = %s", XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.plist",_storyid, _langid, _pageid)).c_str());
    story_type = mapPage["type"].asInt();
    CCLOG("story_type = type %d", story_type);
    
    loadNhanvat();
    display_cheat_playAllGaf();
}

bool TruyenTranhPage::hasNextBoxText() {
    return textStory->hasNextBoxText();
}

bool TruyenTranhPage::hasPrevBoxText() {
    return textStory->hasPrevBoxText();
}

int TruyenTranhPage::getPageIndex() {
    return _pageid;
}

float TruyenTranhPage::getStandardTime()
{
    return m_standardTime;
}

int TruyenTranhPage::getGAFClickedNum() {
    return m_gafClickedNum;
}

int TruyenTranhPage::getIMGClickedNum() {
    return m_imgClickedNum;
}

void TruyenTranhPage::enableInteraction() {
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = [=](EventKeyboard::KeyCode key_code, Event* event) {
        switch (key_code) {
            case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            case EventKeyboard::KeyCode::KEY_DPAD_LEFT:
                this->onKeyboardPage(2);
                break;
            case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            case EventKeyboard::KeyCode::KEY_DPAD_RIGHT:
                this->onKeyboardPage(1);
                break;
            case EventKeyboard::KeyCode::KEY_UP_ARROW:
            case EventKeyboard::KeyCode::KEY_DPAD_UP:
            case EventKeyboard::KeyCode::KEY_DPAD_DOWN:
            case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
                this->onKeyboardPage(3);
                break;
            default:
                break;
        };
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    this->setTouchEnabled(true);
    this->enumerateChildren("//.*", [](Node * child) {
        if (child->getName() != "particle") {
            child->resume();
        }
        return false;
    });
}

void TruyenTranhPage::disableInteraction() {
    this->setTouchEnabled(false);
    this->getEventDispatcher()->removeEventListenersForType(EventListener::Type::KEYBOARD);

    this->enumerateChildren("//.*", [](Node * child) {
        if (child->getName() != "particle") {
            child->pause();
        }
        return false;
    });
}

void TruyenTranhPage::startPlaying() {
    textStory->startPlaying();
}

#pragma mark - onEnter
void TruyenTranhPage::onEnter() {
    Layout::onEnter();
    
    cocos2d::Device::setKeepScreenOn(true);
    Director::getInstance()->getScheduler()->schedule([](float) {
        cocos2d::Device::setKeepScreenOn(false);
    }, Application::getInstance(), 0, 0, 60.f, false, "delay_screen_on");
    
    this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
        isreplacescene_enable = true;
    }), NULL));
    
    auto disable_keyboard_listener = EventListenerCustom::create("story.disable.keyboard", [this](EventCustom *) {
        this->getEventDispatcher()->removeEventListenersForType(EventListener::Type::KEYBOARD);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(disable_keyboard_listener, this);
    
    
    if (CONFIG_MANAGER->isTestStory()) {
        auto page_number_layout = cocos2d::ui::Layout::create();
        page_number_layout->setContentSize(cocos2d::Size(50, 50));
        page_number_layout->setBackGroundColor(Color3B::BLACK);
        page_number_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
        page_number_layout->setOpacity(100);
        page_number_layout->setPosition(Vec2(10, 10));
        this->addChild(page_number_layout, INT_MAX-1);
        
        auto page_number_text = cocos2d::ui::Text::create(StringUtils::toString(_pageid), "fonts/Montserrat-SemiBold.ttf", 30);
        page_number_layout->addChild(page_number_text);
        page_number_text->setPosition(page_number_layout->getContentSize()/2);
        page_number_text->setOpacity(150);
        
    }

}

void TruyenTranhPage::loadNhanvat(){
    auto ScreenSize = XHSLib->getSizeScreen();
    nhanvat_layout = cocos2d::ui::Layout::create();
    nhanvat_layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    nhanvat_layout->setBackGroundColor(cocos2d::Color3B::WHITE);
    nhanvat_layout->setOpacity(255);
    nhanvat_layout->setPosition(Vec2(ScreenSize.width*displayNVSize, 0));

    if (story_type == XHSLib->SIMPLE_TYPE) {
        nhanvat_layout->setContentSize(Size(ScreenSize.width*(1 - displayNVSize), ScreenSize.height));
    }else if (story_type == XHSLib->ADVANCED_TYPE){
        nhanvat_layout->setContentSize(ScreenSize);
        nhanvat_layout->setAnchorPoint(Vec2(0.5, 0.5));
        nhanvat_layout->setPosition(Vec2(ScreenSize.width/2, ScreenSize.height/2));
    }
    nhanvat_layout->setName(cocos2d::StringUtils::format("nhanvatlayout"));
    this->addChild(nhanvat_layout, 98);

    //Add LayerNhanvat
    _nvLayout.clear();
    
    //bg
    // &&
    ValueMap mapIMG;
    if (!mapPage["bg"].isNull() && mapPage["bg"].getType() == Value::Type::MAP){
        mapIMG = mapPage["bg"].asValueMap();
    }
    
    bgPage = cocos2d::ui::ImageView::create(XHSLib->getFilePathStory(mapIMG["name"].asString()));
    XHSLib->wbackground = bgPage->getContentSize().width;
    XHSLib->hbackground = bgPage->getContentSize().height;

    nhanvat_layout->addChild(bgPage);
    bgPage->setAnchorPoint(Vec2(0.5, 0.5));
    bgPage->setName("bgpage");
    bgPage->setPosition(Vec2(nhanvat_layout->getContentSize().width/2, ScreenSize.height/2));
    CCLOG("bgpage = %f | %f", bgPage->getPositionX(), bgPage->getPositionY());
//    bgPage->setColor(Color3B::YELLOW);
    if (story_type == XHSLib->SIMPLE_TYPE) {
//        auto scale = nhanvat_layout->getContentSize().height*1.0/bgPage->getContentSize().height;
//        bgPage->setScale(scale);
    }else{
        auto pos = PointFromString(mapIMG["pos"].asString());
        if (pos.x != 0 && pos.y != 0) {
            bgPage->setPosition(Vec2(pos.x/1024.0*ScreenSize.width, pos.y/768.0*ScreenSize.height));
        }
    }
    
    //check bug
//    bgPage->setScale(0.4);
//    if (XHSLib->pageIndex == 7) {
//        if (FileUtils::getInstance()->isFileExist(XHSLib->getFilePathStory(mapIMG["name"].asString()))) {
//            CCLOG("path bgsacle = %s", XHSLib->getFilePathStory(mapIMG["name"].asString()).c_str());
//        }
//    }
    
    //Nhanvat
    if (!mapPage["nhanvat"].isNull() && mapPage["nhanvat"].getType() == Value::Type::VECTOR) {
        ValueVector _arrNV = mapPage["nhanvat"].asValueVector();
        auto tag = 100;
        auto count_gafplay = 0;
        for (int i = 0; i < (int)_arrNV.size(); i ++) {
            ValueMap mapNVItem = _arrNV[i].asValueMap();
            NhanVatObj nhanvat = NhanVatObj(mapNVItem);
//            CCLOG("postion = %f;%f", nhanvat.posNV.x, nhanvat.posNV.y);
            auto hsNV = NhanVat::createOne(nhanvat.fileName, true, nhanvat);
            if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isTestStory()) {
                if (!hsNV->init_gaf_done) {
                    continue;
                }
            }
            hsNV->setPosition(nhanvat.posNV);
            bgPage->addChild(hsNV, nhanvat.zorder);
            if (nhanvat.istouch) {
                _nvLayout.push_back(hsNV);
            }
            //_nvGAFEffect
            if (nhanvat.isGaf) {
                if (nhanvat.istouch) {
                    if (nhanvat.is_run_with_mc) {
                        _nvGAFPlay.push_back(hsNV);
                    }else{
                        _nvGAF_NoAutoPlay.push_back(hsNV);
                    }
                    
                }else {
                    _nvGAFEffect.push_back(hsNV);
                    hsNV->setTag(tag);
                    tag++;
                }
                
            }else{
                hsNV->setTag(tag);
                tag++;
            }
        }
        
        if (_nvGAFPlay.size() >= 2) {
            sort( _nvGAFPlay.begin( ), _nvGAFPlay.end( ), [](NhanVat* lhs, NhanVat* rhs )
                 {
                     return lhs->nvObj.gaf_order < rhs->nvObj.gaf_order;
                 });
        }

        float trackingGAFDuration = 0.0f;

        for (int i = 0; i < (int) _nvGAFPlay.size(); i++) {
            auto tag_gafplay = (_nvGAFPlay[i]->nvObj.gaf_order+kTagGafPlay)*100+count_gafplay;
            _nvGAFPlay[i]->setTag(tag_gafplay);
            CCLOG("tag_gaf = %d", tag_gafplay);
            count_gafplay++;
            list_gaforder_index.push_back(_nvGAFPlay[i]->nvObj.gaf_order);

            if (auto gaf = _nvGAFPlay[i]->gaf_object)
            {
                trackingGAFDuration = std::max(trackingGAFDuration,((float)gaf->getTotalFrameCount()/(float)gaf->getFps()));
            }
        }

        m_standardTime += trackingGAFDuration;

        dumpGafOrderToMap();
        num_gaf_object = (int)_nvGAFPlay.size();
        
        if (_nvLayout.size() >= 2) {
            sort( _nvLayout.begin( ), _nvLayout.end( ), [](NhanVat* lhs, NhanVat* rhs )
                 {
                     return lhs->nvObj.zorder > rhs->nvObj.zorder;
                 });
        }
        
        if ((int)_nvLayout.size() >= 1) {
            for (int i_eff = 0; i_eff < (int) _nvLayout.size(); i_eff ++) {
                if ((int)_nvLayout[i_eff]->_listGuideTouch.size() > 0) {
                    _nvEffect.push_back(_nvLayout[i_eff]);
                }
            }
            
            if (_nvEffect.size() >= 2) {
                sort( _nvEffect.begin( ), _nvEffect.end( ), [](NhanVat* lhs, NhanVat* rhs )
                     {
                         return lhs->nvObj.star_order > rhs->nvObj.star_order;
                     });
            }
            
            if (CONFIG_MANAGER->isTestStory()) {
                for (int i = 0; i < (int)_nvEffect.size(); i++) {
                    CCLOG("star_order = %s", _nvEffect[i]->nvObj.listTextNV[0].asValueMap()["text"].asString().c_str());
                }
            }
            
            if ((int)_nvEffect.size() >= 1) {
                _nvEffect[0]->nvObj.isEffectEnable = true;
            }
            
        }
    }
    
//    LayerColor *lcolor = LayerColor::create(Color4B(255, 255, 255, 255));
//    lcolor->setContentSize(Size(DIRECTOR->getVisibleSize().width/4, DIRECTOR->getVisibleSize().height/2));
//    nhanvat_layout->addChild(lcolor);
    
    textStory = static_cast<TextStory *>(CSLoader::createNode("csb/TextStory.csb"));
    if (mapPage.find("textstory") != mapPage.end() && !mapPage["textstory"].isNull() && mapPage["textstory"].getType() == Value::Type::VECTOR) {
        textStory->_textMCObj = TextMCObj(mapPage["textstory"].asValueVector(), story_type);
        
        auto rect_normal_str = mapPage["normal_color"].asString();
        if (rect_normal_str == string("")) {
            textStory->normal_color = Color3B::BLACK;
        }else{
            Rect rect_normal = RectFromString(rect_normal_str);
            textStory->normal_color = Color3B((int)rect_normal.origin.x, (int)rect_normal.origin.y , (int)rect_normal.size.width);
        }
        
        auto rect_highlight_str = mapPage["highlight_color"].asString();
        if (rect_highlight_str == "") {
            textStory->highlight_color = Color3B::RED;
        }else{
            auto rect_highlight = RectFromString(rect_highlight_str);
            textStory->highlight_color = Color3B((int)rect_highlight.origin.x, (int)rect_highlight.origin.y , (int)rect_highlight.size.width);
        }
        
    }else{
        textStory->_textMCObj = TextMCObj();
    }
    textStory->fontSize = mapPage["fontsize"].asInt();
//    CCLOG("mapPage = %s", JsonDump::getInstance()->MapToString(mapPage).c_str());
    
    this->addChild(textStory, 9999);
    if (story_type == XHSLib->SIMPLE_TYPE) {
        textStory->setPosition(Vec2(0, 0));
    }else{
        //textStory->setPosition(Vec2(XHSLib->getSizeScreen().width/2 - XHSLib->wbackground/2, 0));
        textStory->setPosition(Vec2(0, 0));
    }
    textStory->didLoadFromCSB();
    
    fixSizeDisplay();

    //[target:tracking data] calculate standard time on page.

	if (mapPage.find("textstory") != mapPage.end())
	{
		auto textStoryVector = mapPage["textstory"].asValueVector();

		for (size_t i = 0; i < textStoryVector.size(); i++)
		{
			auto objs = textStoryVector.at(i).asValueVector();

			for (size_t j = 0; j < objs.size(); j++)
			{
				auto ob1 = objs[j].asValueMap();

				if (ob1["timeline"].isNull() || ob1["timeline"].getType() != Value::Type::VECTOR)
				{
					continue;
				}

				auto value = ob1["timeline"].asValueVector().back().asValueMap();

				if (value["e"].isNull())
				{
					continue;
				}

				m_standardTime += (value["e"].asFloat() / 1000.0f);//miliseconds to seconds
			}
		}
	}

    m_standardTime += KHUNG_SAO_DURATION;
    
}

void TruyenTranhPage::fixSizeDisplay(){
    if (XHSLib->_id == 574 || XHSLib->_id == 96) {
        return;
    }
    
    if (XHSLib->getSizeScreen().width/XHSLib->getSizeScreen().height >= 2.1) {
        nhanvat_layout->setScaleX(1.2);
        return;
    }

    if (XHSLib->getSizeScreen().width/XHSLib->getSizeScreen().height >= 1.9) {
        nhanvat_layout->setScaleX(1.1);
        return;
    }

    
    if (XHSLib->getSizeScreen().width/XHSLib->getSizeScreen().height > 1.82) {
        nhanvat_layout->setScaleX(1.05);
    }
}

void TruyenTranhPage::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}

#pragma mark - onExit
void TruyenTranhPage::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}


#pragma mark - TOUCH
void TruyenTranhPage::onTouchBegan() {
//    if (!isreplacescene_enable) return false;
//    cocos2d::Point location = touch->getLocation();
//    pointTouch = location;
//    return true;
}

void TruyenTranhPage::onTouchMoved() {
//    cocos2d::Point location = this->convertTouchToNodeSpace(touch);
}


void TruyenTranhPage::onTouchEnded() {
    //CCLOG("Done");
    pointTouch = this->getTouchBeganPosition();
    cocos2d::Point location = this->getTouchEndPosition();
    if ((location.x - pointTouch.x) < -100) {
        //Next
        onNextPage(nullptr);
        this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
            if (!isEndStory){
                isreplacescene_enable = true;
            }
        }), NULL));
        return;
    }
    
    if ((location.x - pointTouch.x) > 100) {
        //Back
        onBackPage(nullptr);
        this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
            if (!isEndStory){
                isreplacescene_enable = true;
            }
        }), NULL));
        return;
    }
    
    if ((pointTouch.y - location.y) > 100) {
        isreplacescene_enable = false;
        this->runAction(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]{
            if (!isEndStory){
                isreplacescene_enable = true;
            }
        }), NULL));
        onRefresh(nullptr);
        return;
    }
    
    if (_enable_first == false) return;
    if (_enable_touch == false) return;

    
    auto isTouchFail = (pointTouch.x > XHSLib->getSizeScreen().width/2 && location.x > XHSLib->getSizeScreen().width/2)?false:true;
    if (story_type == XHSLib->ADVANCED_TYPE) {
        isTouchFail = false;
    }
    
    for (int i = 0; i < (int)_nvLayout.size(); i ++){
        auto hsAni = _nvLayout[i];
        CCLOG("Zorder = %d", hsAni->nvObj.zorder);
        auto pos_first = hsAni->convertToNodeSpace(pointTouch);
        auto pos_end = hsAni->convertToNodeSpace(location);
        
        for (int j = 0; j < (int)hsAni->_listTouchPanel.size(); j++) {
            auto touch_panel = hsAni->_listTouchPanel[j];
            if (touch_panel->getBoundingBox().containsPoint(pos_first) && touch_panel->getBoundingBox().containsPoint(pos_end) && !isTouchFail) {
                CCLOG("X1|touch touch %d", hsAni->getTag());
                hsAni->startAction(false, pos_end, j);

                (hsAni->nvObj.isGaf) ? m_gafClickedNum++ : m_imgClickedNum++;

//                mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsEvent(analytics_event_story_interactive, "touchgaf", "true");
                touch_panel = nullptr;
                _guide->startTimer();
                return;
            }
            touch_panel = nullptr;
        }
    }
    
//    for (int i = 0; i < (int)_nvLayout.size(); i ++){
//        auto hsNVLayout = _nvLayout[i];
//        CCLOG("Zorder = %d", hsNVLayout->nvObj.zorder);
//        auto pos_first = hsNVLayout->convertToNodeSpace(pointTouch);
//        auto pos_end = hsNVLayout->convertToNodeSpace(location);
//        
//        if (hsNVLayout->panel->getBoundingBox().containsPoint(pos_first) && hsNVLayout->panel->getBoundingBox().containsPoint(pos_end) && pointTouch.x > XHSLib->getSizeScreen().width/2 && location.x > XHSLib->getSizeScreen().width/2) {
//            CCLOG("touch touch %d", hsNVLayout->getTag());
//            hsNVLayout->startAction(false);
//            return;
//        }
//    }
    
}

void TruyenTranhPage::onKeyboardPage(int index_keyboard){
    if (!XHSLib->enable_keyboard || XHSLib->show_popup_subscribe) return;
    XHSLib->enable_keyboard = false;
    CCLOG("\n\n Vào nào vào nào");
    switch (index_keyboard) {
        case 1://next
            if (XHSLib->pageIndex > XHSLib->pageNumAll) return;
            onNextPage(nullptr);
            if (!XHSLib->enable_keyboard) {
                DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=]{
                    XHSLib->enable_keyboard = true;
                }), NULL));
            }
            break;
        case 2://back
            onBackPage(nullptr);
            if (!XHSLib->enable_keyboard) {
                DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=]{
                    XHSLib->enable_keyboard = true;
                }), NULL));
            }
            break;
        case 3://refresh
            CCLOG("refresh_refresh_refresh_refresh_refresh_refresh \n\n");
            onRefresh(nullptr);
            DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.65f), CallFunc::create([=]{
                XHSLib->enable_keyboard = true;
            }), NULL));
            break;
        default:
            break;
    }
}

void TruyenTranhPage::onStartEffectNV(){
    //start timer guide
    _guide->startTimer();
    
    isHighlightDone = true;
    onNextEffectNV();
}
void TruyenTranhPage::onNextEffectNV(){
//    CCLOG("size = %d", (int)_nvEffect.size());
    if (_nvEffect.size() >= 1) {
        auto hsAni = _nvEffect[0];
        if (hsAni->nvObj.isEffectEnable) {
            hsAni->nvObj.isEffectEnable = false;
            _nvEffect.erase(std::remove_if(_nvEffect.begin(), _nvEffect.end(), [=](NhanVat *pX) {
                return pX->isFirstTouch == true;
            }), _nvEffect.end());
//            CCLOG("size = %d", (int)_nvEffect.size());
            
            if (_nvEffect.size() >= 1) {
                _nvEffect[0]->nvObj.isEffectEnable = true;
                if (isHighlightDone) {
                    _nvEffect[0]->startEffect();
                }
            }
        }
    }
}

#pragma mark - BUTTON
void TruyenTranhPage::onNextPage(cocos2d::Ref *sender){
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    textStory->nextPage();
}

void TruyenTranhPage::onBackPage(cocos2d::Ref *sender){
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    textStory->backPage();
}

void TruyenTranhPage::onRefresh(cocos2d::Ref *sender){
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllEffects();
    EventCustom e("mjstory.replace_page");
    int * edata = new int;
    *edata = 3;
    e.setUserData(edata);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
//    auto home_scene = static_cast<Scene *>(CSLoader::createNode("csb/TruyenTranhPage.csb"));
//    DIRECTOR->replaceScene(TransitionCrossFade::create(0.35, createPage(_storyid, _langid, _pageid)));
}

void TruyenTranhPage::onShow(cocos2d::Ref *sender){
    //textStory->animShow();
}

#pragma mark - PLAY_GAF
void TruyenTranhPage::dumpGafOrderToMap(){
    if ((int)list_gaforder_index.size() <= 0) return;
    sort( list_gaforder_index.begin( ), list_gaforder_index.end( ), [](int lhs, int rhs )
         {
             return lhs < rhs;
         });
    auto ordernumber = list_gaforder_index[0];
    list_gaforder_number.push_back(ordernumber);
    int count_number = 0;
    for (int i = 0; i < (int) list_gaforder_index.size(); i++) {
        CCLOG("list_gaforder_index[%d] = %d = %d", i, list_gaforder_index[i], (list_gaforder_index[i]+kTagGafPlay)*100+i);
        if (list_gaforder_index[i] == ordernumber) {
            count_number++;
            map_gaf_orderindex[StringUtils::toString(list_gaforder_index[i])] = count_number;
        }else{
            ordernumber = list_gaforder_index[i];
            list_gaforder_number.push_back(ordernumber);
            count_number = 1;
            map_gaf_orderindex[StringUtils::toString(list_gaforder_index[i])] = count_number;
        }
    }
    
    CCLOG("Dump = %s | size_numbers = %d", JsonDump::getInstance()->MapToString(map_gaf_orderindex).c_str(), (int)list_gaforder_number.size());
}

void TruyenTranhPage::playGAFOrderCallback(){
    auto num_tag_gaf = map_gaf_orderindex[StringUtils::toString(list_gaforder_number[index_list_gaf])].asInt();
    if (count_gafonturn < num_tag_gaf-1) {
        count_gafonturn++;
        return;
    } else {
        index_list_gaf++;
        count_gafonturn = 0;
    }
    CCLOG("index_list_gaf = %d", index_list_gaf);
    if (index_list_gaf >= (int)list_gaforder_number.size()) {
        //playdone
        _enable_first = true;
        if (XHSLib->story_play_type != XHSLib->AUTOPLAY) {
            onStartEffectNV();
        }else{
            this->runAction(Sequence::create(DelayTime::create(1.0f), CallFunc::create([=]{
                onNextPage(nullptr);
            }), NULL));
        }
    }else{
        playGAFWithOrder();
    }
}

void TruyenTranhPage::playGAFWithOrder(){
    if (index_list_gaf >= (int)list_gaforder_number.size()) return;
    auto num_tag_gaf = map_gaf_orderindex[StringUtils::toString(list_gaforder_number[index_list_gaf])].asInt();
    auto count_index = count_gafplay_done;
    CCLOG("play from = %d to = %d", count_index, count_index + num_tag_gaf - 1);
    for (int i = count_index; i <= count_index + num_tag_gaf - 1; i++) {
        _nvGAFPlay[i]->playGAF();
        count_gafplay_done++;
    }
}

void TruyenTranhPage::onPlayAllGAF(){
    if (_nvGAFPlay.size()  > 0 && XHSLib->story_play_type != XHSLib->READITMYSELF) {
        for (int i = 0; i < (int) _nvGAFPlay.size(); i ++) {
            _nvGAFPlay[i]->playGAF();
        }
    }else{
        onStartEffectNV();
        _enable_touch = true;
        _enable_first = true;
    }
}

void TruyenTranhPage::playGAFWhenMCDone(){
    if (_nvGAFPlay.size()  > 0 && XHSLib->story_play_type != XHSLib->READITMYSELF) {
        if (count_gafplay_done > 0) return;
        playGAFWithOrder();
    }else{
        if (XHSLib->story_play_type == XHSLib->AUTOPLAY) {
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                textStory->nextPage();
            }), NULL));
            
        }else{
            //timer
            
            onStartEffectNV();
            _enable_touch = true;
            _enable_first = true;
        }
    }
}


#pragma mark - StarGuide
void TruyenTranhPage::pauseStarGuide(){
    if ((int)_nvEffect.size() >= 1) {
        for (int i = 0; i < (int)_nvEffect.size(); i ++) {
            _nvEffect[i]->pauseStarEffect();
        }
    }
}

void TruyenTranhPage::resumStarGuide(){
    if ((int)_nvEffect.size() >= 1) {
        for (int i = 0; i < (int)_nvEffect.size(); i ++) {
            _nvEffect[i]->resumStarEffect();
        }
    }
}
#pragma mark - TextNV
void TruyenTranhPage::displayTextNV(std::string _content, float _wtext, Vec2 _posText, float _duration){
    auto label = Label::createWithTTF(_content, XHSLib->fontNameStory, 50);
    this->addChild(label, 10010);
    label->enableOutline(Color4B::BLACK, 2);
    
    if (label->getBoundingBox().size.width > 400) {
        _wtext = 300;
        label->setWidth(_wtext);
    }
    
    CCLOG("Truoc = %f", _wtext);
    if (label->getStringNumLines() > 2) {
        _wtext +=_wtext*0.3;
        label->setWidth(_wtext);
    }
    
    if (label->getStringNumLines() > 2) {
        _wtext +=_wtext*0.3;
        label->setWidth(_wtext);
    }
    
    CCLOG("Sau = %f", _wtext);
    
    auto _convertPos = this->convertToWorldSpace(_posText);
    auto _anchorPoint = Vec2(0.5, 0.5);
    auto _sosanh = 75;
    TextVAlignment canH = TextVAlignment::CENTER;
    TextHAlignment canW = TextHAlignment::CENTER;
    if (_convertPos.x - label->getBoundingBox().size.width/2 < _sosanh) {
        _anchorPoint.x = 0;
        canW = TextHAlignment::LEFT;
    }
    
    if ((_convertPos.x + label->getBoundingBox().size.width/2) > (XHSLib->getSizeScreen().width - _sosanh)) {
        _anchorPoint.x = 1;
        canW = TextHAlignment::RIGHT;
    }
    
    if (_convertPos.y - label->getBoundingBox().size.height/2 < _sosanh) {
        _anchorPoint.y = 0;
        canH = TextVAlignment::BOTTOM;
    }
    
    if ((_convertPos.y + label->getBoundingBox().size.height/2) > (XHSLib->getSizeScreen().height - _sosanh)) {
        _anchorPoint.y = 1;
        canH = TextVAlignment::TOP;
    }
    label->setScale(0);
    label->setPosition(_convertPos);
    label->setAnchorPoint(_anchorPoint);
    auto _rotation = random(0, 10);
    if (random(0, 1) == 0) {
        _rotation = -1*_rotation;
    }
    label->setRotation(_rotation);
    
    
    if (_duration == 0) {
        _duration = 1;
    }
    label->setVerticalAlignment(canH);
    label->setHorizontalAlignment(canW);

    auto lb_size = label->getContentSize();
    CCLOG("Label Size: %f, %f", lb_size.width, lb_size.height);

    auto bg = ui::ImageView::create("mjstory/text_story_background.png");
    bg->setScale9Enabled(true);
    bg->setCapInsets(Rect(14, 14, 2, 2));
    bg->setContentSize(lb_size + Size(20, 20));
    bg->setAnchorPoint(label->getAnchorPoint());
    bg->setPosition(label->getPosition());
    bg->setScale(label->getScale());
    bg->setRotation(label->getRotation());
    bg->setOpacity(150);

    Vec2 translation_vector = Vec2::ZERO;
    if (label->getVerticalAlignment() == TextVAlignment::TOP) {
        translation_vector = translation_vector + Vec2(0, 10);
    }
    if (label->getVerticalAlignment() == TextVAlignment::BOTTOM) {
        translation_vector = translation_vector + Vec2(0, -10);
    }
    if (label->getHorizontalAlignment() == TextHAlignment::LEFT) {
        translation_vector = translation_vector + Vec2(-10, 0);
    }
    if (label->getHorizontalAlignment() == TextHAlignment::RIGHT) {
        translation_vector = translation_vector + Vec2(10, 0);
    }
    bg->setPosition(bg->getPosition() + translation_vector);

    this->addChild(bg, 10009);
    
//    LayerColor *lcolor = LayerColor::create(Color4B(100, 100, 100, 100));
//    lcolor->setContentSize(Size(_wtext, 100));
//    this->addChild(lcolor, 10011);
//    lcolor->setPosition(_convertPos);
//    lcolor->setAnchorPoint(_anchorPoint);

//    _duration+=0.4f;
    auto _scaleDr = 0.25f;
    label->runAction(Sequence::create(Spawn::create(ScaleTo::create(_scaleDr, 1.0f), MoveBy::create(_scaleDr, Vec2(5, 5)), NULL),DelayTime::create(_duration), FadeOut::create(0.25f),CallFunc::create([=]{
        label->removeFromParent();
//        lcolor->removeFromParent();
    }), NULL));

    bg->runAction(Sequence::create(Spawn::create(ScaleTo::create(_scaleDr, 1.0f), MoveBy::create(_scaleDr, Vec2(5, 5)), NULL),DelayTime::create(_duration), FadeOut::create(0.25f),CallFunc::create([=]{
        bg->removeFromParent();
        //        lcolor->removeFromParent();
    }), NULL));
}

void TruyenTranhPage::displayEffectTextMC(std::string _str_object, float duration){
    if (_str_object == "") return;
    if (textStory) {
        vector<string> _list_object;
        for (auto g:XHSLib->_list_bridge_text) {
            if (std::string(g.first) == _str_object) {
                auto arrText = XHSLib->_list_bridge_text[g.first].asValueVector();
                for (int i = 0; i < (int) arrText.size(); i++) {
                    _list_object.push_back(arrText[i].asString());
                }
            }
        }
        
        _list_object.push_back(_str_object);
        
        for (int i = 0; i < (int) _list_object.size(); i++) {
            textStory->playTextMCEffect(_list_object[i], duration);
        }
    }
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback TruyenTranhPage::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(TruyenTranhPage, onShow),
//        CLICK_MAP(TruyenTranhPage, onRefresh),
        
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void TruyenTranhPage::onExit(){
    Layout::onExit();
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void TruyenTranhPage::display_cheat_playAllGaf(){
    if (!CONFIG_MANAGER->isTestStory()) return;
    auto _layout = Layout::create();
    _layout->setTouchEnabled(false);
//    _layout->addClickEventListener(CC_CALLBACK_1(TruyenTranhPage::play_cheat_allgaf, this));
    _layout->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    _layout->setBackGroundColor(Color3B::ORANGE);
    if (mapPage["textstory"].isNull() || mapPage["textstory"].getType() != Value::Type::VECTOR) {
        return;
    }
    //    this->addChild(_layout, 100000);
    auto _textMCObj = TextMCObj(mapPage["textstory"].asValueVector(), story_type);
    if (_textMCObj.textMC[0].isNull() || _textMCObj.textMC[0].getType() != Value::Type::VECTOR) {
        return;
    }
    auto mapText = _textMCObj.textMC[0].asValueVector()[0].asValueMap();
    auto rectBox = RectFromString(mapText["boundingbox"].asString().c_str());
    Vec2 _anchor, _pos;
    _anchor = Vec2(0.5, 0.5);
    _pos = Vec2(rectBox.origin.x + rectBox.size.width*0.5 + (XHSLib->getSizeScreen().width - 1024)/2, rectBox.origin.y + rectBox.size.height*0.5);
    _layout->setContentSize(rectBox.size);
    _layout->setAnchorPoint(_anchor);
    _layout->setPosition(_pos);
    _layout->setOpacity(100);
}

void TruyenTranhPage::play_cheat_allgaf(cocos2d::Ref *sender){
    if (count_gafplay_done > 0) return;
    AUDIO_ENGINE->stopAllEffects();
    playGAFWithOrder();
    dynamic_cast<Layout *>(sender)->setVisible(false);
}

void TruyenTranhPage::stopAllActionAndEffect(){
    this->stopAllActions();
    AUDIO_ENGINE->stopAllEffects();
    this->unscheduleAllCallbacks();
    
    for (int i = 0; i < (int)_nvLayout.size(); i++) {
        _nvLayout[i]->stopAllActionAndGaf();
        _nvLayout[i]->pauseStarEffect();
    }
    
    for (int i = 0; i < (int) _nvGAFEffect.size(); i++) {
        _nvGAFEffect[i]->stopAllActionAndGaf();
    }
}

#pragma mark - PAUSE_ALL_ANIMATION_SOUND
void TruyenTranhPage::onPauseAllAnimationAndSound(){
    CCLOG("onpause all animation and sound!");
    textStory->stopAllActionAndMC();
    stopAllActionAndEffect();
    
}
