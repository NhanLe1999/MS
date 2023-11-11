//
//  MJAPILicense.hpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi
//

#ifndef MJAPILicense_hpp
#define MJAPILicense_hpp

#include "MJRequest.h"

namespace mj {
    class MJAPILicense : public RequestDelegate {
    public:
        static void call(std::string license_key, bool confirm, std::function<void(bool,std::string,rapidjson::Value,int)> callback = nullptr);
        
    private:
        void send();
        MJAPILicense(std::string license_key, bool confirm);
        
        Request * _request = nullptr;
        std::function<void(bool,std::string,rapidjson::Value,int)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPILicense_hpp */
