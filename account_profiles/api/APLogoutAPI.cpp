//
//  APLogoutAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#include "APLogoutAPI.h"
#include "APStoreErrorAPI.h"
#include "APDatabase.h"

namespace ap {
    namespace api {
        void Logout::call(const std::function<void (bool, std::string)> &callback) {
            auto api_call = new Logout(callback);
            api_call->send();
        }
        
        void Logout::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        Logout::Logout(const std::function<void(bool, std::string)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "authen/logout", {}, api::RequestType::POST);
            _request->enableLog();
            _request->setDelegate(this);
        }
    
        void Logout::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "authen/logout", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            
            delete this;
        }
    
        void Logout::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {            
			//Drop account profile database
			//ap::Database::getInstance().dropDatabase();
			if (_callback) {
                _callback(true, "");
            }
            
            delete this;
        }
    }
}
