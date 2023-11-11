//
//  MJAPIVerifyViral.cpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi.
//

#include "MJAPIVerifyViral.h"
#include "MJDefault.h"
#include "GlobalFunc.h"
#include "cocostudiomacro2.h"
namespace mj {
    void MJAPIVerifyViral::call(std::string phone, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new MJAPIVerifyViral(phone);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    MJAPIVerifyViral::MJAPIVerifyViral(std::string phone) {
        _request = new mj::Request(mj::Request::ActionType::VERIFY_INFO_VIRAL, {
            {"phone", phone}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void MJAPIVerifyViral::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void MJAPIVerifyViral::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPIVerifyViral Done");
        auto msg = std::string("");
        if (_callback) {
            _callback(true, msg);
        }
        
        delete this;
    }
    
    void MJAPIVerifyViral::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}

