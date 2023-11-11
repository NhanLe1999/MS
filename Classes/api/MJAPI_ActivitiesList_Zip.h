//
//  MJAPI_Story_Zip.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPI_ActivitiesList_Zip_hpp
#define MJAPI_ActivitiesList_Zip_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"

namespace mj {
    namespace api {
        enum ActivitiesType
        {
            LESSON,
            AI_SPEAKING_LESSON,
        };
        class ActivitiesListZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(ActivitiesType type, std::function<void(bool)> callback = nullptr);

        private:
            void send();
            ActivitiesListZip(ActivitiesType type);

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}
#endif /* MJAPI_ActivitiesList_Zip_hpp */
