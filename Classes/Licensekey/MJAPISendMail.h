//
//  MJAPISendMail.hpp
//  MonkeyJunior
//
//  Created by Ha Van Hoi
//

#ifndef MJAPISendMail_hpp
#define MJAPISendMail_hpp

#include "MJRequest.h"

namespace mj {
    class MJAPISendMail : public RequestDelegate {
    public:
        static void call(std::string mail, std::string phone,std::function<void(bool is_success, std::string msg)> callback = nullptr);
        
    private:
        void send();
        MJAPISendMail(std::string mail, std::string phone);
        
        Request * _request = nullptr;
        std::function<void(bool is_success, std::string msg)> _callback;
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
        
    };
}
#endif /* MJAPISendMail_hpp */
