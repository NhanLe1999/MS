//
//  StoryAudioPlayer.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/24/18.
//

#include "StoryAudioPlayer.h"
#include "MSDownloader.h"
#include "RemoteImageView.h"
#include "StoryAlert.h"
#include "StoryLanguageManager.h"
#include "../external/json/document.h"
#include "AudioEngine.h"
#include "NativeBridge.h"
#include "manager/CleverTapManager.h"
#include "APDatabase.h"
#include "HelperManager.h"

USING_NS_CC;
INITIALIZE_READER(MSStoryAudioPlayer);

std::once_flag ms_story_audio_player;

int MSStoryAudioPlayer::pending_story_id = 0;

MSStoryAudioPlayer * MSStoryAudioPlayer::createView(int story_id) {
    CRASH_AUTO_LOG;
    std::call_once(ms_story_audio_player, [] {
        REGISTER_CSB_READER(MSStoryAudioPlayer);
    });
    
    auto node = reinterpret_cast<MSStoryAudioPlayer *>(CSLoader::createNode("csb/new_ui/StoryAudioPlayer.csb"));
    if (node) {
        node->didLoadFromCSB(story_id);
        node->setName("qr_story_player");
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

MSStoryAudioPlayer::~MSStoryAudioPlayer()
{
    CRASH_AUTO_LOG;

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

		auto list_category = StoryDataModel::getInstance()->getListCategoryWithStoryId(_info.story_id);

		int catalogID = (list_category.size() > 0 ? list_category.front() : -1);

		std::string catalogName = "empty";

		if (!list_category.empty())
		{
			auto catalogInfo = StoryDataModel::getInstance()->getCategoryByID(catalogID);

			if (catalogInfo.count("content"))
			{
				catalogName = catalogInfo.at("content").asString();
			}
		}

		int			gradeID		= _info.levels.empty() ? -1 : _info.levels.front();
		std::string gradeName	= (gradeID!=-1? StoryDataModel::getInstance()->getLevelInfoById(gradeID, _info.lang_id).name : "unknow");

		CleverTapManager::GetInstance()->pushEvent("learn_qrcode",
		{
			{"item_id",cocos2d::Value(_info.story_id)},
			{"item_name",cocos2d::Value(_info.name)},
			{"category_id",cocos2d::Value(catalogID)},
			{"category_name",cocos2d::Value(catalogName)},
			{"grade_id",cocos2d::Value(gradeID)},
			{"grade_name",cocos2d::Value(gradeName)},
			{"total_page",cocos2d::Value(_info.total_page)},
			{"user_profile_age",cocos2d::Value(profileAge)},
			{"completed",cocos2d::Value((int)((float)(_current_page_id+1)/ (float)_info.total_page))},
			{"close_at",cocos2d::Value(_current_page_id==-1? _current_page_id: _current_page_id+1)},
            {"time_purchased", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_purchased",0)))},
            {"time_activated",cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_activated",0)))},
            {"time_expired", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_expire",0))) },
		});

	}
	catch (const std::exception&)
	{

	}
}

void MSStoryAudioPlayer::didLoadFromCSB(int story_id)
{
    MS_LOGGER.logEvent("MSStoryAudioPlayer::didLoadFromCSB", {{"storyID", cocos2d::Value(story_id)}});
    auto visible_size = Director::getInstance()->getVisibleSize();
    
    this->setTouchEnabled(true);
    this->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->setPosition(visible_size / 2);
    
    auto dim = ui::Layout::create();
    dim->setContentSize(visible_size);
    dim->setAnchorPoint(Point::ANCHOR_MIDDLE);
    dim->setPosition(this->getContentSize() / 2);
    dim->setTouchEnabled(true);
    this->addChild(dim, -1);
    
    //create background
    auto pattern = Director::getInstance()->getTextureCache()->addImage("mjstory/new_ui/qr_texture.png");
    const Texture2D::TexParams tex_params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    
    auto background = Sprite::createWithTexture(pattern, Rect(0, 0, visible_size.width, visible_size.height));
    background->getTexture()->setTexParameters(tex_params);
    background->setAnchorPoint(Point::ANCHOR_MIDDLE);
    background->setPosition(this->getContentSize() / 2);
    this->addChild(background, -1);
    
    _current_page_id = _current_audio_id = -1;
    _info = StoryDataModel::getInstance()->getStoryById(story_id);
    _info.total_page++;
    
    //show info
    auto level_info = StoryDataModel::getInstance()->getLevelInfoById(_info.levels[0], _info.lang_id);
    auto level_name = StringUtils::format("%s %s", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "thumb.story.level").c_str(), level_info.name.c_str());
    if (level_name.front() == ' ') level_name.erase(level_name.begin());
    utils::findChild<ui::Text *>(this, "text_level")->setString(level_name);
    utils::findChild<ui::Text *>(this, "text_page_count")->setString(StringUtils::toString(_info.total_page));
    utils::findChild<ui::Text *>(this, "text_lexile")->setString(_info.lexile);
    
    //show indicator
    auto indicator = utils::findChild<ui::Layout *>(this, "panel_indicator");
    indicator->setContentSize(Size(100 * std::min(4, _info.total_page - 1), 15));

    for (auto i = 0; i < std::min(5, _info.total_page); ++i) {
        auto dot = ui::ImageView::create("mjstory/new_ui/qr_dot_normal.png");
        dot->setAnchorPoint(Point::ANCHOR_MIDDLE);
        dot->setPosition(Point(i * 100, 7.5));
        indicator->addChild(dot);
        _dots.push_back(dot);
        
        auto page = ui::Text::create(StringUtils::toString(i + 1), "fonts/Montserrat-Bold.ttf", 30);
        page->setTextColor(Color4B::WHITE);
        page->setAnchorPoint(Point::ANCHOR_MIDDLE);
        page->setPosition(Point(20, 20));
        page->setName("page");
        dot->addChild(page);
    }
    ui::Helper::doLayout(indicator);
    
    auto story_name = utils::findChild<ui::Text *>(this, "story_title");
    story_name->setString(_info.name);
    
    auto is_downloaded = StoryDataModel::getInstance()->getStoryIsDownloadedWith(_info.unique_id, _info.story_id, _info.lang_id);
    if (is_downloaded) {
        onDownloadAndUnzipDone();
    } else {
        utils::findChild(this, "panel_controller")->setVisible(false);
        utils::findChild(this, "button_download")->setVisible(true);
    }
    
    _loading_bar = utils::findChild<ui::LoadingBar *>(this, "loading_bar");
    _progress_percent = utils::findChild<ui::Text *>(this, "text_percent");
    utils::findChild(this, "panel_progress")->setVisible(false);
    utils::findChild(this, "button_pause")->setVisible(false);
    
    auto remoteview = RemoteImageView::createImage(_info.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
        if (is_success) {
//            this->getChildByName("image")->setVisible(false);
            utils::findChild(this, "placeholder")->runAction(FadeOut::create(0.25));
            img->setContentSize(Size(340, 460));
            img->setOpacity(0);
            img->runAction(FadeIn::create(0.25));
        }
    }, RemoteImageView::Type::STORIES);
    
    auto placeholder = utils::findChild(this, "placeholder");
    remoteview->setPosition(placeholder->getPosition());
    remoteview->setAnchorPoint(placeholder->getAnchorPoint());
    this->addChild(remoteview);
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "lb_level"),
            utils::findChild<ui::Text *>(this, "lb_lexile"),
            utils::findChild<ui::Text *>(this, "lb_page"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
        }
    }
    
    {
        std::vector<cocos2d::ui::Text*> textSameFonts =
        {
            utils::findChild<ui::Text *>(this, "label_prev_page"),
            utils::findChild<ui::Text *>(this, "label_play"),
            utils::findChild<ui::Text *>(this, "label_next_page"),
        };
        
        for(auto text : textSameFonts)
        {
            if(!text)
            {
                continue;
            }
            
            text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        }
    }
     
    
    utils::findChild<ui::Text *>(this, "lb_level")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.level"));
    utils::findChild<ui::Text *>(this, "lb_lexile")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.lexile"));
    utils::findChild<ui::Text *>(this, "lb_page")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.page"));
    utils::findChild<ui::Text *>(this, "label_play")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.play"));
    utils::findChild<ui::Text *>(this, "label_play")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.play"));
    utils::findChild<ui::Text *>(this, "label_prev_page")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.prev.page"));
    utils::findChild<ui::Text *>(this, "label_next_page")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.next.page"));
}

void MSStoryAudioPlayer::onEnter() {
    CRASH_AUTO_LOG;
    ui::Layout::onEnter();
}

void MSStoryAudioPlayer::onExit() {
    CRASH_AUTO_LOG;
    ui::Layout::onExit();
    experimental::AudioEngine::stopAll();
}

void MSStoryAudioPlayer::onDownload(Ref * sender) {
    CRASH_AUTO_LOG;
    if(!NativeBridge::networkAvaiable())
    {
        auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
        auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
        alert->setNetworkDownContent(false, true);
        alert->show();
        return;
    }

    auto url = StoryDataModel::getInstance()->getLinkDownloadStory(_info.unique_id);
    CCLOG("link_story = %s", url.c_str());
    
    utils::findChild(this, "panel_progress")->setVisible(true);
    utils::findChild(this, "button_download")->setVisible(false);
    
    //trường hợp đã có zip
    auto fname = mj::helper::getFilenameFromURL(url);
    auto fdirectory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
    auto fpath = fdirectory + fname;
    
    if (FileUtils::getInstance()->isFileExist(fpath)) {
        this->getEventDispatcher()->removeCustomEventListeners(fpath + "_unzip_done");
        this->getEventDispatcher()->removeCustomEventListeners(fpath + "_unzip_error");
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(fpath + "_unzip_done", [this](EventCustom * e) {
            this->onDownloadAndUnzipDone();
        }), this);
        
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(fpath + "_unzip_error", [this](EventCustom * e) {
            this->onUnzipError(-1, "");
        }), this);
        
        mj::helper::unzipAsync(fpath, fdirectory, [=](bool success) {
            if (success) {
                /*
                 * Unzip success
                 */
                MS_LOGGER.logEvent("Unzip Success", {});
                FileUtils::getInstance()->removeFile(fpath);
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_done");
            } else {
                /*
                 * Unzip Error
                 * Push user_id, story_id, device_id, url_download with custom key: story_unzip_error to crashlytics.
                 */
                MS_LOGGER.logEvent("Unzip Error", {});
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                std::string userID = MJDEFAULT->getStringForKey(key_user_id);
                std::string deviceID = MJ_PLATFORM_CONFIG.getDeviceId();
                std::string urlDownload = url;
                std::string storyID = std::to_string(_info.story_id);
                std::string key = "story_unzip_error";
                std::string value = "UserID = " + userID + "\nDeviceID = " + deviceID + "\nStoryID = " + storyID + "\nUrlDownload = " + urlDownload + "\n";
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.MJHelper", "pushCrashlyticWithCustomKeyAndValue", key, value);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#endif
#endif


                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(fpath + "_unzip_error");
            }
        });
        return;
    }
    
    ms::utils::DownloadUnit unit;
    unit.url = url;
    unit.download_id = mj::helper::getFilenameFromURL(url);
    unit.storage_path = FileUtils::getInstance()->getWritablePath() + "mjstory/" + unit.download_id;
    unit.success_event = "download_story_success" + unit.download_id;
    unit.failed_event = "download_story_failed" + unit.download_id;
    unit.progress_event = "download_story_progress" + unit.download_id;
    
    this->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
    this->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
    this->getEventDispatcher()->removeCustomEventListeners(unit.progress_event);
    
    auto filename = mj::helper::getFilenameFromURL(url);
    auto directory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
    auto full_path = directory + filename;
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [=](EventCustom *) {
        mj::helper::unzipAsync(full_path, directory, [=](bool success) {
            if (success) {
                FileUtils::getInstance()->removeFile(fpath);
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(full_path + "_unzip_done");
            } else {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(full_path + "_unzip_error");
            }
        });
    }), this);
    
    this->getEventDispatcher()->removeCustomEventListeners(full_path + "_unzip_done");
    this->getEventDispatcher()->removeCustomEventListeners(full_path + "_unzip_error");
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(full_path + "_unzip_done", [this](EventCustom * e) {
        this->onDownloadAndUnzipDone();
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(full_path + "_unzip_error", [this](EventCustom * e) {
        this->onUnzipError(-1, LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure"));
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [=](EventCustom * e) {
        auto u = (ms::utils::DownloadUnit *) e->getUserData();
        this->onDownloadError(-1,LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure")); // u->error_message);
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.progress_event, [=](EventCustom * e) {
        auto p = reinterpret_cast<double *>(e->getUserData());
        _loading_bar->setPercent(* p);
        _progress_percent->setString(StringUtils::format("%0.1f %%", * p));
    }), this);
    
    ms::utils::Downloader::getInstance().enqueue(unit);
}

void MSStoryAudioPlayer::onDownloadAndUnzipDone() {
    CRASH_AUTO_LOG;
    utils::findChild(this, "panel_progress")->setVisible(false);
    utils::findChild(this, "panel_controller")->setVisible(true);
    utils::findChild(this, "button_download")->setVisible(false);
    
    utils::findChild(this, "button_back_page")->setVisible(false);
    utils::findChild(this, "button_next_page")->setVisible(false);
    utils::findChild(this, "label_prev_page")->setVisible(false);
    utils::findChild(this, "label_next_page")->setVisible(false);
    
    StoryDataModel::getInstance()->downloadSuccess(_info.unique_id, _info.story_id, _info.lang_id);
    parseData();
}

void MSStoryAudioPlayer::onUnzipError(int error_code, std::string error_string) {
    MS_LOGGER.logEvent("MSStoryAudioPlayer::onUnzipError", {{"Error code", cocos2d::Value(error_code)}});
    showNetworkError(error_string);
    utils::findChild(this, "panel_progress")->setVisible(false);
    utils::findChild(this, "button_download")->setVisible(true);
    _loading_bar->setPercent(0);
    _progress_percent->setString("0%%");
}

void MSStoryAudioPlayer::onDownloadError(int error_code, std::string error_string) {
    MS_LOGGER.logEvent("MSStoryAudioPlayer::onDownloadError", {{"Error code", cocos2d::Value(error_code)}});
    showNetworkError(error_string);
    utils::findChild(this, "panel_progress")->setVisible(false);
    utils::findChild(this, "button_download")->setVisible(true);
    _loading_bar->setPercent(0);
    _progress_percent->setString("0%%");
}

void MSStoryAudioPlayer::parseData() {
    CRASH_AUTO_LOG;
    auto metadata = FileUtils::getInstance()->getWritablePath() + cocos2d::StringUtils::format("mjstory/%d_%d_%d.plist", _info.story_id, _info.lang_id, 0);
    if (FileUtils::getInstance()->isFileExist(metadata)) {
        auto _mapT0 = FileUtils::getInstance()->getValueMapFromFile(metadata);
        if (!_mapT0["title"].isNull()) {
            auto _mapTitle = _mapT0["title"].asValueMap();

            _audio_title = _mapTitle["audio"].asString();
            _audio_written = _mapT0["writentby"].asValueMap()["audio"].asString();
            _audio_design = _mapT0["illustratedby"].asValueMap()["audio"].asString();
        }
    }
    
    _list_audio.push_back(std::make_pair(_audio_title, 0));
    _list_audio.push_back(std::make_pair(_audio_written, 0));
    _list_audio.push_back(std::make_pair(_audio_design, 0));
    _list_audio.push_back(std::make_pair("sounds/ting.mp3", 0));

    auto page_id = 1;
    do {
        auto page_filename = StringUtils::format("%d_%d_%d.json", _info.story_id, _info.lang_id, page_id);
        if (!FileUtils::getInstance()->isFileExist(page_filename)) {
            break;
        }
        
        auto json = FileUtils::getInstance()->getStringFromFile(page_filename);
        rapidjson::Document d;
        d.Parse(json.c_str());
        
        if (!d.HasMember("audio")) {
            return;
        }
        
        bool flag_single_audio = false;
        if (d.HasMember("box_type") && d["box_type"].IsString()) {
            flag_single_audio = std::string(d["box_type"].GetString()) == "box_type_2";
        }
        
        auto audio = d["audio"].GetArray();
        for (auto i = 0; i < audio.Size(); ++i) {
            auto single_audio = audio[i].GetObject();
            auto audio_path = std::string(single_audio["path"].GetString());
            
            _list_audio.push_back(std::make_pair(audio_path, page_id));
            if (flag_single_audio)
                break;
        }
        
        _list_audio.push_back(std::make_pair("sounds/ting.mp3", page_id));
        
        page_id++;
    } while (true);
}

bool MSStoryAudioPlayer::isPlaying() {
    return !utils::findChild(this, "button_play")->isVisible();
}

void MSStoryAudioPlayer::onPlay(Ref * sender) {
    CRASH_AUTO_LOG;
    utils::findChild(this, "button_play")->setVisible(false);
    utils::findChild(this, "button_pause")->setVisible(true);
    utils::findChild<ui::Text *>(this, "label_play")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.pause"));
    
    if (!_is_metadata_played)
    {
        utils::findChild(this, "button_pause")->setOpacity(0xaa);
        utils::findChild<ui::Button *>(this, "button_pause")->setTouchEnabled(false);
        //nếu chưa play metadata
        auto instruction_id = experimental::AudioEngine::play2d("sounds/qr_instruction.mp3");
        experimental::AudioEngine::setFinishCallback(instruction_id, [this](int, std::string)
        {
            this->_is_metadata_played = true;
            this->onPlay(utils::findChild(this, "button_play"));
        });
        return;
    }

    utils::findChild(this, "button_pause")->setOpacity(0xff);
    utils::findChild<ui::Button *>(this, "button_pause")->setTouchEnabled(true);
    utils::findChild(this, "button_back_page")->setVisible(true);
    utils::findChild(this, "button_next_page")->setVisible(true);
    utils::findChild(this, "label_prev_page")->setVisible(true);
    utils::findChild(this, "label_next_page")->setVisible(true);
    
    if (_current_page_id == -1) {
        playFromPage(0, 0);
    } else {
        if (_current_audio_id != -1) {
            experimental::AudioEngine::resume(_current_audio_id);
        } else {
            playFromPage(_current_page_id, 0);
        }
    }
}

void MSStoryAudioPlayer::onPause(Ref * sender) {
    CRASH_AUTO_LOG;
    experimental::AudioEngine::pause(_current_audio_id);
    utils::findChild(this, "button_play")->setVisible(true);
    utils::findChild(this, "button_pause")->setVisible(false);
    utils::findChild<ui::Text *>(this, "label_play")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.play"));
}

void MSStoryAudioPlayer::onNextPage(Ref * sender) {
    CRASH_AUTO_LOG;
    if (_current_page_id + 1 < _info.total_page) {
        if (isPlaying()) {
            playFromPage(_current_page_id + 1, 0);
        } else {
            _current_page_id++;
            _current_audio_id = -1;
            setIndicator(_current_page_id);
        }
    }
}

void MSStoryAudioPlayer::onPrevPage(Ref * sender) {
    CRASH_AUTO_LOG;
    if (_current_page_id - 1 >= 0) {
        if (isPlaying()) {
            playFromPage(_current_page_id - 1, 0);
        } else {
            _current_page_id--;
            _current_audio_id = -1;
            setIndicator(_current_page_id);
        }
    }
}

void MSStoryAudioPlayer::playFromPage(int page_id, int audio_in_page)
{
    MS_LOGGER.logEvent("MSStoryAudioPlayer::playFromPage", {
            {"page_id", cocos2d::Value(page_id)},
            {"audio_in_page", cocos2d::Value(audio_in_page)}
    });
    if (page_id >= (int) _list_audio.size()) {
        setIndicator(0);
        _current_page_id = -1;
        utils::findChild(this, "button_play")->setVisible(true);
        utils::findChild(this, "button_pause")->setVisible(false);
        utils::findChild<ui::Text *>(this, "label_play")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("qr.play"));
        return;
    }
    
    auto counter = -1;
    std::string audio_path = "";
    for (auto i = 0; i < (int)_list_audio.size(); ++i) {
        auto p = _list_audio[i];
        if (p.second == page_id) {
            counter++;
            if (counter == audio_in_page) {
                audio_path = p.first;
            }
        }
    }
    
    if (audio_path.empty()) {
        playFromPage(page_id + 1, 0);
        return;
    }
    
    setIndicator(page_id);
    _current_page_id = page_id;
    _current_audio_in_page = audio_in_page;
    
    //if(page_id !=0)
    //{
        experimental::AudioEngine::stopAll();//(_current_audio_id);
    //}

    _current_audio_id = experimental::AudioEngine::play2d(audio_path);
    experimental::AudioEngine::setFinishCallback(_current_audio_id, [=](int, std::string) {
        this->playFromPage(_current_page_id, _current_audio_in_page + 1);
    });
}

void MSStoryAudioPlayer::setIndicator(int page_id) {
    MS_LOGGER.logEvent("MSStoryAudioPlayer::setIndicator", {{"page_id", cocos2d::Value(page_id)}});
    auto front = 0;
    if (page_id < (int)_dots.size() / 2) {
        front = 0;
    } else if (page_id >= _info.total_page - (int)_dots.size() / 2) {
        front = _info.total_page - (int)_dots.size();
    } else {
        front = page_id - 2;
    }
    
    for (auto i = 0; i < (int)_dots.size(); ++i) {
        if (front == page_id) {
            _dots[i]->loadTexture("mjstory/new_ui/qr_dot_highlight.png");
        } else {
            _dots[i]->loadTexture("mjstory/new_ui/qr_dot_normal.png");
        }
        auto page = _dots[i]->getChildByName<ui::Text *>("page");
        page->setString(StringUtils::toString(front + 1));
        front++;
    }
}

void MSStoryAudioPlayer::showNetworkError(std::string message) {
    CRASH_AUTO_LOG;
    if (message.empty()) {
        message = LANGUAGE_MANAGER->getDisplayTextByKey("parent.common.error");
    }
    auto csb_name = mj::helper::getCSBName("csb/StoryAlertPlayGame.csb");
    auto alert = static_cast<StoryAlert *>(CSLoader::createNode(csb_name));
    alert->setContent(message, "Try Again", "", false, 36);
    alert->setTimelineName("csb/StoryAlertPlayGame.csb");
    alert->show();
//    alert->setDelegate(this);
}

#include "MMQRScan.h"
#include "MSSidebar.h"
void MSStoryAudioPlayer::onBack(Ref * sender) {
    CRASH_AUTO_LOG;
    this->removeFromParent();
    MJDEFAULT->setBoolForKey(key_app_should_show_qr, true);
    if (MSSidebar::defaultSidebar) {
        MSSidebar::defaultSidebar->enableQR();
    }
}

ui::Widget::ccWidgetClickCallback MSStoryAudioPlayer::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSStoryAudioPlayer, onDownload),
        CLICK_MAP(MSStoryAudioPlayer, onBack),
        CLICK_MAP(MSStoryAudioPlayer, onPlay),
        CLICK_MAP(MSStoryAudioPlayer, onPause),
        CLICK_MAP(MSStoryAudioPlayer, onPrevPage),
        CLICK_MAP(MSStoryAudioPlayer, onNextPage),
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
