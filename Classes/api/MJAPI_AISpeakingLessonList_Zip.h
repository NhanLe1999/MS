//
//  MJAPI_AISpeakingItemsList_Zip.hpp
//  MJStory
//
//  Created by To TruongBX on 4/19/18.
//

#ifndef MJAPI_AISpeaking_Lesson_List_Zip_hpp
#define MJAPI_AISpeaking_Lesson_List_Zip_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"
#include"MJAPI_ActivitiesList_Zip.h"

namespace mj {
    namespace api {
        class MJAPIAISpeakingLessonListZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);

        private:
            void send();
            MJAPIAISpeakingLessonListZip();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}
#endif /* MJAPI_AISpeaking_Items_List_Zip_hpp */
