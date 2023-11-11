//
//  MJAPIRegisterDevice.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/7/17.
//

#ifndef MJAPIRegisterDevice_hpp
#define MJAPIRegisterDevice_hpp
#include "MJRequest.h"
#include "APBaseAPI.h"

namespace mj {
        class RegisterAPI : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);

        private:
            void send();
            RegisterAPI();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
            void DecodeLocationAndDeviceInfo(int tag, rapidjson::Document* document);
        };
}

#endif /* MJAPIRegisterDevice_hpp */
