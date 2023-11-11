//
//  MJAPIRegisterDevice.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/7/17.
//

//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPIRegisterDevice.h"
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
        using namespace ap;
        void RegisterAPI::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new RegisterAPI();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        RegisterAPI::RegisterAPI() {
            _request = new ap::api::Base(ap::api::PlatformType::EDU_DEVICE, "/api/v2/location/register", { }, ap::api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool RegisterAPI::onReceivedRawData(int tag, const std::string& data)
        {
            return false;
        }

        void RegisterAPI::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            auto map_response = data.GetObject();

            rapidjson::Document* doc = new rapidjson::Document();
            doc->CopyFrom(data, doc->GetAllocator());
            DecodeLocationAndDeviceInfo(tag, doc);
            if (_callback) {
                _callback(true);
            }
        }

        void RegisterAPI::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v2/location/register", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void RegisterAPI::send() {
            if (_request) {
                _request->send();
            }
        }
        void RegisterAPI::DecodeLocationAndDeviceInfo(int tag, rapidjson::Document* document) {

            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLocationAndDeviceInfo(tag, document);
        }
}



