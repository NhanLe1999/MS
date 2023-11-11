//
//  MJAPI_RegisterInstall.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/8/19.
//

#ifndef MJAPI_RegisterInstall_hpp
#define MJAPI_RegisterInstall_hpp

#include "MJRequest.h"
#include <string>

namespace mj { namespace api {
    class RegisterInstall : public RequestDelegate {
    public:
        static RegisterInstall * call(const std::string & name, const std::string & phone, const std::string mail, long long birthday, bool is_verified, std::function<void(bool, std::string)> callback);
        
        static RegisterInstall * confirmGift(const std::string & name, const std::string & phone, const std::string mail, std::function<void(bool, std::string)> callback);
        
    private:
        void send();
        RegisterInstall(const std::string & name, const std::string & phone, const std::string mail);
        RegisterInstall(const std::string & name, const std::string & phone, const std::string mail, long long birthday, bool is_verified);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
    };
}}
#endif /* MJAPI_ChangeLicense_hpp */
