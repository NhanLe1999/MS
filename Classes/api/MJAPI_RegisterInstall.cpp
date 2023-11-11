//
//  MJAPI_RegisterInstall.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/8/19.
//

#include "MJAPI_RegisterInstall.h"
#include "MJHelper.h"

namespace mj { namespace api {
    RegisterInstall * RegisterInstall::call(const std::string & name, const std::string & phone, const std::string mail, long long birthday, bool is_verified, std::function<void(bool, std::string)> callback) {
        auto refresh_call = new RegisterInstall(name, phone, mail, birthday, is_verified);
        refresh_call->_callback = callback;
        refresh_call->send();
        return refresh_call;
    }
    
    RegisterInstall * RegisterInstall::confirmGift(const std::string & name, const std::string & phone, const std::string mail, std::function<void(bool, std::string)> callback) {
        auto refresh_call = new RegisterInstall(name, phone, mail);
        refresh_call->_callback = callback;
        refresh_call->send();
        return refresh_call;
    }
    
    RegisterInstall::RegisterInstall(const std::string & name, const std::string & phone, const std::string mail) {
        _request = new mj::Request(mj::Request::ActionType::REGISTER_INSTALL, {
            {"user_name", name},
            {"phone", phone},
            {"email", mail},
            {"inapp", "1"}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    RegisterInstall::RegisterInstall(const std::string & name, const std::string & phone, const std::string mail, long long birthday, bool is_verified) {
        _request = new mj::Request(mj::Request::ActionType::REGISTER_INSTALL, {
            {"user_name", name},
            {"phone", phone},
            {"email", mail},
            {"child_age", cocos2d::StringUtils::toString(birthday)},
            {"is_verified", is_verified ? "1" : "0"}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void RegisterInstall::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void RegisterInstall::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("RegisterInstall::onRequestSuccess");

        auto msg = std::string("");
        if (data.IsObject()) {
            if (JSON_SAFE_CHECK(data, "voucher", String)) {
                msg = data["voucher"].GetString();
            }
        }
        
        if (_callback) {
            _callback(true, msg);
        }
        
        delete this;
    }
    
    void RegisterInstall::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}}
