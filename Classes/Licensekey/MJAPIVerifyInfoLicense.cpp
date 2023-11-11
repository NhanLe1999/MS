//
//  MJAPIVerifyInfoLicense.cpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi.
//

#include "MJAPIVerifyInfoLicense.h"
#include "MJDefault.h"
namespace mj {
    void MJAPIVerifyInfoLicense::call(std::string info_user, std::string license_key, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new MJAPIVerifyInfoLicense(info_user, license_key);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    MJAPIVerifyInfoLicense::MJAPIVerifyInfoLicense(std::string info_user, std::string license_key) {
        _request = new mj::Request(mj::Request::ActionType::VERIFY_INFO_LICENSE, {
            {"licence", license_key},
            {"infouser", info_user}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void MJAPIVerifyInfoLicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void MJAPIVerifyInfoLicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPIVerifyInfoLicense Done");
        
        if (_callback) {
            _callback(true, "");
        }
        
        delete this;
    }
    
    void MJAPIVerifyInfoLicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}

