//
//  MSPlaylistHelper-mac.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/6/18.
//

#include "MSPlaylistHelper.h"

#include "cocos2d.h"
#include "MJHelper.h"

#include "StoryDataModel.h"
#include "MSAudiobookPlayer.h"

#include "MSDownloader.h"
#include "StoryLanguageManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#define AUDIOBOOK_PLAYER_CLASS "org.cocos2dx.cpp.AudiobookHelper"

extern "C" {
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_didStartPlayingQueueId(JNIEnv * env, jobject thiz, jint index, jboolean playWhenReady) {
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([index, playWhenReady] {
            if(ms::playlist::ABPlayer::getInstance().getOriginalQueue().size()<=0)
                return;
           auto id = ms::playlist::ABPlayer::getInstance().getCurrentPlayingIdWithIndex(index);
            ms::playlist::ABPlayer::getInstance().didStartPlayingQueueItemId(id,index, playWhenReady);
        });
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_didFinishPlayingQueueId(JNIEnv * env, jobject thiz, jint index) {
        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([index] {
            ms::playlist::ABPlayer::getInstance().didFinishPlayingQueueItemId(index);
        });
    }
    JNIEXPORT int JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_getLimitTimePlay(JNIEnv * env, jobject thiz) {
        auto durationTime = ms::playlist::ABPlayer::getInstance().getDuration();
        return  durationTime;
    }
    JNIEXPORT long JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_getStartTimePlay(JNIEnv * env, jobject thiz) {
        auto startTimestamp = ms::playlist::ABPlayer::getInstance().getStartTimestamp();
        return  startTimestamp;
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_getTitleOfIndex(JNIEnv * env, jobject thiz, jint index) {
        auto queue = ms::playlist::ABPlayer::getInstance().getOriginalQueue();
        if (index < (int) queue.size()) {
            CCLOG("playlist get title: %s", queue[index].name.c_str());
        }
//        return cocos2d::StringUtils::newStringUTFJNI(env, queue[index].name);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AudiobookHelper_onError(JNIEnv * env,jobject thiz, jstring myStringParam) {

        const char* str = env->GetStringUTFChars(myStringParam, NULL);

        cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([str] {
            ms::playlist::ABPlayer::getInstance().onError(str);
        });
    }

}

#endif

namespace ms { namespace playlist {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
    void play(std::vector<std::string> queue) {
    }

    void setupRemoteCommand() {
    }

    void setNowPlayingInfo(mj::model::AudiobookInfo info, double elapsed_time) {
    }

    void resetNowPlayingInfo() {
    }
#endif

    ABPlayer & ABPlayer::getInstance() {
        static ABPlayer shared_player;
        return shared_player;
    }

    bool ABPlayer::hasNextItem(int i_id) {

        auto current = getCurrentPlayingIdWithIndex(i_id);//getCurrentPlayingId();

        if (_original_queue.back().book_id == current)
        {
            return false;
        }

        return true;
    }

    bool ABPlayer::hasPreviousItem(int i_id) {

        auto current =  getCurrentPlayingIdWithIndex(i_id);//getCurrentPlayingId();

        if (_original_queue.front().book_id == current)
        {
            return false;
        }
        return true;
    }


    bool ABPlayer::hasNextItem() {
        auto current = getCurrentPlayingId();
        if (_original_queue.back().book_id == current) {
            return false;
        }
        return true;
    }

    bool ABPlayer::hasPreviousItem() {
        auto current = getCurrentPlayingId();
        if (_original_queue.front().book_id == current) {
            return false;
        }
        return true;
    }

    int ABPlayer::getCurrentPlayingIdWithIndex(int i_index) {
        if(_original_queue.size()<i_index)
            return 0;
        return _original_queue[i_index].book_id;
    }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    ABPlayer::ABPlayer() {
    }

    void ABPlayer::init() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "init");
    }
    void ABPlayer::initInfor()
    {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "initInfor");
    }

    void ABPlayer::releasePlayer()
    {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "releasePlayer");
    }
    void ABPlayer::backStopPlayer()
    {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "onBack");
    }
    int ABPlayer::getCurrentPlayingId() {
        auto index = cocos2d::JniHelper::callStaticIntMethod(AUDIOBOOK_PLAYER_CLASS, "getCurrentIndex");
        return _original_queue[index].book_id;
    }

    void ABPlayer::enqueue(mj::model::AudiobookInfo info) {
        auto url = info.audio_url;
        auto local = cocos2d::FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(url);

        auto image_name = mj::helper::getFilenameFromURL(info.thumbnail_url);
        auto cached_image = FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + image_name;

        if (cocos2d::FileUtils::getInstance()->isFileExist(local)) {
            cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "enqueue", local, true, info.name, info.extra, cached_image);
        } else {
            cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "enqueue", url, false, info.name, info.extra, cached_image);
        }

        _queue.push_back(info);
    }

    void ABPlayer::play() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "play");
        MJDEFAULT->setBoolForKey(key_audio_bool_type, true);
    }

    void ABPlayer::clearPlaylist() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "clearPlaylist");
    }

    void ABPlayer::playFromBookId(int book_id, bool first_time,MSAudiobookPlayer::MSAudiobookPlayerType i_type) {

        m_currentAudioBookPlayerType = i_type;

        MJDEFAULT->setBoolForKey(key_audio_bool_type, false);
        auto it = std::find_if(_original_queue.begin(), _original_queue.end(), [book_id](mj::model::AudiobookInfo a) {
            return a.book_id == book_id;
        });

        int index = std::distance(_original_queue.begin(), it);
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "playFromIndex", index, first_time);
    }

    void ABPlayer::resume() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "resume");
    }

    void ABPlayer::pause() {
        if(!MJDEFAULT->getBoolForKey(key_audio_bool_pause))
        {
            cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "pause");
            MJDEFAULT->setBoolForKey(key_audio_bool_pause, false);
        }
    }

    void ABPlayer::stop(std::function<void()> callback) {
        if (checkInvalidState())
        {
            _queue.clear(); // (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            _original_queue.clear(); //(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "stop");
        }
        else
        {
            releasePlayer();
        }
    }

    bool ABPlayer::isSimplePlayerNull(){
        return cocos2d::JniHelper::callStaticBooleanMethod(AUDIOBOOK_PLAYER_CLASS, "isSimplePlayerNull");
    }

    void ABPlayer::next() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "next");
    }

    void ABPlayer::back() {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "back");
    }

    void ABPlayer::onError(const char* i_msg){

        char * msg = const_cast<char*>(i_msg);

        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EXOPLAYER.onError",static_cast<void*>(msg));
    }

    double ABPlayer::getCurrentProgress() {
        return cocos2d::JniHelper::callStaticDoubleMethod(AUDIOBOOK_PLAYER_CLASS, "getCurrentPosition");
    }

    void ABPlayer::seekToTime(double timestamp) {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "seekToTime", (int) timestamp * 1000);
    }

    void ABPlayer::replaceSourceAtIndex(int index, std::string source) {
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "replaceSourceAtIndex", index, source);
    }

    ABState ABPlayer::getState() {

        int state = cocos2d::JniHelper::callStaticIntMethod(AUDIOBOOK_PLAYER_CLASS,"getCurrentState");

        if(state== 1 )
        {
            return ABState::ABStatePlaying;
        }
        else if(state == 2)
        {
            return ABState::ABStateStopped;
        }
        else if(state ==0)
        {
            return ABState::ABStatePaused;
        }

        auto is_paused =cocos2d::JniHelper::callStaticBooleanMethod(AUDIOBOOK_PLAYER_CLASS, "playerPaused");
        if (is_paused) {
            return ABState::ABStatePaused;
        }

        auto is_stopped = cocos2d::JniHelper::callStaticBooleanMethod(AUDIOBOOK_PLAYER_CLASS, "isStopped");
        if (is_stopped) {
            return ABState::ABStateStopped;
        }

        auto is_playing = cocos2d::JniHelper::callStaticBooleanMethod(AUDIOBOOK_PLAYER_CLASS, "isPlaying");
        if (is_playing) {
            return ABState::ABStatePlaying;
        } else {
            return ABState::ABStatePaused;
        }
    }

    bool ABPlayer::isBackgroundState() {

    }
    bool ABPlayer::checkInvalidState() {
        ms::playlist::ABState state = getState();
        if (state != ABState::ABStateDisposed && state != ABState::ABStateStopped && state != ABState::ABStateError)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    void ABPlayer::resetPlayer() {

    }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    ABPlayer::ABPlayer() {
    }

    void ABPlayer::init() {
    }

    int ABPlayer::getCurrentPlayingId() {
        return 0;
    }

    void ABPlayer::enqueue(mj::model::AudiobookInfo info) {
    }

    void ABPlayer::play() {
    }

    void ABPlayer::playFromBookId(int book_id, bool first_time,MSAudiobookPlayer::MSAudiobookPlayerType i_type) {
    }

    void ABPlayer::resume() {
    }

    void ABPlayer::pause() {
    }

    void ABPlayer::stop(std::function<void()> callback) {
    }

    void ABPlayer::next() {
    }

    void ABPlayer::back() {
    }

    double ABPlayer::getCurrentProgress() {
        return 0;
    }

    void ABPlayer::seekToTime(double timestamp) {
    }

    void ABPlayer::replaceSourceAtIndex(int index, std::string source) {
    }

    ABState ABPlayer::getState() {
		return ABState::ABStateStopped;
    }

    bool ABPlayer::isBackgroundState() {
		return false;

    }

    void ABPlayer::resetPlayer() {

    }

    void ABPlayer::clearPlaylist() {

    }
#endif

    std::vector<int> ABPlayer::getNext5Ids() {
        auto current_id = getCurrentPlayingId();
        auto it = std::find_if(_original_queue.begin(), _original_queue.end(), [current_id](mj::model::AudiobookInfo a) {
            return a.book_id == current_id;
        });
        std::vector<int> next_ids;
        for (; it != _original_queue.end(); ++it) {
            next_ids.push_back(it->book_id);
            if (next_ids.size() == 5) {
                break;
            }
        }

        return next_ids;
    }

    void ABPlayer::setOriginalQueue(std::vector<mj::model::AudiobookInfo> audiobooks) {
        _original_queue = audiobooks;
    }

    std::vector<mj::model::AudiobookInfo> ABPlayer::getOriginalQueue() {
        return _original_queue;
    }

    void ABPlayer::setDuration(int value) {
        _playlist_duration = value * 60;
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        _playlist_duration = _playlist_duration / 5;
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "setPlaylistDuration", (int)_playlist_duration);
#endif
    }

    void ABPlayer::setStartTimestamp(long value) {
        _start_timestamp = value;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod(AUDIOBOOK_PLAYER_CLASS, "setStartTimestamp", (long)value);
#endif
    }

    std::vector<mj::model::AudiobookInfo> ABPlayer::getQueuedAudiobooks() {
        return _queue;
    }

    void ABPlayer::didStartPlayingQueueItemId(int item_id,int i_IDOfList, bool playWhenReady) {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        {
            auto current_player = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player");

            if (current_player)
            {
                if (current_player->getInfo().book_id == getCurrentPlayingId())
                {
                    return;
                }
            }

            auto info = StoryDataModel::getInstance()->getAudiobook(item_id);

            if(!MJDEFAULT->getBoolForKey(key_audio_bool_type))
            {
                auto view_player = MSAudiobookPlayer::createView(info, current_player!= nullptr ? current_player->getCurrentAudioBookPlayerType() : m_currentAudioBookPlayerType, i_IDOfList, playWhenReady);

                Director::getInstance()->getRunningScene()->addChild(view_player, 1001001);
            }

            if (current_player) {
                current_player->removeFromParent();
            }
        }
        return;
#endif

        auto current_player = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player");

        if (current_player) {
            if (current_player->getInfo().book_id == getCurrentPlayingId()) {
                return;
            }
        }

        auto info = StoryDataModel::getInstance()->getAudiobook(item_id);
        auto view_player = MSAudiobookPlayer::createView(info,current_player!= nullptr ? current_player->getCurrentAudioBookPlayerType() : m_currentAudioBookPlayerType);

        Director::getInstance()->getRunningScene()->addChild(view_player, 1001001);
        if (current_player) {
                current_player->removeFromParent();
        }

        auto next_ids = getNext5Ids();
        bool flag = false;
        for (auto book_id : next_ids) {
            auto book_info = StoryDataModel::getInstance()->getAudiobook(book_id);
            CCLOG("attemp to download %s ,book_id:%d", book_info.name.c_str(),book_id);

            auto cached_audio = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(book_info.audio_url);

            auto url = mj::helper::getUrlFromDomain(book_info.audio_url, mj::UrlType::MSURL_AUDIO);

            ms::utils::DownloadUnit unit;
            unit.download_id = mj::helper::getFilenameFromURL(url);
            unit.storage_path = cached_audio;
            unit.url = url;
            unit.success_event = StringUtils::format("download_audiobook_success_%s", unit.download_id.c_str());
            unit.progress_event = StringUtils::format("download_audiobook_progress_%s", unit.download_id.c_str());
            unit.failed_event = StringUtils::format("download_audiobook_failed_%s", unit.download_id.c_str());

            ms::utils::Downloader::getInstance().enqueue(unit);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if (!flag) {
                //ignore first item (currently playing)
                flag = true;
                continue;
            }

            Director::getInstance()->getEventDispatcher()->addCustomEventListener(unit.success_event, [unit](EventCustom *) {
                auto queue = ABPlayer::getInstance().getOriginalQueue();
                auto it = std::find_if(queue.begin(), queue.end(), [unit](mj::model::AudiobookInfo a) {
                    return a.audio_url == unit.url;
                });
                if (it != queue.end()) {
                    auto local = cocos2d::FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(unit.url);
                    ABPlayer::getInstance().replaceSourceAtIndex((int)std::distance(queue.begin(), it), local);
                }
                Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
            });
#endif
        }
    }

    void ABPlayer::didFinishPlayingQueueItemId(int item_id) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID // new flow for exoplayer v10.5 , android only
        item_id = _original_queue[item_id].book_id;

        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(cocos2d::StringUtils::format("abplayer.finished.%d", item_id));

        if(auto current_player = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player"))
        {
            if (!current_player->isPlayingPlaylist()) //
            {
                stop();
                current_player->removeFromParent();
                return;
            }
            current_player->removeFromParent();
        }

        auto currentTime = (int)cocos2d::utils::getTimeInMilliseconds() / 1000;

        if (currentTime - _start_timestamp > _playlist_duration && _isAlertShowing == false)
        {
            _isAlertShowing = true;
            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                auto msg = LANGUAGE_MANAGER->getDisplayTextByKey("playlist.end.duration");
                msg = mj::helper::replace_string(msg, "{duration}", StringUtils::toString(_playlist_duration / 60));
                StoryAlert::showAlert(msg, "OK", "");
            });

            return;
        }

        return;
#else
        CCLOG("player stopping id: %d", item_id);
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(cocos2d::StringUtils::format("abplayer.finished.%d", item_id));

        auto current_time = (int)cocos2d::utils::getTimeInMilliseconds() / 1000;
        if (current_time - _start_timestamp > _playlist_duration) {
            stop();
            auto current_player = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player");
            if (!current_player->isPlayingPlaylist()) {
                return;
            }
            if (current_player) {
                current_player->removeFromParent();
            }

            cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                auto msg = LANGUAGE_MANAGER->getDisplayTextByKey("playlist.end.duration");
                msg = mj::helper::replace_string(msg, "{duration}", StringUtils::toString(_playlist_duration / 60));
                StoryAlert::showAlert(msg, "OK", "");
            });

            return;
        }

        if (std::find_if(_queue.begin(), _queue.end(), [item_id](mj::model::AudiobookInfo a) {
            return a.book_id == item_id;
        }) == _queue.end()) {
            return;
        }

        while (!_queue.empty() && _queue.front().book_id != item_id) {
            _queue.erase(_queue.begin());
        }
        if (!_queue.empty()) {
            _queue.erase(_queue.begin());
        }

        if (_queue.empty()) {
            stop();
            auto current_player = Director::getInstance()->getRunningScene()->getChildByName<MSAudiobookPlayer *>("audiobook_player");
            if (current_player) {
                current_player->removeFromParent();
            }

            return;
        }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto cached = FileUtils::getInstance()->getWritablePath() + "/audiobooks/audio/" + mj::helper::getFilenameFromURL(_queue.front().audio_url);
        if (FileUtils::getInstance()->isFileExist(cached)) {
            resetPlayer();
            auto temp = _queue;
            _queue.clear();
            for (auto a : temp) {
                enqueue(a);
            }
        }
#endif
    }

    void ABPlayer::didCancelQueuedItems() {

    }

    void ABPlayer::onAppDidEnterForeground() {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return;
#else
        auto currentScene = Director::getInstance()->getRunningScene();
        
        if(!currentScene) {
            return;
        }
        
        auto current_player = currentScene->getChildByName<MSAudiobookPlayer *>("audiobook_player");
        if (!current_player) {
            return;
        }

        if (current_player) {
            if (_queue.empty()) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                current_player->removeFromParent();
#endif
                return;
            }
            if (current_player->getInfo().book_id == getCurrentPlayingId()) {
                return;
            }

            cocos2d::utils::findChild(current_player, "button_pause")->setVisible(getState() == ABState::ABStatePlaying);
            cocos2d::utils::findChild(current_player, "button_play")->setVisible(getState() != ABState::ABStatePlaying);
        }
        auto info = StoryDataModel::getInstance()->getAudiobook(getCurrentPlayingId());
        auto view_player = MSAudiobookPlayer::createView(info, current_player!= nullptr ? current_player->getCurrentAudioBookPlayerType() : m_currentAudioBookPlayerType);

        Director::getInstance()->getRunningScene()->addChild(view_player, 1001001);
        if (current_player) {
//            if (isBackgroundState()) {
                current_player->removeFromParent();
//            } else {
//                view_player->setOpacity(0);
//                view_player->runAction(FadeIn::create(0.15));
//                current_player->runAction(Sequence::create(FadeOut::create(0.15), RemoveSelf::create(), nullptr));
//            }
        }
#endif
    }
}}
