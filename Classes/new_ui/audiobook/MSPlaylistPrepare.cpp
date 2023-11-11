//
//  MSPlaylistPrepare.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/27/18.
//

#include "MSPlaylistPrepare.h"
#include "MJStoryInfoModel.h"

#include "MSGradeBox.h"
#include "StoryDataModel.h"
#include "MJDefinitions.h"
#include "MSAudiobookPlayer.h"
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"

#include "MSPlaylistHelper.h"
#include <map>
#include "NativeBridge.h"
#include "RemoteImageView.h"

USING_NS_CC;
INITIALIZE_READER(MSPlaylist_Prepare);

std::once_flag ms_playlist_prepare_reader_flag;

MSPlaylist_Prepare * MSPlaylist_Prepare::createView(std::string name, std::string series_id, std::vector<mj::model::AudiobookInfo> audiobooks,int i_gradeID)
{
    std::call_once(ms_playlist_prepare_reader_flag, []
    {
        REGISTER_CSB_READER(MSPlaylist_Prepare);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Playlist_Prepare.csb");
    
    auto view = reinterpret_cast<MSPlaylist_Prepare *>(CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB(name, series_id, audiobooks,i_gradeID);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return view;
}

void MSPlaylist_Prepare::updatePlaylist() {
    // First clear the playlist
    ms::playlist::ABPlayer::getInstance().clearPlaylist();
    for(auto a : _filtered_audiobooks) {
        ms::playlist::ABPlayer::getInstance().enqueue(a);
    }
}

void MSPlaylist_Prepare::didLoadFromCSB(std::string name, std::string series_id, std::vector<mj::model::AudiobookInfo> audiobooks,int i_gradeID)
{
    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height - 240));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender)
    {
        this->onBack(nullptr);
    });
    this->addChild(fake_layout);
    
    _name = name;
    _audiobooks = audiobooks;
    
    _filtered_audiobooks = _audiobooks;

    m_scope = ms::GradeManager::getInstance().getScope();

    auto title = utils::findChild<ui::Text *>(this, "title");
    
    title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(name));
       
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "title"),
            utils::findChild<ui::Text *>(this, "lb.playlist.play"),
            utils::findChild<ui::Text *>(this, "lb.playlist.for"),
            utils::findChild<ui::Text *>(this, "selected_duration"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        }
    }
    
    onChangeDisplayLanguage();
    
    auto button_play = utils::findChild<ui::Button *>(this, "button_play");
    button_play->setCascadeColorEnabled(false);
    
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    
    auto duration = utils::findChild<ui::Button *>(this, "button_duration");
    duration->setCascadeColorEnabled(false);
    auto duration_background = duration->getChildByName("background");
    duration_background->setLocalZOrder(-1);
    duration_background->setCascadeColorEnabled(false);
    duration_background->setColor(mj::helper::colorFromHex(0xe0e0e0));
    if(CONFIG_MANAGER->isVietnameseStories()){
        duration_background->setColor(mj::helper::colorFromHex(0xF6DE00));
    }
    
    for (auto c : duration_background->getChildren()) {
        c->setCascadeColorEnabled(false);
        c->setColor(mj::helper::colorFromHex(0x26B0E6));
        if(CONFIG_MANAGER->isVietnameseStories()){
            c->setColor(mj::helper::colorFromHex(0x6B64FA));
        }
    }
//    duration->runAction(RepeatForever::create(Sequence::create(ScaleTo::create(0.5, 1.2), ScaleTo::create(0.5, 0.8), NULL)));
    
    auto last_x = title->getPositionX() + title->getContentSize().width;
    dot->setPosition(Point(last_x + 25, dot->getPositionY()));
    line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
    line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    
    auto gradebox_placeholder = utils::findChild(this, "gradebox");
    auto gradebox = MSGradeBox::createBox(false,i_gradeID);
    gradebox->setPosition(gradebox_placeholder->getPosition());
    gradebox->setAnchorPoint(gradebox_placeholder->getAnchorPoint());
    gradebox_placeholder->getParent()->addChild(gradebox);
    gradebox_placeholder->removeFromParent();
    gradebox->setDelegate(this);
    gradebox->setName("gradebox");
    gradebox->setEnableInteraction(false);
    
    ui::Helper::doLayout(this);
    
    _model = utils::findChild<ui::Layout *>(this, "model");
    _scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    _model->setContentSize(Size(_scrollview->getContentSize().width, _model->getContentSize().height));
    _model->addClickEventListener(CC_CALLBACK_1(MSPlaylist_Prepare::onSelectItem, this));
    ui::Helper::doLayout(_model);
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }

        //auto grade_id = reinterpret_cast<int *>(e->getUserData());
        //this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    onReceivedEventGradeChanged(ms::GradeManager::getInstance().getCurrentGradeId());

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    ms::playlist::ABPlayer::getInstance().initInfor();

    _didChangeListOrder = true;
    for (auto it = _audiobooks.begin(); it != _audiobooks.end(); ++it)
    {
        ms::playlist::ABPlayer::getInstance().enqueue(* it);
    }
    this->scheduleOnce([](float) {
        ms::playlist::ABPlayer::getInstance().init();
    }, 1, "initPlayer");


    Director::getInstance()->getEventDispatcher()->addCustomEventListener("MSChapter_Prepare.setLoadingLayerVisible", [=](EventCustom *) {

        setLoadingLayerVisible(false);

    });

    Director::getInstance()->getEventDispatcher()->addCustomEventListener("EXOPLAYER.onError", [this](EventCustom * eventCustom) {

        const std::string k_alertName = "alertnetwork";

        StoryAlert* alert = static_cast<StoryAlert*>(cocos2d::Director::getInstance()->getRunningScene()->getChildByName(k_alertName));

        if(alert) return;

        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");

        alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->setDelegate(this);
        alert->setName(k_alertName);
        alert->setOnExitCallback([=] {

            if(auto audiobookPlayer = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer*>("audiobook_player"))
            {
                audiobookPlayer->playTransitionExit();
            }
            else
            {
                this->removeFromParent();
            }
        });

        auto audiobookPlayer = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer*>("audiobook_player");

        cocos2d::Director::getInstance()->getRunningScene()->addChild(alert,audiobookPlayer != nullptr ? audiobookPlayer->getLocalZOrder():this->getLocalZOrder());

    });
#endif
}

void MSPlaylist_Prepare::setContentSize(const Size & size)
{
    ui::Layout::setContentSize(size);
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    auto title = utils::findChild<ui::Text *>(this, "title");
    
    if (dot && line && title) {
        auto last_x = title->getPositionX() + title->getContentSize().width;
        dot->setPosition(Point(last_x + 25, dot->getPositionY()));
        line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
        line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    }
    
    auto gradebox = utils::findChild(this, "gradebox");
    if (gradebox) {
        gradebox->setPosition(Point(size.width - gradebox->getContentSize().width - 25, gradebox->getPositionY()));
    }
    
    auto button_play = utils::findChild(this, "button_play");
    if (button_play) {
        button_play->setPosition(size.width - 300, button_play->getPositionY());
    }
    
    auto collection_layout = utils::findChild(this, "collection");
    if (collection_layout) {
        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto wait_view = cocos2d::ui::Button::create();
        wait_view->setScale9Enabled(true);
        wait_view->setContentSize(collection_layout->getContentSize());
        wait_view->setPosition(collection_layout->getContentSize()/2);
        wait_view->setName("waiting_view");

        auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
        indicator->setPosition(wait_view->getContentSize()/2);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
        indicator->setName("indicator");
        wait_view->addChild(indicator);

        collection_layout->addChild(wait_view,INT_MAX - 1);
        wait_view->setVisible(false);
#endif
    }
    
    auto controller = utils::findChild(this, "controller");
    if (controller) {
        controller->setPositionX(size.width / 2);
    }
    
    if (_scrollview) {
        _scrollview->setContentSize(Size(size.width - 100, _scrollview->getContentSize().height));
        auto items = _scrollview->getChildren();
        for (auto i : items) {
            i->setContentSize(Size(size.width - 100, i->getContentSize().height));
            i->getChildByName("drag")->setPositionX(size.width - 100);
        }
        
        if (_model) {
            _model->setContentSize(Size(_scrollview->getContentSize().width, _model->getContentSize().height));
            ui::Helper::doLayout(_model);
        }
    }
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->onChangeDisplayLanguage();
    }), this);
}

void MSPlaylist_Prepare::setLoadingLayerVisible(bool i_visible )
{

    if (auto collectionLayout = utils::findChild(this, "collection"))
    {
        if(auto waitingView = collectionLayout->getChildByName("waiting_view"))
        {
            if(!i_visible && !_stopAction)
            {
                _stopAction = true;
                if(auto indicator = waitingView->getChildByName("indicator"))
                {
                    indicator->stopAllActions();
                    indicator->setVisible(i_visible);
                }
            }
            waitingView->setVisible(i_visible);
        }
    }
}

void MSPlaylist_Prepare::onChangeDisplayLanguage() {
    // title
    if (auto title = utils::findChild<ui::Text *>(this, "title")) {
        title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_name));
        
        auto dot = utils::findChild(this, "dot");
        auto line = utils::findChild(this, "line");
        
        if (dot && line) {
            auto last_x = title->getPositionX() + title->getContentSize().width;
            dot->setPosition(Point(last_x + 25, dot->getPositionY()));
            line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
            line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
        }
    }
    
    auto controller = utils::findChild<ui::ListView *>(this, "controller");
    
    controller->enumerateChildren("//lb.*", [](Node * n) {
        auto text = dynamic_cast<ui::Text *>(n);
        if (text) {
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
            text->setString(LANGUAGE_MANAGER->getDisplayTextByKey(text->getName()));
            text->setContentSize(Size(text->getVirtualRendererSize().width + 1, text->getContentSize().height));
            
            // căn chỉnh một chút với tiếng Indo
            if (text->getName().compare("lb.playlist.play") == 0) {
                if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE) {
                    // với tiếng Indo thì cho text nhỏ hơn chút
                    text->setFontSize(20);
                } else {
                    // giá trị fontSize mặc định xem trong file .csd
                    text->setFontSize(22);
                }
            }
        }
        return false;
    });
    
    controller->enumerateChildren("//lb.playlist.minute", [](Node * n) {
        auto text = dynamic_cast<ui::Text *>(n);
        if (text) {
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
            text->getParent()->setContentSize(Size(text->getPositionX() + text->getContentSize().width + 35, text->getParent()->getContentSize().height));
        }
        return false;
    });
    
    auto background = utils::findChild(controller, "background");
    background->setContentSize(Size(background->getParent()->getContentSize().width, background->getContentSize().height));
    ui::Helper::doLayout(background);
    
    controller->setTouchEnabled(false);
    controller->setScrollBarEnabled(false);
    auto controller_width = (controller->getItems().size() - 1) * controller->getItemsMargin();
    for (auto item : controller->getItems()) {
        controller_width += item->getContentSize().width;
    }
    controller->setContentSize(Size(controller_width, 50));
}

#define key_onboard_playlist_conflict_chapters "key.onboard.playlist.chapter"
void MSPlaylist_Prepare::onEnter() {
    ui::Layout::onEnter();
    this->unscheduleUpdate();
    
    if (_filtered_audiobooks.size() * 110 >= _scrollview->getContentSize().height) {
        this->schedule(schedule_selector(MSPlaylist_Prepare::updateScrollviewItems), 0.25);
    } else {
        this->updateScrollviewItems(0);
        _scrollview->setTouchEnabled(false);
        _scrollview->setScrollBarEnabled(false);
        _scrollview->setInnerContainerSize(Size(_scrollview->getContentSize().width, _audiobooks.size() * 110));
        _scrollview->setPosition(_scrollview->getPosition() + Point(0, (_scrollview->getContentSize().height - _audiobooks.size() * 110 - 20)));
        _scroll_velocity = 0;
    }
    
//    if (!MJDEFAULT->getBoolForKey(key_onboard_playlist_conflict_chapters, false)) {
//        MJDEFAULT->setBoolForKey(key_onboard_playlist_conflict_chapters, true);
//        StoryAlert::showAlert(StoryLanguageManager::getInstance()->getDisplayTextByKey("playlist.exclude.novels"), "OK", "");
//    }
}

void MSPlaylist_Prepare::onExit()
{
    ui::Layout::onExit();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("MSChapter_Prepare.setLoadingLayerVisible");
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("EXOPLAYER.onError");
    ms::playlist::ABPlayer::getInstance().stop();
#endif
}

void MSPlaylist_Prepare::onBack(cocos2d::Ref * sender) {
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, false);
    this->removeFromParent();
}

void MSPlaylist_Prepare::onPlay(cocos2d::Ref * sender) {

    if (!NativeBridge::networkAvaiable()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");;
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, true);
    MJDEFAULT->setBoolForKey(key_audio_bool_type, false);
    ms::playlist::ABPlayer::getInstance().setCurrentAudioBookPlayerType(MSAudiobookPlayer::MSAudiobookPlayerType::NORMAL_LIST);

    auto duration = utils::findChild<ui::Text *>(this, "selected_duration");
    auto current = std::atoi(duration->getString().c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) //on android platform , we will init exoplayer in didLoadFromCSB function.
    ms::playlist::ABPlayer::getInstance().init();
#endif
    ms::playlist::ABPlayer::getInstance().setDuration(current);
    ms::playlist::ABPlayer::getInstance().setStartTimestamp((long)mj::helper::getTime());
    ms::playlist::ABPlayer::getInstance().setOriginalQueue(_filtered_audiobooks);
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID) //on android platform , we will enqueue in didLoadFromCSB function.
    for (auto a : _filtered_audiobooks) {
        ms::playlist::ABPlayer::getInstance().enqueue(a);
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    ms::playlist::ABPlayer::getInstance().beginReceivingRemoteEvents();

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    setLoadingLayerVisible(true);

    updatePlaylist();
    ms::playlist::ABPlayer::getInstance().playFromBookId(_filtered_audiobooks[0].book_id, true,MSAudiobookPlayer::MSAudiobookPlayerType::NORMAL_LIST);//onPlay() called => id = 0
#endif
}

void MSPlaylist_Prepare::onChangeDuration(cocos2d::Ref *sender) {
    auto background = reinterpret_cast<Node *>(sender)->getChildByName("background");
    if (background->isVisible()) {
        background->runAction(Sequence::create(FadeOut::create(0.15), Hide::create(), NULL));
    } else {
        background->setVisible(true);
        background->setOpacity(0);
        background->runAction(FadeIn::create(0.15));
    }
}

void MSPlaylist_Prepare::onSelectDuration(cocos2d::Ref * sender) {
    auto duration = reinterpret_cast<Node *>(sender)->getTag();
    utils::findChild<ui::Text *>(this, "selected_duration")->setString(StringUtils::toString(duration));
    onChangeDuration(utils::findChild(this, "button_duration"));
}

void MSPlaylist_Prepare::addItem(int index) {
    if (_scrollview->getChildByTag((int) _filtered_audiobooks.size() - index - 1)) {
        return;
    }
    auto item = _model->clone();
    
    auto info = _filtered_audiobooks[index];
    
    auto text = item->getChildByName<ui::Text *>("audiobook_title");
    text->setString(info.name);
    
    
    auto duration_in_minute = floor(info.duration / 60000.f);
    auto duration_in_second = info.duration / 1000.f - duration_in_minute * 60;
    if (duration_in_second >= 30) {
        duration_in_minute += 1;
    }
    
    auto text_duration = item->getChildByName<ui::Text *>("audiobook_duration");
    if (text_duration)
    {
        text_duration->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

        text_duration->setString(StringUtils::format("%0.0f %s", duration_in_minute, LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audiobooks.duration").c_str()));
    }
    
    auto thumb = item->getChildByName("thumb");
    auto url = _filtered_audiobooks[index].thumbnail_url;
    
    auto remote_thumb = RemoteImageView::createImage(_filtered_audiobooks[index].thumbnail_url, [url](RemoteImageView * img, bool is_success) {
        if (is_success) {
            img->setContentSize(Size(100, 100));
            
//            auto img_data = utils::captureNode(img);
//            auto texture = Director::getInstance()->getTextureCache()->addImage(img_data, mj::helper::getFilenameFromURL(url));
//            auto sprite = Sprite::createWithTexture(texture);
//            sprite->setPosition(img->getPosition());
//            sprite->setAnchorPoint(img->getAnchorPoint());
//            sprite->setContentSize(Size(100, 100));
//            img->getParent()->addChild(sprite);
//
//            delete img_data;
//
//            img->removeFromParent();
        }
    });
    
    item->getChildByName("check")->setVisible(_filtered_audiobooks[index].read);
    item->getChildByName("check")->setLocalZOrder(1);
    item->getChildByName("check")->setPosition(Point(95, 95));
    
    item->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(StringUtils::format("event.audiobook.change.state.read.%d", _filtered_audiobooks[index].book_id), [item](EventCustom *) {
        item->getChildByName("check")->setVisible(true);
    }), item);
    
    remote_thumb->setAnchorPoint(thumb->getAnchorPoint());
    remote_thumb->setPosition(thumb->getPosition());
    item->addChild(remote_thumb);
    item->setPosition(Point(0, ((int) _filtered_audiobooks.size() - index - 1) * 110));
    
    auto drag = item->getChildByName<ui::Widget *>("drag");
    drag->addTouchEventListener(CC_CALLBACK_2(MSPlaylist_Prepare::onDraggingItem, this));
    drag->setTouchEnabled(true);
    drag->setSwallowTouches(true);
    drag->setPropagateTouchEvents(false);
    
    item->setTag((int) _filtered_audiobooks.size() - index - 1);
    
    _scrollview->addChild(item);
}

void MSPlaylist_Prepare::reloadData() {
    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    scrollview->removeAllChildren();
    
    scrollview->setInnerContainerSize(Size(scrollview->getContentSize().width, _filtered_audiobooks.size() * 110 + 10));
}

void MSPlaylist_Prepare::onReceivedEventGradeChanged(int grade_id)
{
    _filtered_audiobooks.clear();

    if (grade_id == -1)
    {
        _filtered_audiobooks = _audiobooks;
    }
    else
    {
        std::copy_if(_audiobooks.begin(), _audiobooks.end(), std::back_inserter(_filtered_audiobooks), [grade_id](mj::model::AudiobookInfo a) {
            return a.grade_id == grade_id;
        });
    }
    
    auto str_order = MJDEFAULT->getStringForKey(StringUtils::format("series_order_%s_%d", _series_id.c_str(), ms::GradeManager::getInstance().getCurrentGradeId()), "");
    
    if (!str_order.empty())
    {
        std::map<int, int> map_order;
        auto order = mj::helper::stringToVectorInt(str_order);
        for (auto i = 0; i < (int) order.size(); ++i)
        {
            map_order[order[i]] = i;
        }
        
        for (auto a : _filtered_audiobooks) {
            if (map_order.count(a.book_id) == 0) {
                map_order[a.book_id] = 0;
            }
        }
        
        std::stable_sort(_filtered_audiobooks.begin(), _filtered_audiobooks.end(), [map_order](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) {
            return map_order.at(a.book_id) < map_order.at(b.book_id);
        });
    }
    
    reloadData();
}

void MSPlaylist_Prepare::saveOrder() {
    std::vector<int> current_order;
    for (auto i = 0; i < (int)_filtered_audiobooks.size(); ++i) {
        current_order.push_back(_filtered_audiobooks[i].book_id);
    }
    
    auto str_order = mj::helper::vectorIntToString(current_order);
    MJDEFAULT->setStringForKey(StringUtils::format("series_order_%s_%d", _series_id.c_str(), ms::GradeManager::getInstance().getCurrentGradeId()), str_order);
}

void MSPlaylist_Prepare::onSelectGradeBox() {
    
}

void MSPlaylist_Prepare::onDeselectGradeBox(int grade_id) {
    
}

void MSPlaylist_Prepare::onSelectItem(cocos2d::Ref *sender) {

    if (!NativeBridge::networkAvaiable()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }

    ms::playlist::ABPlayer::getInstance().setCurrentAudioBookPlayerType(MSAudiobookPlayer::MSAudiobookPlayerType::NORMAL_LIST);

    auto wg = reinterpret_cast<ui::Widget *>(sender);
    wg->setTouchEnabled(false);
    wg->runAction(Sequence::create(DelayTime::create(1), CallFunc::create([wg] {
        wg->setTouchEnabled(true);
    }), NULL));
    
    if (_scroll_velocity > 10) {
        return;
    }
    
    auto tag = reinterpret_cast<Node *>(sender)->getTag();
    auto index = (int)_filtered_audiobooks.size() - 1 - tag;
    
    auto duration = utils::findChild<ui::Text *>(this, "selected_duration");
    
    auto current = std::atoi(duration->getString().c_str());
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
    ms::playlist::ABPlayer::getInstance().init();
#endif
    ms::playlist::ABPlayer::getInstance().setDuration(current);
    ms::playlist::ABPlayer::getInstance().setStartTimestamp((long)mj::helper::getTime());
    ms::playlist::ABPlayer::getInstance().setOriginalQueue(_filtered_audiobooks);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    for (auto it = _filtered_audiobooks.begin() + index; it != _filtered_audiobooks.end(); ++it) {
        ms::playlist::ABPlayer::getInstance().enqueue(* it);
    }
    
    ms::playlist::ABPlayer::getInstance().beginReceivingRemoteEvents();
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    setLoadingLayerVisible(true);

    updatePlaylist();
    ms::playlist::ABPlayer::getInstance().playFromBookId(_filtered_audiobooks[index].book_id, true,MSAudiobookPlayer::MSAudiobookPlayerType::NORMAL_LIST);
#endif
}

void MSPlaylist_Prepare::onDraggingItem(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType event) {
    auto drag = reinterpret_cast<ui::Widget *>(sender);
    auto item = reinterpret_cast<ui::Widget *>(drag->getParent());
    auto scrollview = utils::findChild<ui::ScrollView *>(this, "scrollview");
    
    if (event == ui::Widget::TouchEventType::BEGAN) {
        _moving_item = drag;
    }
    
    if (event == ui::Widget::TouchEventType::MOVED) {
        if (!this->isScheduled(schedule_selector(MSPlaylist_Prepare::update))) {
            this->scheduleUpdate();
        }
        auto world_position = drag->getTouchMovePosition();
        auto position = item->getParent()->convertToNodeSpace(world_position);
        item->setPositionY(position.y - 40);
        item->setLocalZOrder(1);
        
        auto new_row = int(position.y) / 110;
        new_row = std::min(new_row, (int)_filtered_audiobooks.size() - 1);
        new_row = std::max(new_row, 0);
        
        if (new_row != item->getTag()) {
            auto switch_item = item->getParent()->getChildByTag(new_row);
            if (switch_item) {
                _didChangeListOrder = true;
                std::swap(_filtered_audiobooks[_filtered_audiobooks.size() - 1 - new_row], _filtered_audiobooks[_filtered_audiobooks.size() - 1 - item->getTag()]);
                switch_item->stopAllActions();
                switch_item->runAction(MoveTo::create(0.15, Vec2(switch_item->getPositionX(), item->getTag() * 110)));
                switch_item->setTag(item->getTag());
                item->setTag(new_row);
            }
        }
        
        if (world_position.y < 100) {
            if (!scrollview->isAutoScrolling()) {
                auto inner_pos = scrollview->getInnerContainerPosition();
                auto scroll_time = abs(inner_pos.y) / 1200.f;
                scrollview->scrollToBottom(scroll_time, false);
            }
        } else if (world_position.y > 568) {
            if (!scrollview->isAutoScrolling()) {
                auto inner_pos = scrollview->getInnerContainerPosition();
                CCLOG("inner pos y: %f", inner_pos.y);
                auto scroll_time = (scrollview->getInnerContainerSize().height - abs(inner_pos.y)) / 1200.f;
                scrollview->scrollToTop(scroll_time, false);
            }
        } else {
            scrollview->stopAutoScroll();
        }
    }
    
    if (event == ui::Widget::TouchEventType::ENDED || event == ui::Widget::TouchEventType::CANCELED) {
        auto position = drag->getTouchEndPosition();
        position = item->getParent()->convertToNodeSpace(position);
        item->setPositionY(position.y - 40);
        item->setLocalZOrder(1);
        
        auto new_row = int(position.y) / 110;
        new_row = std::min(new_row, (int)_filtered_audiobooks.size() - 1);
        new_row = std::max(new_row, 0);
        
        if (new_row != item->getTag()) {
            std::swap(_filtered_audiobooks[_filtered_audiobooks.size() - 1 - new_row], _filtered_audiobooks[_filtered_audiobooks.size() - 1 - item->getTag()]);
            auto switch_item = item->getParent()->getChildByTag(new_row);
            if (switch_item) {
                switch_item->stopAllActions();
                switch_item->runAction(MoveTo::create(0.15, Vec2(switch_item->getPositionX(), item->getTag() * 110)));
                switch_item->setTag(item->getTag());
            }
        }
        item->setTag(new_row);
        item->stopAllActions();
        item->runAction(MoveTo::create(0.15, Vec2(item->getPositionX(), new_row * 110)));
        
        scrollview->stopAutoScroll();
        _moving_item = nullptr;
        this->unscheduleUpdate();
        this->saveOrder();
    }
}

void MSPlaylist_Prepare::update(float dt) {
    if (_moving_item)
        this->onDraggingItem(_moving_item, ui::Widget::TouchEventType::MOVED);
}

void MSPlaylist_Prepare::updateScrollviewItems(float dt) {
    auto viewport = _scrollview->getBoundingBox();
    viewport.origin = _scrollview->getInnerContainer()->convertToNodeSpace(viewport.origin);
    
    _scroll_velocity = std::abs(viewport.origin.y - _last_scroll_position_y);
    _last_scroll_position_y = viewport.origin.y;
        
    auto top_row = (int)_filtered_audiobooks.size() - 1 - ((int)viewport.origin.y + 110) / 110;
    auto bottom_row = (int)_filtered_audiobooks.size() - 1 - (int)(viewport.origin.y + viewport.size.height) / 110;
    
    bottom_row = std::max(0, bottom_row - 5);
    top_row = std::min(top_row + 5, (int)_filtered_audiobooks.size() - 1);
    
    auto items = _scrollview->getChildren();
    bool flag_removed = false;
    for (auto i : items) {
        auto tag = i->getTag();
        tag = (int)_filtered_audiobooks.size() - tag -1;
        if (tag < bottom_row || tag > top_row) {
            flag_removed = true;
            i->removeFromParent();
        }
    }
    
    for (auto i = bottom_row; i <= top_row; ++i) {
        addItem(i);
    }
    
    if (flag_removed) {
        auto action_clear_cache = Sequence::create(DelayTime::create(0.5), CallFunc::create([] {
            Director::getInstance()->getTextureCache()->removeUnusedTextures();
            std::cout << Director::getInstance()->getTextureCache()->getCachedTextureInfo() << std::endl;
        }), NULL);
        action_clear_cache->setFlags(0x231954);
        this->stopActionsByFlags(0x231954);
        this->runAction(action_clear_cache);
    }
}

ui::Widget::ccWidgetClickCallback MSPlaylist_Prepare::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSPlaylist_Prepare, onBack),
        CLICK_MAP(MSPlaylist_Prepare, onChangeDuration),
        CLICK_MAP(MSPlaylist_Prepare, onPlay),
        CLICK_MAP(MSPlaylist_Prepare, onSelectDuration),
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}

void MSPlaylist_Prepare::onAlertOK(cocos2d::Ref * sender)
{
    this->removeFromParentAndCleanup(true);
}

void MSPlaylist_Prepare::onAlertCancel(cocos2d::Ref * sender)
{
    this->removeFromParentAndCleanup(true);
}
