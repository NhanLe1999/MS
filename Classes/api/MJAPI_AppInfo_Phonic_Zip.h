//
//  MJAPIZip.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPIZip_hpp
#define MJAPIZip_hpp

#include "MJRequest.h"

namespace mj {
    namespace api {
        class AppInfoPhonicZip : public mj::RequestDelegate {
        public:
            static void call(std::function<void(bool, int, int)> callback = nullptr);
            
        private:
            void send();
            AppInfoPhonicZip();
            
            Request * _request = nullptr;
            std::function<void(bool, int, int)> _callback;
            
            int onRequestRaw(int tag, std::string data) override;
            void onRequestSuccess(int tag, rapidjson::Value data) override;
            void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
            
            static bool shouldBeCalled();
        };
    }
}


#endif /* MJAPIZip_hpp */
