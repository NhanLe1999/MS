//
//  MJAPI_ForgotLicense.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#include "MJAPI_ForgotLicense.h"
#include "MJHelper.h"

namespace mj { namespace api {
    ForgotLicense * ForgotLicense::call(std::string info_user, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new ForgotLicense(info_user);
        refresh_call->_callback = callback;
        refresh_call->send();
        return refresh_call;
    }
    
    ForgotLicense::ForgotLicense(std::string info_user) {
        _request = new mj::Request(mj::Request::ActionType::FORGOT_LICENSE, {
            {"infouser", info_user}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void ForgotLicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void ForgotLicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPIGiveLicense Done");
        
        auto msg = std::string("");
        if (data.IsObject()) {
            if (JSON_SAFE_CHECK(data, "message", String)) {
                msg = data["message"].GetString();
            }
        }
        
        if (_callback) {
            _callback(true, msg);
        }
        
        delete this;
    }
    
    void ForgotLicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}}
