//
//  MJAPILicense.cpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi.
//

#include "MJAPILicense.h"
#include "MJDefault.h"
namespace mj {
    void MJAPILicense::call(std::string license_key, bool confirm, std::function<void(bool,std::string,rapidjson::Value,int)> callback){
        auto refresh_call = new MJAPILicense(license_key, confirm);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    MJAPILicense::MJAPILicense(std::string license_key, bool confirm) {
        if (!confirm) {
            _request = new mj::Request(mj::Request::ActionType::ACTIVE_LICENSE_V2, {
                {"licence", license_key}
            }, mj::Request::RequestType::POST);
        } else {
            _request = new mj::Request(mj::Request::ActionType::ACTIVE_LICENSE_V2, {
                {"licence", license_key},
                {"is_confirm", "1"}
            }, mj::Request::RequestType::POST);
        }
        _request->setDelegate(this);
    }
    
    void MJAPILicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void MJAPILicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("Active Licence Success");
        
        std::string msg = "";
        if (data.HasMember("message") && data["message"].IsString()) {
            msg = data["message"].GetString();
        }
        
        if (_callback) {
            _callback(true, msg, data["data"].GetObject(), 1);
        }
        
        delete this;
    }
    
    void MJAPILicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        CCLOG("Active Licence Error = %d", error_code);
        if (_callback) {
//            rapidjson::Value a(rapidjson::kObjectType);
            _callback(false, message, data.GetObject(), error_code);
        }
        delete this;
    }
}

