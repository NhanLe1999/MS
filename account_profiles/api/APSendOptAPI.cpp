//
//  APSendOptPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APSendOptAPI.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "APStoreErrorAPI.h"
namespace ap {
    namespace api {
    
        // AUTHOR: Hieu Nguyen
        void SendOpt::call(
                           bool forEmail,
                           const std::string & phone,
                           const std::string & email,
                           const std::function<void(bool is_success, std::string msg)> & callback,
                           api::PlatformType type)
        {
            api::APIConfig config = api::APIConfig();
            ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new SendOpt(forEmail, phone, email, callback, type);
            refresh_call->send();
        }
        // -- end --
    
        void SendOpt::call(
                         const std::string & phone,
                         const std::function<void(bool is_success, std::string msg)> & callback) {
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new SendOpt(phone, callback);
            refresh_call->send();
        }
        
        SendOpt::SendOpt(
                      const std::string & phone,
                      const std::function<void(bool is_success, std::string msg)> & callback) :
			_phone(phone),_callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/send-opt-verify", {
                {"phone", phone},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        // AUTHOR: Hieu Nguyen
        SendOpt::SendOpt(
                         bool forEmail,
                         const std::string & phone,
                         const std::string & email,
                         const std::function<void(bool is_success, std::string msg)> & callback,
                         api::PlatformType type) : _callback(callback)
        {
            if (forEmail) {
                _request = new Base(type, "account/send-opt-verify-email", {
                    {"type", "2"},
                    {"email", email},
                }, api::RequestType::POST);
            } else {
                _request = new Base(type, "account/send-opt-verify-email", {
                    {"phone", phone},
                }, api::RequestType::POST);
            }
            
            _request->setDelegate(this);
            _request->enableLog();
        }
        // -- end --
        
        void SendOpt::send() const {
            if (_request) {
                _request->send();
            }
        }
        
        void SendOpt::onRequestSuccess(int /*tag*/, int /*code*/, const std::string & /*message*/, const rapidjson::Value & data) {
            int user_id = 0;
            if (_callback) {
                _callback(true, "");
            }
            
            delete this;
        }
        
        void SendOpt::onRequestError(int /*tag*/, int /*error_code*/, const std::string & message, const rapidjson::Value & /*data*/) {
			//StoreError::call(_request->getURL(), message, "account/send-opt-verify", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
