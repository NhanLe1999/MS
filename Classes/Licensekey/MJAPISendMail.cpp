//
//  MJAPISendMail.cpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi.
//

#include "MJAPISendMail.h"
#include "MJDefault.h"
#include "GlobalFunc.h"
#include "cocostudiomacro2.h"
namespace mj {
    void MJAPISendMail::call(std::string mail, std::string phone, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new MJAPISendMail(mail, phone);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    MJAPISendMail::MJAPISendMail(std::string mail, std::string phone) {
        _request = new mj::Request(mj::Request::ActionType::SAVE_MAIL, {
            {"email", mail},
            {"phone", phone}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void MJAPISendMail::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void MJAPISendMail::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPISendMail Done");
        auto msg = std::string("");
        if (_callback) {
            _callback(true, msg);
        }
        
        delete this;
    }
    
    void MJAPISendMail::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}

