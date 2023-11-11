//
//  APStoreErrorAPI.hpp
//  stories_debug
//
//  Created by AnhBNG on 8/26/2019
//

#ifndef APStoreErrorAPI_hpp
#define APStoreErrorAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class StoreError final : public RequestDelegate {
        public:
            static void call(std::string url, std::string err_msg, std::string action_name, std::string body_request);
            
        private:
            void send() const;
            
            StoreError(std::string url, std::string err_msg, std::string action_name, std::string body_request);
            
            std::function<void(bool is_success, std::string msg)> _callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;

            Base * _request = nullptr;
        };
    }
}
#endif /* APStoreErrorAPI_hpp */
