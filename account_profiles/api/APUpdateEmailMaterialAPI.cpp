//
//  APUpdateEmailMaterialAPI.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 11/17/20.
//

#include "APUpdateEmailMaterialAPI.h"

namespace ap {
    namespace api {
        void UpdateEmailMaterial::call(const std::string& email, const std::string& user_id, const std::function<void(bool is_success, std::string msg)> & callback){
            auto api_call = new UpdateEmailMaterial(email, user_id, callback);
            api_call->send();
        }
        
        UpdateEmailMaterial::UpdateEmailMaterial(const std::string& email, const std::string& user_id, const std::function<void(bool is_success, std::string msg)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "email/config-email-worksheet", {
                {"user_id", user_id},
                {"email", email},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
        
        void UpdateEmailMaterial::send() const {
            if (_request) {
                _request->send();
            }
        }
        
        void UpdateEmailMaterial::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(true, message);
            }
            delete this;
        }
        
        void UpdateEmailMaterial::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
