//
//  APUpdatePasswordAPI.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/21/20.
//

#include "APUpdatePasswordAPI.h"

namespace ap {
    namespace api 
	{
        void UpdatePassword::call(const std::string& old_password, const std::string& new_password, const std::function<void(bool is_success, std::string msg)> & callback){
            auto api_call = new UpdatePassword(old_password, new_password, callback);
            api_call->send();
        }

		void UpdatePassword::callDefaulPasswordChange(const std::string & new_access_token, const std::string & password, const std::string & token, const std::function<void(bool is_success, std::string msg)>& callback) 
		{
			auto api_call = new UpdatePassword(new_access_token, password, token, callback);
			api_call->send(token);
		}
    
        UpdatePassword::UpdatePassword(const std::string& old_password, const std::string& new_password, const std::function<void(bool is_success, std::string msg)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/confirm-pw", {
                {"old_password", old_password},
                {"new_password", new_password},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }

		UpdatePassword::UpdatePassword(const std::string & new_access_token, const std::string & password, const std::string & old_token, const std::function<void(bool is_success, std::string msg)>& callback)
			: _callback(callback) 
		{
			_request = new Base(api::PlatformType::NEW, "change-pw-user-crm", {
			 {"new_access_token", new_access_token},
			 {"password", password},
				}, api::RequestType::POST);
			_request->setDelegate(this);
			_request->enableLog();
		}
    
        void UpdatePassword::send(std::string i_token) const {
            if (_request) {
                _request->send(i_token);
            }
        }
    
        void UpdatePassword::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(true, message);
            }

            delete this;
        }
    
        void UpdatePassword::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
