//
//  MJAPI_ForgotLicense.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#ifndef MJAPI_ForgotLicense_hpp
#define MJAPI_ForgotLicense_hpp

#include "MJRequest.h"

namespace mj { namespace api {
    class ForgotLicense : public RequestDelegate {
    public:
        static ForgotLicense * call(std::string info_user,std::function<void(bool is_success, std::string msg)> callback = nullptr);
        inline void setCallback(std::function<void(bool is_success, std::string msg)> callback) { _callback = callback; }
        
    private:
        void send();
        ForgotLicense(std::string info_user);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
    };
}}

#endif /* MJAPI_ForgotLicense_hpp */
