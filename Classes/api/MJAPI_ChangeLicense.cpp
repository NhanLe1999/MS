//
//  MJAPI_ChangeLicense.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/1/18.
//

#include "MJAPI_ChangeLicense.h"
#include "MJHelper.h"
#include "base64.h"
#include <iostream>
#include <ctime>

namespace mj { namespace api {
    ChangeLicense * ChangeLicense::call(Type type, std::string license, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new ChangeLicense(type, license);
        refresh_call->_callback = callback;
        refresh_call->send();
        return refresh_call;
    }
    
    ChangeLicense::ChangeLicense(Type type, std::string license) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
        auto key = cocos2d::utils::getTimeInMilliseconds() / 1000 + 30;
#else
		long long key = time(NULL) + 30;
#endif
        auto string_key = cocos2d::StringUtils::format("STRING_HAS_KEY|%lld", key);
        string_key = base64::encode(string_key);
        _request = new mj::Request(mj::Request::ActionType::CHANGE_LICENSE, {
            {"type", type == Type::PHONE ? "1" : "2"},
            {"licence", license},
            {"string_key", string_key}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void ChangeLicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void ChangeLicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("ChangeLicense::onRequestSuccess ");

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
    
    void ChangeLicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}}

