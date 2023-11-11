//
//  APSendOptAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APSendOptAPI_hpp
#define APSendOptAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class SendOpt final : public RequestDelegate {
        public:
            static void call(
                             const std::string & phone,
                             const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
            // AUTHOR: Hieu Nguyen
            static void call(
                             bool forEmail,
                             const std::string & phone,
                             const std::string & email,
                             const std::function<void(bool is_success, std::string msg)> & callback = nullptr,
                             api::PlatformType type = api::PlatformType::NEW);
            // -- end --
            
        private:
            void send() const;
            
            SendOpt(
                  const std::string & phone,
                  const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
            // AUTHOR: Hieu Nguyen
            SendOpt(
                    bool forEmail,
                    const std::string & phone,
                    const std::string & email,
                    const std::function<void(bool is_success, std::string msg)> & callback = nullptr,
                    api::PlatformType type = api::PlatformType::NEW);
            // -- end --
            
			const std::string _phone;
            std::function<void(bool is_success, std::string msg)> _callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
        };
    }
}
#endif /* APSendOptAPI_hpp */
