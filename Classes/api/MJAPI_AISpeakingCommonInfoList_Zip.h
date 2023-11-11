//
//  MJAPI_AISpeakingTopicList_Zip_hpp.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPI_AISpeaking_Common_List_Zip_hpp
#define MJAPI_AISpeaking_Common_List_Zip_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"
#include "MJAPI_FirstInstall_Thumb_Zip.h"

namespace mj {
    namespace api {
        class MJAPIAISpeakingCommonListZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);
            static void GetInstance();
        private:
            void send();
            MJAPIAISpeakingCommonListZip();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;
            //void onDownloadVideoCommons(std::vector<mj::model::TalkingCommonInfo> talkingCommonInfos);

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}
#endif /* MJAPI_AISpeaking_Common_List_Zip_hpp */
