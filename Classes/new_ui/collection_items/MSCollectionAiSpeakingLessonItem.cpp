////
////  MSCollectionAiSpeakingLessonItem.cpp
////  MJStory
////
////  Created by To Ngoc Linh on 11/13/17.
////
//
//#include "MSCollectionAiSpeakingLessonItem.h"
//#include "RemoteImageView.h"
//#include "MJAPILessonDetails.h"
//#include "MSLessonPreview.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
//#include "StoryLanguageManager.h"
//#include "MJDefinitions.h"
//#include "StoryConfigManager.h"
//#include "StoryAlert.h"
//#include "AudioEngine.h"
//#include "GAFWrapper.h"
//#include "MSGradeManager.h"
//#include "MMFirebaseDatabase.h"
//#include "APProfileManager.h"
//USING_NS_CC;
//INITIALIZE_READER(MSCollectionAiSpeakingLessonItem);
//
//#define tag_popup_question_download 1112
//#define tag_popup_download_failed   1113
//
//int MSCollectionAiSpeakingLessonItem::_lesson_locked_audio_id = -1;
//bool MSCollectionAiSpeakingLessonItem::_isDownloadingCommonVideo = false;
//std::once_flag ms_collection_ai_speaking_lesson_item;
//cocos2d::Data ms_collection_ai_speaking_lesson_node_data;
//MSCollectionAiSpeakingLessonItem* MSCollectionAiSpeakingLessonItem::createItem(mj::model::AISpeakingLessonInfo item, bool isLessonLearn) {
//    std::call_once(ms_collection_ai_speaking_lesson_item, [] {
//        REGISTER_CSB_READER(MSCollectionAiSpeakingLessonItem);
//     //   ms_collection_lesson_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/LessonItem.csb");
//        ms_collection_ai_speaking_lesson_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/AISpeaking/AISpeakingItem.csb");
//    });
//
//    auto node = reinterpret_cast<MSCollectionAiSpeakingLessonItem *>(CSLoader::createNode(ms_collection_ai_speaking_lesson_node_data));
//    if (node) {
//        node->didLoadFromCSB(item, isLessonLearn);
//        return node;
//    }
//
//    CC_SAFE_DELETE(node);
//    return nullptr;
//}
//
//void MSCollectionAiSpeakingLessonItem::setStateInfo(mj::model::AISpeakingLessonInfo::State state)
//{
//    _aISpeakingLessonInfo.state = state;
//}
//void MSCollectionAiSpeakingLessonItem::onSaveLessonState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state == mj::model::AISpeakingLessonInfo::State::LEARNED)
//    {
//        setLessonStateToView(_aISpeakingLessonInfo.state);
//        return;
//    }
//    _aISpeakingLessonInfo.state = state;
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//        mj::model::AISpeakingLessonInfo aISpeakingLessonInfoNeedsave = getLessonAISpeakingInfo();
//        if (aISpeakingLessonInfoNeedsave.state == mj::model::AISpeakingLessonInfo::State::LEARNED)
//            // not save state learned to main DB, because using for multi profile
//            return;
//        mj::model::AISpeakingLessonInfo aISpeakingLessonInfo = ms::LessonAISpeakingManager::getInstance().onSaveLessonAISpeakingState(getLessonAISpeakingInfo());
//        setLessonAISpeakingInfo(aISpeakingLessonInfo);
//        setLessonStateToView(aISpeakingLessonInfo.state);
//        });
//}
//
//void MSCollectionAiSpeakingLessonItem::onUpdateTopicProccess()
//{
//    _delegate ? _delegate->onUpdateTopicProccess() : (void) nullptr;
//}
//
//void MSCollectionAiSpeakingLessonItem::didLoadFromCSB(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo, bool isLessonLearn)
//{
//    _aISpeakingLessonInfo = aISpeakingLessonInfo;
//    _isLessonLearned = isLessonLearn;
//    _activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_aISpeakingLessonInfo.lesson_id);
//    initialViewInfo(aISpeakingLessonInfo);
//    changeTextDisplayLang();
//
//    setLessonStateToView(loadLessonstate());
//
//}
//
//void MSCollectionAiSpeakingLessonItem::onEnter()
//{
//
//    cocos2d::ui::Layout::onEnter();
//
//    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
//    {
//        this->changeTextDisplayLang();
//    });
//
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
//
//    auto reload_state = EventListenerCustom::create(key_reload_lesson_state, [=](EventCustom * e) {
//
//    });
//
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(reload_state, this);
//
//    _isExitLesson = false;
//    if(MSCollectionAiSpeakingLessonItem::_isDownloadingCommonVideo)
//    {
//        addDownloadVideoCommonProgressListener();
//        addFirstActivityProgressListener();
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::addFirstActivityProgressListener() {
//    if (_activities.size() <= 0)
//        return;
//    auto first_activity = _activities[0];
//    auto listener_first_activity_download = EventListenerCustom::create(ms::LessonAISpeakingManager::getInstance().getKeyEventActivityProgress(first_activity), [=](EventCustom * e) {
//        auto progress = reinterpret_cast<double *>(e->getUserData());
//        _percentDownloading = (int)*progress;
//        if (_percentDownloading >= 50)
//            _percentDownloading = 50;
//        updateDownloadLessonProcess("", _percentDownloading);
//    });
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_first_activity_download, this);
//
//}
//
//void MSCollectionAiSpeakingLessonItem::addFirstActivityReadyListener() {
//    if (_activities.size() <= 0)
//        return;
//    auto first_activity = _activities[0];
//    auto listener_lesson_download_ready = EventListenerCustom::create(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonReady(_aISpeakingLessonInfo), [this](EventCustom *) {
//        _isDownloadingCommonVideo = false;
//        onUnzipToGetConfig([=](bool isSuccess, std::string faile_reason) {
//            std::string name1 = "";
//            if (isSuccess)
//            {
//                this->onLessonReady();
//            }
//            else
//            {
//                this->onLessonError(faile_reason);
//            }
//
//            }, ms::CommonDownloadType::ON_DOWNLOAD);
//    });
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_ready, this);
//}
//void MSCollectionAiSpeakingLessonItem::addDownloadVideoCommonProgressListener() {
//    removeDownloadVideoCommonProgressListener();
//    if (_activities.size() <= 0)
//        return;
//    auto first_activity = _activities[0];
//    auto listener_first_activity_download = EventListenerCustom::create(ms::LessonAISpeakingManager::getInstance().getKeyEventCommonProgress(first_activity), [=](EventCustom * e) {
//        auto progress = reinterpret_cast<float *>(e->getUserData());
//        _percentDownloading = (int)*progress;
//        updateDownloadLessonProcess("", _percentDownloading);
//
//    });
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_first_activity_download, this);
//}
//
//void MSCollectionAiSpeakingLessonItem::removeFirstActivityProgressListener()
//{
//    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonError(_aISpeakingLessonInfo));
//    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonReady(_aISpeakingLessonInfo));
//
//    if (_activities.empty()) {
//    }
//    else {
//        this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventActivityProgress(_activities[0]));
//    }
//}
//void MSCollectionAiSpeakingLessonItem::removeFirstActivityReadyListener()
//{
//    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonReady(_aISpeakingLessonInfo));
//
//}
//void MSCollectionAiSpeakingLessonItem::removeDownloadVideoCommonProgressListener()
//{
//    if (_activities.empty()) {
//    }
//    else {
//        this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventCommonProgress(_activities[0]));
//    }
//}
//void MSCollectionAiSpeakingLessonItem::onUnzipToGetConfig(std::function<void(bool, std::string)> callback, ms::CommonDownloadType type)
//{
//    int nextPercent = _percentDownloading;
//    if (!_activities.empty()) {
//
//        auto activityInfo = _activities.front();
//
//        auto game_info = StoryDataModel::getInstance()->getGameById(activityInfo.sub_id);
//
//        std::vector<std::pair<std::string, std::string>> unzip_data;
//
//        activityInfo.zip_urls.erase(unique(activityInfo.zip_urls.begin(), activityInfo.zip_urls.end()), activityInfo.zip_urls.end());
//
//        for (auto i = 0; i < activityInfo.zip_urls.size(); ++i)
//        {
//            auto path = FileUtils::getInstance()->getWritablePath() + "cached/zip/" + mj::helper::getFilenameFromURL(activityInfo.zip_urls[i]);
//            auto extentionFile = FileUtils::getInstance()->getFileExtension(path);
//
//            if(FileUtils::getInstance()->isDirectoryExist(path)|| !FileUtils::getInstance()->isFileExist(path))
//                continue;
//            auto directory = FileUtils::getInstance()->getWritablePath() + "extracted/" + StringUtils::toString(i / 2) + "/";
//            unzip_data.push_back(std::make_pair(path, directory));
//        }
//
//        mj::helper::unzipAsync(unzip_data, [=](bool success) {
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
//
//                if (success) {
//                    if (unzip_data.size() > 0)
//
//                    {
//                        std::string name = ms::LessonAISpeakingManager::getInstance().getActorNameFromConfig(unzip_data.at(0).second + "config.json");
//                        if (name != "")
//                        {
//                            MJDEFAULT->setStringForKey(CURRENT_ACTOR_AISPEAKING, name);
//                            std::vector<std::string> actorNames;
//                            actorNames.push_back(name);
//                            std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos =
//                            StoryDataModel::getInstance()->getAiSpeakingCommonInfosByActor(actorNames);
//                            auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_aISpeakingLessonInfo.lesson_id);
//                            bool isNeedDownload = ms::LessonAISpeakingManager::getInstance().onDownloadVideoCommons(talkingCommonInfos, [=](bool isSuccess) {
//                                _isDownloadingCommonVideo = false;
//
//                                std::string name1 = "";
//                                removeDownloadVideoCommonProgressListener();
//                                if (isSuccess)
//                                {
//                                    callback(true, "Download video common true");
//                                }
//                                else
//                                {
//                                    callback(false, "Download video common false");
//                                }
//
//                                }, [=](float percent) {
//                                    _isDownloadingCommonVideo = true;
//                                    float percentForView = (float)nextPercent + percent / 2;
//                                    if (percentForView >= 100)
//                                        percentForView = 95;
//
//                                    EventCustom e(ms::LessonAISpeakingManager::getInstance(). getKeyEventCommonProgress(activities[0]));
//                                    e.setUserData(&percentForView);
//                                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
//
//                                }, type);
//                            if(isNeedDownload)
//                                addDownloadVideoCommonProgressListener();
//
//                        }
//                        else
//                        {
//                            callback(false, "activities config not contain actor name");
//                        }
//                    }
//                    else {
//                        callback(false, "data unzip size is empty");
//                    }
//
//
//                }
//                else
//                {
//                    callback(false, "unzip the activity false");
//                }
//                });
//            });
//
//    }
//    else
//    {
//        callback(false, "lesson not haven't activities");
//    }
//}
//
//
//void MSCollectionAiSpeakingLessonItem::setLessonStateToView(mj::model::AISpeakingLessonInfo::State state) {
//
//    if(_isExitLesson)
//        return;
//    bool isButtonRelearnVisible = false;
//    CCLOG("%d___%s", _aISpeakingLessonInfo.lesson_id, std::to_string(state).c_str());
//
//    if(auto button = utils::findChild(this, "button_relearn"))
//    {
//        isButtonRelearnVisible = button->isVisible();
//    }
//    if(_aISpeakingLessonInfo.state!= mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//    _outLineLayout->setOpacity(0);
//
//    switch (state)
//    {
//    case mj::model::AISpeakingLessonInfo::LOCKED:
//    {
//        setLessonLockedState(state);
//        break;
//
//    }
//    case mj::model::LessonInfo::LEARNED:
//    {
//
//        setLessonLearnedState(state);
//        break;
//
//    }
//    case mj::model::LessonInfo::UNLOCKED:
//    {
//
//        setLessonUnLockedState(state);
//        break;
//
//    }
//    case mj::model::LessonInfo::PREPARING:
//    {
//        setLessonPreparingState(state);
//
//        break;
//
//    }
//    case mj::model::LessonInfo::READY:
//    {
//        setLessonReadyState(state);
//        break;
//
//    }
//    default:
//        break;
//    }
//    onDeleteDataExpected(_aISpeakingLessonInfo);
//
//    auto is_last = ms::LessonAISpeakingManager::getInstance().getLessonShouldLearn(_aISpeakingLessonInfo.category_id) == _aISpeakingLessonInfo.lesson_id;
//    std::string key = StringUtils::format(key_last_lesson_x, _aISpeakingLessonInfo.category_id);
//    int profileID = ap::ProfileManager::getInstance().getCurrentProfileId();
//    mm::RealtimeDatabase::getInstance().updateProfileKeysByKeyAndValue(profileID, key, _aISpeakingLessonInfo.lesson_id);
//
//}
//
//mj::model::AISpeakingLessonInfo::State MSCollectionAiSpeakingLessonItem::loadLessonstate()
//{
//    mj::model::AISpeakingLessonInfo::State state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
//    if (ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo))
//        state = mj::model::AISpeakingLessonInfo::READY;
//
//    if (_isLessonLearned)
//    {
//        state = mj::model::AISpeakingLessonInfo::LEARNED;
//    }
//
//    switch (_aISpeakingLessonInfo._stateOfPreLesson)
//    {
//    case mj::model::AISpeakingLessonInfo::LOCKED:
//        state = mj::model::AISpeakingLessonInfo::State::LOCKED;
//        break;
//    case mj::model::AISpeakingLessonInfo::LEARNED:
//        //state = mj::model::AISpeakingLessonInfo::State::UNLOCKED;
//        break;
//    case mj::model::AISpeakingLessonInfo::UNLOCKED:
//        state = mj::model::AISpeakingLessonInfo::State::LOCKED;
//        break;
//    case mj::model::AISpeakingLessonInfo::PREPARING:
//        state = mj::model::AISpeakingLessonInfo::State::LOCKED;
//        break;
//    case mj::model::AISpeakingLessonInfo::READY:
//        state = mj::model::AISpeakingLessonInfo::State::LOCKED;
//        break;
//    default:
//        break;
//    }
//
//    return state;
//}
//
//void MSCollectionAiSpeakingLessonItem::onResize(Size size)
//{
//    this->setContentSize(size);
//    utils::findChild<ui::Layout*>(this, "Panel_1_0")->setContentSize(size);
//    utils::findChild<ui::Layout*>(this, "panel_backGround")->setContentSize(size);
//    utils::findChild<ui::ImageView*>(this, "image_background")->setContentSize(size);
//    utils::findChild<ui::Layout*>(this, "control")->setContentSize(size);
//    utils::findChild<ui::Button*>(this, "button_locked")->setContentSize(size);
//    utils::findChild<ui::Button*>(this, "button_relearn")->setContentSize(size);
//    utils::findChild<ui::Button*>(this, "button_learn")->setContentSize(size);
//    utils::findChild<ui::Button*>(this, "download_progress")->setContentSize(size);
//
//    //Layout* _panel_backGround = nullptr, * _panel_Download = nullptr, * _maskLayout = nullptr, * _control = nullptr, * _Panel_2_0 = nullptr, * _greenLayout = nullptr, * _outLineLayout = nullptr;
//
//}
//
//void MSCollectionAiSpeakingLessonItem::onExit()
//{
//    _isExitLesson = true;
//    if(MSCollectionAiSpeakingLessonItem::_isDownloadingCommonVideo)
//    {
//        removeDownloadVideoCommonProgressListener();
//        removeFirstActivityProgressListener();
//    }
//    cocos2d::ui::Layout::onExit();
//
//    if (_storyAlert) {
//
//        _storyAlert->setDelegate(nullptr);
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::onLearn(Ref * sender) {
//
//    if (!checkPermissionCameraAndAudio())
//    {
//        showPermissionPopup();
//        return;
//    }
//	mj::helper::playButtonFX();
//    _button_learn->setTouchEnabled(false);
//
//
//	if (!ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//		onDownload(nullptr);
//		return;
//	}
//    checkCommonVideoReady(_aISpeakingLessonInfo, [=](bool isSuccess) {
//        std::string name1 = "";
//        _button_learn->setTouchEnabled(true);
//        if (isSuccess)
//        {
//            //_commonVideoExits = true;
//            this->onLessonReady();
//            onSaveLessonState(mj::model::AISpeakingLessonInfo::State::READY);
//            _delegate ? _delegate->onLearnLesson(_aISpeakingLessonInfo) : (void) nullptr;
//
//        }
//        else
//        {
//            //_commonVideoExits = false;
//            onDownload(nullptr);
//        }
//    });
//
//}
//
//void MSCollectionAiSpeakingLessonItem::onRelearn(Ref *sender) {
//    if (!checkPermissionCameraAndAudio())
//    {
//        showPermissionPopup();
//        return;
//    }
//    mj::helper::playButtonFX();
//    //if (CONFIG_MANAGER->isAppActive()) {
//        //preview
//    if (ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//        checkCommonVideoReady(_aISpeakingLessonInfo, [=](bool isSuccess) {
//            std::string name1 = "";
//            if (isSuccess)
//            {
//                this->onLessonReady();
//                this->getEventDispatcher()->removeCustomEventListeners("event_lesson_auto_open_from_qr");
//                onLearn(nullptr);
//            }
//            else
//            {
//                onDownload(nullptr);
//            }
//
//        });
//    }
//    else{
//        onDownload(nullptr);
//    }
////    } else {
////        StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "sub.popup.feature.free"), "OK", "");
////    }
//}
//
//void MSCollectionAiSpeakingLessonItem::onLocked(cocos2d::Ref *sender) {
//    mj::helper::playButtonFX();
//
//    if(getStateInfo() == mj::model::AISpeakingLessonInfo::State::LOCKED)
//    {
//        if(!CONFIG_MANAGER->isAppActive())
//        {
//            showSubscribePopup();
//        }
//        else
//        {
//            if (MJDEFAULT->getBoolForKey(key_is_submit, false))
//            {
//                StoryAlert::showAlert(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.popup.prior"), "OK", "");
//            }
//            auto path = CONFIG_MANAGER->isVietnameseStories()?"sounds/ui/lesson_guide_vi.mp3":"sounds/ui/lesson_lock.mp3";
//            cocos2d::experimental::AudioEngine::stopAll();
//            _lesson_locked_audio_id = cocos2d::experimental::AudioEngine::play2d(path);
//            cocos2d::experimental::AudioEngine::setFinishCallback(_lesson_locked_audio_id, [](int, std::string) {
//                MSCollectionAiSpeakingLessonItem::_lesson_locked_audio_id = -1;
//            });
//        }
//
//    }
//    _delegate ? _delegate->onTryToLearnLockedLessons(_aISpeakingLessonInfo) : (void) nullptr;
//}
//
//void MSCollectionAiSpeakingLessonItem::onDownload(Ref * sender) {
//    if (!checkPermissionCameraAndAudio())
//    {
//        showPermissionPopup();
//        return;
//    }
//    //return;
//    onSaveLessonState(mj::model::AISpeakingLessonInfo::State::UNLOCKED);
//    MJDEFAULT->setStringForKey(key_live_in_screen, "lesson");
//    _is_downloadingAiSpeaking = false;
//    if (_is_downloadingAiSpeaking) {
//        CCLOG("blocked download");
//        return;
//    }
//    cocos2d::experimental::AudioEngine::stopAll();
//    mj::helper::playButtonFX();
//
//
//    utils::findChild(this, "download_progress")->setVisible(true);
//    utils::findChild(this, "download_progress")->setLocalZOrder(1);
//
//    /*
//    * Add listener for progressbar here
//    */
//    if (_activities.empty()) {
//        auto listener_details = EventListenerCustom::create(StringUtils::format("%s_%d", key_event_got_lesson_details, _aISpeakingLessonInfo.lesson_id), [=](EventCustom * e) {
//            this->addFirstActivityProgressListener();
//            this->addFirstActivityReadyListener();
//        });
//
//        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_details, this);
//    } else {
//        this->addFirstActivityProgressListener();
//        this->addFirstActivityReadyListener();
//    }
//
//    /*
//    * Listener for error while downloading
//    */
//    auto listener_lesson_download_error = EventListenerCustom::create(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonError(_aISpeakingLessonInfo), [this](EventCustom *) {
//        this->onLessonError("download lesson error");
//    });
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_error, this);
//
//
//    if (ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//        if(_commonVideoExits)
//            onLessonReady();
//        else
//            showAlertDownloadLesson();
//    } else {
//        showAlertDownloadLesson();
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::onAlertOK(cocos2d::Ref *sender) {
//    //_is_downloadingAiSpeaking = true;
//    if (auto alert = dynamic_cast<StoryAlert *>(sender))
//    {
//        switch (alert->getTag())
//        {
//            case tag_popup_question_download:
//            {
//                //download và chuẩn bị bài học
//                _isNeedDownLoad = true;
//                onSaveLessonState(mj::model::AISpeakingLessonInfo::State::PREPARING);
//                setLessonStateToView(mj::model::AISpeakingLessonInfo::State::PREPARING);
//                                /*
//                * Listener for error while downloading
//                */
//                auto listener_lesson_download_error = EventListenerCustom::create(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonError(_aISpeakingLessonInfo), [this](EventCustom*) {
//                    this->onLessonError("download lesson error");
//                    });
//                this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_download_error, this);
//                ms::LessonAISpeakingManager::getInstance().resetIndex(0);
//                ms::LessonAISpeakingManager::getInstance().prepareLesson(_aISpeakingLessonInfo);
//                _isDownloadingCommonVideo = true;
//
//            }
//                break;
//
//            default:
//                break;
//        }
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::onAlertCancel(cocos2d::Ref *sender) {
//    if (auto alert = dynamic_cast<StoryAlert *>(sender))
//    {
//        switch (alert->getTag())
//        {
//            case tag_popup_question_download:
//            {
//                //đổi trạng thái thanh download
//               // utils::findChild<ui::Text *>(this, "text_download")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.item.download"));
//                _is_downloadingAiSpeaking = false;
//            }
//                break;
//
//            case tag_popup_download_failed:
//            {
//                // re-download
//                this->onDownload(nullptr);
//            }
//                break;
//
//            default:
//                break;
//        }
//    }
//}
//
//
//bool MSCollectionAiSpeakingLessonItem::onDeleteDataExpected(mj::model::AISpeakingLessonInfo aISpeakingLessonInfo)
//{
//    bool result = true;
//    if ((aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::State::LOCKED &&
//        aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::State::UNLOCKED &&
//        aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::State::PREPARING) ||
//        aISpeakingLessonInfo._timeLearnedExpected <= 0)
//        return result;
//    int curreentTimeInDays = mj::helper::getTimeInDays();
//    if(curreentTimeInDays - aISpeakingLessonInfo._timeLearnedExpected >=30)
//        result = ms::LessonAISpeakingManager::getInstance().onDeleteDataExperted(aISpeakingLessonInfo);
//    return result;
//}
//
//float MSCollectionAiSpeakingLessonItem::getLessonSize(std::vector<mj::model::ActAISpeakingInfo> acts){
//    float size = 0;
//    for(auto act: acts){
//        size += StoryDataModel::getInstance()->getFileSize(act.data_zip);
//    }
//
//    return size;
//}
//
//ui::Widget::ccWidgetClickCallback MSCollectionAiSpeakingLessonItem::onLocateClickCallback(const std::string & callback_name)
//{
//    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
//        CLICK_MAP(MSCollectionAiSpeakingLessonItem, onLearn),
//        CLICK_MAP(MSCollectionAiSpeakingLessonItem, onDownload),
//        CLICK_MAP(MSCollectionAiSpeakingLessonItem, onRelearn),
//        CLICK_MAP(MSCollectionAiSpeakingLessonItem, onLocked)
//    };
//
//    if (function_map.count(callback_name) > 0) {
//        return function_map.at(callback_name);
//    }
//
//    return nullptr;
//}
//
//void MSCollectionAiSpeakingLessonItem::updateDownloadLessonProcess(std::string cirlceProccess, float percent, bool downloadError)
//{
//    if(_isExitLesson)
//        return;
//    std::string imageName = mj::helper::getFilenameFromURL(cirlceProccess);
//
//    std::string	_cached_image = "mjstory/AISpeaking/proccessCircle/Green Circle.png";
//
//    if (!FileUtils::getInstance()->isFileExist(_cached_image))
//        return;
//    if (!_greenLayout || !_outLineLayout)
//    {
//        return;
//    }
//    _outLineLayout->setOpacity(0);
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/DownLoading.png");
//
//    if (_progcess == nullptr)
//    {
//        auto bar = Sprite::create(_cached_image);
//        bar->setContentSize(Size(_outLineLayout->getContentSize()));
//        _progcess = ProgressTimer::create(bar);
//        _progcess->setType(ProgressTimer::Type::RADIAL);
//        _progcess->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//        _progcess->setPosition(_outLineLayout->getContentSize() / 2);
//        _progcess->setName("ProgressTimer");
//        _outLineLayout->addChild(_progcess);
//    }
//    if (downloadError)
//    {
//        _progcess->setVisible(true);
//            _progcess->setPercentage(0);
//    }
//    if (percent > 0 && percent < 100)
//    {
//        _progcess->setVisible(true);
//        if (_progcess->getPercentage() < percent)
//            _progcess->setPercentage(percent);
//
//    }
//    if(percent==100)
//    {
//        _outLineLayout->runAction(Sequence::create(DelayTime::create(0.5f), CallFunc::create([=]() {
//                _progcess->setPercentage(0);
//                _progcess->setVisible(false);
//            })
//            , nullptr));
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::showPermissionPopup()
//{
//    _delegate ? _delegate->showPermissionPopup() : (void) nullptr;
//}
//
//bool MSCollectionAiSpeakingLessonItem::checkPermissionCameraAndAudio()
//{
//    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)){
//#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
//        return CameraHelper::GetInstance()->getAuthorizationStatus();
//#endif;
//        return true;
//    }
//    return true;
//}
//
//void MSCollectionAiSpeakingLessonItem::requestPermission()
//{
//}
//
//void MSCollectionAiSpeakingLessonItem::onLessonReady()
//{
//    if(_isExitLesson)
//        return;
//    _percentDownloading = 100;
//    updateDownloadLessonProcess("", _percentDownloading);
//    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonError(_aISpeakingLessonInfo));
//    this->getEventDispatcher()->removeCustomEventListeners(ms::LessonAISpeakingManager::getInstance().getKeyEventLessonReady(_aISpeakingLessonInfo));
//    this->runAction(Sequence::create(DelayTime::create(0.8f), CallFunc::create([=] {
//        onSaveLessonState(mj::model::AISpeakingLessonInfo::State::READY);
//        setLessonStateToView(_aISpeakingLessonInfo.state);
//    }), NULL));
//
//    if(_isNeedDownLoad)
//    {
//        pushEventAIspeakingToCleaverTap(true);
//        _isNeedDownLoad = false;
//    }
//}
//
//
//void MSCollectionAiSpeakingLessonItem::onLessonError(std::string reason)
//{
//    onSaveLessonState(mj::model::AISpeakingLessonInfo::State::UNLOCKED);
//    _is_downloadingAiSpeaking = false;
//    removeFirstActivityProgressListener();
//    removeFirstActivityReadyListener();
//    if (ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//        //không hiện thông báo khi lesson ready
//        return;
//    }
//
//    if (_aISpeakingLessonInfo.state == mj::model::AISpeakingLessonInfo::State::LOCKED) {
//        //không hiện thông báo nếu lesson locked
//        return;
//    }
//
//    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
//            auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");
//            _storyAlert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
//            _storyAlert->setNetworkDownContent(false, true);
//            _storyAlert->show();
//            _storyAlert->setTag(tag_popup_download_failed);
//            _storyAlert->setDelegate(this);
//        });
//    updateDownloadLessonProcess("", 0.0f, true);
//    pushEventAIspeakingToCleaverTap(false, reason);
//}
//
//void MSCollectionAiSpeakingLessonItem::changeTextDisplayLang()
//{
//
//    if (!_labName)
//    {
//        _labName = utils::findChild<Label*>(this, "_name");
//
//    }
//    if (_labName)
//    {
//        std::string title = StringUtils::format("%s %d", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "aispeaking.conversation.item.title").c_str(), _aISpeakingLessonInfo.lesson_order);
//        int maxTextLengthInCurrentSize = 11;
//        cocos2d::TTFConfig config = _labName->getTTFConfig();
//
//        int currentFontSize = config.fontSize;
//        float newFontSize = currentFontSize * ((float)maxTextLengthInCurrentSize / (float)title.length());
//        config.fontSize = newFontSize;
//        _labName->setTTFConfig(config);
//        _labName->setString(StringUtils::format("%s %d", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "aispeaking.conversation.item.title").c_str(), _aISpeakingLessonInfo.lesson_order));
//
//    }
//}
//
//
//void MSCollectionAiSpeakingLessonItem::setDisplayMode()
//{
//}
//
//void MSCollectionAiSpeakingLessonItem::enableDeleteMode()
//{
//}
//
//void MSCollectionAiSpeakingLessonItem::disableDeleteMode()
//{
//    for (auto c : this->getChildren()) {
//        c->setOpacity(0xff);
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::openLessonItemForce()
//{
//    auto listener_lesson_auto_open = EventListenerCustom::create("event_lesson_auto_open_from_qr", [this](EventCustom *) {
//        onLearn(nullptr);
//        this->getEventDispatcher()->removeCustomEventListeners("event_lesson_auto_open_from_qr");
//    });
//
//    switch (_aISpeakingLessonInfo.state) {
//        case mj::model::AISpeakingLessonInfo::State::READY: {
//            onLearn(nullptr);
//        } break;
//
//        case mj::model::AISpeakingLessonInfo::State::LEARNED: {
//            onRelearn(nullptr);
//            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
//        } break;
//
//        case mj::model::AISpeakingLessonInfo::State::LOCKED: {
//            onLocked(nullptr);
//        } break;
//
//        case mj::model::AISpeakingLessonInfo::State::UNLOCKED: {
//            onDownload(nullptr);
//            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
//        } break;
//
//        case mj::model::AISpeakingLessonInfo::State::PREPARING: {
//            this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_lesson_auto_open, this);
//        } break;
//
//        default:
//            break;
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::setLastLesson()
//{
//
//}
//
//void MSCollectionAiSpeakingLessonItem::initialViewInfo(mj::model::AISpeakingLessonInfo item)
//{
//    _panel_backGround = utils::findChild<ui::Layout*>(this, "panel_backGround");
//    _panel_backGround->setLocalZOrder(INT_MAX - 2);
//
//    _panel_Download = utils::findChild<ui::Layout*>(this, "panel_Download");
//    _panel_Download->setLocalZOrder(INT_MAX - 1);
//
//    _control = utils::findChild<ui::Layout*>(this, "control");
//    _control->setVisible(true);
//    _control->setLocalZOrder(INT_MAX-1);
//
//    _button_locked = utils::findChild<ui::Button*>(this, "button_locked");
//    _button_locked->setTouchEnabled(true);
//
//    _button_relearn = utils::findChild<ui::Button*>(this, "button_relearn");
//    _button_relearn->setTouchEnabled(true);
//
//    _button_learn = utils::findChild<ui::Button*>(this, "button_learn");
//    _button_learn->setTouchEnabled(true);
//
//    _button_download = utils::findChild<ui::Button*>(this, "download_progress");
//    _button_download->setTouchEnabled(true);
//    Size size = _button_download->getContentSize();
//
//    _Panel_2_0 = utils::findChild<ui::Layout*>(this, "Panel_2_0");
//    _Panel_2_0->setLocalZOrder(INT_MAX - 1);
//
//    _name = utils::findChild<ui::Text*>(this, "name");
//    _name->setLocalZOrder(INT_MAX - 1);
//    _name->setString(item.name);
//    _name->setTextColor(Color4B::WHITE);
//
//    cocos2d::TTFConfig config;
//    config.fontSize = 40;
//    config.fontFilePath = "fonts/Nunito-ExtraBold.ttf";
//    config.bold = true;
//    if (!_labName)
//    {
//        _labName = Label::createWithTTF(config, item.name, cocos2d::TextHAlignment::CENTER);
//        _name->getParent()->addChild(_labName);
//        _name->setVisible(false);
//        _labName->setAnchorPoint(_name->getAnchorPoint());
//        _labName->setPosition(_name->getPosition());
//        _labName->setZOrder(_name->getZOrder());
//        _labName->setName("_name");
//        _labName->setContentSize(_name->getContentSize());
//    }
//
//    _order = utils::findChild<ui::Text*>(this, "order");
//    _order->setLocalZOrder(INT_MAX - 1);
//    _order->setString(std::to_string(item.lesson_order));
//
//    _greenLayout = utils::findChild<ui::Layout*>(this, "greenLayout");
//    _outLineLayout = utils::findChild<ui::Layout*>(this, "outLineLayout");
//    _outLineLayout->setBackGroundImage("mjstory/AISpeaking/proccessCircle/Green Circle.png");
//    _outLineLayout->setOpacity(0);
//
//}
//
//void MSCollectionAiSpeakingLessonItem::setLessonLockedState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//    _panel_backGround->setBackGroundImage("mjstory/AISpeaking/RectanglightBlue.png");
//    _name->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
//    _labName->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/Locked.png");
//
//    _button_locked->setVisible(true);
//    _button_locked->setLocalZOrder(INT_MAX);
//    _button_relearn->setVisible(false);
//    _button_learn->setVisible(false);
//    _button_download->setVisible(false);
//}
//
//void MSCollectionAiSpeakingLessonItem::setLessonUnLockedState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//    _panel_backGround->setBackGroundImage("mjstory/AISpeaking/Rectangle_blue.png");
//    _name->setTextColor(Color4B::WHITE);
//    _labName->setTextColor(Color4B::WHITE);
//
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/Download.png");
//
//    _button_locked->setVisible(false);
//    _button_relearn->setVisible(false);
//    _button_learn->setVisible(false);
//    _button_download->setVisible(true);
//    _button_download->setLocalZOrder(INT_MAX);
//    if (ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//        state = mj::model::AISpeakingLessonInfo::State::READY;
//        if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//            _aISpeakingLessonInfo.state = state;
//        _button_locked->setVisible(false);
//        _button_relearn->setVisible(false);
//        _button_learn->setVisible(true);
//        utils::findChild<ui::Button*>(this, "download_progress")->setVisible(true);
//        //_button_learn->setLocalZOrder(INT_MAX);
//        _button_download->setVisible(false);
//    }
//
//}
//
//void MSCollectionAiSpeakingLessonItem::setLessonLearnedState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//
//    _panel_backGround->setBackGroundImage("mjstory/AISpeaking/RectanglightBlue.png");
//    _name->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
//    _labName->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
//
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/Completed.png");
//
//    _button_locked->setVisible(false);
//    _button_relearn->setVisible(true);
//    _button_relearn->setLocalZOrder(INT_MAX);
//    _button_learn->setVisible(false);
//    _button_download->setVisible(false);
//}
//
//void MSCollectionAiSpeakingLessonItem::setLessonPreparingState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//    _name->setTextColor(Color4B::WHITE);
//    _labName->setTextColor(Color4B::WHITE);
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/Download.png");
//    _panel_backGround->setBackGroundImage("mjstory/AISpeaking/Rectangle_blue.png");
//
//    _button_locked->setVisible(false);
//    _button_relearn->setVisible(false);
//    _button_learn->setVisible(false);
//    _button_download->setVisible(false);
//}
//
//void MSCollectionAiSpeakingLessonItem::setLessonReadyState(mj::model::AISpeakingLessonInfo::State state)
//{
//    if (_aISpeakingLessonInfo.state != mj::model::AISpeakingLessonInfo::LEARNED)
//        _aISpeakingLessonInfo.state = state;
//    _name->setTextColor(Color4B::WHITE);
//    _labName->setTextColor(Color4B::WHITE);
//    _greenLayout->setBackGroundImage("mjstory/AISpeaking/Ready_for_learn.png");
//    _panel_backGround->setBackGroundImage("mjstory/AISpeaking/Rectangle_blue.png");
//
//    _button_locked->setVisible(false);
//    _button_relearn->setVisible(false);
//    _button_learn->setVisible(true);
//    _button_learn->setLocalZOrder(INT_MAX);
//    _button_download->setVisible(false);
//    if (!ms::LessonAISpeakingManager::getInstance().isLessonReady(_aISpeakingLessonInfo)) {
//        setLessonStateToView(mj::model::AISpeakingLessonInfo::State::UNLOCKED);
//    }
//}
//
//void MSCollectionAiSpeakingLessonItem::showSubscribePopup() {
//    _delegate ? _delegate->showSubscribePopup() : (void) nullptr;
//
//}
//void MSCollectionAiSpeakingLessonItem::pushEventAIspeakingToCleaverTap(bool successfulStatus, std::string failed_reason){
//    CleverTapManager::GetInstance()->pushEvent("talk_download_lesson", {
//                        {"lesson_topic",cocos2d::Value(getLessonAISpeakingInfo().category_id)},
//                        {"lesson_id",cocos2d::Value(getLessonAISpeakingInfo().lesson_id)},
//                        {"lesson_number",cocos2d::Value(getLessonAISpeakingInfo().lesson_order)},
//                        {"successful",cocos2d::Value(successfulStatus)},
//                        {"failed_reason",cocos2d::Value(failed_reason)},
//                });
//}
//
//void MSCollectionAiSpeakingLessonItem::checkCommonVideoReady(mj::model::AISpeakingLessonInfo info, std::function<void(bool)> callback){
//    if(_commonVideoExits)
//    {
//        callback(true);
//        return;
//    }
//    mj::helper::showWaitView();
//    bool activitiesReady = ms::LessonAISpeakingManager::getInstance().isLessonReady(info);
//    if(!activitiesReady)
//    {
//        mj::helper::deleteWaitView();
//        _commonVideoExits = false;
//        callback(false);
//        return;
//    }
//    onUnzipToGetConfig([=](bool isSuccess, std::string faile_reason) {
//        std::string name1 = "";
//        mj::helper::deleteWaitView();
//        if (isSuccess)
//        {
//            _commonVideoExits = true;
//            _isDownloadingCommonVideo = false;
//            callback(true);
//        }
//        else
//        {
//            _commonVideoExits = false;
//            callback(false);
//        }
//
//        }, ms::CommonDownloadType::ON_LEARN);
//}
//
//void MSCollectionAiSpeakingLessonItem::showAlertDownloadLesson() {
//    if(_activities.empty()){
//        auto info = _aISpeakingLessonInfo;
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//
//                    //hiện pop up hỏi download
//                     auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(_aISpeakingLessonInfo.lesson_id);
//                     std::vector<mj::model::ActAISpeakingInfo> acts;
//
//                     for (auto activity : activities) {
//                         acts.push_back(mj::model::ActAISpeakingInfo(activity));
//                     }
//                    auto size = getLessonSize(acts);
//                    int fontSize = 28.f * (CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f);
//                    const std::string& text_ok = CONFIG_MANAGER->isVietnameseStories() ? LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button") : "OK";
//                    auto alert = StoryAlert::showAlert(StringUtils::format("%s%0.2fMB.\n%s", LANGUAGE_MANAGER->getDisplayTextByKey("download.content.size").c_str(), size, LANGUAGE_MANAGER->getDisplayTextByKey("download.confirm").c_str()), text_ok, "", false, fontSize, this);
//                    alert->setTag(tag_popup_question_download);
//
//            });
//
//    }else{
//        //hiện pop up hỏi download
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
//                std::vector<mj::model::ActAISpeakingInfo> acts;
//
//                for (auto activity : _activities) {
//                    acts.push_back(mj::model::ActAISpeakingInfo(activity));
//                }
//                auto size = getLessonSize(acts);
//                int fontSize = 28.f * (CONFIG_MANAGER->isVietnameseStories() ? 1.25f : 1.f);
//                const std::string& text_ok = CONFIG_MANAGER->isVietnameseStories() ? LANGUAGE_MANAGER->getDisplayTextByKey("viral.gift.button") : "OK";
//                auto alert = StoryAlert::showAlert(StringUtils::format("%s %0.2fMB.\n%s", LANGUAGE_MANAGER->getDisplayTextByKey("download.content.size").c_str(), size, LANGUAGE_MANAGER->getDisplayTextByKey("download.confirm").c_str()), text_ok, "", false, fontSize, this);
//                alert->setTag(tag_popup_question_download);
//            });
//    }
//}
//
