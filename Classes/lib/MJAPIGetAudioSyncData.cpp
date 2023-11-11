//
//  MJAPIGetAudioSyncData.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/28/18.
//

#include "MJAPIGetAudioSyncData.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "MJDefault.h"

USING_NS_CC;

namespace mj {
    AudioSyncDataAPI * AudioSyncDataAPI::call(int audiobook_id, std::function<void(bool)> callback) {
        auto refresh_call = new AudioSyncDataAPI(audiobook_id);
        refresh_call->_callback = callback;
        refresh_call->send();
        return refresh_call;
    }
    
    AudioSyncDataAPI::AudioSyncDataAPI(int audiobook_id) {
        _audiobook_id = audiobook_id;
        _request = new mj::Request(mj::Request::ActionType::SYNC_AUDIO, {
            {"id", StringUtils::toString(_audiobook_id)}
        }, mj::Request::RequestType::GET, REQUEST_TIME_OUT);
        _request->setDelegate(this);
    }
    
    AudioSyncDataAPI::~AudioSyncDataAPI() {
        _callback = nullptr;
        if (_request) {
            _request->setDelegate(nullptr);
        }
    }
    
    void AudioSyncDataAPI::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void AudioSyncDataAPI::onRequestSuccess(int tag, rapidjson::Value d) {
        if (JSON_SAFE_CHECK(d, "tag_aegisub", String)) {
            auto sync = d["tag_aegisub"].GetString();
            cocos2d::FileUtils::getInstance()->writeStringToFile(sync, FileUtils::getInstance()->getWritablePath() + StringUtils::format("/audiobooks/%d.json", _audiobook_id));
        }
        
        if (_callback) {
            _callback(true);
        }
        
        delete this;
    }
    
    void AudioSyncDataAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_reason_error, message);
        if (_callback) {
            _callback(false);
        }
        delete this;
    }
}
