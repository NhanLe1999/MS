//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_AISpeakingLessonList_Zip.h"
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
        void MJAPIAISpeakingLessonListZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new MJAPIAISpeakingLessonListZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        MJAPIAISpeakingLessonListZip::MJAPIAISpeakingLessonListZip() {
            int api_version_ai_speaking_lesson_list_local = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_local, 0);
            _request = new ap::api::Base(ap::api::PlatformType::EDU_LESSON, "/api/v1/lesson/list-lesson-monkey-talking",
                { 
                    {"lang_id", "1"}, 
                    {"version", std::to_string(api_version_ai_speaking_lesson_list_local)},
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool MJAPIAISpeakingLessonListZip::onReceivedRawData(int tag, const std::string& data)
        {
            auto unzip_json = mj::helper::unzip_json(data);
            if (!unzip_json) {
                if (_callback) {
                    _callback(false);
                }
                return false;
            }

            CCLOG("MJAPIAISpeakingItemsListZip::onReceivedRawData, response: %s", unzip_json);

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
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAISpeakingLessonListData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update, map_response);
            MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_local, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_local, -1)));

            if (_callback) {
                _callback(true);
            }
            return true;
        }

        void MJAPIAISpeakingLessonListZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            mj::api::ActivitiesListZip::call(mj::api::ActivitiesType::AI_SPEAKING_LESSON, nullptr);
            delete this;
        }

        void MJAPIAISpeakingLessonListZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/list-lesson-monkey-talking", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void MJAPIAISpeakingLessonListZip::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
