//
//  MJAPI_RemoveLicense.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/14/18.
//

#ifndef MJAPI_RemoveLicense_hpp
#define MJAPI_RemoveLicense_hpp

#include "MJRequest.h"

namespace mj { namespace api {
    class RemoveLicense : public RequestDelegate {
    public:
        static void call(std::function<void(bool is_success, std::string msg)> callback = nullptr);
        
    private:
        void send();
        RemoveLicense();
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}}

#endif /* MJAPI_RemoveLicense_hpp */
