//
//  APVerifyOptAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APVerifyOptAPI_hpp
#define APVerifyOptAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class VerifyOpt final : public RequestDelegate {
        public:
            static void call(
							 const std::string & otp,
                             const std::function<void(bool is_success, std::string msg,std::string condition )> & callback = nullptr);
            
        private:
            void send() const;
            
            VerifyOpt(
				  const std::string & otp,
                  const std::function<void(bool is_success, std::string msg, std::string condition)> & callback = nullptr);
            
			const std::string _otp;
            std::function<void(bool is_success, std::string msg, std::string condition)> _callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
        };
    }
}
#endif /* APVerifyOptAPI_hpp */
