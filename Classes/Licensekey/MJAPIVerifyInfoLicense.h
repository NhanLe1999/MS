//
//  MJAPIVerifyInfoLicense.hpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi
//

#ifndef MJAPIVerifyInfoLicense_hpp
#define MJAPIVerifyInfoLicense_hpp

#include "MJRequest.h"

namespace mj {
    class MJAPIVerifyInfoLicense : public RequestDelegate {
    public:
        static void call(std::string info_user, std::string license_key, std::function<void(bool is_success, std::string msg)> callback = nullptr);
        
    private:
        void send();
        MJAPIVerifyInfoLicense(std::string info_user, std::string license_key);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
    };
}
#endif /* MJAPIVerifyInfoLicense_hpp */
