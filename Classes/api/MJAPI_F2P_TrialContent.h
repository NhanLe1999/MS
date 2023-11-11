//
//  MJAPI_Story_Zip.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#ifndef MJAPI_F2P_TrialContent_hpp
#define MJAPI_F2P_TrialContent_hpp

#include "MJRequest.h"
#include "APBaseAPI.h"
#include "MJStoryInfo.h"
#include "APProfileManager.h"
#include "APProfile.h"
#include "APDatabase.h"
namespace mj {
    namespace api {
        class MJAPI_F2P_TrialContent : public ap::api::RequestDelegate, public cocos2d::Ref {
        public:
            static void call(const ap::Profile& _profile, const std::function<void(bool)> callback = nullptr);
        private:
            void send();
            MJAPI_F2P_TrialContent(const ap::Profile profile);

            ap::api::Base* _request = nullptr;
            ap::Profile _profile;
            std::function<void(bool)> _callback;
            //ap::Profile _profile = ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id));
            bool onReceivedRawData(int tag, const std::string& data)override;
            void onRequestSuccess(int tag, int code, const std::string& message, const rapidjson::Value& data) override;

            void onRequestError(int tag, int error_code, const std::string& message, const rapidjson::Value& data) override;

        };

    }
}
#endif /* MJAPI_Story_Zip_hpp */
