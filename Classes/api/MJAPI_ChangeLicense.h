//
//  MJAPI_ChangeLicense.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/1/18.
//

#ifndef MJAPI_ChangeLicense_hpp
#define MJAPI_ChangeLicense_hpp

#include "MJRequest.h"

namespace mj { namespace api {
    class ChangeLicense : public RequestDelegate {
    public:
        enum class Type {
            PHONE,
            MAIL
        };
        static ChangeLicense * call(Type type, std::string license, std::function<void(bool is_success, std::string msg)> callback = nullptr);
        inline void setCallback(std::function<void(bool is_success, std::string msg)> callback) { _callback = callback; }
        
    private:
        void send();
        ChangeLicense(Type type, std::string license);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
    };
}}
#endif /* MJAPI_ChangeLicense_hpp */
