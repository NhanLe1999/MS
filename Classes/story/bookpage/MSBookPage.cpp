
#include "MSBookPage.h"
#include "HSAudioEngine.h"
#include "JsonDump.h"
#include "MJHelper.h"
#include "MSBookConfig.h"
#include "HSLib.h"
#include "MSStoryGuide.h"
#include "MSEventLogger.h"
#include "MJDefinitions.h"

INITIALIZE_READER(MSBookPage);
#define speed_opacity 15
USING_NS_CC;
std::once_flag msbookpage_reader;
#define CSB_NAME "csb/book_page/MSBookPage.csb"

MSBookPage * MSBookPage::createPage(int story_id, int lang_id, int page_id) {
    std::call_once(msbookpage_reader, [] {
        REGISTER_CSB_READER(MSBookPage);
    });
    auto p = static_cast<MSBookPage *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(story_id, lang_id, page_id);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MSBookPage::didLoadFromCSB(int story_id, int lang_id, int page_id) {
    this->setClippingEnabled(true);

    
    _pageid = page_id;
    _is_dragging = false;
    _guide = MSStoryGuide::createGuide();
    this->addChild(_guide, 1000000);
    
    auto json_file = XHSLib->getFilePathStory(cocos2d::StringUtils::format("%d_%d_%d.json", story_id, lang_id, page_id));

//    CCLOG("json_file = %s", json_file.c_str());
    auto buffer_string = FileUtils::getInstance()->getStringFromFile(json_file);
    rapidjson::Document value_data;
    bool is_convert_plist_2_json = false;
    if (is_convert_plist_2_json) {
        auto value_map = FileUtils::getInstance()->getValueMapFromFile("plist/msbookpage.plist");
        buffer_string = JsonDump::getInstance()->MapToString(value_map);
    }
    
    value_data.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    if (value_data.HasParseError()) {
        CCLOG("JSON ERROR! \n\n %s", buffer_string.c_str());
        return;
    }
    // log
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    value_data.Accept(writer);
    //CCLOG("JSON - Page %d: \n%s", XHSLib->pageIndex, rj_buffer.GetString());
#endif
    generateData(value_data.GetObject());
    
    addBackgroud();
    addObject();
    addTextMC();
    
    if (XHSLib->story_play_type == XHSLib->AUTOPLAY) {
        XHSLib->enable_keyboard = false;
    }
    else{
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
    }

	calculateStandardTime();
}

bool MSBookPage::hasNextBoxText() {
    return book_text->hasNextBoxText();
}

bool MSBookPage::hasPrevBoxText() {
    return book_text->hasPrevBoxText();
}

int MSBookPage::getPageIndex() {
    return _pageid;
}

float MSBookPage::getStandardTime() {

    return m_standardTime;
}

int MSBookPage::getGAFClickedNum() {
    return m_gafClickedNum;
}

int MSBookPage::getIMGClickedNum() {
    return m_imgClickedNum;
}

void MSBookPage::enableInteraction() {
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

void MSBookPage::disableInteraction() {
    this->setTouchEnabled(false);
    this->getEventDispatcher()->removeEventListenersForType(EventListener::Type::KEYBOARD);
    this->enumerateChildren("//.*", [](Node * child) {
        if (child->getName() != "particle") {
            child->pause();
        }
        return false;
    });
}

void MSBookPage::startPlaying() {

    CRASH_AUTO_LOG;
    if (book_text != nullptr) {
        book_text->startHighlightText();
    }
	else {
		return;
	}
    
    for (auto o : list_object_ani) {
        o->startPlaying();
    }

    for (auto o : list_object_touch) {
        o->startPlaying();
    }

    for (auto o : list_guide_effect) {
        o->startPlaying();
    }

    for (auto o : list_ani_effect) {
        o->startPlaying();
    }

    MS_LOGGER.logEvent("COMPLETE_startPlaying", {});
}

void MSBookPage::onEnter() {

    cocos2d::ui::Layout::onEnter();
    
    cocos2d::Device::setKeepScreenOn(true);
    _original_transform = this->getNodeToParentTransform();
    Director::getInstance()->getScheduler()->schedule([](float) {
        cocos2d::Device::setKeepScreenOn(false);
    }, Application::getInstance(), 0, 0, 60.f, false, "delay_screen_on");
    
//    loadPriceCoin();
    
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

#pragma mark - Add Object Item
void MSBookPage::addBackgroud(){
    background_layout = Layout::create();
    auto pos_bg = cocos2d::PointFromString(book_data.book_bg_img.position);
    auto pos_convert = Vec2(512 - pos_bg.x, 384 - pos_bg.y);
    auto bg_img = ImageView::create(book_data.book_bg_img.path);
    background_layout->setContentSize(bg_img->getContentSize());
    background_layout->addChild(bg_img);
    bg_img->setAnchorPoint(Vec2(0.5,0.5));
    bg_img->setPosition(bg_img->getContentSize()/2);
    if (book_data.box_type == TYPE_BOX_0) {
        background_layout->setPosition(Vec2(XHSLib->getSizeScreen().width*0.75, XHSLib->getSizeScreen().height/2));
    }else{
        background_layout->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2) - pos_convert);
    }
    background_layout->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(background_layout);
	fixSizeDisplay();
}

void MSBookPage::addObject(){
    for (int i = 0; i < (int) book_data.list_image.size(); i++) {
        auto object_item = MSBookObject::createObject(book_data.list_image[i]);
        if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isTestStory()) {
            if (!object_item->init_gaf_done) {
                continue;
            }
        }
		object_item->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
		object_item->setBackGroundColor(Color3B::BLACK);
		object_item->setBackGroundColorOpacity(0x44);
        object_item->setPosition(PointFromString(book_data.list_image[i].position));
        object_item->setDelegate(this);
        object_item->setSwallowTouches(true);
        
        if (book_data.list_image[i].touchable) {
            if (book_data.list_image[i].type == OBJECT_TYPE_ANIMATION) {
                if (book_data.list_image[i].animation_type == ANI_TYPE_END) {
                    list_object_ani.push_back(object_item);
                }else if (book_data.list_image[i].animation_type == ANI_TYPE_SEQUENCE){
                    total_gaf_sequence++;
                }
            }
            if ((int)book_data.list_image[i].list_touch.size() > 0 && object_item->check_have_star) {
                list_guide_effect.push_back(object_item);
            }
            
            list_object_touch.push_back(object_item);
        }else if (book_data.list_image[i].type == OBJECT_TYPE_ANIMATION){
            list_ani_effect.push_back(object_item);
        }
        
        background_layout->addChild(object_item, book_data.list_image[i].z_order);

    }
    
    if ((int)list_object_touch.size() >= 2) {
        sort( list_object_touch.begin( ), list_object_touch.end( ), [](MSBookObject* lhs, MSBookObject* rhs )
             {
                 return lhs->_book_image_info.z_order > rhs->_book_image_info.z_order;
             });
    }
    
    if (list_guide_effect.size() >= 2) {
        sort( list_guide_effect.begin( ), list_guide_effect.end( ), [](MSBookObject* lhs, MSBookObject* rhs )
             {
                 return lhs->_book_image_info.star_order > rhs->_book_image_info.star_order;
             });
    }
    
    if (CONFIG_MANAGER->isTestStory()) {
        for (int i = 0; i < (int)list_guide_effect.size(); i++) {
            CCLOG("star_order = %s", list_guide_effect[i]->_book_image_info.audio_info.text.c_str());
        }
    }
    
    if ((int)list_guide_effect.size() >= 1) {
        list_guide_effect[0]->isEffectEnable = true;
    }
    
    if ((int)list_object_ani.size() >= 2) {
        sort( list_object_ani.begin( ), list_object_ani.end( ), [](MSBookObject* lhs, MSBookObject* rhs )
             {
                 return lhs->_book_image_info.animation_order < rhs->_book_image_info.animation_order;
             });
    }
    
    auto count_gafplay = 0;

	m_totalTimeGAF = 0.0f;

    for (int i = 0; i < (int) list_object_ani.size(); i++) {
//        if (list_object_ani[i]->_book_image_info.animation_type != ANI_TYPE_END) continue;
        auto tag_gafplay = (list_object_ani[i]->_book_image_info.animation_order+2)*100+count_gafplay;
        list_object_ani[i]->setTag(tag_gafplay);
        CCLOG("tag_gaf = %d", tag_gafplay);
        count_gafplay++;
        list_gaforder_index.push_back(list_object_ani[i]->_book_image_info.animation_order);

		//calculate gaf time for tracking

        if(auto aniObject = list_object_ani[i])
        {
            if (auto animation = aniObject->getAnimation())
            {
                if (auto gaf = animation->getGAFObject())
                {
					m_totalTimeGAF = std::max(m_totalTimeGAF,((float)gaf->getTotalFrameCount()/(float)gaf->getFps()));
                }
            }
        }
    }

	m_standardTime += m_totalTimeGAF;

	m_totalTimeGAF = 0.0f; //reset

	//calculate deylay time for autoplay mode.
	for (size_t i = 0; i < list_object_touch.size(); i++)
	{
		if (auto aniObject = list_object_touch[i])
		{
			if (auto animation = aniObject->getAnimation())
			{
				if (auto gaf = animation->getGAFObject())
				{
					m_totalTimeGAF = std::max(m_totalTimeGAF, ((float)gaf->getTotalFrameCount() / (float)gaf->getFps()));
				}
			}
		}
	}

	m_totalTimeGAF = std::max(m_totalTimeGAF, 0.5f);//the default deylay time = 0.5s for autoplay mode.

    dumpGafOrderToMap();
}

void MSBookPage::addTextMC(){
    book_text = MSBookText::createBookText(book_data.list_text, book_data.list_audio, book_data.box_type);
    this->addChild(book_text, 1000);
    book_text->setDelegate(this);
}

void MSBookPage::onRunAniObjectWithMCDone(){
    playGAFOrderCallback();
}

void MSBookPage::onRunAniWithSequenceDone(){
    if (XHSLib->story_play_type == XHSLib->AUTOPLAY) return;
    count_gaf_sequence_done++;
    if (count_gaf_sequence_done == total_gaf_sequence && isHighlightDone && !is_start_star_effect){
        is_start_star_effect = true;
        onStartEffectNV();
    }
}

void MSBookPage::dumpGafOrderToMap(){
    if ((int)list_gaforder_index.size() <= 0) return;
    sort( list_gaforder_index.begin( ), list_gaforder_index.end( ), [](int lhs, int rhs )
         {
             return lhs < rhs;
         });
    auto ordernumber = list_gaforder_index[0];
    list_gaforder_number.push_back(ordernumber);
    int count_number = 0;
    for (int i = 0; i < (int) list_gaforder_index.size(); i++) {
        CCLOG("list_gaforder_index[%d] = %d = %d", i, list_gaforder_index[i], (list_gaforder_index[i]+2)*100+i);
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

void MSBookPage::playGAFOrderCallback(){
    auto num_tag_gaf = map_gaf_orderindex[StringUtils::toString(list_gaforder_number[index_list_gaf])].asInt();
    if (count_gafonturn < num_tag_gaf-1) {
        count_gafonturn++;
        return;
    }else {
        index_list_gaf++;
        count_gafonturn = 0;
    }
    CCLOG("index_list_gaf = %d", index_list_gaf);
    if (index_list_gaf >= (int)list_gaforder_number.size()) {
        //playdone
//        _enable_first = true;
        if (XHSLib->story_play_type != XHSLib->AUTOPLAY) {
            onStartEffectNV();
        }else{
            this->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]{
                onNextPage();
            }), NULL));
        }
    }else{
        playGAFWithOrder();
    }
}

void MSBookPage::playGAFWithOrder(){
    if (index_list_gaf >= (int)list_gaforder_number.size()) return;
    auto num_tag_gaf = map_gaf_orderindex[StringUtils::toString(list_gaforder_number[index_list_gaf])].asInt();
    auto count_index = count_gafplay_done;
    CCLOG("play from = %d to = %d", count_index, count_index + num_tag_gaf - 1);
    for (int i = count_index; i <= count_index + num_tag_gaf - 1; i++) {
        list_object_ani[i]->runAniWithMC();
        count_gafplay_done++;
    }
}

#pragma mark - StarGuide
void MSBookPage::onActionGuideEffectObject(std::string action){
    if (action == "pause"){
        pauseStarGuide();
        return;
    }
    
    if (action == "next"){
        onNextEffectNV();
        return;
    }
    
    if (action == "resum"){
        resumStarGuide();
        return;
    }
}

void MSBookPage::onStartEffectNV(){
    _guide->startTimer();
    
    touch_object_enable = true;
    for (int i = 0; i < (int) list_object_touch.size(); i++) {
        list_object_touch[i]->touch_enable = true;
    }
    onNextEffectNV();
}
void MSBookPage::onNextEffectNV(){
//    CCLOG("size = %d", (int)list_guide_effect.size());
    if (list_guide_effect.size() >= 1) {
        auto hsAni = list_guide_effect[0];
        if (hsAni->isEffectEnable) {
            hsAni->isEffectEnable = false;
            list_guide_effect.erase(std::remove_if(list_guide_effect.begin(), list_guide_effect.end(), [=](MSBookObject *pX) {
                return pX->isFirstTouch == true;
            }), list_guide_effect.end());
//            CCLOG("size = %d", (int)list_guide_effect.size());
            
            if (list_guide_effect.size() >= 1) {
                list_guide_effect[0]->isEffectEnable = true;
//                if (isHighlightDone) {
                    list_guide_effect[0]->startEffect();
//                }
            }
        }
    }
}

void MSBookPage::pauseStarGuide(){
    count_gaf_play++;
    if ((int)list_guide_effect.size() >= 1) {
        for (int i = 0; i < (int)list_guide_effect.size(); i ++) {
            list_guide_effect[i]->pauseStarEffect();
        }
    }
}

void MSBookPage::resumStarGuide(){
//    _enable_touch = true;
    count_gaf_done++;
    if (count_gaf_done >= count_gaf_play) {
        count_gaf_play = count_gaf_done = 0;
    }else
        return;
    
    if ((int)list_guide_effect.size() >= 1) {
        for (int i = 0; i < (int)list_guide_effect.size(); i ++) {
            list_guide_effect[i]->resumStarEffect();
        }
    }
}

#pragma mark -- TOUCH
void MSBookPage::onTouchBegan() {
    CCLOG("touch began.");
//    touch_pos = touch->getLocation();
    
//    return true;
}

void MSBookPage::transformPageByAngle(double angle) {
    auto transform = _original_transform;
    transform.rotateY(angle);
    auto scale_x = (M_PI / 3 + angle) / (M_PI / 3);
    transform.scale(scale_x, 1, 1);
    
    this->setNodeToParentTransform(transform);
}

void MSBookPage::transformPageTurn(double distance) {
    auto ratio = (distance - 150) / (Director::getInstance()->getVisibleSize().width - 400);
    _angle = - M_PI / 3 * ratio;
//    CCLOG("transform distance: %f", distance);
//    CCLOG("transform ratio: %f", ratio);
//    CCLOG("transform angle: %f", _angle);
    
    transformPageByAngle(_angle);
}

void MSBookPage::onTouchMoved() {
//    auto touch_end = touch->getLocation();
//    if (_is_dragging) {
//        transformPageTurn(fabs(touch_end.x - touch_pos.x));
//        return;
//    }
//
//    if (touch_end.x >= 0 && touch_end.y <= XHSLib->getSizeScreen().width) {
//        if (touch_end.x - touch_pos.x < -150) {
//            _is_dragging = true;
//            auto s = fabs(touch_end.x - touch_pos.x);
//            transformPageTurn(s);
//            return;
//        }
//    }
}

void MSBookPage::onTouchEnded() {
    if (_is_dragging) {
        _is_dragging = false;
        this->runAction(ActionFloat::create(0.5, _angle, 0, [this](float v) {
            this->transformPageByAngle(v);
        }));
        return;
    }
    
    auto touch_end = this->getTouchEndPosition();
    auto touch_pos = this->getTouchBeganPosition();
    if (touch_end.y <= XHSLib->getSizeScreen().width) {
        if (touch_end.x - touch_pos.x < -250) {
            onNextPage();
            return;
        }
        
        if (touch_end.x - touch_pos.x > 250) {
            onBackPage();
            return;
        }
        
        if (touch_pos.y - touch_end.y > 200) {
            onRefresh();
            return;
        }
        
        
        
        auto isTouchFail = (touch_end.x > XHSLib->getSizeScreen().width/2 && touch_pos.x > XHSLib->getSizeScreen().width/2)?false:true;
        if (book_data.box_type != TYPE_BOX_0) {
            isTouchFail = false;
        }
        
        if (touch_end.distance(touch_pos) < 10 && !isTouchFail) {
            for (int i = 0 ; i < (int) list_object_touch.size(); i++){
                for (int j = 0 ; j < (int) list_object_touch[i]->list_touch_box.size(); j++){
                    auto bb = list_object_touch[i]->list_touch_box[j]->getBoundingBox();
                    auto te = list_object_touch[i]->convertToNodeSpace(touch_end);
                    
                    auto pl = list_object_touch[i]->list_touch_box[j]->getVertices();
                    if (pl.empty()) {
                        if (bb.containsPoint(te)) {
                            list_object_touch[i]->playAudioObject(touch_end);
                            _guide->startTimer();
                            return;
                        }
                    } else {
                        if (mj::helper::isPointInPoly(te, pl)) {
                            list_object_touch[i]->playAudioObject(touch_end);
                            _guide->startTimer();
                            return;
                        }
                    }
                }
            }
        }
    }
}

void MSBookPage::onNextPage(){
    if (book_text->onNextBoxText()) {
        CCLOG("onNextBox");
        auto layout_disable_touch = Layout::create();
        layout_disable_touch->setContentSize(this->getContentSize());
        layout_disable_touch->setTouchEnabled(true);
        layout_disable_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
        layout_disable_touch->setPosition(Vec2(0, 0));
        Director::getInstance()->getRunningScene()->addChild(layout_disable_touch, 99999999);
        layout_disable_touch->runAction(Sequence::create(DelayTime::create(0.5), RemoveSelf::create(), NULL));
    }else {
        CCLOG("onNextPage");
        
//        auto layout_disable_touch = Layout::create();
//        layout_disable_touch->setContentSize(this->getContentSize());
//        layout_disable_touch->setTouchEnabled(true);
//        layout_disable_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        layout_disable_touch->setPosition(Vec2(0, 0));
//        this->addChild(layout_disable_touch, 99999999);
//
//        this->unscheduleAllCallbacks();
//        this->unscheduleUpdate();
//        this->stopAllActions();
        book_text->resetBox();
        auto index_endpage = XHSLib->pageNumAll;
        if (CONFIG_MANAGER->isTestGame()) {
            index_endpage = 1;
        }
        if (XHSLib->pageIndex >= index_endpage) {//XHSLib->pageNumAll
            stopAllActionAndEffect();
        }
        XHSLib->pageIndex++;
        
        EventCustom e("mjstory.replace_page");
        int * edata = new int;
        *edata = 1;
        e.setUserData(edata);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    }
}

void MSBookPage::onBackPage(){
    if (book_text->onBackBoxText()) {
        CCLOG("onBackBox");
    }else {
        CCLOG("onBackPage");
        if (XHSLib->pageIndex == 1) {
            return;
        }
        else
        {
            int totalTurnPreviousPage = MJDEFAULT->getIntegerForKey(key_turn_previous_page, 0) + 1;
            MJDEFAULT->setIntegerForKey(key_turn_previous_page, totalTurnPreviousPage);
        }
        
//        auto layout_disable_touch = Layout::create();
//        layout_disable_touch->setContentSize(this->getContentSize());
//        layout_disable_touch->setTouchEnabled(true);
//        layout_disable_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
//        layout_disable_touch->setPosition(Vec2(0, 0));
//        this->addChild(layout_disable_touch, 99999999);
        
        
//        this->unscheduleAllCallbacks();
//        this->unscheduleUpdate();
//        this->stopAllActions();
        
        XHSLib->pageIndex--;
        
        EventCustom e("mjstory.replace_page");
        int * edata = new int;
        *edata = 2;
        e.setUserData(edata);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    }
}

void MSBookPage::onRefresh(){
    CCLOG("onRefresh");
    auto layout_disable_touch = Layout::create();
    layout_disable_touch->setContentSize(this->getContentSize());
    layout_disable_touch->setTouchEnabled(true);
    layout_disable_touch->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    layout_disable_touch->setPosition(Vec2(0, 0));
    this->addChild(layout_disable_touch, 99999999);
    
    book_text->stopAllActionAndSound();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    this->stopAllActions();
    AUDIO_ENGINE->stopAllEffects();
    EventCustom e("mjstory.replace_page");
    int * edata = new int;
    *edata = 3;
    e.setUserData(edata);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    
}

void MSBookPage::stopAllActionAndEffect() {
    this->stopAllActions();
    AUDIO_ENGINE->stopAllEffects();
    this->unscheduleAllCallbacks();
    this->unscheduleUpdate();
    for (int i = 0; i < (int) list_object_touch.size(); i++) {
        list_object_touch[i]->stopAllActionAndEffect();
    }
    for (int i = 0; i < (int) list_ani_effect.size(); i++) {
        list_ani_effect[i]->stopAllActionAndEffect();
    }
}

#pragma mark - Delegate
void MSBookPage::onDoneHighlightTextMC(){
    isHighlightDone = true;
    if ((int)list_object_ani.size() > 0 && XHSLib->story_play_type != XHSLib->READITMYSELF) {
        playGAFWithOrder();
    }else {
        if (XHSLib->story_play_type == XHSLib->AUTOPLAY) {
            this->runAction(Sequence::create(DelayTime::create(m_isGAFPlayedFirst ? 0.5f : m_totalTimeGAF), CallFunc::create([=]{
                onNextPage();
            }), NULL));
         }else{
            if (XHSLib->story_play_type == XHSLib->READITMYSELF || total_gaf_sequence <= 0) {
                onStartEffectNV();
            }else {
                if (count_gaf_sequence_done == total_gaf_sequence && !is_start_star_effect){
                    is_start_star_effect = true;
                    onStartEffectNV();
                }
            }
        }
    }
}

void MSBookPage::onTouchObject(std::string _content, float _wtext, cocos2d::Vec2 _posText, float _duration,mj::BOOK_IMAGE_INFO i_info){

    (i_info.type == OBJECT_TYPE_ANIMATION) ? m_gafClickedNum++ : m_imgClickedNum++;

    //CCLOG("XXXX|MSBookPage::onTouchObject() type:%s m_gafClickedNum:%d m_imgClickedNum:%d",i_info.type.c_str(),m_gafClickedNum,m_imgClickedNum);

    displayTextNV(_content, _wtext, _posText, _duration);
}

#pragma mark - KEY_BOARD
void MSBookPage::onKeyboardPage(int index_keyboard){
    if (!XHSLib->enable_keyboard || XHSLib->show_popup_subscribe) return;
    XHSLib->enable_keyboard = false;
    CCLOG("\n\n Vào nào vào nào");
    switch (index_keyboard) {
        case 1://next
            if (XHSLib->pageIndex > XHSLib->pageNumAll) return;
            onNextPage();
            if (!XHSLib->enable_keyboard) {
                DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=]{
                    XHSLib->enable_keyboard = true;
                }), NULL));
            }
            break;
        case 2://back
            onBackPage();
            if (!XHSLib->enable_keyboard) {
                DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=]{
                    XHSLib->enable_keyboard = true;
                }), NULL));
            }
            break;
        case 3://refresh
            CCLOG("refresh_refresh_refresh_refresh_refresh_refresh \n\n");
            onRefresh();
            DIRECTOR->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.65f), CallFunc::create([=]{
                XHSLib->enable_keyboard = true;
            }), NULL));
            break;
        default:
            break;
    }
}


#pragma mark - TextNV
void MSBookPage::displayTextNV(std::string _content, float _wtext, Vec2 _posText, float _duration){
    auto screen_size = Director::getInstance()->getWinSize();
//    CCLOG("font = %s", MSBookConfig::getInstance()->getFontNameBook().c_str());
    auto label = Label::createWithTTF(_content, MSBookConfig::getInstance()->getFontNameBook(), 50);
    this->addChild(label, 10010);
    label->enableOutline(Color4B::BLACK, 2);
    
    if (label->getBoundingBox().size.width > 400) {
        _wtext = 350;
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
    
    if ((_convertPos.x + label->getBoundingBox().size.width/2) > (screen_size.width - _sosanh)) {
        _anchorPoint.x = 1;
        canW = TextHAlignment::RIGHT;
    }
    
    if (_convertPos.y - label->getBoundingBox().size.height/2 < _sosanh) {
        _anchorPoint.y = 0;
        canH = TextVAlignment::BOTTOM;
    }
    
    if ((_convertPos.y + label->getBoundingBox().size.height/2) > (screen_size.height - _sosanh)) {
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
    
    displayEffectTextMC(_content, _duration);
}

void MSBookPage::displayEffectTextMC(std::string _str_object, float duration){
    if (_str_object == "") return;
    if (book_text) {
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
            book_text->playTextMCEffect(_list_object[i], duration);
        }
    }
}

void MSBookPage::onPauseAllAnimationAndSound(){
//    touch
    book_text->stopAllActionAndSound();
    stopAllActionAndEffect();
}

#pragma callback
ui::Widget::ccWidgetClickCallback MSBookPage::onLocateClickCallback(const std::string &callback_name) {
//    if (callback_name == "onRefresh") {
//        return CC_CALLBACK_1(MSBookPage::onRefresh, this);
//    }
    return nullptr;
}

void MSBookPage::onExitTransitionDidStart(){
    cocos2d::ui::Layout::onExitTransitionDidStart();
    Director::getInstance()->getScheduler()->schedule([](float) {
        SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
        Director::getInstance()->getTextureCache()->removeUnusedTextures();
    }, Application::getInstance(), 0, 0, 1.25, false, "remove_cached_textures_stories");
}

void MSBookPage::generateData(rapidjson::Value value_data){
    //Text_MC.
    if (value_data.HasMember("box_type") && value_data["box_type"].IsString()){
        book_data.box_type = value_data["box_type"].GetString();
    }
    if (value_data.HasMember("audio") && value_data["audio"].IsArray()) {
        auto list_audio = value_data["audio"].GetArray();
        for (int index = 0; index < (int) list_audio.Size(); index ++) {
            mj::BOOK_AUDIO_INFO book_audio_info;
            auto audio_data = list_audio[index].GetObject();
            book_audio_info.path = audio_data["path"].GetString();
            book_audio_info.start = audio_data["start"].GetFloat();
            auto list_sync_data = audio_data["sync"].GetArray();
            for (int i = 0; i < (int) list_sync_data.Size(); i++) {
                auto sync_data = list_sync_data[i].GetObject();
                mj::BOOK_SYNC_TEXT book_sync_text;
                book_sync_text.w = sync_data["w"].GetString();
                book_sync_text.s = sync_data["s"].GetFloat();
                book_sync_text.e = sync_data["e"].GetFloat();
                book_sync_text.ts = sync_data["ts"].GetInt();
                book_sync_text.te = sync_data["te"].GetInt();
                book_audio_info.list_sync.push_back(book_sync_text);
            }
            
            if (JSON_SAFE_CHECK(audio_data, "pause", Array)) {
                auto arr_pause = audio_data["pause"].GetArray();
                for (auto i = 0; i < arr_pause.Size(); ++i) {
                    if (!arr_pause[i].IsObject()) continue;
                    auto obj_pause = arr_pause[i].GetObject();
                    std::pair<float, float> cf;
                    if (JSON_SAFE_CHECK(obj_pause, "tp", Int) && JSON_SAFE_CHECK(obj_pause, "td", Int)) {
                        cf.first = obj_pause["tp"].GetInt();
                        cf.second = obj_pause["td"].GetInt();
						m_isGAFPlayedFirst = true;

                        book_audio_info.delay_config.push_back(cf);
                    }
                }
            }
            
            //re-sync
            if (!book_audio_info.delay_config.empty()) {
                auto total_delay_duration = 0;
                auto last_delay_index = 0;
                for (auto i = 0; i < (int)book_audio_info.list_sync.size(); ++i){
                    auto sync_info = &book_audio_info.list_sync[i];
                    while (last_delay_index < (int)book_audio_info.delay_config.size() && book_audio_info.delay_config[last_delay_index].first <= sync_info->s) {
                        total_delay_duration += book_audio_info.delay_config[last_delay_index].second;
                        last_delay_index++;
                    }
                    sync_info->s += total_delay_duration;

                    while (last_delay_index < (int)book_audio_info.delay_config.size() && book_audio_info.delay_config[last_delay_index].first <= sync_info->e) {
                        total_delay_duration += book_audio_info.delay_config[last_delay_index].second;
                        last_delay_index++;
                    }
                    sync_info->e += total_delay_duration;
                }
            }
            
            book_data.list_audio.push_back(book_audio_info);
        }
    }
    
    std::string normal_color = "{{0, 0}}, {{0, 0}}";
    std::string highlight_color = "{{255, 0}}, {{0, 0}}";
    if (value_data.HasMember("normal_color") && value_data["normal_color"].IsString()) {
        normal_color = value_data["normal_color"].GetString();
    }
    if (value_data.HasMember("highlight_color") && value_data["highlight_color"].IsString()) {
        highlight_color = value_data["highlight_color"].GetString();
    }
    int font_size;
    if (value_data.HasMember("fontsize") && value_data["fontsize"].IsInt()  ) {
         font_size = value_data["fontsize"].GetInt();
    }
    
    int line_space = 0;
    if (value_data.HasMember("line_height") && value_data["line_height"].IsInt()) {
        line_space = value_data["line_height"].GetInt() - font_size;
    } else {
        line_space = font_size;
    }
    
    if (value_data.HasMember("text") && value_data["text"].IsArray()) {
        auto list_text = value_data["text"].GetArray();
        for (int i = 0; i < (int)list_text.Size(); i++) {
            mj::BOOK_TEXT_INFO book_text_info;
            auto text_data = list_text[i].GetObject();
            book_text_info.text = text_data["text"].GetString();
            book_text_info.boundingbox = text_data["boundingbox"].GetString();
            book_text_info.highlight_color = highlight_color;
            book_text_info.normal_color = normal_color;
            
            auto temp = RectFromString(highlight_color);
            book_text_info.highlight_color_3b = Color3B(temp.origin.x, temp.origin.y, temp.size.width);
            temp = RectFromString(normal_color);
            book_text_info.normal_color_3b = Color3B(temp.origin.x, temp.origin.y, temp.size.width);

            book_text_info.line_space = line_space;
            
            book_text_info.start = text_data["start"].GetFloat();
            book_text_info.end = text_data["end"].GetFloat();
            book_text_info.fontsize = font_size;
            
            if (JSON_SAFE_CHECK(text_data, "config_image", Array)) {
                for (int u = 0; u < (int) text_data["config_image"].Size(); u++) {
                    if (text_data["config_image"][u].IsObject()) {
                        mj::BOOK_INLINE_IMAGE inline_image;

                        auto obj = text_data["config_image"][u].GetObject();
                        inline_image.text = JSON_SAFE_CHECK(obj, "text", String) ? obj["text"].GetString() : "";
                        inline_image.image_width = JSON_SAFE_CHECK(obj, "image_width", Int) ? obj["image_width"].GetInt() : 100;
                        inline_image.image_height = JSON_SAFE_CHECK(obj, "image_height", Int) ? obj["image_height"].GetInt() : 100;
                        if (inline_image.image_width == 0) {
                            inline_image.image_width = 125;
                        }
                        if (inline_image.image_height == 0) {
                            inline_image.image_height = 125;
                        }
                        inline_image.image_path = JSON_SAFE_CHECK(obj, "image_path", String) ? obj["image_path"].GetString() : "";
                        inline_image.audio_path = JSON_SAFE_CHECK(obj, "audio_path", String) ? obj["audio_path"].GetString() : "";

                        if (JSON_SAFE_CHECK(obj, "position_num", Array)) {
                            for (auto c = 0; c < obj["position_num"].Size(); ++c) {
                                if (obj["position_num"][c].IsInt()) {
                                    inline_image.position.push_back(obj["position_num"][c].GetInt());
                                }
                            }
                        }

                        book_text_info.inline_images.push_back(inline_image);
                    }
                }
            }

            if (JSON_SAFE_CHECK(text_data, "config_audio", Array)) {
                for (int u = 0; u < (int) text_data["config_audio"].Size(); u++) {
                    if (text_data["config_audio"][u].IsObject()) {
                        mj::BOOK_INLINE_TRANSLATION inline_translation;

                        auto obj = text_data["config_audio"][u].GetObject();
                        inline_translation.text = JSON_SAFE_CHECK(obj, "text", String) ? obj["text"].GetString() : "";
                        inline_translation.audio_path = JSON_SAFE_CHECK(obj, "audio_path", String) ? obj["audio_path"].GetString() : "";
                        inline_translation.translation = JSON_SAFE_CHECK(obj, "desc", String) ? obj["desc"].GetString() : "";
                        inline_translation.translation_path = JSON_SAFE_CHECK(obj, "desc_audio_path", String) ? obj["desc_audio_path"].GetString() : "";

                        if (JSON_SAFE_CHECK(obj, "position_num", Array)) {
                            for (auto c = 0; c < obj["position_num"].Size(); ++c) {
                                if (obj["position_num"][c].IsInt()) {
                                    inline_translation.position.push_back(obj["position_num"][c].GetInt());
                                }
                            }
                        }

                        book_text_info.inline_translations.push_back(inline_translation);
                    }
                }
            }
            
            book_data.list_text.push_back(book_text_info);
        }
    }
    
    //Object
    if (value_data.HasMember("image") && value_data["image"].IsArray()) {
        auto list_image = value_data["image"].GetArray();
        for (int i = 0; i < (int) list_image.Size(); i++) {
            auto image_info = list_image[i].GetObject();
            mj::BOOK_IMAGE_INFO book_image_info;
            book_image_info.path = image_info["path"].GetString();
            book_image_info.type = image_info["type"].GetString();
            book_image_info.position = image_info["position"].GetString();
            book_image_info.contentsize = image_info["contentsize"].GetString();
            book_image_info.animation_type = image_info["animation_type"].GetString();
            book_image_info.effect = image_info["effect"].GetString();
            book_image_info.animation_reset = image_info["animation_reset"].GetBool();
            book_image_info.touchable = image_info["touchable"].GetBool();
            
            book_image_info.z_order = image_info["z_order"].GetInt();
            book_image_info.animation_order = image_info["animation_order"].GetInt();
            book_image_info.star_order = image_info["star_order"].GetInt();
            
            if (image_info.HasMember("audio") && image_info["audio"].IsObject()) {
                auto audio_info = image_info["audio"].GetObject();
                mj::BOOK_IMAGE_AUDIO book_image_audio;
                book_image_audio.path = audio_info["path"].GetString();
                book_image_audio.text = audio_info["text"].GetString();
                book_image_audio.duration = audio_info["duration"].GetFloat();
                book_image_audio.w_text = audio_info["w_text"].GetFloat();
                book_image_info.audio_info = book_image_audio;
            }
            
            if (image_info.HasMember("touch") && image_info["touch"].IsArray()) {
                auto list_touch = image_info["touch"].GetArray();
                for (auto touch_id = 0; touch_id < (int)list_touch.Size(); touch_id++) {
                    auto touch_info = list_touch[touch_id].GetObject();
                    mj::BOOK_IMAGE_TOUCH book_image_touch;
                    if (touch_info.HasMember("boundingbox") && touch_info["boundingbox"].IsString()) {
                        book_image_touch.boundingbox = touch_info["boundingbox"].GetString();
                    }
                    book_image_touch.star_position = touch_info["star_position"].GetString();
                    
                    std::vector<cocos2d::Point> vertices;
                    if (touch_info.HasMember("vertices") && touch_info["vertices"].IsArray()) {
                        auto varr = touch_info["vertices"].GetArray();
                        for (auto i = 0; i < varr.Size(); ++i) {
                            if (!varr[i].IsString()) continue;
                            auto pstr = varr[i].GetString();
                            auto p = cocos2d::PointFromString(pstr);
                            p = p - cocos2d::PointFromString(book_image_info.position) + PointFromString(book_image_info.contentsize) / 2;
                            book_image_touch.vertices.push_back(p);
                        }
                    }
                    
                    book_image_info.list_touch.push_back(book_image_touch);
                }
            }
            
            if (book_image_info.type == OBJECT_TYPE_ANIMATION && book_image_info.animation_type == ANI_TYPE_SEQUENCE) {
                if (image_info.HasMember("sequence") && image_info["sequence"].IsArray()) {
                    auto list_sequence = image_info["sequence"].GetArray();
                    for (auto se_id = 0; se_id < (int) list_sequence.Size(); se_id++) {
                        auto se_info = list_sequence[se_id].GetObject();
                        mj::BOOK_IMAGE_SEQUENCE book_image_se;
                        book_image_se.name = se_info["name"].GetString();
                        book_image_se.start = se_info["start"].GetFloat();
                        book_image_se.end = se_info["end"].GetFloat();
                        book_image_se.loop = se_info["loop"].GetBool();
                        book_image_se.effect = se_info["effect"].GetString();
                        
                        book_image_info.list_sequence.push_back(book_image_se);
                    }
                }
            }
            book_data.list_image.push_back(book_image_info);
        }
    }
    
    auto bg_info = value_data["bg_img"].GetObject();
    book_data.book_bg_img.path = bg_info["path"].GetString();
    book_data.book_bg_img.position = bg_info["position"].GetString();
    
}

void MSBookPage::fixSizeDisplay(){
    //những truyện liên quan đến hình dạng, không cho phép scale vì bị méo hình
    if (XHSLib->_id == 574 || XHSLib->_id == 96 || XHSLib->_id == 894 || XHSLib->_id == 34 || XHSLib->_id == 1494 || XHSLib->_id == 1055) {
        return;
    }
    
    if (background_layout->getContentSize().width < Director::getInstance()->getVisibleSize().width) {
        auto x_ratio = Director::getInstance()->getVisibleSize().width / background_layout->getContentSize().width;
        x_ratio = std::min(x_ratio, 1.2f);
        background_layout->setScaleX(x_ratio);
    }
    
    return;
    
//    if (XHSLib->getSizeScreen().width*1.0/XHSLib->getSizeScreen().height >= 2.1) {
//        background_layout->setScaleX(1.2);
//        return;
//    }
//
//    if (XHSLib->getSizeScreen().width*1.0/XHSLib->getSizeScreen().height >= 1.9) {
//        background_layout->setScaleX(1.1);
//        return;
//    }
//
//    if (XHSLib->getSizeScreen().width*1.0/XHSLib->getSizeScreen().height > 1.82) {
//        background_layout->setScaleX(1.05);
//        return;
//    }

}

void MSBookPage::calculateStandardTime() 
{
	if (!book_data.list_audio.empty())
	{
		for (int i = 0; i < book_data.list_audio.size(); ++i)
		{
			auto audioInfo = book_data.list_audio.at(i);

			if (!audioInfo.list_sync.empty())
			{
				m_standardTime += (audioInfo.list_sync.back().e / 1000.0f);//miliseconds to seconds
			}
			//
		}
	}
	m_standardTime += KHUNG_SAO_DURATION;
}
