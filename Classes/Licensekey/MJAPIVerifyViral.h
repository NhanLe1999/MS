//
//  MJAPIVerifyViral.hpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi
//

#ifndef MJAPIVerifyViral_hpp
#define MJAPIVerifyViral_hpp

#include "MJRequest.h"

namespace mj {
    class MJAPIVerifyViral : public RequestDelegate {
    public:
        static void call(std::string phone, std::function<void(bool is_success, std::string msg)> callback = nullptr);
        
    private:
        void send();
        MJAPIVerifyViral(std::string phone);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
        
    };
}
#endif /* MJAPIVerifyViral_hpp */
