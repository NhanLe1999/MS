//
//  APLogoutAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#ifndef APLogoutAPI_hpp
#define APLogoutAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class Logout : public RequestDelegate {
        public:
            static void call(const std::function<void(bool, std::string)> & callback);
        private:
            Logout(const std::function<void(bool, std::string)> & callback);
            
            void send() const;
        
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            std::function<void(bool, std::string)> _callback;
            Base * _request = nullptr;
        };
    }
}

#endif /* APLogoutAPI_hpp */
