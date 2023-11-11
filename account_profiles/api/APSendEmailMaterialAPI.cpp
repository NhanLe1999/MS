//
//  APSendEmailMaterialAPI.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 11/20/20.
//

#include "APSendEmailMaterialAPI.h"

namespace ap {
    namespace api {
        void SendEmailMaterial::call(const std::string& email, const std::string& profile_name, const std::string& worksheet_ids, const std::function<void(bool is_success, std::string msg)> & callback){
            auto api_call = new SendEmailMaterial(email, profile_name, worksheet_ids, callback);
            api_call->send();
        }
        
        SendEmailMaterial::SendEmailMaterial(const std::string& email, const std::string& profile_name, const std::string& worksheet_ids, const std::function<void(bool is_success, std::string msg)> & callback) : _callback(callback) {
            _request = new Base(api::PlatformType::NEW, "email/send-email-worksheet", {
                {"id_worksheet", worksheet_ids},
                {"email", email},
                {"name_profile", profile_name},
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
        
        void SendEmailMaterial::send() const {
            if (_request) {
                _request->send();
            }
        }
        
        void SendEmailMaterial::onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(true, message);
            }
            delete this;
        }
        
        void SendEmailMaterial::onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) {
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
