//
//  APVerifyOptAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APVerifyOptAPI.h"
#include "SQLiteCpp/SQLiteCpp.h"
#include "cocos2d.h"
#include "../APDatabase.h"
#include "APStoreErrorAPI.h"
#include "MJHelper.h"
#include "MJDebugView.h"
#include "StoryDataModel.h"
#include "APAuth.h"
namespace ap {
    namespace api {
        void VerifyOpt::call(
						 const std::string & otp,
                         const std::function<void(bool is_success, std::string msg, std::string condition)> & callback) {
			api::APIConfig config = api::APIConfig();
			ap::api::Base::setGlobalConfiguration(config);
            auto refresh_call = new VerifyOpt(otp, callback);
            refresh_call->send();
        }
        
        VerifyOpt::VerifyOpt(
					  const std::string & otp,
                      const std::function<void(bool is_success, std::string msg, std::string condition)> & callback) :
			_otp(otp),_callback(callback) {
            _request = new Base(api::PlatformType::NEW, "account/verify-opt", {
                {"code", _otp}
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
        
        void VerifyOpt::send() const {
            if (_request) {
                _request->send();
            }
        }
        
        void VerifyOpt::onRequestSuccess(int /*tag*/, int /*code*/, const std::string & message, const rapidjson::Value & data) {
			int user_id = 0;
			std::string token = "";
			if (!data.IsNull() && data.IsObject()) {
				auto map_response = data.GetObject();
				if (JSON_SAFE_CHECK(map_response, "user_id", Int)) {
					user_id = map_response["user_id"].GetInt();
				}
				if (JSON_SAFE_CHECK(map_response, "access_token", String)) {
					token = map_response["access_token"].GetString();
				}
			}
			if (user_id != 0 && token != "") {
                ap::Auth::clearUserData();
                MJDEFAULT->setBoolForKey(key_logged_in, true);
                Database::getInstance().saveIdentity(std::to_string(user_id), token);
			}
			if (_callback) {
                _callback(true, message, "");
            }
            delete this;
        }
        void VerifyOpt::onRequestError(int /*tag*/, int /*error_code*/, const std::string & message, const rapidjson::Value & /*data*/) {
			StoreError::call(_request->getURL(), message, "account/verify-opt", _request->getParam());
            if (_callback) {
                _callback(false, message, "");
            }
            delete this;
        }
    }
}
