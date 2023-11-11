//
//  MJAPI_Story_Zip.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPI_Story_Zip_hpp
#define MJAPI_Story_Zip_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"
#include "MJStoryInfo.h"

namespace mj {
    namespace api {
        struct HomeResponse {
            std::vector<std::pair<int, std::string>> features;
            std::vector<mj::model::CharacterInfo> characters;
            std::vector<mj::model::CategoryInfo> categories;
            std::vector<int> new_stories;
            std::vector<int> popular_stories;
        };
        class StoryZip : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(std::function<void(bool)> callback = nullptr);
        private:
            void send();
            StoryZip();

            ap::api::Base* _request = nullptr;
            std::function<void(bool)> _callback;
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;
        };

    }
}
#endif /* MJAPI_Story_Zip_hpp */
