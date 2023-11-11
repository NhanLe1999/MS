//
//  MJAPI_Audiobook_Zip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_Categories_Zip .h"
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
        void CategoriesZip::call(std::function<void(bool)> callback) {
            ap::api::APIConfig config = ap::api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new CategoriesZip();
            refresh_call->_callback = callback;
            refresh_call->send();
        }

        CategoriesZip::CategoriesZip() {
            int api_version_categories_local = MJDEFAULT->getIntegerForKey(key_api_version_categories_local, 0);
            _request = new ap::api::Base(ap::api::PlatformType::EDU_LESSON, "/api/v1/categories/list", 
                { 
                    {"lang_id", "1"}, 
                    {"version", std::to_string(api_version_categories_local)},
                }, ap::api::RequestType::GET);

            _request->enableLog();
            _request->setDelegate(this);
            this->retain();
        }

        bool CategoriesZip::onReceivedRawData(int tag, const std::string& data)
        {
            CCLOG("CategoriesZip::onReceivedRawData ");

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
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeCategoriesInfoData(tag, map_response);
            MJDEFAULT->setIntegerForKey(key_api_version_categories_local, MJDEFAULT->getIntegerForKey(key_api_version_categories_fromServer, MJDEFAULT->getIntegerForKey(key_api_version_categories_local, -1)));

            if (_callback) {
                _callback(true);
            }
            return true;
        }

        void CategoriesZip::onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data)
        {
            delete this;
        }

        void CategoriesZip::onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data)
        {
            ap::api::StoreError::call(_request->getURL(), message, "/api/v1/categories/list", _request->getParam());
            MJDEFAULT->setStringForKey(key_reason_error, message);
            if (_callback) {
                _callback(false);
            }
            this->autorelease();
        }
        void CategoriesZip::send() {
            if (_request) {
                _request->send();
            }
        }
    }
}
