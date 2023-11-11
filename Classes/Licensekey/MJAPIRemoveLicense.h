//
//  MJAPIRemoveLicense.hpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi
//

#ifndef MJAPIRemoveLicense_hpp
#define MJAPIRemoveLicense_hpp

#include "MJRequest.h"

namespace mj {
    class MJAPIRemoveLicense : public RequestDelegate {
    public:
        static void call(std::string device_id, std::string license_key, std::string device_id_remove, std::function<void(bool is_success, std::string msg)> callback = nullptr);
        
    private:
        void send();
        MJAPIRemoveLicense(std::string device_id, std::string license_key, std::string device_id_remove);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        std::string device_id_remove;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
    };
}
#endif /* MJAPIRemoveLicense_hpp */
