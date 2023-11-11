//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_Audiobook_Zip.h"
#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryDataModel.h"
#include "MJStoryInfoModel.h"
#include "StoryFreeManager.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>
#include "MJAPI_DecodeObjectJsonInfo.h"
#include"APStoreErrorAPI.h"

#if TURN_ON_NEW_DOMAIN
#define DOMAIN_NEW "/api/v2/audiobook/list"
#else
#define DOMAIN_NEW "/api/v1/audiobook/list"
#endif

namespace mj { 
    namespace api {
    using namespace ap;
    void AudiobookZip::call(std::function<void(bool)> callback) {
        ap::api::APIConfig config = ap::api::APIConfig();
        ap::api::Base::setGlobalConfiguration(config);
        auto refresh_call = new AudiobookZip();
        refresh_call->_callback = callback;
        refresh_call->send();
    }

    AudiobookZip::AudiobookZip() {
        int api_version_audio_local = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0);
#if TURN_ON_NEW_DOMAIN
        _request = new ap::api::Base(ap::api::PlatformType::EDU_STORY, DOMAIN_NEW,
            {
                {"lang_id", "1"},
                {"version", std::to_string(api_version_audio_local)},
            } , ap::api::RequestType::GET);
#else
        _request = new ap::api::Base(ap::api::PlatformType::EDU_LESSON, DOMAIN_NEW,
            {
                {"lang_id", "1"},
                {"version", std::to_string(api_version_audio_local)},
            } , ap::api::RequestType::GET);
#endif
        _request->enableLog();
        _request->setDelegate(this);
        this->retain();
    }

    bool AudiobookZip::onReceivedRawData(int tag, const std::string& data)
    {
        CCLOG("AudiobookZip::onReceivedRawData ");

       auto unzip_json = mj::helper::unzip_json(data);

       //auto unzip_json = mj::helper::unzip(unzip_json, FileUtils::getInstance()->getWritablePath());

        if (!unzip_json) {
            if (_callback) {
                _callback(false);
            }
            return false;
        }

        rapidjson::Document doc;
        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);

        if (doc.HasParseError()) {
            doc.GetParseError();
            if (_callback) {
                _callback(false);
            }
            return false;
        }

        if (!doc.IsObject()) {
            if (_callback) {
                _callback(false);
            }
            return false;
        }

        auto map_response = doc.GetObject();
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAudioData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update, map_response);
        MJDEFAULT->setIntegerForKey(key_api_version_audio_local, MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_audio_local, -1)));
        if (_callback) {
            _callback(true);
        }
        return true;
    }

    void AudiobookZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
    {
        delete this;
    }

    void AudiobookZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
    {
        ap::api::StoreError::call(_request->getURL(), message, DOMAIN_NEW, _request->getParam());
        MJDEFAULT->setStringForKey(key_reason_error, message);
        if (_callback) {
            _callback(false);
        }
        this->autorelease();
    }

    void AudiobookZip::send() {
        if (_request) {
            _request->send();
        }
    }
}
}
