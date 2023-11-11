//
//  APUpdateEmailMaterialAPI.h
//  MJStory
//
//  Created by Hieu Nguyen on 11/17/20.
//

#ifndef APUpdateEmailMaterialAPI_h
#define APUpdateEmailMaterialAPI_h

#include <stdio.h>
#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
        class UpdateEmailMaterial final : public RequestDelegate {
        public:
            // MARK: - Static Call
            static void call(const std::string& email, const std::string& user_id, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
        private:
            // MARK: - Contructor & Send
            UpdateEmailMaterial(const std::string& email, const std::string& user_id, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
            void send() const;
            
            // MARK: - onSuccess & onError
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            // MARK: - Properties
            Base * _request = nullptr;
            std::function<void(bool is_success, std::string msg)> _callback;
        };
    }
}

#endif /* APUpdateEmailMaterialAPI_h */
