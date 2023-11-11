//
//  MSAudiobookPlayer.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/16/17.
//

#include "MSAudiobookPlayer.h"
#include "AudioEngine.h"
#include "../external/json/document.h"
#include "RemoteImageView.h"
#include "StoryDataModel.h"
#include "MJAPIGetAudiobook.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
#include "StoryFreeManager.h"
#include <cctype>
#include <cmath>
#include "StoryRateAction.h"
#include "GAFWrapper.h"
#include "NativeBridge.h"
#include "MJPlatformConfig.h"
#include "StoryAlert.h"
#include "MSEventLogger.h"
#include "MSTabAudiobooks.h"
#include "MSPlaylistHelper.h"
#include "APProfileManager.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MSGradeManager.h"
#include "../../story/HSAudioEngine.h"
#include "MSPlaylist_Player.h"
#include "HelperManager.h"
#include "MJAPI_Audiobook_Detail.h"
#include "MsNewDownloadFile.h"
#include "MSCacheManager.h"
#include "MsNewRequest.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#define key_audio_api_called_success "key.event.api.audio.call.success"
#define key_audio_api_called_error "key.event.api.audio.call.error"

#define tag_network_not_available       1000
#define tag_download_audiobook_failed   1001

#define TEXT_OFFSET 5

USING_NS_CC;
INITIALIZE_READER(MSAudiobookPlayer);

std::once_flag ms_audiobook_play_reader;
MSAudiobookPlayer * MSAudiobookPlayer::createView(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type,int i_id, bool playWhenReady) {
    std::call_once(ms_audiobook_play_reader, [] {
        REGISTER_CSB_READER(MSAudiobookPlayer);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/AudiobookPlayer.csb");
    auto node = reinterpret_cast<MSAudiobookPlayer *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (node) {
        node->didLoadFromCSB(info, i_type,i_id, playWhenReady);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

Rect MSAudiobookPlayer::getThumbBoundingBox() {
    auto thumb = utils::findChild<ui::Layout *>(this, "thumb_container");
    auto bb = thumb->getBoundingBox();
    auto p = thumb->getPosition();
    bb.origin = thumb->getParent()->convertToWorldSpace(bb.origin);
    return bb;
}

void MSAudiobookPlayer::didLoadFromCSB(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type, int i_id,  bool playWhenReady)
{
    _playWhenReady = playWhenReady;
    ms::NewDownload::getInstance().clearListDownLoadStory();
    //if(_typeError.length() == 0)
    {
        _typeError = MJDEFAULT->getStringForKey(key_error_url_stories, "None");
    }
    
    if (_valueDomainDownloadAudioBookAndActivity.empty())
    {
        _valueDomainDownloadAudioBookAndActivity = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
    }
    MJDEFAULT->setStringForKey(key_live_in_screen, "audiobook_list");
    MJDEFAULT->setStringForKey(key_name_last_learn_activity, info.name);
    CleverTapManager::GetInstance()->pushProfileByID(MJDEFAULT->getIntegerForKey(key_current_profile_id),CleverTapManager::PUSH_TYPE::LOGIN,nullptr, true);
    this->setTag(CONFIG_MANAGER->isAppActive() ? 999 : 111);
    this->setName("audiobook_player");
    _alert = nullptr;
    _is_playlist = (i_type != MSAudiobookPlayerType::SINGLE_AUDIO) ;
    m_currentAudioBookPlayerType = i_type;

    if (_is_playlist) {
        
        if(auto btn = utils::findChild(this, "btdownload"))
        {
            btn->removeFromParent();
        }
        
        utils::findChild(this, "player")->removeFromParent();
        utils::findChild(this, "player_playlist")->setName("player");
        auto button_next = utils::findChild<ui::Button *>(this, "button_next");
        auto button_prev = utils::findChild<ui::Button *>(this, "button_prev");
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
        if (!ms::playlist::ABPlayer::getInstance().hasNextItem(i_id)) {
            button_next->setEnabled(false);
        }
        if (!ms::playlist::ABPlayer::getInstance().hasPreviousItem(i_id)) {
            button_prev->setEnabled(false);
        }
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("MSChapter_Prepare.setLoadingLayerVisible");
#else // for ios platform
        if (!ms::playlist::ABPlayer::getInstance().hasNextItem()) {
            button_next->setEnabled(false);
        }
        if (!ms::playlist::ABPlayer::getInstance().hasPreviousItem()) {
            button_prev->setEnabled(false);
        }
#endif
        button_next->setTouchEnabled(false);
        button_prev->setTouchEnabled(false);
        
        button_next->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([button_next] {
            button_next->setTouchEnabled(true);
        }), NULL));
        
        button_prev->runAction(Sequence::create(DelayTime::create(0.5), CallFunc::create([button_prev] {
            button_prev->setTouchEnabled(true);
        }), NULL));

        if(m_currentAudioBookPlayerType == MSAudiobookPlayerType::NOVEL_LIST)
        {
            if(auto favouriteButton = utils::findChild<ui::Button *>(this, "button_favourite"))
            {
                favouriteButton->setVisible(false);
            }
        }

    } else {
        utils::findChild(this, "player_playlist")->removeFromParent();
    }
    
    if(auto downloadLabel = utils::findChild<ui::Text *>(this, "download_label"))
    {
        downloadLabel->setString("");
    }
    
    experimental::AudioEngine::lazyInit();
    //get latest info of this audiobook
    _info = StoryDataModel::getInstance()->getAudiobook(info.book_id);
    
    _should_animate_scrolling = false;
    auto text_title = utils::findChild<ui::Text *>(this, "text_title");
    auto title = _info.name;
    title.erase(title.begin(), std::find_if(title.begin(), title.end(), [](int c) {
        return (!std::isspace(c)) || (!std::isalnum(c));
    }));
    text_title->setString(title);
    auto title_bb = text_title->getBoundingBox();
    if (title_bb.getMaxX() > Director::getInstance()->getVisibleSize().width - 110) {
        auto saved_trim = 0;
        //trim title
        Rect bb = Rect(title_bb.origin.x, title_bb.origin.y, 0, 0);
        for (auto i = 0; i < StringUtils::getCharacterCountInUTF8String(title); ++i) {
            auto letter = text_title->getLetter(i);
            if (letter) {
                auto bb_letter = letter->getBoundingBox();
                bb_letter.origin = bb_letter.origin + title_bb.origin;
                bb = bb.unionWithRect(bb_letter);
                if (bb.getMaxX() > Director::getInstance()->getVisibleSize().width - 125) {
                    saved_trim = i - 1;
                    break;
                }
            }
        }
        
        StringUtils::StringUTF8 utf = StringUtils::StringUTF8(title);
        while (utf.length() > saved_trim) {
            utf.deleteChar(saved_trim);
        }
        
        title = utf.getAsCharSequence();
        title.erase(title.begin(), std::find_if(title.begin(), title.end(), [](int c) {
            return (!std::isspace(c)) || (!std::isalnum(c));
        }));
        title += "...";
        text_title->setString(title);
    }
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        AUDIO_HIGHLIGHT_COLOR = Color3B(249,153,42);
        AUDIO_NORMAL_COLOR = Color3B::WHITE;
        
    }
    _text = utils::findChild<ui::Text *>(this, "text_story");
    
    auto fontsize = MJ_PLATFORM_CONFIG.isTablet() ? 35 : 45;
    _text->setFontSize(fontsize);
    _text->setColor(AUDIO_NORMAL_COLOR);
    _text->setTextColor(Color4B::WHITE);
    _text->setOpacity(0);

    if(auto text_filesize = utils::findChild<Text*>(this, "text_filesize"))
    {
        text_filesize->setString(StringUtils::format("%.2f MB", StoryDataModel::getInstance()->getFileSize(info.audio_url)));
    }
    
    auto duration_in_minute = floor(_info.duration / 60000.f);
    auto duration_in_second = _info.duration / 1000.f - duration_in_minute * 60;
    if (duration_in_second >= 30) {
        duration_in_minute += 1;
    }
    
    auto time_label = utils::findChild<ui::Text *>(this, "time_total");
    auto minute = (int)floor(_info.duration / 60000.f);
    auto second = (int)floor(_info.duration / 1000.f - minute * 60);
    if (int(second + 0.5) == 60) {
        minute++; second = 0;
    }
    time_label->setString(StringUtils::format("%02d:%02d", minute, second));
    
    auto button_favourite = utils::findChild<ui::Button *>(this, "button_favourite");
    auto gaf = GAFWrapper::createGAF("gaf/sidebar/favourite/favourite.gaf");
    gaf->setPosition(Point(button_favourite->getContentSize()) / 2 + Point(0, 5));
    gaf->setAnchorPoint(Point::ANCHOR_MIDDLE);
    gaf->setName("gaf");
    button_favourite->addChild(gaf);
    
    auto favourite = StoryDataModel::getInstance()->getFavouriteAudiobooksByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    auto it = std::find_if(favourite.begin(), favourite.end(), [this](mj::model::AudiobookInfo a) {
        return a.book_id == _info.book_id;
    });
    if (it != favourite.end()) {
        _is_favourited = true;
        gaf->playSequence("idle2");
    } else {
        _is_favourited = false;
        gaf->playSequence("idle");
    }

    auto thumb_container = utils::findChild(this, "thumb_container");
    
    auto content_bg = utils::findChild(this, "content");
    auto content_bb = content_bg->getBoundingBox();
    content_bb.origin = content_bg->getParent()->convertToWorldSpace(content_bb.origin);
    content_bb.origin = thumb_container->getParent()->convertToNodeSpace(content_bb.origin);
    thumb_container->setPosition(Point(content_bb.origin.x + 50 + 120, content_bb.origin.y + 85 + 120));

    auto text_summary = utils::findChild<ui::Text *>(this, "summary");
    auto list_summary = utils::findChild<ui::ListView *>(this, "list_summary");
    list_summary->setScrollBarEnabled(false);
    auto thumb_bb = thumb_container->getBoundingBox();
    list_summary->setPosition(Point(thumb_bb.getMaxX(), thumb_bb.getMaxY()) + Point(55, -5));
    text_summary->setString(_info.summary);
    mj::helper::setTextWidth(text_summary, text_summary->getContentSize().width);
    
    auto list_extra = utils::findChild<ui::ListView *>(this, "list_extra");
    list_extra->setGravity(ui::ListView::Gravity::RIGHT);
    list_extra->setScrollBarEnabled(false);
    list_extra->setLeftPadding(25);
    list_extra->setItemsMargin(25);
    
    {
         std::vector<cocos2d::ui::Text*> textSameFonts =
         {
             utils::findChild<ui::Text *>(this, "extra_duration_title"),
             utils::findChild<ui::Text *>(this, "extra_level_title"),
             utils::findChild<ui::Text *>(this, "extra_title"),
         };
         
         for(auto text : textSameFonts)
         {
             if(!text)
             {
                 continue;
             }
             
             text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
         }
     }
    {
         std::vector<cocos2d::ui::Text*> textSameFonts =
         {
            utils::findChild<ui::Text *>(this, "extra_duration"),
            utils::findChild<ui::Text *>(this, "extra_level"),
            utils::findChild<ui::Text *>(this, "extra"),
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
     
    
    utils::findChild<ui::Text *>(this, "extra_duration")->setString(StringUtils::format("%0.0f %s", duration_in_minute, LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audiobooks.duration").c_str()));
    utils::findChild<ui::Text *>(this, "extra_level")->setString(ms::GradeManager::getInstance().getNameByLanguage(ms::GradeManager::getInstance().getGradeInfoById(_info.grade_id).name, LANGUAGE_MANAGER->getDisplayLangId()));
    utils::findChild<ui::Text *>(this, "extra_duration_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audio.player.duration"));
    utils::findChild<ui::Text *>(this, "extra_level_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audio.player.level"));
    
    if (info.extra.empty())
    {
        list_extra->removeItem(2);
    }
    else
    {
        auto extra_split = mj::helper::split(info.extra, ':');
        
        if (extra_split.size() > 1)
        {
            auto text_extra = extra_split[1];
            while (text_extra.front() == ' ')
            {
                text_extra.erase(text_extra.begin());
            }
            utils::findChild<ui::Text *>(this, "extra")->setString(text_extra);
            
            if(CONFIG_MANAGER->isVietnameseStories())
            {
                if(extra_split[0].find("lại") != std::string::npos)
                {
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("author.rewrite"));
                }
                else
                {
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
                }
//                utils::findChild<ui::Text *>(this, "extra_title")->setString(extra_split[0] + ":");
            }
            else
            {
                utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
            }
        }
        else
        {
            utils::findChild<ui::Text *>(this, "extra")->setString(_info.extra);
            utils::findChild<ui::Text *>(this, "extra_title")->setString("");
        }
    }
    
    if(auto autorText = utils::findChild<ui::Text *>(this, "extra"))
    {
        m_author = autorText->getString();
    }
    
    auto lw = 0.f;
    
    for (auto item : list_extra->getItems())
    {
        auto w = 0.f;
        
        for (auto c : item->getChildren())
        {
            w = MAX(w, c->getContentSize().width);
        }
        item->setContentSize(Size(w, item->getContentSize().height));
        lw += w + list_extra->getItemsMargin();
    }
    lw -= list_extra->getItemsMargin() - list_extra->getLeftPadding();
    list_extra->setContentSize(Size(lw, list_extra->getContentSize().height));
    
    _slider = utils::findChild<ui::Slider *>(this, "progress");
    _slider->addEventListener(CC_CALLBACK_2(MSAudiobookPlayer::onSliderEvent, this));
    
    _story_container = utils::findChild<ui::Layout *>(this, "story_container");
   
    this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));
    CCLOG("created");
    

    
    if(auto text = utils::findChild<ui::Text *>(this, "text_btdownload"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/monkeystories_ultra.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.download"));
    }

    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    
    _apiErrorCallback = [=](){
        
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        _alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        _alert->setNetworkDownContent(false, true);
        _alert->show();
        _alert->setTag(tag_download_audiobook_failed);
        _alert->setDelegate(this);
        
    };
    
    _downloadErrorCallback = [this](ms::utils::DownloadUnit unit){
        
        this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
        this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
        this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
        
        _countClickRetry++;
            if ((int)_valueDomainDownloadAudioBookAndActivity.size() > _countClickRetry)
            {
			#if TURN_ON_NEW_DOMAIN
				getData(true);
			#else
				getData(true, true);
			#endif
                
            }
            else {
                auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
                _alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
                _alert->setNetworkDownContent(false, true);
                _alert->show();
                _alert->setTag(tag_download_audiobook_failed);
                _alert->setDelegate(this);
            }
        
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_audio_api_called_error, [=](EventCustom * e) {
  
        if(_apiErrorCallback) {
            
            _apiErrorCallback();
        }
        
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_audio_api_called_success, [=](EventCustom * e) {
        
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.remote.command.pause", [=](EventCustom * e) {
        this->onPlayerPause(nullptr);
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.remote.command.play", [=](EventCustom * e) {
        this->onPlayerPlay(nullptr);
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event.remote.command.seek", [=](EventCustom * e) {
        auto seek_time = (double *) e->getUserData();
        CCLOG("seek_time: %lf", * seek_time);
        
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(StringUtils::format("abplayer.finished.%d", _info.book_id), [=](EventCustom * e) {
        this->onAudiobookFinish();
    }), this);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    this->schedule([=](float dt) {

        auto state = ms::playlist::ABPlayer::getInstance().getState();

        if(state == ms::playlist::ABState::ABStatePlaying)
        {
            if(_is_playing) return;

            utils::findChild(this, "button_pause")->setVisible(true);
            utils::findChild(this, "button_play")->setVisible(false);
            _is_playing = true;
        }
        else if(state == ms::playlist::ABState::ABStatePaused||state == ms::playlist::ABState::ABStateStopped)
        {
            if(!_is_playing) return;

            utils::findChild(this, "button_pause")->setVisible(false);
            utils::findChild(this, "button_play")->setVisible(true);
            _is_playing = false;
        }

    },0.1f, "updateSchedule"); // start updateListViewSchedule function with 0.15s/1 check .

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
    {

        this->scheduleOnce([this](float dt) {
            if (auto btn = utils::findChild(this, "btdownload"))
            {
                onDownload(btn);
            }
            else {
                onPlay(utils::findChild(this, "button_play"));
            }

        }, 1.0f, "delay_auto_audiobook_download");

        if (auto btn_go_back = utils::findChild(this, "btn_go_back"))
        {
            btn_go_back->setVisible(true);
        }

        if (auto btn_skip = utils::findChild(this, "btn_skip"))
        {
            btn_skip->setVisible(true);
        }

    }
//#endif

#endif
}

void MSAudiobookPlayer::setAduioFreePlayMode(){
    is_app_active = CONFIG_MANAGER->isAppActive();
    if (!is_app_active){
        if (FREE_STORY_MANAGER->checkFreeTimeAvailable()) {//thời gian 7 ngày đầu.
            book_is_available = true;
        }else{
            book_is_available = FREE_STORY_MANAGER->isAudiobookOnListFree(_info.book_id);
        }
        
        if (book_is_available){
            auto percent_read = FREE_STORY_MANAGER->getPercentAvaialbleActionFreePlayed(ACTION_AUDIOBOOK);
            if (percent_read >= 100) {
                time_play_enable = _info.duration/1000.0f + 1;
            }else {
                time_play_enable = _info.duration/1000.0f*percent_read/100.0f;
                time_play_enable += FREE_STORY_MANAGER->getAudioMapRecent(_info.book_id);
                if (time_play_enable < max_time_free_read) {
                    time_play_enable = max_time_free_read;
                }
            }
        }else {
            time_play_enable = max_time_free_read;
        }
        
        CCLOG("time_play_enable = %f", time_play_enable);
    }
}

void MSAudiobookPlayer::onEnter() {
    if(CONFIG_MANAGER->isVietnameseStories()){
        AUDIO_ENGINE->stopBackgroundMusic();
    }
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventViewAudiobookPlayer, {
        {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
    });
    
   // _saved_time = utils::getTimeInMilliseconds();
    ui::Layout::onEnter();
    cocos2d::Device::setKeepScreenOn(true);
    this->setAduioFreePlayMode();
    ShowHidePlayOneUI();
    CCLOG("entered");
    auto needed_getinfo = false;
//    if (_info.sync_path.empty() || _info.audio_url.empty()) {
//        needed_getinfo = true;
//    } else {
        if (!FileUtils::getInstance()->isFileExist(std::string("audiobooks/audio/") + mj::helper::getFilenameFromURL(_info.audio_url))) {
            needed_getinfo = true;
        }
//    }
    if (needed_getinfo) {
        if (!NativeBridge::networkAvaiable()) {
            auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
            _alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
            _alert->setNetworkDownContent(false, true);
            _alert->show();
            _alert->setTag(tag_network_not_available);
            _alert->setDelegate(this);
            _alert->setOnExitCallback([this] {
                this->playTransitionExit();
            });
            } else {
				#if !NEW_AUDIO_BOOK
				                //get details from server
                if (_is_playlist && _info.sync_path.empty() && this->getReferenceCount()>0) {
                    this->getData_detail();
                }
                #endif
        }
    }
    else
    {
        if (!_is_playlist)
        {
            readData();
        }
    }
    if (_is_playlist)
    {
        PrepareSyncTextData([=](bool isNeedPrePareSynctext) {
            readData();
            auto button_play = utils::findChild(this, "button_play_main");
            onPlay(nullptr);
        });
    }

}
void  MSAudiobookPlayer::PrepareSyncTextData(std::function<void(bool)> callback){
    auto json_data = mj::helper::getForcedAlignment(_cached_audio);
    if (json_data.empty()) {
        json_data = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + StringUtils::format("audiobooks/%d.json", _info.book_id));
    }
    if (json_data.empty()) {
        mj::helper::showWaitView();
        PerpareData(true, false, [=](void) {
                        mj::helper::deleteWaitView();
                        if(callback)
                        {
                            callback(true);
                        }
                    }
        );
    }
    else
    {
        if(callback)
        {
            callback(false);
        }
    }
}
void MSAudiobookPlayer::PerpareData(bool isDownloadFile, bool isCallDetailAPI, std::function<void()> callback) {

#if !NEW_AUDIO_BOOK
    static bool s_isDownloadSuccess = false;
	static bool s_isAudiobookDetailAPISuccess = false;

	s_isDownloadSuccess = !isDownloadFile;
	s_isAudiobookDetailAPISuccess = !isCallDetailAPI;
#else
#endif
    std::string pathForder = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/";

    _cached_audio = pathForder + mj::helper::getFilenameFromURL(_info.audio_url);

    auto url = mj::helper::getUrlFromDomain(_info.audio_url, mj::UrlType::MSURL_AUDIO, _countClickRetry, _valueDomainDownloadAudioBookAndActivity, _typeError);

    ms::utils::DownloadUnit unit;
    unit.typeUrl = StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME;
    unit.download_id = mj::helper::getFilenameFromURL(url);
    unit.storage_path = _cached_audio;
    unit.forderPath = pathForder;
    unit.url = url;
    unit.success_event = StringUtils::format("download_audiobook_success_%s", unit.download_id.c_str());
    unit.progress_event = StringUtils::format("download_audiobook_progress_%s", unit.download_id.c_str());
    unit.failed_event = StringUtils::format("download_audiobook_failed_%s", unit.download_id.c_str());

    auto onSuccess = [=]() {

        StoryDataModel::getInstance()->updateAudiobookUrlPath(_info);

        this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
        this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
        this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
        if(callback)
        {
            callback();
        }
    };

#if !NEW_AUDIO_BOOK
#endif

    if (isDownloadFile) {

        auto newurl = MJDEFAULT->getStringForKey(key_alternative_domain_audio);

        if (!newurl.empty())
        {
            url = mj::helper::replaceDomain(url, MJDEFAULT->getStringForKey(key_alternative_domain_audio));
        }
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            //this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [=](EventCustom *) {
#if !NEW_AUDIO_BOOK
                s_isDownloadSuccess = true;

			if (s_isDownloadSuccess && s_isAudiobookDetailAPISuccess) {
				onSuccess();
			}
#else
                onSuccess();
#endif

            //}), this);
        });

        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [this, unit](EventCustom *e) {

            this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
            this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
            this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);

            if(_downloadErrorCallback) {

                _downloadErrorCallback(unit);
            }

        }), this);
        if (_is_playlist) {

        }
        if(ms::NewDownload::getInstance().isNewDownload())
        {
            ms::NewDownload::getInstance().initDownload(unit, "Audiobook_list", "url_audiobook", ms::NewDownload::TYPE_LINK_URL::OTHER);
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ms::NewDownload::getInstance().beginDownload();
            });
        }else{
            ms::utils::Downloader::getInstance().enqueue(unit);
        }
    }
#if !NEW_AUDIO_BOOK
    if (isCallDetailAPI) {
		mj::api::MJAPI_Audiobook_Detail::call(std::to_string(_info.book_id), [=](bool isSuccess, std::string msg) {
			if (isSuccess) {
				_info.sync_path = msg;

				audiobookDetailAPIPercent = 10.0f;

				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

					if (loading) {
						loading->setPercent(loading->getPercent() + audiobookDetailAPIPercent);
					}
					if (auto text = utils::findChild<ui::Text *>(this, "download_label")) {

						text->setString(StringUtils::format("%0.1f%%", (loading->getPercent() + audiobookDetailAPIPercent)));

					}

				});

				s_isAudiobookDetailAPISuccess = true;

				if (s_isDownloadSuccess && s_isAudiobookDetailAPISuccess) {
					onSuccess();
				}
			} else {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    this->getData(s_isDownloadSuccess, s_isAudiobookDetailAPISuccess);

                });
			}

		});
	}
#endif
}
void  MSAudiobookPlayer::ShowHidePlayOneUI(bool iShow){
    auto background = utils::findChild<ui::Layout *>(this, "controller");
    auto story_container = utils::findChild<ui::Layout *>(this, "story_container");

    if(_is_playlist)
    {
        if(background)
        {
            if(auto item = utils::findChild<ui::Button *>(background, "button_play_main"))
            {
                item->setVisible(false);
            }
            if(auto item = utils::findChild<ui::Button *>(background, "button_retry"))
            {
                item->setVisible(false);
            }
            if(auto item = utils::findChild<ui::Button *>(background, "btdownload"))
            {
                item->setVisible(false);
            }
            if(auto content = utils::findChild(this, "content"))
            {
                content->setContentSize(Size(Director::getInstance()->getVisibleSize().width + 200, 543 + 25));
                _text->setOpacity(0xff);
                if(auto item = utils::findChild(this, "list_extra"))
                {
                    item->setVisible(false);
                }
            }


        }
        auto textAudio = utils::findChild<ui::ListView *>(this, "list_summary");
        if(textAudio)
        {
            textAudio->setVisible(iShow);
        }
    }

}
void MSAudiobookPlayer::getData(bool isDownloadFile, bool isCallDetailAPI, std::function<void()> callback) {

    auto loading = utils::findChild<ui::LoadingBar *>(this, "download_progress");

#if !NEW_AUDIO_BOOK
    static bool s_isDownloadSuccess = false;
	static bool s_isAudiobookDetailAPISuccess = false;

	s_isDownloadSuccess = !isDownloadFile;
	s_isAudiobookDetailAPISuccess = !isCallDetailAPI;

	if (!isDownloadFile && isCallDetailAPI) {
		loading->setPercent(90);
	}
	else if (isDownloadFile && !isCallDetailAPI) {
		loading->setPercent(10);
	}
	else {
		loading->setPercent(0);
	}
#else
        loading->setPercent(0);
#endif

    if(auto downloadLabel = utils::findChild<ui::Text *>(this, "download_label"))
    {
        downloadLabel->setString(". . .");
    }

	std::string pathForder = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/";
    
    _cached_audio = pathForder + mj::helper::getFilenameFromURL(_info.audio_url);

    _typeError = MJDEFAULT->getStringForKey(key_error_url_stories, "None");

    std::string ty = "None";

    std::vector<std::string> list = {"nhan", "nhan"};

    auto url = mj::helper::getUrlFromDomain(_info.audio_url, mj::UrlType::MSURL_AUDIO, _countClickRetry, _valueDomainDownloadAudioBookAndActivity, _typeError);

	ms::utils::DownloadUnit unit;
    unit.typeUrl = StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME;
	unit.download_id = mj::helper::getFilenameFromURL(url);
	unit.storage_path = _cached_audio;
	unit.forderPath = pathForder;
	unit.url = url;
	unit.success_event = StringUtils::format("download_audiobook_success_%s", unit.download_id.c_str());
	unit.progress_event = StringUtils::format("download_audiobook_progress_%s", unit.download_id.c_str());
	unit.failed_event = StringUtils::format("download_audiobook_failed_%s", unit.download_id.c_str());

	auto onSuccess = [=]() {
        if (MJDEFAULT->getStringForKey(key_read_from) == "Library")
        {
            CleverTapManager::GetInstance()->pushEvent("redownload_item", {
                {"item_name",cocos2d::Value(_info.name)},
                {"item_id",cocos2d::Value(_info.book_id)},
                {"item_type",cocos2d::Value("audiobook")},
                {"disk_space_available",cocos2d::Value(StringUtils::format("%0.2fG", mj::helper::getDiskSpace()))},
                {"total_downloaded_cache",cocos2d::Value(StringUtils::format("%0.2fMB", mj::helper::getTotalSizeFileDownload()))},
                });
        }


		StoryDataModel::getInstance()->updateAudiobookUrlPath(_info);

		this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
		this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
		this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
		this->readData();

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
        {
            this->scheduleOnce([=](float dt) {
                onPlay(utils::findChild(this, "button_play"));
            }, 0.5f, "delay_auto_play_autiobook");
        }
//#endif

	};

#if !NEW_AUDIO_BOOK
	static float audiobookDetailAPIPercent = 0.0f;
	
	audiobookDetailAPIPercent = 0.0f;
#endif

	if (isDownloadFile) {

		auto newurl = MJDEFAULT->getStringForKey(key_alternative_domain_audio);

		if (!newurl.empty())
		{
			url = mj::helper::replaceDomain(url, MJDEFAULT->getStringForKey(key_alternative_domain_audio));
		}

        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [=](EventCustom *) {
            #if !NEW_AUDIO_BOOK
	        s_isDownloadSuccess = true;

			if (s_isDownloadSuccess && s_isAudiobookDetailAPISuccess) {
				onSuccess();
			}
        #else
                onSuccess();
        #endif
            
        }), this);
        
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [this, unit](EventCustom *e) {
			
            this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
            this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
            this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
            
            if(_downloadErrorCallback) {
                
                _downloadErrorCallback(unit);
            }
            
        }), this);
    
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.progress_event, [this, loading](EventCustom * e) {
            
            auto progress = reinterpret_cast<double*>(e->getUserData());
			
#if NEW_AUDIO_BOOK
			loading->setPercent(*progress);
			
            if(auto label1 = utils::findChild<ui::Text *>(this, "download_label"))
            {
                label1->setString(StringUtils::format("%0.1f%%", (*progress)));
            }
#else
			loading->setPercent((*progress)*0.9f + audiobookDetailAPIPercent);
            
            if(auto label1 = utils::findChild<ui::Text *>(this, "download_label"))
            {
                label1->setString(StringUtils::format("%0.1f%%", ((*progress)*0.9f + audiobookDetailAPIPercent)));
            }
            
#endif
            
            
        }), this);
        
        if(ms::NewDownload::getInstance().isNewDownload())
        {
            ms::NewDownload::getInstance().initDownload(unit, "Audiobook_list", "url_audiobook", ms::NewDownload::TYPE_LINK_URL::OTHER);
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                ms::NewDownload::getInstance().beginDownload();
            });
        }else{
            ms::utils::Downloader::getInstance().enqueue(unit);
        }
    }
#if !NEW_AUDIO_BOOK
	if (isCallDetailAPI) {
		mj::api::MJAPI_Audiobook_Detail::call(std::to_string(_info.book_id), [=](bool isSuccess, std::string msg) {
			if (isSuccess) {
				_info.sync_path = msg;

				audiobookDetailAPIPercent = 10.0f;

				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
				
					if (loading) {
						loading->setPercent(loading->getPercent() + audiobookDetailAPIPercent);
					}
					if (auto text = utils::findChild<ui::Text *>(this, "download_label")) {

						text->setString(StringUtils::format("%0.1f%%", (loading->getPercent() + audiobookDetailAPIPercent)));

					}
				
				});
				
				s_isAudiobookDetailAPISuccess = true;

				if (s_isDownloadSuccess && s_isAudiobookDetailAPISuccess) {
					onSuccess();
				}
			} else {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

                    this->getData(s_isDownloadSuccess, s_isAudiobookDetailAPISuccess);

                });
			}

		});
	}
#endif
}

#if !NEW_AUDIO_BOOK
void MSAudiobookPlayer::getData_detail(bool is_retry) {
    if(this->getReferenceCount()<=0 || this->getReferenceCount() >= 10)
        return;
    auto onSuccess = [this]() {
        if(this->getReferenceCount()<=0)
            return ;
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
    
            if(this->getReferenceCount()<=0 || this->getReferenceCount() >= 10 ) {

                return ;
            }
    
            StoryDataModel::getInstance()->updateAudiobookUrlPath(_info);
            this->readData();
            indexing();
            _text->setVisible(true);
            _text->setOpacity(0);
            _text->runAction(FadeIn::create(0.25f));
        });
    };
    
    mj::api::MJAPI_Audiobook_Detail::call(std::to_string(_info.book_id), [=](bool isSuccess, std::string msg) {
        if(this->getReferenceCount()<=0)
            return ;
        if (isSuccess) {
            _info.sync_path = msg;
            onSuccess();
        } else {
            if (is_retry) {
                if (_info.book_id == ms::playlist::ABPlayer::getInstance().getCurrentPlayingId() && this->getReferenceCount()>0) {
                    this->getData_detail();
                }
            }
        }
    });
}
#endif

void MSAudiobookPlayer::onAlertOK(cocos2d::Ref *sender) {

}

void MSAudiobookPlayer::onAlertClose(cocos2d::Ref* sender)
{
    MJDEFAULT->deleteValueForKey(key_error_url_stories);
    utils::findChild(this, "button_retry")->setVisible(true);
    utils::findChild(this, "button_retry")->setOpacity(255);
    utils::findChild(this, "button_play_main")->setVisible(false);
    _countClickRetry = -1;
}

void MSAudiobookPlayer::onAlertCancel(cocos2d::Ref *sender) {

    if (auto alert = dynamic_cast<StoryAlert *>(sender))
    {
        switch (alert->getTag())
        {
            case tag_network_not_available:
                break;
                
            case tag_download_audiobook_failed:
                _countClickRetry = -1;
                this->onDownload(nullptr);
                break;
                
            default:
                break;
        }
    }
}

void MSAudiobookPlayer::onRetry(cocos2d::Ref * sender) {
    utils::findChild(this, "button_retry")->setVisible(false);
    utils::findChild(this, "button_retry")->setOpacity(0);
    utils::findChild(this, "button_play_main")->setVisible(true);
#if NEW_AUDIO_BOOK
	getData(true);
#else
	getData(true,true);
#endif
}

#include "MJAPIGetAudioSyncData.h"

void MSAudiobookPlayer::readData() {
    CCLOG("audiobook start indexing: %lld", utils::getTimeInMilliseconds());
#if !NEW_AUDIO_BOOK
    if(_info.sync_path.empty() || _info.sync_path.length() == 0 || this->getReferenceCount()<=0)
    {
        return;
    }
#endif
    m_willCallFromSyncRequest = false;
    
    if(_sync.size() > 0)
    {
        _sync.clear();
    }
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
        // có thread khác của cocos2d-x gọi hàm readData() này nên phải quay về CocosThread
        if(auto btdownload = utils::findChild(this, "btdownload"))
        {
            btdownload->removeFromParent();
        }
        
        if(auto download_label = utils::findChild(this, "download_label"))
        {
            download_label->removeFromParent();
        }
        
        if (auto button_play_main = utils::findChild<ui::Button *>(this, "button_play_main")) {
            button_play_main->removeChildByName("download_progress");
            button_play_main->loadTextureNormal("mjstory/new_ui/button_audiobook_play.png");
            button_play_main->setEnabled(true);
            button_play_main->setBright(true);
        }
    });
    
    _cached_audio = FileUtils::getInstance()->getWritablePath() + "audiobooks/audio/" + mj::helper::getFilenameFromURL(_info.audio_url);

    auto text_story = _info.sync_path;
    auto idx = 0;
    while (idx < text_story.length()) {
        if (text_story[idx] == '*') {
            text_story.erase(text_story.begin() + idx);
            _italic_idx.push_back(idx);
        } else {
            idx++;
        }
    }
    
    _text->setString(text_story);
    _text->setVisible(false);
    _text->ignoreContentAdaptWithSize(false);
    _text->setTextAreaSize(cocos2d::Size(Director::getInstance()->getVisibleSize().width - 200, 0));
    _text->setContentSize(_text->getVirtualRendererSize());

    for (auto i = 0; i < _italic_idx.size(); ++i) {
        auto f = _italic_idx[i];
        auto temp = text_story.substr(0, f);
        f = StringUtils::getCharacterCountInUTF8String(temp);
        auto l = _italic_idx[i + 1];
        temp = text_story.substr(0, l);
        l = StringUtils::getCharacterCountInUTF8String(temp);
        ++i;

        for (auto c = f; c < l; ++c) {
            auto letter = _text->getLetter(c);

            if (letter) {
                letter->setSkewX(10);
            }
        }
    }
    
    CCLOG("audiobook init italic: %lld", utils::getTimeInMilliseconds());
    
    auto json_data = mj::helper::getForcedAlignment(_cached_audio);
    
    if (json_data.empty()) {
        json_data = FileUtils::getInstance()->getStringFromFile(FileUtils::getInstance()->getWritablePath() + StringUtils::format("audiobooks/%d.json", _info.book_id));
    }
    
    if (json_data.empty() && _sync_request == nullptr) {

        m_willCallFromSyncRequest = true;

        _sync_request = mj::AudioSyncDataAPI::call(_info.book_id, [this](bool success) {
            if (success) {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                    CCLOG("sync data request done.");
                    auto node = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player");
                    if (node == this) {
                        this->readData();
                        this->indexing();
                        _text->setOpacity(0xff);
                        _text->setVisible(true);
                        m_willCallFromSyncRequest = false;
                    }
                });
            }
            _sync_request = nullptr;
        });
        
        return;
    }
    
    auto j = mj::helper::convertForcedAlignment(json_data, _text->getString());
    rapidjson::Document d;
    d.Parse(j.c_str());
    
    if (d.HasParseError()) {
        auto btplay = utils::findChild<ui::Button *>(this, "button_play_main");
        if (btplay) {
            btplay->setEnabled(true);
            btplay->setBright(true);
        }
        return;
    }
    
    auto arr = d.GetArray();
    for (auto i = 0; i < arr.Size(); ++i) {
        if (arr[i].IsObject()) {
            auto block = arr[i].GetObject();
            
            mj::model::SyncBlock tb;
            
            if (JSON_SAFE_CHECK(block, "w", String)) {
                tb.content = block["w"].GetString();
            }
            
            if (JSON_SAFE_CHECK(block, "s", Float)) {
                tb.time_start = block["s"].GetFloat();
            }
            
            if (JSON_SAFE_CHECK(block, "e", Float)) {
                tb.time_end = block["e"].GetFloat();
            }
            
            if (JSON_SAFE_CHECK(block, "ts", Int)) {
                tb.text_start = block["ts"].GetInt();
            }
            
            if (JSON_SAFE_CHECK(block, "te", Int)) {
                tb.text_end = block["te"].GetInt();
            }
            
            _sync.push_back(tb);
        }
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    if(m_currentAudioBookPlayerType == MSAudiobookPlayerType::NOVEL_LIST)
    {
        m_textContent = _text->getString();

        if(_text->getString().size() > 500)
        {
            _text->setString(_text->getString().substr(0,500));
        }
    }

#endif
}

void MSAudiobookPlayer::showSubScribePopup(){
    auto subscirbe_popup = StorySubscribePopup::createSubscribePopup(_info.book_id, StorySubscribePopup::SUB_AUDIO);
    this->addChild(subscirbe_popup, 1999999999);
    subscirbe_popup->setDelegate(this);
    subscirbe_popup->onShowHidePopUp(true);
}

void MSAudiobookPlayer::onSubscribeOk() {
    StorySubscribePopupDelegate::onSubscribeOk();
    this->playTransitionExit();
}
void MSAudiobookPlayer::onSubscribeCancel() {
    this->playTransitionExit();
}

void MSAudiobookPlayer::onSubscribeFree() {
    StorySubscribePopupDelegate::onSubscribeFree();
    this->playTransitionExit();
}

void MSAudiobookPlayer::updateSync(float dt) {
    float current_time;
    if (_is_playing) {

    }

    current_time = ms::playlist::ABPlayer::getInstance().getCurrentProgress() * 1000;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (_is_playlist)
    {
        auto currentSysTime = (long)mj::helper::getTime();
        auto startTimestamp = ms::playlist::ABPlayer::getInstance().getStartTimestamp();
        auto duration = ms::playlist::ABPlayer::getInstance().getDuration();
        if (currentSysTime - ms::playlist::ABPlayer::getInstance().getStartTimestamp() > ms::playlist::ABPlayer::getInstance().getDuration())
        {

            this->playTransitionExit();
            if(_isAlertShowing == true)
            {
                return;
            }
            _isAlertShowing = true;
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                if(this->getReferenceCount()<=0)
                    return;
                auto msg = LANGUAGE_MANAGER->getDisplayTextByKey("playlist.end.duration");
                msg = mj::helper::replace_string(msg, "{duration}", StringUtils::toString(ms::playlist::ABPlayer::getInstance().getDuration() / 60));
                StoryAlert::showAlert(msg, "OK", "");//->setDelegate(this);
            });

            return;
        }
    }
    if(current_time >=  _info.duration && _is_playlist)
    {
        this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;
        onNextTrack(nullptr);
        return;
    }
#endif

    if (m_isSliderBarSeekingEnable)
    {
        _slider->setPercent(std::min(100.f, current_time * 100.f / _info.duration));
    }

    _percent_played = MAX(_percent_played, current_time / _info.duration * 100);
    current_time = MIN(current_time, _info.duration);

    if(m_isSliderBarSeekingEnable)
    {
        updateCurrentTimeLabel(current_time);
    }

    if (!is_app_active) {
        if (book_is_available) {
            FREE_STORY_MANAGER->setAudioMapRecent(_info.book_id, current_time/1000.0f, _info.duration/1000.0f);
        }
        if (current_time > time_play_enable*1000) {
            showSubScribePopup();
            this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;
            utils::findChild(this, "button_pause")->setVisible(false);
            utils::findChild(this, "button_play")->setVisible(true);

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            if(_is_playlist) {
                ms::playlist::ABPlayer::getInstance().pause();
            }else{
                ms::playlist::ABPlayer::getInstance().stop();
            }
#else
            ms::playlist::ABPlayer::getInstance().stop();
#endif
            _is_playing = false;

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
            {
                onBack(nullptr);
                Director::getInstance()->getRunningScene()->scheduleOnce([=](float dt) {

                    ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlayAudiobook();

                }, 0.5f, "delay_back_auto_palayStroy");
                return;
            }
//#endif
            return;
        }
    }
    
    auto it = std::find_if(std::begin(_sync), std::end(_sync), [current_time](mj::model::SyncBlock tb) {
        return (tb.time_start <= current_time) && (tb.time_end >= current_time);
    });
    
    if (it == std::end(_sync)) {
        return;
    }

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)

    if(m_currentAudioBookPlayerType == MSAudiobookPlayerType::NOVEL_LIST)
    {
        updateText();
    }

#endif

    auto index = std::distance(std::begin(_sync), it);
    if (index != _current_highlight_index) {
        if (_current_highlight_index != -1) {
            mj::model::SyncBlock current_block = _sync[_current_highlight_index];
            for (auto i = current_block.text_start; i <= current_block.text_end; ++i) {
                auto letter = _text->getLetter(i);
                letter ? letter->setColor(AUDIO_NORMAL_COLOR) : (void) nullptr;
            }
        }
        _current_highlight_index = (int)index;
        
        float miny, maxy;
        maxy = 0;
        miny = 10000;
        
        mj::model::SyncBlock current_block = _sync[_current_highlight_index];
        for (auto i = current_block.text_start; i <= current_block.text_end; ++i) {
            auto letter = _text->getLetter(i);
            if (!letter) {
                continue;
            }
            letter ? letter->setColor(AUDIO_HIGHLIGHT_COLOR) : (void) nullptr;
            auto bb = letter->getBoundingBox();
            bb.origin = letter->getParent()->convertToWorldSpace(bb.origin);
            bb.origin = _text->getParent()->getParent()->convertToNodeSpace(bb.origin);
            miny = std::min(miny, bb.getMinY());
            maxy = std::max(maxy, bb.getMaxY());
        }
        
        auto container_bb = _story_container->getBoundingBox();

        if (miny < container_bb.getMinY()) {
            auto destination = _letter_positioning[current_block.text_start];
            if (_should_animate_scrolling) {
                _text->stopAllActions();
                _text->runAction(MoveTo::create(0.25, destination));
            } else {
                _text->setPosition(destination - Point(0, TEXT_OFFSET));
            }
//            _page_index = min_page_id;
        }
        
        if (maxy > container_bb.getMaxY()) {
//            _page_index = max_page_id;
            auto destination = _letter_positioning[current_block.text_end];
//            if (_page_index == 0) {
//                destination = Point(container_bb.size.width / 2, container_bb.size.height);
//            }
            if (_should_animate_scrolling) {
                _text->stopAllActions();
                _text->runAction(MoveTo::create(0.25, destination));
            } else {
                _text->setPosition(destination - Point(0, TEXT_OFFSET));
            }
        }
    }
}

#include "APProfileManager.h"
void MSAudiobookPlayer::onAudiobookFinish() 
{
	//m_completedPercent = (_slider == nullptr ? 0 : _slider->getPercent());


	RatingManager::GetInstance()->showRatingPopup();

    StoryDataModel::getInstance()->setAudioRead(_info.book_id);
    std::vector <int> valueAudio = StoryDataModel::getInstance()->getValueAudioReadById(_info.book_id);


    if (valueAudio.size() == 3)
    {
        PROFILE_MANAGER.insertValueToTableReadAudioBook(valueAudio);
        mm::RealtimeDatabase::getInstance().updateReadAudios(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _info.book_id);
    }
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(StringUtils::format("event.audiobook.change.state.read.%d", _info.book_id));
    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishListening, {
        {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
    });
    
    _audio_id = -1;
    _slider->setPercent(100);
    _text->setColor(AUDIO_NORMAL_COLOR);
    utils::findChild(this, "button_pause")->setVisible(false);
    utils::findChild(this, "button_play")->setVisible(true);
    _should_animate_scrolling = false;
    _is_playing = false;
    this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;
    
    ms::playlist::resetNowPlayingInfo();
    
    if (book_is_available) {
        FREE_STORY_MANAGER->map_audibook_recent.clear();
    }
    this->setAduioFreePlayMode();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    if (auto msTabAudiobooks = dynamic_cast<MSTabAudiobooks*>(utils::findChild(Director::getInstance()->getRunningScene(), "MSTabAudiobooks")))
    {
        msTabAudiobooks->reloadData();
    }
#endif

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
    {
        onBack(nullptr);
        Director::getInstance()->getRunningScene()->scheduleOnce([=](float dt) {

            ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlayAudiobook();

        }, 0.5f, "delay_back_auto_palayStroy");

        return;
    }
//#endif

}

#define AB_PLAYER_TRANSITION(x) EaseQuadraticActionIn::create(x)

void MSAudiobookPlayer::onPlay(Ref * sender) 
{

    _time = mj::helper::getTime();
    if(this->getReferenceCount()<=0)
        return;
    _isPushEvent= true;
    _timeStartAudio = int(utils::getTimeInMilliseconds() / 1000);
    _saved_time = utils::getTimeInMilliseconds();
	//for tracking data
	if (MJDEFAULT->getStringForKey("AUDIOBOOK_START_FROM", "unknow") == "audiobook_search_tab")
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("push_clevertap_event_for_audiobook_tab");
	}

    _time_start_playing = (int)(utils::getTimeInMilliseconds() / 1000);
    if(sender)
    {
        reinterpret_cast<ui::Button *>(sender)->setEnabled(false);
        reinterpret_cast<ui::Button *>(sender)->setTouchEnabled(false);
    }

    this->indexing();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

    if(m_currentAudioBookPlayerType == MSAudiobookPlayerType::NOVEL_LIST)
    {
        if(!m_willCallFromSyncRequest)
        {
            _text->setVisible(true);
            m_willCallFromSyncRequest = false;
        }
    }
    else
    {
        _text->setVisible(true);
    }

#else
    _text->setVisible(true);
#endif

    if (_is_playlist) {
        this->playTransitionPlayWithoutAnimation();
    } else {
        this->playTransitionPlay();
    }
    this->scheduleOnce([=](float) {
        if (!_is_playlist) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            ms::playlist::ABPlayer::getInstance().setOriginalQueue({_info});
            ms::playlist::ABPlayer::getInstance().initInfor();
            ms::playlist::ABPlayer::getInstance().enqueue(_info);
            ms::playlist::ABPlayer::getInstance().init();
            ms::playlist::ABPlayer::getInstance().play();
#else
            ms::playlist::ABPlayer::getInstance().init();
            ms::playlist::ABPlayer::getInstance().setOriginalQueue({_info});
            ms::playlist::ABPlayer::getInstance().enqueue(_info);
            ms::playlist::ABPlayer::getInstance().play();
#endif
        } else if (StoryDataModel::getInstance()->isChapterOfAudiobook(_info.book_id)) {
            auto original_book = StoryDataModel::getInstance()->getOriginalBookOfChapter(_info.book_id);
            //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, original_book.book_id), _info.book_id);
            PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_time_played_chapter_x, original_book.book_id), _info.book_id);

            //auto last_time_played = MJDEFAULT->getIntegerForKey(StringUtils::format(key_last_time_played_x, _info.book_id), 0);
            auto last_time_played = PROFILE_MANAGER.getIntegerForKey(StringUtils::format(key_last_time_played_x, _info.book_id), 0);
            if (last_time_played - _info.duration / 1000.f < -10) { //nếu chỉ còn khoảng 10s nữa là hết chương, thì không tiếp tục nghe mà nghe lại từ đầu
                ms::playlist::ABPlayer::getInstance().seekToTime(last_time_played);
            }
        }
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartListening, {
            {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
        });

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(_is_playlist)
        {
            ms::playlist::ABPlayer::getInstance().resume();
        }
        if(!_playWhenReady)
        {
            ms::playlist::ABPlayer::getInstance().pause();
        }
#endif
        _should_animate_scrolling = true;
        _is_playing = true;
        this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);
    }, .5f, "schedule_playing");
    
    StoryDataModel::getInstance()->addAudioBookRecentRead(_info.book_id);
    StoryDataModel::getInstance()->setAudioRead(_info.book_id);
    std::vector <int> valueAudio = StoryDataModel::getInstance()->getValueAudioReadById(_info.book_id);
    if (valueAudio.size() == 3)
    {
        PROFILE_MANAGER.insertValueToTableReadAudioBook(valueAudio);
        mm::RealtimeDatabase::getInstance().updateReadAudios(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _info.book_id);
    }
    auto image_name = mj::helper::getFilenameFromURL(_info.thumbnail_url);
    auto cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + image_name;
    
    ms::playlist::setupRemoteCommand();
#if !NEW_AUDIO_BOOK
	if (_info.sync_path.empty() && this->getReferenceCount()>0) {
        this->getData_detail();
    }
#endif

}

void MSAudiobookPlayer::indexing() {
    _text->ignoreContentAdaptWithSize(false);
    _text->setTextAreaSize(cocos2d::Size(Director::getInstance()->getVisibleSize().width - Director::getInstance()->getVisibleSize().width * 0.15 , 0));
    _text->setContentSize(_text->getVirtualRendererSize());
    _text->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    _text->setPosition(Point(_story_container->getContentSize().width / 2, _story_container->getContentSize().height - TEXT_OFFSET));
    
    auto container_bb = _story_container->getBoundingBox();

    long letter_count = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     letter_count = StringUtils::getCharacterCountInUTF8String( (m_currentAudioBookPlayerType == MSAudiobookPlayerType::NOVEL_LIST) ? m_textContent : _text->getString());
#else
     letter_count = StringUtils::getCharacterCountInUTF8String(_text->getString());
#endif
    _letter_positioning.resize(letter_count, Point::ZERO);
    
    Rect union_bb;
    
    for (auto i = 0; i < _sync.size(); ++i) {
        auto maxy = 0.f;
        auto miny = 10000.f;
        for (auto u = _sync[i].text_start; u <= _sync[i].text_end; ++u) {
            auto letter = _text->getLetter(u);
            if (!letter) {
                _letter_positioning[u] = _text->getPosition();
                continue;
            }
            auto bb = letter->getBoundingBox();
            if (union_bb.size.width == 0 && union_bb.size.height == 0) {
                union_bb = bb;
            } else {
                union_bb = union_bb.unionWithRect(bb);
            }
            bb.origin = _text->convertToWorldSpace(bb.origin);
            bb.origin = _text->getParent()->getParent()->convertToNodeSpace(bb.origin);
            miny = std::min(miny, bb.getMinY());
            maxy = std::max(maxy, bb.getMaxY());
        }
        
        if (miny < container_bb.getMinY()) {
            auto destination = _text->getPosition() + Vec2(0, container_bb.getMaxY() - maxy);
            _text->setPosition(destination - Point(0, TEXT_OFFSET));
        }
        
        for (auto u = _sync[i].text_start; u <= _sync[i].text_end; ++u) {
            _letter_positioning[u] = _text->getPosition();
        }
    }
        
    _text->setPosition(Point(container_bb.size.width / 2, container_bb.size.height - TEXT_OFFSET));
    _text->setColor(AUDIO_NORMAL_COLOR);
    _text->setOpacity(0);
//    }, 0.1f, "schedule_indexing");
    if (union_bb.size.width != 0) {
        auto x_position = (container_bb.size.width - union_bb.size.width) / 2 - 100;
        x_position = MAX(x_position, 0.f);
        for (auto &p : _letter_positioning) {
            p.x += x_position;
        }
        _text->setPositionX(_text->getPositionX() + x_position);
//        _text->setTextAreaSize(Size(union_bb.size.width, 0));
    }
    _text->setContentSize(_text->getVirtualRendererSize());
    
    CCLOG("audiobook end indexing: %lld", utils::getTimeInMilliseconds());
}

void MSAudiobookPlayer::playTransitionPlayWithoutAnimation() {
    auto content = utils::findChild(this, "content");
    content->setContentSize(Size(Director::getInstance()->getVisibleSize().width + 200, 543 + 25));
    content->setPosition(content->getPosition() + Vec2(0, 25));
    _text->setOpacity(0xff);
    
    if(auto extra = utils::findChild(this, "list_extra"))
        extra->setOpacity(0);

    if(auto player = utils::findChild(this, "player"))
        player->setPosition(player->getPosition() + Vec2(0, player->getContentSize().height));

    if(auto button_play_main = utils::findChild(this, "button_play_main"))
    {
        button_play_main->setVisible(false);
    }

    if(utils::findChild(this, "button_pause"))
    {
        utils::findChild(this, "button_pause")->setVisible(true);
    }
    //    auto gradient = utils::findChild(this, "gradient");
    //    gradient->setVisible(true);
    //    gradient->setOpacity(0);
    //    gradient->runAction(FadeIn::create(0.25));
    if(utils::findChild(this, "list_summary"))
    {
        utils::findChild(this, "list_summary")->setOpacity(0);
    }

    if(utils::findChild(this, "thumb_container"))
    {
        utils::findChild(this, "thumb_container")->setScale(0);
    }
    
    if (!CONFIG_MANAGER->isAppActive()) {
        for (int i = 1; i <= 3; i++) {
            utils::findChild(this, "layout_touch"+StringUtils::toString(i))->setVisible(true);
        }
    }
}

void MSAudiobookPlayer::playTransitionPlay() {
    const float transition_duration = 0.5f;
    
    auto content = utils::findChild(this, "content");
    
    content->runAction(Sequence::create(AB_PLAYER_TRANSITION(ResizeTo::create(transition_duration, Size(Director::getInstance()->getVisibleSize().width + 200, 543 + 25))), CallFunc::create([=] {
        _text->runAction(FadeIn::create(transition_duration / 2));
    }), nullptr));
    content->runAction(MoveBy::create(transition_duration, Vec2(0, 25)));
    
    auto extra = utils::findChild(this, "list_extra");
    extra->runAction(FadeOut::create(0.25));
    
    auto player = utils::findChild(this, "player");
    player->runAction(AB_PLAYER_TRANSITION(MoveBy::create(transition_duration, Vec2(0, player->getContentSize().height))));
    
    auto button_play_main = utils::findChild(this, "button_play_main");
    auto button_play = utils::findChild(this, "button_play");
    auto new_position = Point(button_play->getPositionX(), player->getContentSize().height / 2);
    new_position = button_play->getParent()->convertToWorldSpace(new_position);
    button_play_main->runAction(AB_PLAYER_TRANSITION(MoveTo::create(transition_duration, button_play_main->getParent()->convertToNodeSpace(new_position) + Point(0, player->getContentSize().height))));
    button_play_main->runAction(FadeTo::create(transition_duration, 200));
    button_play_main->runAction(Sequence::create(AB_PLAYER_TRANSITION(ResizeTo::create(transition_duration, button_play->getContentSize())), CallFunc::create([=] {
        button_play_main->removeFromParent();
        utils::findChild(this, "button_pause")->setVisible(true);
    }), nullptr));
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (CONFIG_MANAGER->isAppActive()) {
        auto onboard_text = utils::findChild<ui::Text *>(this, "text_onboard_screensaver");
        if (onboard_text)
        {
            onboard_text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
            onboard_text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("audio.onboard.screensaver"));
        }
        auto onboard_count = MJDEFAULT->getIntegerForKey("onboard.screensaver.show.count", 0);
        if (onboard_count < 5) {
            auto screensaver = utils::findChild(this, "onboard_screensaver");
            screensaver->runAction(Sequence::create(DelayTime::create(1), AB_PLAYER_TRANSITION(MoveBy::create(transition_duration, Vec2(0, screensaver->getContentSize().height))), DelayTime::create(7), CallFunc::create([this] {
                this->onDismissOnboard(nullptr);
            }), nullptr));
        }
    }
#endif
    
//    auto gradient = utils::findChild(this, "gradient");
//    gradient->setVisible(true);
//    gradient->setOpacity(0);
//    gradient->runAction(FadeIn::create(0.25));
    
    utils::findChild(this, "list_summary")->runAction(FadeOut::create(transition_duration));
    utils::findChild(this, "thumb_container")->runAction(EaseBackIn::create(ScaleTo::create(transition_duration, 0)));
    
    if (!CONFIG_MANAGER->isAppActive()) {
        for (int i = 1; i <= 3; i++) {
            utils::findChild(this, "layout_touch"+StringUtils::toString(i))->setVisible(true);
        }
    }
}

#define AB_PLAYER_TRANSITION_ENTER(x) EaseQuadraticActionOut::create(x)

void MSAudiobookPlayer::playTransitionEnter() {
    auto controller = this->getChildByName("controller");
    controller->setOpacity(0);
    
    this->setVisible(true);
    controller->runAction(FadeIn::create(0.5));
    
    auto summary = this->getChildByName("list_summary");
    summary->setOpacity(0);
    summary->runAction(FadeIn::create(0.5));
    
    auto button_back = utils::findChild(this, "button_back");
    auto pos = button_back->getPosition();
    button_back->setPosition(pos + Vec2(40, 80));//fix to fit to oppo screen
    button_back->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    auto text_title = utils::findChild(this, "text_title");
    pos = text_title->getPosition();
    text_title->setPosition(pos + Vec2(0, 100));
    text_title->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    auto button_play_main = utils::findChild(this, "button_play_main");
    pos = button_play_main->getPosition();
    button_play_main->setPosition(pos + Vec2(0, -200));
    button_play_main->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    if(auto button_download = utils::findChild(this, "btdownload"))
    {
        pos = button_download->getPosition();
        button_download->setPosition(pos + Vec2(0, -200));
        button_download->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    }
}

#define AB_PLAYER_TRANSITION_EXIT(x) EaseQuadraticActionIn::create(x)

void MSAudiobookPlayer::playTransitionExit() {
    if(this->getReferenceCount()>0)
        utils::findChild(this, "button_retry")->setVisible(false);
    if(isScheduled(schedule_selector(MSAudiobookPlayer::updateSync)))
        this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));
    if(this->isScheduled("updateSchedule"))
        this->unschedule("updateSchedule");
    if(this->isScheduled("schedule_playing"))
        this->unschedule("schedule_playing");

    CCLOG("current time exit: %0.2f", ms::playlist::ABPlayer::getInstance().getCurrentProgress() * 1000);
    
    if (StoryDataModel::getInstance()->isChapterOfAudiobook(_info.book_id)) {
        //MJDEFAULT->setIntegerForKey(StringUtils::format(key_last_time_played_x, _info.book_id), int(ms::playlist::ABPlayer::getInstance().getCurrentProgress()));
        PROFILE_MANAGER.setIntegerForKey(StringUtils::format(key_last_time_played_x, _info.book_id), int(ms::playlist::ABPlayer::getInstance().getCurrentProgress()));
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.update.last.time.played.chapter");
    }


    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    ms::playlist::ABPlayer::getInstance().stop();
    ms::playlist::ABPlayer::getInstance().endReceivingRemoteEvents();
#endif

    const float transition_duration = .5f;

    auto controller = this->getChildByName("controller");
    controller->runAction(FadeOut::create(0.5));

    auto button_back = utils::findChild(this, "button_back");
    button_back->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(transition_duration, Vec2(-100, 100))));

    auto text_title = utils::findChild(this, "text_title");
    text_title->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(transition_duration, Vec2(0, 100))));

    auto content = utils::findChild(this, "content");
    auto bb = content->getBoundingBox();
    content->setAnchorPoint(Point::ANCHOR_MIDDLE);
    content->setPosition(bb.origin + bb.size / 2);
    content->runAction(AB_PLAYER_TRANSITION_EXIT(ResizeTo::create(transition_duration, Size(Director::getInstance()->getVisibleSize().width + 200, 850))));

    auto button_play_main = utils::findChild(this, "button_play_main");
    if (button_play_main)
        button_play_main->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(transition_duration, Vec2(0, -500))));
    
    auto button_download = utils::findChild(this, "btdownload");
    if (button_download) {
        button_download->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(transition_duration, Vec2(0, -500))));
    }

    auto thumb = utils::findChild(this, "thumb_container");
    if (thumb) {
        thumb->runAction(FadeOut::create(transition_duration));
    }

    auto summary = utils::findChild(this, "list_summary");
    if (summary) {
        summary->runAction(FadeOut::create(transition_duration));
    }

    auto player = utils::findChild(this, "player");
    player->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(transition_duration, Vec2(0, -player->getContentSize().height))));

    _downloadErrorCallback = nullptr;

    _apiErrorCallback = nullptr;
    
    if (_text)
        _text->runAction(FadeOut::create(transition_duration));
    
    if (_alert) {
        _alert->setDelegate(nullptr);
    }
    
    this->runAction(Sequence::create(DelayTime::create(1), RemoveSelf::create(), NULL));
}

void MSAudiobookPlayer::onExit() {
    
    if (StoryDataModel::getInstance()->isChapterOfAudiobook(_info.book_id)) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event.update.last.time.played.chapter");
    }
    
    ui::Layout::onExit();
    
    _downloadErrorCallback = nullptr;

    _apiErrorCallback = nullptr;
   
    cocos2d::Device::setKeepScreenOn(false);
    if (_sync_request) {
        CC_SAFE_DELETE(_sync_request);
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(_is_playlist)
    {
        ms::playlist::ABPlayer::getInstance().pause();
    }
    else
    {
        ms::playlist::ABPlayer::getInstance().stop();
    }
#endif

}

#include "GroupPromotion.h"
void MSAudiobookPlayer::onBack(Ref * sender) 
{
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, false);
    _timetotalListenAudio = int(utils::getTimeInMilliseconds() / 1000) - _timeStartAudio;
	m_isCloseClick = true;

    _downloadErrorCallback = nullptr;

    _apiErrorCallback = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(!ms::playlist::ABPlayer::getInstance().isSimplePlayerNull())
        ms::playlist::ABPlayer::getInstance().backStopPlayer();
#endif
    this->playTransitionExit();
    mj::helper::playButtonFXClose();
    
    auto now = utils::getTimeInMilliseconds();
    if (now - _saved_time > 30 * 1000) {
        mjrate::StoryRateAction::updateActionCompleted();
        if (mjrate::StoryRateAction::shouldShowRate()) {
            auto rate_action = mjrate::StoryRateAction::createPage(1);
            Director::getInstance()->getRunningScene()->addChild(rate_action, INT_MAX - 1);
        } else {
            GroupPromotion::attempToShow();
        }
    }
    
    if(MJDEFAULT->getBoolForKey(key_background_music) && CONFIG_MANAGER->isVietnameseStories()){
        AUDIO_ENGINE->playBackgroundMusic("sounds/bg_music.mp3");
    }
    
    if (auto msTabAudiobooks = dynamic_cast<MSTabAudiobooks*>(utils::findChild(Director::getInstance()->getRunningScene(), "MSTabAudiobooks")))
    {
        msTabAudiobooks->reloadData();
    }
}

void MSAudiobookPlayer::onPlayerPause(Ref * sender) {

	m_clickPause++;
    _playWhenReady = false;
    utils::findChild(this, "button_pause")->setVisible(false);
    utils::findChild(this, "button_play")->setVisible(true);
//    experimental::AudioEngine::pause(_audio_id);
    ms::playlist::ABPlayer::getInstance().pause();
    _is_playing = false;
}

void MSAudiobookPlayer::onPlayerPlay(Ref * sender) {
    _playWhenReady = true;
    utils::findChild(this, "button_pause")->setVisible(true);
    utils::findChild(this, "button_play")->setVisible(false);
    _is_playing = true;
//    auto state = experimental::AudioEngine::getState(_audio_id);
    auto state = ms::playlist::ABPlayer::getInstance().getState();
    if (state & (ms::playlist::ABState::ABStateError | ms::playlist::ABState::ABStateStopped | ms::playlist::ABState::ABStateDisposed)) {
        schedule(CC_SCHEDULE_SELECTOR(MSAudiobookPlayer::updateSync));
//    if (state == experimental::AudioEngine::AudioState::ERROR) {
        if (_slider->getPercent() == 100) {
            _slider->setPercent(0);
        }
//        _audio_id = experimental::AudioEngine::play2d(_cached_audio);
//        ms::playlist::ABPlayer::getInstance().enqueue(_info);
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartListening, {
            {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
        });
        
//        experimental::AudioEngine::setFinishCallback(_audio_id, [=](int audio_id, std::string path) {
//            //end;
//            this->onAudiobookFinish();
//        });
//        experimental::AudioEngine::setVolume(_audio_id, 0);
        _should_animate_scrolling = true;
        auto current_time = _info.duration * _slider->getPercent() / 100.f / 1000;
        
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        if(ms::playlist::ABPlayer::getInstance().isSimplePlayerNull()){
            ms::playlist::ABPlayer::getInstance().setOriginalQueue({_info});
            ms::playlist::ABPlayer::getInstance().initInfor();
            ms::playlist::ABPlayer::getInstance().enqueue(_info);
            ms::playlist::ABPlayer::getInstance().init();
            ms::playlist::ABPlayer::getInstance().play();
        }else{
            ms::playlist::ABPlayer::getInstance().enqueue(_info);
            ms::playlist::ABPlayer::getInstance().seekToTime(current_time);
        }
#else
        ms::playlist::ABPlayer::getInstance().enqueue(_info);
        ms::playlist::ABPlayer::getInstance().seekToTime(current_time);
#endif
        
//        this->scheduleOnce([=](float) {
//            experimental::AudioEngine::setVolume(_audio_id, 1);
//            experimental::AudioEngine::setCurrentTime(_audio_id, current_time + 0.1);
//            this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);;
//        }, 0.1f, "delay_seek");
    } else if (state == ms::playlist::ABState::ABStatePaused) {
//        experimental::AudioEngine::resume(_audio_id);
        ms::playlist::ABPlayer::getInstance().resume();
    }
}

void MSAudiobookPlayer::onPlayerForward(Ref * sender) {

	m_clickNext++;

    if (_audio_id != -1) {
        float current_time = ms::playlist::ABPlayer::getInstance().getCurrentProgress();
        current_time += 30;
        current_time = std::min(current_time, _info.duration / 1000.f);
        ms::playlist::ABPlayer::getInstance().seekToTime(current_time);
        updateSync(0);
    } else {
        auto current_time = _info.duration / 1000.f * _slider->getPercent() / 100.f;
        current_time += 30.f;
        current_time = std::min(current_time, _info.duration / 1000.f);
        _slider->setPercent(current_time / (_info.duration / 1000.f) * 100);
        updateSync(0);
    }
}

void MSAudiobookPlayer::onPlayerBackward(Ref * sender) {

	m_clickPrevious++;

    if (_audio_id != -1) {
        float current_time = ms::playlist::ABPlayer::getInstance().getCurrentProgress();
        current_time -= 30;
        current_time = std::max(0.f, current_time);
        ms::playlist::ABPlayer::getInstance().seekToTime(current_time);
        updateSync(0);
    } else {
        auto current_time = _info.duration / 1000.f * _slider->getPercent() / 100.f;
        current_time -= 30.f;
        current_time = std::max(current_time, 0.f);
        _slider->setPercent(current_time / (_info.duration / 1000.f) * 100);
        updateSync(0);
    }
}

void MSAudiobookPlayer::onNextTrack(cocos2d::Ref *sender) {
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, true);
	if (auto button = dynamic_cast<ui::Button*>(sender))  // avoid user spam ( or double click ) on the previous button -> if users spam next or previous button ,the apps will run with the wrong behavior
	{
		button->setEnabled(false);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(_is_playlist)
    {
        if(auto playerList = utils::findChild(this, "player"))
        {
            auto wait_view = cocos2d::ui::Button::create();
            wait_view->setScale9Enabled(true);
            wait_view->setContentSize(playerList->getContentSize());
            wait_view->setPosition(playerList->getContentSize()/2.0f);

            auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
            indicator->setPosition(utils::findChild(this, "button_play")->getPosition());
            indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));

            playerList->addChild(wait_view,INT_MAX - 1);
            playerList->addChild(indicator,INT_MAX - 1);
        }
    }
#endif
    ms::playlist::ABPlayer::getInstance().next();
}

void MSAudiobookPlayer::onPreviousTrack(cocos2d::Ref *sender) 
{
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, true);
	if (auto button = dynamic_cast<ui::Button*>(sender))  // avoid user spam ( or double click ) on the previous button -> if users spam next or previous button ,the apps will run with the wrong behavior
	{
		button->setEnabled(false);
	}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(_is_playlist)
    {
        if(auto playerList = utils::findChild(this, "player"))
        {
            auto wait_view = cocos2d::ui::Button::create();
            wait_view->setScale9Enabled(true);
            wait_view->setContentSize(playerList->getContentSize());
            wait_view->setPosition(playerList->getContentSize()/2.0f);

            auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
            indicator->setPosition(utils::findChild(this, "button_play")->getPosition());
            indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));

            playerList->addChild(wait_view,INT_MAX - 1);
            playerList->addChild(indicator,INT_MAX - 1);
        }
    }
#endif
    ms::playlist::ABPlayer::getInstance().back();
}

void MSAudiobookPlayer::onSliderEvent(Ref * sender, ui::Slider::EventType event) {

    switch (event)
    {;
        case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
         {
             m_isSliderBarSeekingEnable = false;
             float duration = (_info.duration*(_slider->getPercent() / 100.f));
             updateCurrentTimeLabel(duration);
//            experimental::AudioEngine::setCurrentTime(_audio_id, duration);
            break;
        }
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
        {
            m_isSliderBarSeekingEnable =false;
            _should_animate_scrolling = false;
            //if (!_is_playing && _audio_id == -1)
            //    this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);
            break;
        }
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
        {
            double duration = _info.duration;
            duration = duration * _slider->getPercent() / 100.f;

#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
            if(duration>=_info.duration && _is_playlist)
            {
                this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;
                onNextTrack(nullptr);
                return;
            }
#endif
            ms::playlist::ABPlayer::getInstance().seekToTime(duration / 1000.f);

            m_isSliderBarSeekingEnable =true;

            updateSync(0);

            _should_animate_scrolling = true;
            //if (!_is_playing && _audio_id == -1)
                //this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));
            break;
        }
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
        {
            m_isSliderBarSeekingEnable =true;

            _should_animate_scrolling = true;
            if (!_is_playing)
                this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;
            break;
        }
    }
}

void MSAudiobookPlayer::onFavourite(cocos2d::Ref *sender) {
    auto button = reinterpret_cast<ui::Button *>(sender);
    auto gaf = button->getChildByName<GAFWrapper *>("gaf");
    if (!_is_favourited)
    {
        experimental::AudioEngine::play2d("sounds/ui/button_fx/9.mp3");
        gaf->playSequence("action");
        
        PROFILE_MANAGER.updateLoveAudios(MJDEFAULT->getIntegerForKey(key_current_profile_id), _info.book_id);
        
        mm::RealtimeDatabase::getInstance().updateLoveAudios(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id));

        StoryDataModel::getInstance()->addFavouriteAudiobooksByLanguage(_info.book_id, _info.lang_id);
    }
    else
    {
        mj::helper::playButtonFX();
        gaf->playSequence("action2");
        
        PROFILE_MANAGER.deleteLoveAudios(MJDEFAULT->getIntegerForKey(key_current_profile_id),_info.book_id);
        mm::RealtimeDatabase::getInstance().deleteLoveAudios(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id),_info.book_id);
        
        StoryDataModel::getInstance()->removeFavouriteAudiobookByLanguage(_info.book_id, _info.lang_id);
    }
    _is_favourited = !_is_favourited;
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
}

void MSAudiobookPlayer::onDownload(cocos2d::Ref *sender) {
    if(mj::helper::showPopupWarningFullDisk())
    {
        return;
    }
    MJDEFAULT->deleteValueForKey(key_error_url_stories);
    utils::findChild(this, "button_retry")->setVisible(false);
    utils::findChild(this, "button_retry")->setOpacity(0);
    utils::findChild(this, "button_play_main")->setVisible(true);
    ms::utils::ThumbQueue::getInstance().unDownloadImage();
    ms::utils::Downloader::getInstance().unEnqueue();
    if (!FileUtils::getInstance()->isFileExist(std::string("audiobooks/audio/") + mj::helper::getFilenameFromURL(_info.audio_url))) {
	#if NEW_AUDIO_BOOK
	getData(true);
	#else
	getData(true,true);
	#endif
    } else {
#if NEW_AUDIO_BOOK
readData();
#else
		if(_info.sync_path.empty() || _info.sync_path.length() == 0 || this->getReferenceCount()<=0)
        {
            getData(false,true);
        }
        else
        {
            readData();
        }
#endif
    }

    auto senderButton = dynamic_cast<ui::Button *>(sender);
    if(senderButton) senderButton->setVisible(false);
}

void MSAudiobookPlayer::onPlaylist(cocos2d::Ref * sender)
{
    auto button = reinterpret_cast<ui::Button *>(sender);
    button->setTouchEnabled(false);
    
    auto playlist_player = MSPlaylist_Player::createView();
    playlist_player->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    playlist_player->setPosition(Point(0, Director::getInstance()->getVisibleSize().height));
    playlist_player->setOpacity(0);
    playlist_player->setTouchEnabled(true);
    playlist_player->runAction(FadeIn::create(0.25));
    playlist_player->setOnExitCallback([button] {
        button->setTouchEnabled(true);
    });
    this->addChild(playlist_player, 100);
    
    auto text_title = utils::findChild(this, "text_title");
    auto button_back = utils::findChild(this, "button_back");
    if (text_title) {
        text_title->runAction(FadeOut::create(0.25));
    }
    if (button_back) {
        button_back->runAction(FadeOut::create(0.25));
    }
    
    playlist_player->setOnExitCallback([text_title, button_back, button] {
        if (text_title) {
            text_title->runAction(FadeIn::create(0.25));
        }
        if (button_back) {
            button_back->runAction(FadeIn::create(0.25));
        }
        button->setTouchEnabled(true);
    });
}

void MSAudiobookPlayer::onDismissOnboard(cocos2d::Ref * sender) {
    auto onboard = utils::findChild(this, "onboard_screensaver");
    if (onboard) {
        if (sender) {
            auto onboard_count = MJDEFAULT->getIntegerForKey("onboard.screensaver.show.count", 0);
            MJDEFAULT->setIntegerForKey("onboard.screensaver.show.count", onboard_count + 1);
        }
        onboard->runAction(AB_PLAYER_TRANSITION_EXIT(MoveBy::create(0.5, Point(0, - onboard->getContentSize().height))));
    }
}

ui::Widget::ccWidgetClickCallback MSAudiobookPlayer::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSAudiobookPlayer, onPlay),
        CLICK_MAP(MSAudiobookPlayer, onBack),
        CLICK_MAP(MSAudiobookPlayer, onPlayerPlay),
        CLICK_MAP(MSAudiobookPlayer, onPlayerPause),
        CLICK_MAP(MSAudiobookPlayer, onPlayerBackward),
        CLICK_MAP(MSAudiobookPlayer, onPlayerForward),
        CLICK_MAP(MSAudiobookPlayer, onFavourite),
        CLICK_MAP(MSAudiobookPlayer, onRetry),
        CLICK_MAP(MSAudiobookPlayer, onDownload),
        CLICK_MAP(MSAudiobookPlayer, onDismissOnboard),
        CLICK_MAP(MSAudiobookPlayer, onPlaylist),
        CLICK_MAP(MSAudiobookPlayer, onNextTrack),
        CLICK_MAP(MSAudiobookPlayer, onPreviousTrack),
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        CLICK_MAP(MSAudiobookPlayer, onGoback),
        CLICK_MAP(MSAudiobookPlayer, onSkip),
#endif
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSAudiobookPlayer::updateCurrentTimeLabel(const float i_duration)
{
    auto currentTimeLabel = utils::findChild<ui::Text *>(this, "time");

    if(!currentTimeLabel)
    {
        return;
    }

    auto minute = (int) floor(i_duration / 1000 / 60);

    auto second = (int) floor(i_duration / 1000 - minute * 60);

    if (int(second + 0.5) == 60)
    {
        minute++;
        second = 0;
    }

    currentTimeLabel->setString(StringUtils::format("%02d:%02d", minute, second));
}


void MSAudiobookPlayer::updateText()
{
    m_textUpdateCount++;

    if(m_textUpdateCount > 1)
    {
        return;
    }

    _text->setVisible(false);
    _text->setString(m_textContent);
    indexing();
    _text->setOpacity(255);
    _text->setVisible(true);
}

void MSAudiobookPlayer::pushCleverTapTrackingData()
{
    try
    {
        auto userInfo = ap::Database::getInstance().getUserInfo();

        int profileAge = -1;
        auto proflies = ap::Database::getInstance().getProfiles();
        auto currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

        for (auto profile : proflies)
        {
            if (profile.profile_id == currentProfileID)
            {
                profileAge = profile.age;
                break;
            }
        }

        /*Lấy tên của chủ để nghe theo id của bài nghe*/
        auto audiobook_series = StoryDataModel::getInstance()->getAudiobookSeries();
        std::string name_category_of_audiobook = "";
        for (auto series : audiobook_series)
        {
            bool check_breack = false;
            auto list_book_id = series.jsonBooks;

            list_book_id.erase(std::remove_if(list_book_id.begin(), list_book_id.end(), [](char x) {
                return (x == '[' || x == ']');
                }), list_book_id.end());

            std::vector<std::string> bookID = mj::helper::split(list_book_id, ',');

            for (int i = 0; i < bookID.size(); i++)
            {
                std::string so = std::to_string(_info.book_id);
                if (so == bookID[i])
                {
                    name_category_of_audiobook = series.title;
                    check_breack = true;
                    break;
                }
            }
            if (check_breack) break;
        }

        //_info.time_read
        auto _gradeName = ms::GradeManager::getInstance().getNameByLanguage(ms::GradeManager::getInstance().getGradeInfoById(_info.grade_id).name, LANGUAGE_EN_CODE);
        std::string gradeName = "";
        if (_gradeName.find("Cấp độ 1 & 2") != std::string::npos ||
            _gradeName.find("Prasekolah & TK") != std::string::npos ||
            _gradeName.find("ระดับ 1 และ 2") != std::string::npos ||
            _gradeName.find("Pre-K & Kinder") != std::string::npos) 
        {
            gradeName = "Pre-K & Kinder";
        }
        if (_gradeName.find("Cấp độ 3") != std::string::npos ||
            _gradeName.find("Level 3") != std::string::npos ||
            _gradeName.find("ระดับ 3") != std::string::npos ||
            _gradeName.find("Grade 1") != std::string::npos)
        {
            gradeName = "Grade 1";
        }
        if (_gradeName.find("Cấp độ 4") != std::string::npos ||
            _gradeName.find("Level 4") != std::string::npos ||
            _gradeName.find("ระดับ 4") != std::string::npos ||
            _gradeName.find("Grade 2") != std::string::npos)
        {
            gradeName = "Grade 2";
        }
        std::string readFrom = MJDEFAULT->getStringForKey(key_read_from_of_audiobook);
        int timeAudioBook = (int) (_info.duration / 1000.0);
        if(_timetotalListenAudio == 0)
        {
            _timetotalListenAudio = int(utils::getTimeInMilliseconds() / 1000.0) - _timeStartAudio;
        }
        if(_timetotalListenAudio > timeAudioBook)
        {
            _timetotalListenAudio = timeAudioBook;
        }

        long long timeLearnAudioBook = mj::helper::getTime() - _time;
        if (timeLearnAudioBook >= timeAudioBook)
        {
            _completed = true;
        }
        int completed = 0;
        _completed ? completed = 100 : completed = 0;
        _completed = false;

        //int completed = ((float) (_timetotalListenAudio) / (float) (timeAudioBook)) * 100.f;

        CleverTapManager::GetInstance()->pushEvent("audiobooks_learning_properties",
            {
                {"audiobooks_id",cocos2d::Value(_info.book_id)},
                {"categories",cocos2d::Value(name_category_of_audiobook)},
                {"audiobooks_name",cocos2d::Value(_info.name)},
                {"grade_id",cocos2d::Value(_info.grade_id)},
                {"grade_name",cocos2d::Value(gradeName)},
                {"story_author",cocos2d::Value(m_author)},
                {"duration",cocos2d::Value(_timetotalListenAudio)},
                {"read_from",cocos2d::Value(cocos2d::Value(readFrom))},
                {"user_profile_age",cocos2d::Value(profileAge)},
                {"completed",cocos2d::Value(completed)},
                {"total_time_reading",cocos2d::Value(timeAudioBook)},
                {"favorite",cocos2d::Value(_is_favourited)},//
                {"click_next",cocos2d::Value(m_clickNext)},//
                {"click_pause",cocos2d::Value(m_clickPause)},//
                {"click_previous",cocos2d::Value(m_clickPrevious)},//
                {"close_audiobook",cocos2d::Value(m_isCloseClick)},//
                //những event push time bên dưới, để clevertap nhận phải giữ nguyên định dạng
                //và code ở native để push, vui lòng đừng sửa nhé -> nếu convert bằng C++ đúng định dạng ngày giờ, hiển thị đc trên clevertap
                // nhưng nó không nhận được => sẽ ko tạo được campaign => cái này chỉ xử lý native.
                {"time_purchased", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_purchased",0)))},
                {"time_activated",cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_activated",0)))},
                {"time_expired", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_expire",0))) },
                {"play_mode", cocos2d::Value(_is_playlist ? "autoplay" : "normal") },
            });
    }
    catch (const std::exception&)
    {

    }
}

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
void MSAudiobookPlayer::onGoback(cocos2d::Ref* sender)
{
    CCLOG("1234");
    AUDIO_ENGINE->stopAllAudio();
    AUDIO_ENGINE->stopAllSound();
    AUDIO_ENGINE->stopAllEffects();
    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->unscheduleUpdate();
    this->stopAllActions();

    onBack(this);

    Director::getInstance()->getRunningScene()->scheduleOnce([=](float dt) {

        ms::CacheManager::getInstance().ms::CacheManager::getInstance().onBackAudiobook();

    }, 0.5f, "delay_back_auto_palayStroy");

}

void MSAudiobookPlayer::onSkip(cocos2d::Ref* sender)
{
    CCLOG("1234");
    AUDIO_ENGINE->stopAllAudio();
    AUDIO_ENGINE->stopAllSound();
    AUDIO_ENGINE->stopAllEffects();

    this->unscheduleAllCallbacks();
    this->unscheduleAllSelectors();
    this->unscheduleUpdate();
    this->stopAllActions();

    onBack(this);

    Director::getInstance()->getRunningScene()->scheduleOnce([=](float dt) {

        ms::CacheManager::getInstance().ms::CacheManager::getInstance().onSkipAudibook();

    }, 0.5f, "delay_back_auto_palayStroy");
}
#endif

MSAudiobookPlayer::~MSAudiobookPlayer()
{
    if(_isPushEvent) {
        pushCleverTapTrackingData();
    }
}
#endif
