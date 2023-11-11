//
//  APActiveLicenseAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#include "APActiveLicenseAPI.h"
#include "APStoreErrorAPI.h"
#include "MJDefault.h"
#include "MJDefinitions.h"

namespace ap {
    namespace api {
        void ActiveLicense::call(const std::string &license, const std::function<void (bool, std::string)> &callback) {
            auto api_call = new ActiveLicense(license, callback);
            api_call->send();
        }
    
        void ActiveLicense::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        ActiveLicense::ActiveLicense(const std::string & license, const std::function<void(bool,std::string)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/licence-active", {
                {"licence", license}
            }, api::RequestType::POST);
            _request->enableLog();
            _request->setDelegate(this);
        }
    
        void ActiveLicense::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "account/licence-active", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
        
        void ActiveLicense::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {
            auto response = data.GetObject();
            auto expire_time = response["time_expire"].GetInt();
            if (expire_time == 0) {
                expire_time = -1;
            }
            MJDEFAULT->setIntegerForKey(keyTimeExpire, expire_time);
            
            std::string msg = "";
            if (response.HasMember("message") && response["message"].IsString()) {
                msg = response["message"].GetString();
            }
            
            if (_callback) {
                _callback(true, message);
            }
            delete this;
        }
    }
}
