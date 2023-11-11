//
//  MJFirebaseListener.hpp
//  Azu7-mobile
//
//  Created by To Ngoc Linh on 6/7/17.
//

#ifndef MJFirebaseListener_hpp
#define MJFirebaseListener_hpp

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "firebase/messaging.h"
#include "firebase/analytics.h"
#endif
#endif
#include "MJRequest.h"

#define analytics_event_read_story "read_story"
#define analytics_event_close_story "close_story"
#define analytics_event_game "game_event"
#define analytics_event_play_game "play_game"
#define analytics_event_close_game "close_game"
#define analytics_event_done_game "done_game"
#define analytics_event_skip_game "skip_game_story"
#define analytics_event_rate_story "rate_story"
#define analytics_event_playstory_type "playstory_type"
#define analytics_event_download_story "download_story"
#define analytics_event_download_story_success "download_story_success"
#define analytics_event_download_story_fail "download_story_fail"
#define analytics_event_read_category "read_category"
#define analytics_event_close_category "close_category"
#define analytics_event_welcomepopup "welcomepopup"
#define analytics_event_welcomepopup_free "welcomepopup_free"
#define analytics_event_welcomepopup_subscribe "welcomepopup_subscribe"
#define analytics_event_story_interactive "story_interactive"

namespace mj {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    class FirebaseMessagingHelper : public firebase::messaging::Listener, public mj::RequestDelegate {
//    class FirebaseMessagingHelper : public mj::RequestDelegate {
    public:
        static FirebaseMessagingHelper & getInstance() {
            static FirebaseMessagingHelper firebase_messaging_instance;
            return firebase_messaging_instance;
        };
        /// Called on the client when a message arrives.
        ///
        /// @param[in] message The data describing this message.
        void OnMessage(const firebase::messaging::Message& message) override;
        
        /// Called on the client when a registration token arrives. This function
        /// will eventually be called in response to a call to
        /// firebase::messaging::Initialize(...).
        ///
        /// @param[in] token The registration token.
        void OnTokenReceived(const char* token) override;
        
        void initFirebaseCloudMessaging();
        void firebaseAnalyticsEvent(std::string event_name, std::string parameter_name = "", std::string parameter_value = "");
        void firebaseAnalyticsEvent(std::string event_name, std::string parameter_name = "", int parameter_value_int = 0);
        void firebaseAnalyticsParametersEvent(std::string event_name, cocos2d::ValueMap dict_parmeters);
        void firebaseAnalyticsScene(std::string scene_name, std::string scene_classes);
        void firebaseAnalyticsSetDeviceID(std::string device_id);
        void firebaseAnalyticsGameEvent(std::string event_name, cocos2d::ValueMap dict_parmeters);
    protected:
        explicit FirebaseMessagingHelper() {};
        
    private:
        FirebaseMessagingHelper(FirebaseMessagingHelper const &);
        void operator = (FirebaseMessagingHelper const &);
        
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
        std::string _token;
    };
    
#else
    class FirebaseMessagingHelper : public mj::RequestDelegate {
    public:
        static FirebaseMessagingHelper & getInstance() {
            static FirebaseMessagingHelper firebase_messaging_instance;
            return firebase_messaging_instance;
        };
        /// Called on the client when a registration token arrives. This function
        /// will eventually be called in response to a call to
        /// firebase::messaging::Initialize(...).
        ///
        /// @param[in] token The registration token.
        void OnTokenReceived(const char* token);
        
        void initFirebaseCloudMessaging();
        void firebaseAnalyticsEvent(std::string event_name, std::string parameter_name = "", std::string parameter_value = "");
        void firebaseAnalyticsEvent(std::string event_name, std::string parameter_name, int parameter_value_int);
        void firebaseAnalyticsParametersEvent(std::string event_name, cocos2d::ValueMap dict_parmeters);
        void firebaseAnalyticsScene(std::string scene_name, std::string scene_classes);
        void firebaseAnalyticsSetDeviceID(std::string device_id);
        void firebaseAnalyticsGameEvent(std::string event_name, cocos2d::ValueMap dict_parmeters);

    protected:
        explicit FirebaseMessagingHelper() {};
        
    private:
        void onRequestSuccess(int tag, rapidjson::Value data) override;
        void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) override;
        
        std::string _token;
    };
#endif
    
}

#endif /* MJFirebaseListener_hpp */
