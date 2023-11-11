//
//  MJAPIRemoveLicense.cpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi.
//

#include "MJAPIRemoveLicense.h"
#include "MJDefault.h"
namespace mj {
    void MJAPIRemoveLicense::call(std::string device_id, std::string license_key, std::string device_id_remove, std::function<void(bool is_success, std::string msg)> callback){
        auto refresh_call = new MJAPIRemoveLicense(device_id, license_key, device_id_remove);
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    MJAPIRemoveLicense::MJAPIRemoveLicense(std::string device_id, std::string license_key, std::string device_id_remove) {
        _request = new mj::Request(mj::Request::ActionType::REMOVE_LICENSE, {
            {"device_id", device_id},
            {"licence", license_key},
            {"device_id_remove", device_id_remove}
        }, mj::Request::RequestType::POST);
        _request->setDelegate(this);
    }
    
    void MJAPIRemoveLicense::send() {
        if (_request) {
            _request->send();
        }
    }
    
    void MJAPIRemoveLicense::onRequestSuccess(int tag, rapidjson::Value data) {
        CCLOG("MJAPIRemoveLicense Done");
        
        if (_callback) {
            _callback(true, "");
        }
        
        delete this;
    }
    
    void MJAPIRemoveLicense::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, message);
        }
        delete this;
    }
}

