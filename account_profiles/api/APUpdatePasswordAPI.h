//
//  APUpdatePasswordAPI.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/21/20.
//

#ifndef APUpdatePasswordAPI_h
#define APUpdatePasswordAPI_h

#include <stdio.h>
#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
        class UpdatePassword final : public RequestDelegate {
        public:
            // MARK: - Static Call
            static void call(const std::string& old_password, const std::string& new_password, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
			// MARK: change defaut password:
			static void callDefaulPasswordChange(const std::string& new_access_token, const std::string& password,const std::string & token, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

        private:
            // MARK: - Contructor & Send
            UpdatePassword(const std::string& old_password, const std::string& new_password, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
			// MARK: change defaut password:
			UpdatePassword(const std::string& new_access_token, const std::string& password,const std::string & old_token, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

            void send(std::string i_token = "") const;
            
            // MARK: - onSuccess & onError
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            // MARK: - Properties
            Base * _request = nullptr;
            std::function<void(bool is_success, std::string msg)> _callback;
        };
    }
}

#endif /* APUpdatePasswordAPI_h */
