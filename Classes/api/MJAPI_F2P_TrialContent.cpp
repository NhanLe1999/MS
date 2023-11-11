//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_F2P_TrialContent.h"
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
        void MJAPI_F2P_TrialContent::call(const ap::Profile& profile, const std::function<void(bool)> callback)/* : _profile(profile), _callback(callback)*/
        {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new MJAPI_F2P_TrialContent(profile);
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        MJAPI_F2P_TrialContent::MJAPI_F2P_TrialContent(const ap::Profile profile) {
            _profile = profile;
            _request = new ap::api::Base(ap::api::PlatformType::EDU_STORY, "/api/v1/story/trial-items",
                {
                    {"profile_id", std::to_string(_profile.profile_id)},
                }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool MJAPI_F2P_TrialContent::onReceivedRawData(int tag, const std::string& data)
        {
           /* CCLOG("MJAPI_F2P_TrialContent::onReceivedRawData");

            auto unzip_json = mj::helper::unzip_json(data);
            if (!unzip_json) {
                return false;
            }

            rapidjson::Document doc;
            doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);

            if (doc.HasParseError()) {
                doc.GetParseError();
                return false;
            }

            if (!doc.IsObject()) {
                return false;
            }

            auto map_response = doc.GetObject();
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeF2PTrialItemsData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update , map_response);
            if (_callback) {
                _callback(true);
            }*/
            return false;
        }

        void MJAPI_F2P_TrialContent::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            rapidjson::Document* doc = new rapidjson::Document();
            doc->CopyFrom(data, doc->GetAllocator());
            if (doc->IsObject())
            {
                auto map_response = doc->GetObject();
                mj::api::MJAPIDecodeObjectJsonInfo::onDecodeF2PTrialItemsData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Update, map_response);
            }
            if (_callback) {
                _callback(true);
            }
            //this->autorelease();
        }

        void MJAPI_F2P_TrialContent::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/story/trial-items", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void MJAPI_F2P_TrialContent::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
