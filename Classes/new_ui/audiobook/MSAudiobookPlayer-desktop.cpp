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

#include "MJPlatformConfig.h"

#include "StoryAlert.h"

#include "MSDownloader.h"

#include "MSEventLogger.h"

#include "MSTabAudiobooks.h"
#include "APProfileManager.h"
#include "MMFirebaseDatabase.h"
#include "APDatabase.h"
#include "MSGradeManager.h"
#include "../../story/HSAudioEngine.h"

#include "GroupPromotion.h"
#include "MJAPI_Audiobook_Detail.h"
#include "MSCacheManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

#define key_audio_api_called_success "key.event.api.audio.call.success"
#define key_audio_api_called_error "key.event.api.audio.call.error"

#define TEXT_OFFSET 5

USING_NS_CC;
INITIALIZE_READER(MSAudiobookPlayer);

//#define AUDIO_HIGHLIGHT_COLOR Color3B::RED
//#define AUDIO_NORMAL_COLOR Color3B::BLACK

std::once_flag ms_audiobook_play_reader;

MSAudiobookPlayer * MSAudiobookPlayer::createView(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type, int i_id, bool playWhenReady) {

    std::call_once(ms_audiobook_play_reader, [] {
        REGISTER_CSB_READER(MSAudiobookPlayer);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/AudiobookPlayer.csb");
    auto node = reinterpret_cast<MSAudiobookPlayer *>(CSLoader::createNodeWithVisibleSize(csb_name));
    if (node) {
        node->didLoadFromCSB(info, i_type, i_id);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

Rect MSAudiobookPlayer::getThumbBoundingBox() 
{
    auto thumb = utils::findChild<ui::Layout *>(this, "thumb_container");
    auto bb = thumb->getBoundingBox();
    auto p = thumb->getPosition();
    bb.origin = thumb->getParent()->convertToWorldSpace(bb.origin);
    return bb;
}

void MSAudiobookPlayer::didLoadFromCSB(mj::model::AudiobookInfo info, MSAudiobookPlayerType i_type, int i_id, bool playWhenReady)
{
   // this->setTouchEnabled(true);
    if(_typeError.length() == 0)
    {
        _typeError = MJDEFAULT->getStringForKey(key_error_url_stories, "None");
    }
    
    if (_valueDomainDownloadAudioBookAndActivity.empty())
    {
        _valueDomainDownloadAudioBookAndActivity = StoryDataModel::getInstance()->getListDomainStories(StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME);
    }

    MJDEFAULT->setStringForKey(key_live_in_screen, "audiobook_list");
    this->setTag(CONFIG_MANAGER->isAppActive() ? 999 : 111);
    this->setName("audiobook_player");
    _alert = nullptr;
    _is_playlist = (i_type != MSAudiobookPlayerType::SINGLE_AUDIO);
    
    utils::findChild<ui::Text *>(this, "download_label")->setString("");
    
    experimental::AudioEngine::lazyInit();
    //get latest info of this audiobook
    _info = StoryDataModel::getInstance()->getAudiobook(info.book_id);
    
    _should_animate_scrolling = false;
    auto text_title = utils::findChild<ui::Text *>(this, "text_title");
    auto title = _info.name;

	auto utf8 = StringUtils::StringUTF8(title);
	while (utf8.length() > 0 && utf8.getString().front()._char == " ") {
		utf8.deleteChar(0);
	}
	title = utf8.getAsCharSequence();

    text_title->setString(title);
    
//    utils::findChild<ui::Text *>(this, "placeholder")->setString(_info.name);
    
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

    utils::findChild<Text*>(this, "text_filesize")->setString(StringUtils::format("%.2f MB", StoryDataModel::getInstance()->getFileSize(info.audio_url)));

    if(CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild<Text*>(this, "text_filesize")->setVisible(false);
    }
    
    auto duration_in_minute = floor(_info.duration / 60000.f);
    auto duration_in_second = _info.duration / 1000.f - duration_in_minute * 60;
    if (duration_in_second >= 30) {
        duration_in_minute += 1;
    }//    if (duration_in_minute != (int) duration_in_minute) {
//        duration_in_minute += 1;
//    }
    
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
    
    if (info.extra.empty()) {
        list_extra->removeItem(2);
    } else {
        auto extra_split = mj::helper::split(info.extra, ':');
        if (extra_split.size() > 1) {
            auto text_extra = extra_split[1];
            while (text_extra.front() == ' ') {
                text_extra.erase(text_extra.begin());
            }
            utils::findChild<ui::Text *>(this, "extra")->setString(text_extra);
            if(CONFIG_MANAGER->isVietnameseStories()){
                if(extra_split[0].find("lại") != std::string::npos){
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("author.rewrite"));
                }else{
                    utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
                }
//                utils::findChild<ui::Text *>(this, "extra_title")->setString(extra_split[0] + ":");
                
            }else{
                utils::findChild<Text *>(this, "extra_title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("extra.title"));
            }
        } else {
            utils::findChild<ui::Text *>(this, "extra")->setString(_info.extra);
            utils::findChild<ui::Text *>(this, "extra_title")->setString("");
        }
    }
    
    auto lw = 0.f;
    for (auto item : list_extra->getItems()) {
        auto w = 0.f;
        for (auto c : item->getChildren()) {
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
    
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_audio_api_called_error, [=](EventCustom * e) {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        _alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        _alert->setNetworkDownContent(false, true);
        _alert->show();
        _alert->setDelegate(this);
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_audio_api_called_success, [=](EventCustom * e) {
        
    }), this);
//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
    if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
    {

        this->scheduleOnce([this](float dt) {
            if (utils::findChild(this, "btdownload")->isVisible())
            {
                onDownload(utils::findChild(this, "btdownload"));
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
    
    _saved_time = utils::getTimeInMilliseconds();
    ui::Layout::onEnter();
    cocos2d::Device::setKeepScreenOn(true);
    this->setAduioFreePlayMode();
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
        //get details from server
//        getData();
    } else {
        //ready to play
        readData();
    }
}

void MSAudiobookPlayer::getData(bool isDownloadFile, bool isCallDetailAPI, std::function<void()> callback) {

    auto loading = utils::findChild<ui::LoadingBar*>(this, "download_progress");

    loading->setPercent(0);

    utils::findChild<ui::Text *>(this, "download_label")->setString(". . .");
    
    _cached_audio = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(_info.audio_url);
    
    auto url = mj::helper::getUrlFromDomain(_info.audio_url, mj::UrlType::MSURL_AUDIO, _countClickRetry, _valueDomainDownloadAudioBookAndActivity, _typeError);

	ms::utils::DownloadUnit unit;
    unit.typeUrl = StoryDataModel::TYPEURL::AUDIOBOOK_ACTIVITY_GAME;
	unit.download_id = mj::helper::getFilenameFromURL(url);
	unit.storage_path = _cached_audio;
	unit.url = url;
	unit.success_event = StringUtils::format("download_audiobook_success_%s", unit.download_id.c_str());
	unit.progress_event = StringUtils::format("download_audiobook_progress_%s", unit.download_id.c_str());
	unit.failed_event = StringUtils::format("download_audiobook_failed_%s", unit.download_id.c_str());

	auto onSuccess = [=]() {

        if (MJDEFAULT->getStringForKey(key_read_from) == "Library")
        {
            CleverTapManager::GetInstance()->pushEvent("redownload_item", {
                {"item_name",cocos2d::Value(_info.name)},
                {"item_url",cocos2d::Value(_info.audio_url)},
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

	static float audiobookDetailAPIPercent = 0.0f;
	
	audiobookDetailAPIPercent = 0.0f;

	if (isDownloadFile) {

		auto newurl = MJDEFAULT->getStringForKey(key_alternative_domain_audio);

		if (!newurl.empty())
		{
			url = mj::helper::replaceDomain(url, MJDEFAULT->getStringForKey(key_alternative_domain_audio));
		}

		ms::utils::Downloader::getInstance().enqueue(unit);


		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [=](EventCustom *) {

            onSuccess();

		}), this);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [this, unit](EventCustom *e) {
			this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
			this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
			this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
            _countClickRetry++;
            if ((int)_valueDomainDownloadAudioBookAndActivity.size() > _countClickRetry)
            {
                getData(true, true);
            }
            else {
                auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
                _alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
                _alert->setNetworkDownContent(false, true);
                _alert->show();
                _alert->setDelegate(this);
            }
		}), this);

		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.progress_event, [this, loading](EventCustom * e) {
			auto progress = reinterpret_cast<double *>(e->getUserData());

			loading->setPercent((*progress)*0.9f + audiobookDetailAPIPercent);
			utils::findChild<ui::Text *>(this, "download_label")->setString(StringUtils::format("%0.1f%%", ((*progress)*0.9f + audiobookDetailAPIPercent)));
		}), this);
	}

}

void MSAudiobookPlayer::onAlertOK(cocos2d::Ref *sender) {
//    getData();
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
//    getData();
    utils::findChild(this, "button_retry")->setVisible(true);
    utils::findChild(this, "button_retry")->setOpacity(255);
    utils::findChild(this, "button_play_main")->setVisible(false);
    _countClickRetry = -1;
    onRetry(this);
}

void MSAudiobookPlayer::onRetry(cocos2d::Ref * sender) {
    utils::findChild(this, "button_retry")->setVisible(false);
    utils::findChild(this, "button_retry")->setOpacity(0);
    utils::findChild(this, "button_play_main")->setVisible(true);
    getData(true,true);
}


void MSAudiobookPlayer::readData() {
    utils::findChild(this, "btdownload")->setVisible(false);
    
    utils::findChild(this, "download_label")->setVisible(false);

    if (auto downloadProgress = utils::findChild<ui::LoadingBar*>(this, "download_progress"))
    {
        downloadProgress->removeFromParent();
    }

    utils::findChild<ui::Button *>(this, "button_play_main")->loadTextureNormal("mjstory/new_ui/button_audiobook_play.png");
    _cached_audio = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(_info.audio_url);

        auto text_story = std::string(_info.sync_path);
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
            auto l = _italic_idx[i + 1];
            ++i;
            
            std::string italic = "";
            
            for (auto c = f; c < l; ++c) {
                italic += text_story[c];
                std::string temp = std::string(text_story.begin(), text_story.begin() + c);
                if (temp.empty() && c != 0) {
                    continue;
                }
                auto count = StringUtils::getCharacterCountInUTF8String(temp);
                auto letter = _text->getLetter((int)count);
                if (count == 0 && c != 0) {
                    continue;
                }
                if (letter) {
                    letter->setSkewX(10);
                }
            }
        }
//    }
    
    auto json_data = mj::helper::getForcedAlignment(_cached_audio);
    auto j = mj::helper::convertForcedAlignment(json_data, _text->getString());
    rapidjson::Document d;
    d.Parse(j.c_str());
    
    
    if (d.HasParseError()) {
        auto btplay = utils::findChild<ui::Button *>(this, "button_play_main");
        btplay->setEnabled(true);
        btplay->setBright(true);
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
    
    auto btplay = utils::findChild<ui::Button *>(this, "button_play_main");
    btplay->setEnabled(true);
    btplay->setBright(true);
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
    if (_is_playing) 
	{
        auto state = experimental::AudioEngine::getState(_audio_id);
        if (state == experimental::AudioEngine::AudioState::ERROR) {
            _audio_id = -1;
            _slider->setPercent(100);
            _text->setColor(AUDIO_NORMAL_COLOR);
            utils::findChild(this, "button_pause")->setVisible(false);
            utils::findChild(this, "button_play")->setVisible(true);
            _should_animate_scrolling = false;
            _is_playing = false;
            this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));;

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            if (MJDEFAULT->getBoolForKey("debugkey_auto_audiobook", false))
            {
                Director::getInstance()->getRunningScene()->scheduleOnce([=](float dt) {
                    onBack(this);

                    ms::CacheManager::getInstance().ms::CacheManager::getInstance().beginAutoPlayAudiobook();

                    }, 0.5f, "delay_back_auto_palayStroy");
            }
//#endif

            return;
        }
        current_time = experimental::AudioEngine::getCurrentTime(_audio_id) * 1000;
		if (m_isSliderBarSeekingEnable)
		{
			_slider->setPercent(std::min(100.f, current_time * 100.f / _info.duration));
		}
    } 
	else 
	{
        current_time = _info.duration * _slider->getPercent() / 100;
        if (_audio_id != -1) {
            auto state = experimental::AudioEngine::getState(_audio_id);
            if (state == experimental::AudioEngine::AudioState::PLAYING || state == experimental::AudioEngine::AudioState::PAUSED)
                current_time = experimental::AudioEngine::getCurrentTime(_audio_id) * 1000;
        }
    }

    _percent_played = MAX(_percent_played, current_time / _info.duration);
    current_time = MIN(current_time, _info.duration);
    
	if (m_isSliderBarSeekingEnable)
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
            experimental::AudioEngine::stop(_audio_id);
            _is_playing = false;
            return;
        }
    }
    
    auto it = std::find_if(std::begin(_sync), std::end(_sync), [current_time](mj::model::SyncBlock tb) {
        return (tb.time_start <= current_time) && (tb.time_end >= current_time);
    });
    
    if (it == std::end(_sync)) {
        return;
    }
    
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

void MSAudiobookPlayer::onAudiobookFinish() {

	//m_completedPercent = (_slider == nullptr ?0: _slider->getPercent());

	RatingManager::GetInstance()->showRatingPopup();

    ms::EventLogger::getInstance().logEvent(ms::analytics::kEventFinishListening, {
        {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
    });
    
    if (book_is_available) {
        FREE_STORY_MANAGER->map_audibook_recent.clear();
    }
    this->setAduioFreePlayMode();
}

#define AB_PLAYER_TRANSITION(x) EaseQuadraticActionIn::create(x)

void MSAudiobookPlayer::onPlay(Ref * sender) 
{
	if (MJDEFAULT->getStringForKey("AUDIOBOOK_START_FROM", "unknow") == "audiobook_search_tab")
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("push_clevertap_event_for_audiobook_tab");
	}
    _isPushEvent = true;
    reinterpret_cast<ui::Button *>(sender)->setEnabled(false);
    reinterpret_cast<ui::Button *>(sender)->setTouchEnabled(false);
    this->indexing();
    _text->setVisible(true);
    this->playTransitionPlay();
    this->scheduleOnce([=](float) {
        _audio_id = experimental::AudioEngine::play2d(_cached_audio);
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartListening, {
            {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
        });
        experimental::AudioEngine::setFinishCallback(_audio_id, [=](int audio_id, std::string path) {
            //end;
            this->onAudiobookFinish();
        });
        
        _should_animate_scrolling = true;
        _is_playing = true;
        this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);
    }, .5f, "schedule_playing");
    StoryDataModel::getInstance()->setAudioRead(_info.book_id);
    StoryDataModel::getInstance()->addAudioBookRecentRead(_info.book_id);
    std::vector <int> valueAudio = StoryDataModel::getInstance()->getValueAudioReadById(_info.book_id);

    if (valueAudio.size() == 3)
    {
        cocos2d::AsyncTaskPool::getInstance()->enqueue(cocos2d::AsyncTaskPool::TaskType::TASK_IO, [=](void* param) {
            mm::RealtimeDatabase::getInstance().updateReadAudios(ap::Database::getInstance().getUserInfo()._id, MJDEFAULT->getIntegerForKey(key_current_profile_id), _info.book_id);
            }, nullptr, [=]() {
                PROFILE_MANAGER.insertValueToTableReadAudioBook(valueAudio);
            });
    }

    //PROFILE_MANAGER.saveReadAudioBooks(StoryDataModel::getInstance()->getAllAudioRead());
}

void MSAudiobookPlayer::indexing() {
    _text->ignoreContentAdaptWithSize(false);
    _text->setTextAreaSize(cocos2d::Size(Director::getInstance()->getVisibleSize().width - Director::getInstance()->getVisibleSize().width * 0.15, 0));
    _text->setContentSize(_text->getVirtualRendererSize());
    _text->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    _text->setPosition(Point(_story_container->getContentSize().width / 2, _story_container->getContentSize().height - TEXT_OFFSET));
    
    auto container_bb = _story_container->getBoundingBox();
    
    auto letter_count = StringUtils::getCharacterCountInUTF8String(_text->getString());
    _letter_positioning.resize(letter_count, Point::ZERO);
    
    Rect union_bb;
    
//    this->scheduleOnce([=](float) {
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
    _text->setTextAreaSize(Size(union_bb.size.width + 1, 0));
    _text->setContentSize(_text->getVirtualRendererSize());
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
        if (onboard_text) {
            onboard_text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-SemiBoldItalic.ttf");
            onboard_text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("audio.onboard.screensaver"));
        }
        auto onboard_count = MJDEFAULT->getIntegerForKey("onboard.screensaver.show.count", 0);
        if (onboard_count < 5 || true) {
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
    
    if (!is_app_active) {
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
    button_back->setPosition(pos + Vec2(100, 100));
    button_back->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    auto text_title = utils::findChild(this, "text_title");
    pos = text_title->getPosition();
    text_title->setPosition(pos + Vec2(0, 100));
    text_title->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    auto button_play_main = utils::findChild(this, "button_play_main");
    pos = button_play_main->getPosition();
    button_play_main->setPosition(pos + Vec2(0, -200));
    button_play_main->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
    
    auto button_download = utils::findChild(this, "btdownload");
    pos = button_download->getPosition();
    button_download->setPosition(pos + Vec2(0, -200));
    button_download->runAction(AB_PLAYER_TRANSITION_ENTER(MoveTo::create(.5f, pos)));
}

#define AB_PLAYER_TRANSITION_EXIT(x) EaseQuadraticActionIn::create(x)

void MSAudiobookPlayer::playTransitionExit() {
    utils::findChild(this, "button_retry")->setVisible(false);
    this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));
    experimental::AudioEngine::setFinishCallback(_audio_id, nullptr);
    experimental::AudioEngine::stopAll();

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

    if (_text)
        _text->runAction(FadeOut::create(transition_duration));
    
    if (_alert) {
        _alert->setDelegate(nullptr);
    }
    
    this->runAction(Sequence::create(DelayTime::create(1), RemoveSelf::create(), NULL));
}

void MSAudiobookPlayer::onExit() {
    ui::Layout::onExit();
    cocos2d::Device::setKeepScreenOn(false);
}


void MSAudiobookPlayer::onBack(Ref * sender) {
    _timetotalListenAudio = int(utils::getTimeInMilliseconds() / 1000) - _timeStartAudio;
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
    utils::findChild(this, "button_pause")->setVisible(false);
    utils::findChild(this, "button_play")->setVisible(true);
    experimental::AudioEngine::pause(_audio_id);
    _is_playing = false;
}

void MSAudiobookPlayer::onPlayerPlay(Ref * sender) {
    utils::findChild(this, "button_pause")->setVisible(true);
    utils::findChild(this, "button_play")->setVisible(false);
    _is_playing = true;
    auto state = experimental::AudioEngine::getState(_audio_id);
    if (state == experimental::AudioEngine::AudioState::ERROR) {
        if (_slider->getPercent() == 100) {
            _slider->setPercent(0);
        }
        _audio_id = experimental::AudioEngine::play2d(_cached_audio);
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventStartListening, {
            {ms::analytics::kPropertyAudiobookName, cocos2d::Value(_info.name)}
        });
        experimental::AudioEngine::setFinishCallback(_audio_id, [=](int audio_id, std::string path) {
            //end;
            this->onAudiobookFinish();
        });
        experimental::AudioEngine::setVolume(_audio_id, 0);
        _should_animate_scrolling = true;
        auto current_time = _info.duration * _slider->getPercent() / 100.f / 1000;
        this->scheduleOnce([=](float) {
            experimental::AudioEngine::setVolume(_audio_id, 1);
            experimental::AudioEngine::setCurrentTime(_audio_id, current_time + 0.1);
            this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);;
        }, 0.1f, "delay_seek");
    } else if (state == experimental::AudioEngine::AudioState::PAUSED) {
        experimental::AudioEngine::resume(_audio_id);
    }
}

void MSAudiobookPlayer::onPlayerForward(Ref * sender) {
    if (_audio_id != -1) {
        auto current_time = experimental::AudioEngine::getCurrentTime(_audio_id);
        current_time += 30;
        current_time = std::min(current_time, _info.duration / 1000.f);
        experimental::AudioEngine::setCurrentTime(_audio_id, current_time);
    } else {
        auto current_time = _info.duration / 1000.f * _slider->getPercent() / 100.f;
        current_time += 30.f;
        current_time = std::min(current_time, _info.duration / 1000.f);
        _slider->setPercent(current_time / (_info.duration / 1000.f) * 100);
        updateSync(0);
    }
}

void MSAudiobookPlayer::onPlayerBackward(Ref * sender) {
    if (_audio_id != -1) {
        auto current_time = experimental::AudioEngine::getCurrentTime(_audio_id);
        current_time -= 30;
        current_time = std::max(0.f, current_time);
        experimental::AudioEngine::setCurrentTime(_audio_id, current_time);
    } else {
        auto current_time = _info.duration / 1000.f * _slider->getPercent() / 100.f;
        current_time -= 30.f;
        current_time = std::max(current_time, 0.f);
        _slider->setPercent(current_time / (_info.duration / 1000.f) * 100);
        updateSync(0);
    }
}

void MSAudiobookPlayer::onSliderEvent(Ref * sender, ui::Slider::EventType event) {

	switch (event) 
	{
        case cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
		{
            //auto duration = experimental::AudioEngine::getDuration(_audio_id);
            //duration = duration * _slider->getPercent() / 100;
            //experimental::AudioEngine::setCurrentTime(_audio_id, duration);
			m_isSliderBarSeekingEnable = false;
			float duration = (_info.duration*(_slider->getPercent() / 100.f));
			updateCurrentTimeLabel(duration);
            break;
        }
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_DOWN:
		{
			m_isSliderBarSeekingEnable = false;
			_should_animate_scrolling = false;
            //_should_animate_scrolling = false;
            //if (!_is_playing && _audio_id == -1)
            //    this->schedule(schedule_selector(MSAudiobookPlayer::updateSync), 0.1);
            break;
		}
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_UP:
		{
			double duration = _info.duration;
			duration = duration * _slider->getPercent() / 100.f;
			experimental::AudioEngine::setCurrentTime(_audio_id, duration / 1000.f);
			m_isSliderBarSeekingEnable = true;
			updateSync(0);
			_should_animate_scrolling = true;
            //_should_animate_scrolling = true;
            //if (!_is_playing && _audio_id == -1)
            //    this->unschedule(schedule_selector(MSAudiobookPlayer::updateSync));
            break;
		}
        case cocos2d::ui::Slider::EventType::ON_SLIDEBALL_CANCEL:
		{
			m_isSliderBarSeekingEnable = true;
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
    if (!_is_favourited) {
        experimental::AudioEngine::play2d("sounds/ui/button_fx/9.mp3");
        gaf->playSequence("action");
        StoryDataModel::getInstance()->addFavouriteAudiobooksByLanguage(_info.book_id, _info.lang_id);
    } else {
        mj::helper::playButtonFX();
        gaf->playSequence("action2");
        StoryDataModel::getInstance()->removeFavouriteAudiobookByLanguage(_info.book_id, _info.lang_id);
    }
    _is_favourited = !_is_favourited;
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
}

void MSAudiobookPlayer::onDownload(cocos2d::Ref *sender) {
    MJDEFAULT->deleteValueForKey(key_error_url_stories);
    utils::findChild(this, "button_retry")->setVisible(false);
    utils::findChild(this, "button_retry")->setOpacity(0);
    utils::findChild(this, "button_play_main")->setVisible(true);
    if (!FileUtils::getInstance()->isFileExist(std::string("audiobooks/audio/") + mj::helper::getFilenameFromURL(_info.audio_url))) {
        getData(true,true);
    } else {
        readData();
    }
    reinterpret_cast<ui::Button *>(sender)->setVisible(false);
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

	if (!currentTimeLabel)
	{
		return;
	}

	auto minute = (int)floor(i_duration / 1000 / 60);

	auto second = (int)floor(i_duration / 1000 - minute * 60);

	if (int(second + 0.5) == 60)
	{
		minute++;
		second = 0;
	}

	currentTimeLabel->setString(StringUtils::format("%02d:%02d", minute, second));
}

void MSAudiobookPlayer::pushCleverTapTrackingDataForWinMac()
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

        std::string gradeName = ms::GradeManager::getInstance().getNameByLanguage(ms::GradeManager::getInstance().getGradeInfoById(_info.grade_id).name, LANGUAGE_EN_CODE);
        std::string readFrom = MJDEFAULT->getStringForKey(key_read_from_of_audiobook);
        int timeAudioBook = (int)(_info.duration / 1000.0);
        if (_timetotalListenAudio == 0)
        {
            _timetotalListenAudio = int(utils::getTimeInMilliseconds() / 1000.0) - _timeStartAudio;
        }
        if (_timetotalListenAudio > timeAudioBook)
        {
            _timetotalListenAudio = timeAudioBook;
        }

        int completed = ((float)(_timetotalListenAudio) / (float)(timeAudioBook)) * 100.f;

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
                {"play_mode", cocos2d::Value(_is_playlist ? "autoplay" : "normal") },
            });
    }
    catch (const std::exception&)
    {

    }
}

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
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
//#endif

MSAudiobookPlayer::~MSAudiobookPlayer()
{
    if (_isPushEvent) {
        pushCleverTapTrackingDataForWinMac();
    }
}
#endif
