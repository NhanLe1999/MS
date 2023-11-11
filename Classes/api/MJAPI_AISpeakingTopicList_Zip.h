//
//  MJAPI_AISpeakingTopicList_Zip_hpp.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPI_AISpeaking_Topic_List_Zip_hpp
#define MJAPI_AISpeaking_Topic_List_Zip_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"
#include "MJAPI_Download_MultiFile.h"

namespace mj {
    namespace api {
        class MJAPIAISpeakingTopicListZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);

        private:
            void send();
            MJAPIAISpeakingTopicListZip();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onDownloadIconCategorys(std::vector<mj::model::AISpeakingCategoryInfo> aISpeakingCategoryInfo);
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            bool IsTopicThumbExits(mj::model::AISpeakingCategoryIconInfo aISpeakingCategoryIconInfo);
            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}
#endif /* MJAPI_AISpeaking_Topic_List_Zip_hpp */
