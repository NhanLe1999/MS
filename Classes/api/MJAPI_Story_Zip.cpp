//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_Story_Zip.h"
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
#include"MJAPI_LessonInterative_Zip.h"

#if TURN_ON_NEW_DOMAIN
#define DOMAIN_NEW "/api/v2/story/list"
#else
#define DOMAIN_NEW "/api/v1/story/list"
#endif

namespace mj {
    namespace api {
        using namespace ap;
        void StoryZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new StoryZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        StoryZip::StoryZip() {
            int api_version_story_local = MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0);
            _request = new ap::api::Base(ap::api::PlatformType::EDU_STORY, DOMAIN_NEW,
                { 
                    {"lang_id", "1"},
                    {"ver", std::to_string(api_version_story_local)},
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool StoryZip::onReceivedRawData(int tag, const std::string& data)
        {
            CCLOG("StoryZip::onReceivedRawData");

            auto unzip_json = mj::helper::unzip_json(data);
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
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update , map_response);
            MJDEFAULT->setIntegerForKey(key_api_version_story_local, MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_story_local, -1)));
            if (_callback) {
                _callback(true);
            }
            return true;
        }

        void StoryZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            mj::api::LessonInterativeZip::call(nullptr);
            delete this;
        }

        void StoryZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/story/list", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void StoryZip::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
