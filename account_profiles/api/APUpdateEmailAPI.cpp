//
//  APUpdateEmailAPI.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/20/20.
//

#include "APUpdateEmailAPI.h"

namespace ap {
    namespace api {
        void UpdateEmail::call(const std::string& email, const std::function<void(bool is_success, std::string msg)> & callback){
            auto api_call = new UpdateEmail(email, callback);
            api_call->send();
        }
    
        UpdateEmail::UpdateEmail(const std::string& email, const std::function<void(bool is_success, std::string msg)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/send-ebook-via-email", {
                {"email", email},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        void UpdateEmail::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        void UpdateEmail::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(true, message);
            }
            delete this;
        }
    
        void UpdateEmail::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
