//
//  MJAPI_Material_Zip.h
//  MJStory
//
//  Created by Hieu Nguyen on 11/17/20.
//

#ifndef MJAPI_Material_Zip_h
#define MJAPI_Material_Zip_h

#include "MJRequest.h"
#include "APBaseAPI.h"
#include "MJStoryInfo.h"

namespace mj {
    namespace api {
        class MaterialZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);
        private:
            void send();
            MaterialZip();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}

#endif /* MJAPI_Material_Zip_h */
