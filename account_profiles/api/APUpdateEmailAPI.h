//
//  APUpdateEmailAPI.h
//  MJStory
//
//  Created by Hieu Nguyen on 7/20/20.
//

#ifndef APUpdateEmailAPI_h
#define APUpdateEmailAPI_h

#include <stdio.h>
#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
        class UpdateEmail final : public RequestDelegate {
        public:
            // MARK: - Static Call
            static void call(const std::string& email, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
        private:
            // MARK: - Contructor & Send
            UpdateEmail(const std::string& email, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
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

#endif /* APUpdateEmailAPI_h */
