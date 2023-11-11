//
//  MJAPI_RemoveLicense.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//


#include "MJAPI_RemoveLicense.h"
#include "MJDefault.h"
#include "GlobalFunc.h"
#include "MJPay.h"

//pdwnd
//gkxqy

namespace mj { namespace api {
    void RemoveLicense::call(std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new RemoveLicense();
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    RemoveLicense::RemoveLicense() {
        _request = new mj::Request(mj::Request::ActionType::REMOVE_LICENSE, {
            {"license", mj::Pay::getInstance().getLicense()}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void RemoveLicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void RemoveLicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPIRemoveLicense Done");
        
        if (_callback) {
            _callback(true, "");
        }
        
        delete this;
    }
    
    void RemoveLicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}}
