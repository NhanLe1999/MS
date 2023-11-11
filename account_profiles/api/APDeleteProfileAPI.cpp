//
//  APDeleteProfileAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#include "APDeleteProfileAPI.h"
#include "APStoreErrorAPI.h"
#include "APDatabase.h"

namespace ap {
    namespace api {
        void DeleteProfile::call(const int &profile_id, const std::function<void (bool, std::string)> &callback) {
            auto api_call = new DeleteProfile(profile_id, callback);
            api_call->send();
        }
    
        void DeleteProfile::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        DeleteProfile::DeleteProfile(const int & profile_id, const std::function<void(bool, std::string)> & callback) : _callback(callback), _profile_id(profile_id) {
            _request = new Base(api::PlatformType::NEW, "account/delete-profile", {
                {"profile_id", std::to_string(profile_id)}
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        void DeleteProfile::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "account/delete-profile", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            
            delete this;
        }
    
        void DeleteProfile::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {
            //TODO: delete profile
			ap::Database::getInstance().deleteProfile(_profile_id);
			if (_callback) {
                _callback(true, "");
            }
            
            delete this;
        }
    }
}
