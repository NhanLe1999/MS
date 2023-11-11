//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_LessonInterative_Zip.h"
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

namespace mj {
    namespace api {
        using namespace ap;
        void LessonInterativeZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new LessonInterativeZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        LessonInterativeZip::LessonInterativeZip() {
            std::string updateStoryIdsStr = MJDEFAULT->getStringForKey(key_update_story_ids, "");

            if (updateStoryIdsStr.empty())
            {
                if (_callback) {
                    _callback(true);
                }
            }
            _request = new ap::api::Base(ap::api::PlatformType::EDU_LESSON, "/api/v1/story/lesson-interactive",
                { 
                    {"lang_id", "1"},  
                {"stories_id",updateStoryIdsStr}
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        void LessonInterativeZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            CCLOG("LessonInterativeZip::onRequestSuccess");

            if (!data.IsArray())
            {
                if (_callback) {
                    _callback(false);
                }
            }
            rapidjson::Document* doc = new rapidjson::Document();
            doc->CopyFrom(data, doc->GetAllocator());
            auto map_response1 = doc->GetArray();
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeStoryLessonInteractiveData(tag, map_response1);
            if (_callback) {
                _callback(true);
            }
        }

        void LessonInterativeZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void LessonInterativeZip::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
