//
//  MSActivityBase.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/17.
//

#include "MSActivityWrapper.h"
#include "MSDownloader.h"
#include "GameBuildWall.h"
#include "BuildWord_GamePlay.h"
#include "MSLessonManager.h"
#include "StoryInfoCover.h"

#include "MSLessonManager.h"
#include "MSLessonEnd.h"
#include "MSActivityWait.h"

#include "MSActivityEnd.h"

#include "MSLessonEndFX.h"
#include "StoryFreeManager.h"
#include "MJDefinitions.h"

#include "PoolManager.h"
#include "manager/RatingManager.h"
#include "MSEventLogger.h"

#include "StoryRateAction.h"
#include "GroupPromotion.h"
#include "StoryPageVC.h"
#include "APWaitView.h"

#include "CleverTapManager.h"
#include "HelperManager.h"
#include "GameQuizLayout.h"
#include "APProfileManager.h"

USING_NS_CC;

void MSActivityWrapper::preSetupBeforeStartGame() {
    
    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + "extracted/0/", true);
    CCLOG("add searchpath");
    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath() + "extracted/0/" + (StoryData::getInstance()->isHdr ? "hdr/" : "hd/"), true);
    CCLOG("add searchpath");
    
}

MSActivityWrapper * MSActivityWrapper::createWithInfo(mj::model::ActInfo info,START_FROM i_startFrom )
{
    auto p = new (std::nothrow) MSActivityWrapper();
    
    if (p && p->initWithInfo(info,i_startFrom))
    {
        p->autorelease();
        return p;
    }
    else
    {
        CC_SAFE_DELETE(p);
        return nullptr;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MSActivityWrapper::initWithInfo(mj::model::ActInfo info,START_FROM i_startFrom)
{
    if (!ui::Layout::init())
    {
        return false;
    }
    
    _info = info;
    m_startFrom = i_startFrom;
    this->setContentSize(Director::getInstance()->getVisibleSize());
    this->setBackGroundColor(Color3B::BLACK);
    this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onEnter()
{
    MS_LOGGER.logEventStartActivity(MJDEFAULT->getBoolForKey(key_is_phonic_lesson) ? "PHONICS" : "STORIES",_info);

    ui::Layout::onEnter();

    this->setTouchEnabled(true);
    
    _callbacknResourceDownloaded = nullptr;
    _callbacknshowActivity = nullptr;
    _callbacknOnActivityEndWithReason = nullptr;
    
    std::call_once(_flag_show_activity, [=]
    {
        if (_info.type == mj::model::ActInfo::STORY)
        {
            auto view = MSActivityWait::createView();
            view->setName("MSActivityWait");
            this->addChild(view);
            
            this->scheduleOnce([=](float dt)
            {
                    
                if(ms::LessonManager::getInstance().isActivityAvail(_info)) {
                    _info.isUnzipError = false;
                    this->showActivity();
                } else {
                    _info.isUnzipError = true;
                    if (auto popupStart = utils::findChild(Director::getInstance()->getRunningScene(), "popup_start")) {
                        popupStart->removeFromParent();
                    }
                    auto popupError = UnzipErrorPopup::createPopup(UnzipErrorPopup::PopupType::STORY_IN_LESSON, _info, this);
                    auto runningSceneSize = Director::getInstance()->getRunningScene()->getContentSize();
                    popupError->setContentSize(runningSceneSize);
                    Director::getInstance()->getRunningScene()->addChild(popupError, INT_MAX);
                }
                
            }, 0.75, "delay_loading");
            
            return;
        }
        
        if (isResourceReady())
        {
            auto view = MSActivityWait::createView();
            view->setName("MSActivityWait");
            this->addChild(view);
            
            this->scheduleOnce([=](float dt) {
                this->onResourceDownloaded();
            }, 0.75, "delay_loading");
        }
        else
        {
            waitAndStartDownloading();
        }
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_subscribe_page, [=](EventCustom *) {
        this->removeFromParent();
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_go_to_free_page, [=](EventCustom *) {
        this->removeFromParent();
    }), this);

    setCallbackResourceDownloaded();
    setCallbackShowActivity();
    setCallbackOnActivityEndWithReason();
    setCallbackOnUnzip();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onExit()
{
    _callbacknResourceDownloaded = nullptr;
    _callbacknshowActivity = nullptr;
    _callbacknOnUnzip = nullptr;
    if(_info.type != mj::model::ActInfo::Type::STORY)
    {
        _callbacknOnActivityEndWithReason = nullptr;
    }
    ui::Layout::onExit();
    mj::helper::purgeTextureData();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MSActivityWrapper::isResourceReady()
{
    CRASH_AUTO_LOG;
    
	return ms::LessonManager::getInstance().isActivityReady(_info);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::waitAndStartDownloading()
{
    auto view = MSActivityWait::createView();
    view->setName("MSActivityWait");
    this->addChild(view);
    
    auto listener_activity_ready = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventActivityReady(_info), [=](EventCustom * e) {
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](){
            
            this->onActivityReady();
        });
    
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_activity_ready, this);
    
    auto listener_activity_error = EventListenerCustom::create(ms::LessonManager::getInstance().getKeyEventActivityError(_info), [=](EventCustom * e) {
        this->onActivityError();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_activity_error, this);
    
    if(m_startFrom == START_FROM::END_STORY)
    {
        onDowloadingResource();
    }
    else
    {
        ms::LessonManager::getInstance().prepareActivity(_info);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onDowloadingResource()
{
    if (isResourceReady()) {
        //        callback ? callback(true) : (void) nullptr;
        onActivityReady();
        return;
    }
    
    auto success_event = StringUtils::format("download_success_%d_%d", _info.lesson_id, _info.activity_id);
    auto error_event = StringUtils::format("download_error_%d_%d", _info.lesson_id, _info.activity_id);
    auto progress_event = StringUtils::format("download_progress_%d_%d", _info.lesson_id, _info.activity_id);
    
    auto relative_directory = std::string("cached/zip/");
    if (_info.type == mj::model::ActInfo::Type::STORY)
    {
        relative_directory = "mjstory/";
    }
    
    for (auto url : _info.zip_urls)
    {
        ms::utils::DownloadUnit unit;
        unit.url = url;
        unit.download_id = mj::helper::getFilenameFromURL(unit.url);
        unit.storage_path = FileUtils::getInstance()->getWritablePath() + relative_directory + unit.download_id;
        unit.success_event = success_event;
        unit.failed_event = error_event;
        unit.progress_event = progress_event;
        
        //        if (is_retry) {
        //            if (!MJDEFAULT->getStringForKey(key_alternative_domain_lesson).empty())
        //                unit.url = mj::helper::replaceDomain(unit.url, MJDEFAULT->getStringForKey(key_alternative_domain_lesson));
        //        }
        
        CCLOG("activity: %s", _info.name.c_str());
        CCLOG("activity url: %s", url.c_str());
        
        if (!FileUtils::getInstance()->isFileExist(unit.storage_path)) {
            ms::utils::Downloader::getInstance().enqueue(unit);
        }
    }
    
    CCLOG("remove listener %s", success_event.c_str());
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
    
    CCLOG("add listener %s", success_event.c_str());
    Director::getInstance()->getEventDispatcher()->addCustomEventListener(success_event, [=](EventCustom * e) {
        if (this->isResourceReady()) {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
            
            //                callback ? callback(true) : (void) nullptr;
            this->onActivityReady();
            
        }
        
        auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
        CCLOG("Download success: %s", unit->storage_path.c_str());
    });
    
    auto acts = StoryDataModel::getInstance()->getActivitiesOfStory (_info.lesson_id);
    if (acts.empty()) {
        return;
    }
    if (acts[0].activity_id == _info.activity_id) {
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(progress_event, [=](EventCustom * e) {
            auto progress1 = reinterpret_cast<double *>(e->getUserData());
            auto progress = *progress1;
            auto relative_directory = std::string("cached/zip/");
            auto counter = 0;
            
            for (auto url : _info.zip_urls) {
                auto path = FileUtils::getInstance()->getWritablePath() + relative_directory + mj::helper::getFilenameFromURL(url);
                
                if (FileUtils::getInstance()->isFileExist(path)) {
                    counter++;
                }
            }
            
            progress = counter * 100.f / _info.zip_urls.size() + progress * (1.f / _info.zip_urls.size());
            
            EventCustom event(StringUtils::format("event_activity_progress_%d", _info.activity_id));
            event.setUserData(&progress);
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            
        });
        
        CCLOG("add listener %s", error_event.c_str());
        Director::getInstance()->getEventDispatcher()->addCustomEventListener(error_event, [=](EventCustom * e) {
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(error_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(success_event);
            Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(progress_event);
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(StringUtils::format("event_activity_error_%d", _info.activity_id));
            auto unit = reinterpret_cast<ms::utils::DownloadUnit *>(e->getUserData());
            CCLOG("Download error: %s", unit->url.c_str());
        });
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onResourceDownloaded()
{
    CRASH_AUTO_LOG;
    
    if (_callbacknResourceDownloaded) {
        _callbacknResourceDownloaded();
    }
    else
    {
        MS_LOGGER.logEvent("onResourceDownloaded_resource_not_ready", {});
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::showActivity()
{
    if (_callbacknshowActivity)
    {
        _callbacknshowActivity();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onActivityEnd(Ref * sender, ms::delegate::Activity::EndReason reason) {

    CRASH_AUTO_LOG;
    
    if(auto mjgame = utils::findChild(Director::getInstance()->getRunningScene(), "MJGameInstructionOverlay"))
    {
        mjgame->removeFromParent();
    }
    
    mpool->resetAll();
    mj::helper::purgeTextureData();

    mj::helper::hideTap();
    CCLOG("Finished");
	AUDIO_ENGINE->stopAllEffects();
    
    if(m_startFrom == START_FROM::END_STORY) // cau hoi tuong tac.
    {
        auto parent = dynamic_cast<StoryPageVC*>(this->getParent());
        
        if (parent)
        {
            parent->showGameOver(reason == ms::delegate::Activity::EndReason::INTERRUPTED);
            
            return;
        }
    }
    
    if(m_startFrom == START_FROM::LESSON)
    {
        FileUtils::getInstance()->removeDirectory(FileUtils::getInstance()->getWritablePath() + "extracted/");
        
        if (_info.type == mj::model::ActInfo::GAME)
        {
            auto searchpath = FileUtils::getInstance()->getSearchPaths();
            CCLOG("Remove directory: %s", searchpath.begin()->c_str());
            searchpath.erase(searchpath.begin());
            CCLOG("Remove directory: %s", searchpath.begin()->c_str());
            searchpath.erase(searchpath.begin());
            FileUtils::getInstance()->setSearchPaths(searchpath);
        }
    }
    
    onActivityEndWithReason(reason);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onActivityReady()
{
    CRASH_AUTO_LOG;
    
    this->scheduleOnce([=](float dt)
    {
        this->onResourceDownloaded();
        
    }, 0.25, "delay_loading");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onActivityError()
{
    CRASH_AUTO_LOG;
    auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
    auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
    alert->setNetworkDownContent(false, true);
    alert->show();
    this->removeFromParent();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onStartOver()
{
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onContinueLearn(mj::model::ActInfo info)
{
    mj::helper::purgeTextureData();

    auto next_activity = ms::LessonManager::getInstance().getNextActivity(info);
    info.startFrom = mj::model::ActInfo::START_LESOON;
    auto next = MSActivityWrapper::createWithInfo(next_activity);
    next->setName("wrapper");
    next->setVisible(false);
    Director::getInstance()->getRunningScene()->addChild(next);
    
    auto close_listener = EventListenerCustom::create(key_event_activity_wrapper_ready, [=](EventCustom*) {

        this->removeFromParent();
        //if (auto tmp = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper")) {
        //    tmp->removeFromParent();
        //}

    });

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(close_listener, this);

}

void MSActivityWrapper::onActivityEndWithReason(ms::delegate::Activity::EndReason reason)
{
    if (_callbacknOnActivityEndWithReason)
    {
        _callbacknOnActivityEndWithReason(reason);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MSActivityWrapper::onBackToLesson()
{
    this->removeFromParent();
}

void MSActivityWrapper::pushCleverTapTrackingData_Vmonkey(const mj::model::LessonInfo& lesson, int numb_of_activities, int currentActivityID)
{

    if (CONFIG_MANAGER->isVietnameseStories())
    {
        try
        {
    
            // tránh crash cho hàm std::stoll. Vì hàm đó nếu giá trị truyền vào trống là sẽ bị crash
            auto startTime = MJDEFAULT->getStringForKey("vm_phonics_start_time");
            
            auto value = startTime.empty() ? 0: std::stoll(startTime);
            
            // thay thế các ký tự ',' sang '_'
            std::string str_lesson_name = lesson.name;
            mj::helper::replace_string_all(str_lesson_name, ",", "_");
            
            // TODO: CleverTap-pushEvent: "learn_lesson"
            CleverTapManager::GetInstance()->pushEvent("learn_lesson",
            {
                {"lesson_id",cocos2d::Value(lesson.lesson_id)},
                {"lesson_name",cocos2d::Value(str_lesson_name)},
                {"course_type",cocos2d::Value("lesson_phonics")},
                {"grade_id",cocos2d::Value(lesson.category_id)},
                {"grade_name",cocos2d::Value(MJDEFAULT->getStringForKey("GRADE_NAME", "unknow"))},
                {"completed",cocos2d::Value((int)(((float)(currentActivityID)/ (float)numb_of_activities)*100))},
                {"number_activities",cocos2d::Value((int)numb_of_activities)},
                {"time_spent",cocos2d::Value((int)(mj::helper::getTime() - value))},
                {"stop_at_activity",cocos2d::Value(currentActivityID)},
                {"time_purchased", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_purchased",0)))},
                {"time_activated",cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_activated",0)))},
                {"time_expired", cocos2d::Value(HelperManager::GetInstance()->convertToDatetimeForClevertap(MJDEFAULT->getIntegerForKey("key_time_expire",0))) },
            });
        }
        catch (const std::exception&)
        {
            
        }
    }
}

void MSActivityWrapper::setCallbackResourceDownloaded()
{
    _callbacknResourceDownloaded = [=] {
        if (isResourceReady())
        {
            auto game_info = StoryDataModel::getInstance()->getGameById(_info.sub_id);

            std::vector<std::pair<std::string, std::string>> unzip_data;

			_info.zip_urls.erase(unique(_info.zip_urls.begin(), _info.zip_urls.end()), _info.zip_urls.end());

            for (auto i = 0; i < _info.zip_urls.size(); ++i)
            {
                auto path = FileUtils::getInstance()->getWritablePath() + "cached/zip/" + mj::helper::getFilenameFromURL(_info.zip_urls[i]);
                auto directory = FileUtils::getInstance()->getWritablePath() + "extracted/" + StringUtils::toString(i / 2) + "/";
                unzip_data.push_back(std::make_pair(path, directory));
            }

            if (_callbacknshowActivity)
            {
                CCLOG("chuan bi crash0");

                mj::helper::unzipAsync(unzip_data, [=](bool success) {

                    CCLOG("unzip end");
                    if (_callbacknOnUnzip)
                    {
                        _callbacknOnUnzip(success);
                    }

                    });

            }
        }
        else
        {
            MS_LOGGER.logEvent("onResourceDownloaded_resource_not_ready", {});
        }

    };
}

void MSActivityWrapper::setCallbackShowActivity()
{
    _callbacknshowActivity = [=] {
        std::call_once(_display_activity, [=]
            {
                switch (_info.type)
                {
                case mj::model::ActInfo::STORY:
                {
                    MS_LOGGER.logEvent("STORY_showActivity", {});
                    auto info = StoryDataModel::getInstance()->getStoryById(_info.sub_id);
                    auto storyinfocover = static_cast<StoryInfoCover*>(CSLoader::createNode("csb/StoryInfoCover.csb"));
                    StoryInfoObj obj;
                    obj.name = info.name;
                    obj.mostpopular = info.total_words;
                    if (!info.levels.empty())
                    {
                        obj.readinglevel = info.levels[0];
                    }
                    obj.languageid = info.lang_id;
                    obj.storyid = info.story_id;
                    obj.newest = info.unique_id;
                    obj.total_page = info.total_page;
                    obj.word_count = info.total_words;
                    obj.lexile = info.lexile;
                    obj._id = info.unique_id;
                    obj._storyid = info.story_id;
                    obj._version = info.version;
                    obj.downloaded = false;
                    obj.thumbnail = info.thumbnail_url;
                    obj.written = info.author;
                    obj.design = info.designer;
                    obj.image = info.thumbnail_url;
                    obj.typeStory = mj::model::ActInfo::Type::GAME;
                    storyinfocover->is_activity_play = true;

                    auto lesson_ids = ms::LessonManager::getInstance().getListLessonFree();

                    if (lesson_ids.empty())
                    {
                        int currentCategory = ms::LessonManager::getInstance().getCurrentLessonForCategory();
                        lesson_ids = ms::LessonManager::getInstance().getFreeLessonId(currentCategory);
                    }

                    auto it = std::find_if(lesson_ids.begin(), lesson_ids.end(), [=](int a) {
                        return _info.lesson_id == a;
                        });
                    storyinfocover->_isReadStories = (it == lesson_ids.end()) ? false : true;
                    storyinfocover->initWithData(obj, false);
                    storyinfocover->setDelegate(this);

                    MJDEFAULT->setIntegerForKey("clevertap_lesson_id", _info.lesson_id); //for tracking only.
                    MJDEFAULT->setStringForKey("clevertap_lesson_name", _info.name);//for tracking only.
                    MJDEFAULT->setStringForKey("STORY_START_FROM", "lesson");

                    this->addChild(storyinfocover, 1000);
                    MS_LOGGER.logEvent("STORY_showActivity_success", {});
                    break;
                }
                case mj::model::ActInfo::GAME:
                {
                    MS_LOGGER.logEvent("GAME_showActivity", {});

                    preSetupBeforeStartGame();
                    
                    std::vector<math::resource::Image*> images = math::resource::ResHelper::getImages("icons.json");

                    FileUtils::getInstance()->purgeCachedEntries();

                    ms::EventLogger::getInstance().logEvent("MSActivityWrapper_showActivity", {
                        {"country", cocos2d::Value(MJDEFAULT->getStringForKey(key_country_code,"none"))},
                        {"game_subid", cocos2d::Value(_info.sub_id)},
                        {"ac_id", cocos2d::Value(_info.activity_id)},
                        {"type", cocos2d::Value("game")}
                        });

                    ms::BaseActivity* game = ms::BaseActivity::createActivity(_info, images);
                    if (game)
                    {
                        game->setActivityDelegate(this);
                        game->setTag(m_startFrom == START_FROM::LESSON ? STARTING_FROM_LESSON_TAG : STARTING_FROM_STORY_TAG);
                        game->setName("game_activity");
                        this->addChild(game, 1000);
                        MS_LOGGER.logEvent("GAME_showActivity_success", {});
                        break;
                    }
                }
                case mj::model::ActInfo::QUIZZES:
                {
                    break;
                }
                }

                this->setVisible(true);
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_activity_wrapper_ready);
            });
    };
}

void MSActivityWrapper::setCallbackOnActivityEndWithReason()
{
    _callbacknOnActivityEndWithReason = [=](ms::delegate::Activity::EndReason reason) {
        _callbacknResourceDownloaded = nullptr;
        _callbacknshowActivity = nullptr;

        if (_listActivityOfLesson.first =!_info.lesson_id)
        {
            _listActivityOfLesson.first = _info.lesson_id;
            _listActivityOfLesson.second = StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id);
        }

        auto activities = _listActivityOfLesson.second;

    auto acti_id = _info.activity_id;
    for (int i = 0; i < (int)activities.size(); i++) {
        if (activities[i].quizz_id[(int)activities[i].quizz_id.size() - 1] == _info.activity_id) {
            acti_id = activities[i].activity_id;
            _info = activities[i];
            break;
        }
    }

    auto next_activity = ms::LessonManager::getInstance().getNextActivity(StoryDataModel::getInstance()->getActivityById(acti_id));

    if (reason == ms::delegate::Activity::EndReason::FINISHED && next_activity.activity_id == -1) {
        auto waitView = APWaitView::createView(std::string(""), false, true);
        waitView->setName("waitView");
        this->addChild(waitView, 2000);
        auto lesson = ms::LessonManager::getInstance().getLessonById(_info.lesson_id);
        MS_LOGGER.logEventFinishActivity(MJDEFAULT->getBoolForKey(key_is_phonic_lesson) ? "PHONICS" : "STORIES", _info);
        MS_LOGGER.logEventFinishLesson(MJDEFAULT->getBoolForKey(key_is_phonic_lesson) ? "PHONICS" : "STORIES", lesson);
        ms::LessonManager::getInstance().setLessonLearned(_info.lesson_id);

        auto valueLessonLearn = StoryDataModel::getInstance()->getLessonLearnByID(_info.lesson_id);

        if (valueLessonLearn.size() > 0)
        {
            PROFILE_MANAGER.insertValueToTableLessonLearn(valueLessonLearn);
            mm::RealtimeDatabase::getInstance().updateTrackingAndLearnedLessons(lesson, _info, true, _info.lesson_id);
        }

        ms::LessonManager::getInstance().updateLastActivityLearned(_info);
        auto next_lesson = ms::LessonManager::getInstance().getNextLesson(lesson);
        if (next_lesson.lesson_id != _info.lesson_id) {
            ms::LessonManager::getInstance().prepareLesson(next_lesson);
        }
        else {// trường hợp xảy ra khi là lesson cuối cùng của grade 
            if (CONFIG_MANAGER->isAutoPlay()) {
                // các cấp độ của grade 1,2,3 tương ứng với gradeId 84,216,217

                auto gradeID = ms::GradeManager::getInstance().getCurrentGradeId();
                //trường hợp đang là level 2


                //trường hợp đang là level 1
                if (gradeID == 84) {
                    gradeID = 216;
                    ms::GradeManager::getInstance().setCurrentGradeId(gradeID);
                }
                else {
                    if (gradeID == 217) return;
                    gradeID = 217;
                    ms::GradeManager::getInstance().setCurrentGradeId(gradeID);
                }

                auto idLessonShouldLearn = ms::LessonManager::getInstance().getLessonShouldLearn(gradeID);
                if (idLessonShouldLearn != 0 && idLessonShouldLearn != -1) {
                    auto lessonShouldLearn = ms::LessonManager::getInstance().getLessonById(idLessonShouldLearn);
                    if (lessonShouldLearn.lesson_id != 0) {
                        ms::LessonManager::getInstance().setCurrentLesson(lessonShouldLearn);
                        ms::LessonManager::getInstance().prepareLesson(lessonShouldLearn);
                    }
                }
            }
        }

        if (!CONFIG_MANAGER->isAppActive()) {
            auto max_acti_can_play = FREE_STORY_MANAGER->getMaxActiFreeCanPlay();
            FREE_STORY_MANAGER->setPercentActionFreePlayed(100.0f / (max_acti_can_play * 1.0f));
        }

        auto the_end = MSLessonEndFX::createFX(_info.lesson_id);
        this->getParent()->addChild(the_end, 1000);
        this->scheduleOnce([=](float) {
            this->removeFromParent();

            mjrate::StoryRateAction::updateActionCompleted();
            if (mjrate::StoryRateAction::shouldShowRate()) {
                auto rate_action = mjrate::StoryRateAction::createPage(mjrate::StoryRateAction::ALL_ACTION);
                Director::getInstance()->getRunningScene()->addChild(rate_action, INT_MAX - 1);
            }
            else {
                GroupPromotion::attempToShow();
            }

            RatingManager::GetInstance()->showRatingPopup();

            }, 3, "delay_exit");

        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("reload_all_lesson_items");

        // TODO: pushCleverTapTrackingData for Vmonkey
        if (CONFIG_MANAGER->isVietnameseStories())
        {
            auto lesson = ms::LessonManager::getInstance().getLessonById(_info.lesson_id);
            int activities_size = (int)StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id).size();
            int currentActivityID = _info.activity_id;

            MSActivityWrapper::pushCleverTapTrackingData_Vmonkey(lesson, activities_size, currentActivityID);
        }
    }
    else {
        if (reason == ms::delegate::Activity::EndReason::FINISHED) {
            auto waitView = APWaitView::createView(std::string(""), false, true);
            waitView->setName("waitView");
            this->addChild(waitView, 2000);
            MS_LOGGER.logEventFinishActivity(MJDEFAULT->getBoolForKey(key_is_phonic_lesson) ? "PHONICS" : "STORIES", _info);
            // Xóa màn hình endACtivity cũ đi  (nếu có )
            if (auto oldPopUpStart = utils::findChild(Director::getInstance()->getRunningScene(), "popup_start")) {
                oldPopUpStart->removeFromParent();
            }
            auto popup = MSActivityEnd::createPopup(_info); //MSPreActivity::createView(_info);
            popup->setName("popup_start");
            popup->setDelegate(this);
            this->addChild(popup, 10000);
            ms::LessonManager::getInstance().prepareActivity(next_activity);
            ms::LessonManager::getInstance().updateLastActivityLearned(_info);
            if (!CONFIG_MANAGER->isAppActive()) {
                auto max_acti_can_play = FREE_STORY_MANAGER->getMaxActiFreeCanPlay();
                FREE_STORY_MANAGER->setPercentActionFreePlayed(100.0f / (max_acti_can_play * 1.0f));
            }
            
            if(auto gameActivityLayout = dynamic_cast<GameQuizLayout*>(this->getChildByName("game_activity"))) {
                
                gameActivityLayout->clearData();
            }

            this->removeChildByName("game_activity");
            
            mpool->resetAll();

        }
        else if (reason == ms::delegate::Activity::EndReason::INTERRUPTED) {
            // TODO: pushCleverTapTrackingData for Vmonkey
            if (CONFIG_MANAGER->isVietnameseStories())
            {
                auto lesson = ms::LessonManager::getInstance().getLessonById(_info.lesson_id);
                int activities_size = (int)StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id).size();
                int currentActivityID = _info.activity_id;

                    MSActivityWrapper::pushCleverTapTrackingData_Vmonkey(lesson, activities_size, currentActivityID);
                }

                this->removeAllChildren();
                this->removeFromParent();
            }
        }
    };
}

void MSActivityWrapper::setCallbackOnUnzip()
{
    _callbacknOnUnzip = [=](bool success) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
            if (!_callbacknshowActivity)
            {
                return;
            }
            if (success)
            {
                _info.isUnzipError = false;
                if (_callbacknshowActivity)
                {
                    _callbacknshowActivity();
                }
            }
            else
            {
                _info.isUnzipError = true;
                if (auto popupStart = utils::findChild(Director::getInstance()->getRunningScene(), "popup_start")) {
                    popupStart->removeFromParent();
                }
                auto errorPopup = UnzipErrorPopup::createPopup(UnzipErrorPopup::PopupType::ACTIVITY, _info, this);
                auto runningSceneSize = Director::getInstance()->getRunningScene()->getContentSize();
                errorPopup->setContentSize(runningSceneSize);
                Director::getInstance()->getRunningScene()->addChild(errorPopup, INT_MAX);
                MS_LOGGER.logEvent("unzip_failed", { {"from",cocos2d::Value("MSActivityWrapper::onResourceDownloaded")} });
            }

            });
    };

}
