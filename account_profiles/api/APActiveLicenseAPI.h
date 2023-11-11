//
//  APActiveLicenseAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#ifndef APActiveLicenseAPI_hpp
#define APActiveLicenseAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class ActiveLicense : public RequestDelegate {
        public:
            static void call(const std::string & license, const std::function<void(bool, std::string)> & callback);
        private:
            ActiveLicense(const std::string & license, const std::function<void(bool, std::string)> & callback);
            
            void send() const;
        
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
            Base * _request = nullptr;
            std::function<void(bool, std::string)> _callback = nullptr;
        };
    }
}

#endif /* APActiveLicenseAPI_hpp */
