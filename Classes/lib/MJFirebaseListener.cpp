//
//  MJFirebaseListener.cpp
//  Azu7-mobile
//
//  Created by To Ngoc Linh on 6/7/17.
//

#include "MJFirebaseListener.h"
#include "MJDefault.h"
#include "MJPlatformConfig.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "MSInstallPromotion.h"
#include "MSEventLogger.h"

#define k_DEVICE_FCM_TOKEN "mj_firebase_push_token"

namespace mj {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void FirebaseMessagingHelper::OnMessage(const firebase::messaging::Message &message) {
        CCLOG("Receive message from: %s", message.from.c_str());
        CCLOG("Message ID: %s", message.message_id.c_str());
        CCLOG("Message Link: %s", message.link.c_str());
        auto userinfo = message.data;
        if (message.notification_opened && userinfo.find("action") != userinfo.end() && userinfo["action"] == "web") {
            if (userinfo.find("url") == userinfo.end()) {
                return;
            }
            
            std::string url = userinfo["url"];
            bool flag = false;
            
            if (userinfo.find("url_type") != userinfo.end() && userinfo["url_type"] == "group_facebook") {
                flag = true;
            }
            
            cocos2d::Director::getInstance()->getScheduler()->schedule([url, flag](float) {
                if (flag) {
                    ms::EventLogger::getInstance().setUserProperties("click_to_join_fb_group", "yes");
                    MJDEFAULT->setBoolForKey("user.clicked.to.join.group", true);
                }
                Application::getInstance()->openURL(url);
            }, Application::getInstance(), 0, 0, 1, false, "delay_noti_open_url");
        }
        
        if (message.notification_opened && userinfo.find("action") != userinfo.end() && userinfo["action"] == "install_promotion_popup") {
            Director::getInstance()->getScheduler()->schedule([](float) {
                auto is_loading = Director::getInstance()->getRunningScene()->getChildByName("loadview") != nullptr;
                if (is_loading) {
                    MJDEFAULT->setBoolForKey("key.force.show.install.promotion", true);
                } else {
                    auto popup = MSInstallPromotion::createView(true);
                    Director::getInstance()->getRunningScene()->addChild(popup);
                }
            }, Application::getInstance(), 0, 0, 1, false, "delay_open_popup_install_promotion");
        }
 
        if (message.notification_opened && userinfo.find("action") != userinfo.end() && userinfo["action"] == "update") {
            if (userinfo.find("version") == userinfo.end()) {
                return;
            }
            
            auto version = userinfo["version"];
            auto current_version = PlatformConfig::getInstance().getAppVersion();
            auto v_split = mj::helper::split(version, '.');
            auto v_current_split = mj::helper::split(current_version, '.');
            
            auto need_update = false;
            for (auto i = 0; i < std::min(v_split.size(), v_current_split.size()); ++i) {
                auto c_split = atoi(v_split[i].c_str());
                auto c_c_split = atoi(v_current_split[i].c_str());
                
                if (c_c_split < c_split) {
                    need_update = true;
                    break;
                }
            }
            
            if (need_update)
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                if(CONFIG_MANAGER->isVietnameseStories()){
                    cocos2d::Application::getInstance()->openURL(key_appstore_app_vn_url);
                }else{
                    cocos2d::Application::getInstance()->openURL(key_appstore_app_url);
                }
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            if(CONFIG_MANAGER->isVietnameseStories()){
                cocos2d::Application::getInstance()->openURL(key_app_store_app_vn_android);
            }else{
                cocos2d::Application::getInstance()->openURL(key_app_store_app_android);
            }
#else
            if(CONFIG_MANAGER->isVietnameseStories()){
                cocos2d::Application::getInstance()->openURL(key_app_store_app_vn_win32);
            }else{
                cocos2d::Application::getInstance()->openURL(key_app_store_app_win32);
            }
#endif
        }
    }
#endif
    
    void FirebaseMessagingHelper::OnTokenReceived(const char *token) {
        CCLOG("firebase token: %s", token);
        _token = std::string(token);

        auto saved_token = MJDEFAULT->getStringForKey(k_DEVICE_FCM_TOKEN);
        if (saved_token == _token) {
            CCLOG("Token not change, do not upload to server.");
            return;
        }

        CCLOG("Firebase Token: %s", token);
        mj::Request::Data data = {
            {std::string("device_token"), _token}
        };
        //auto request = new mj::Request(mj::Request::ActionType::REGISTER_PUSH_TOKEN, data, mj::Request::RequestType::POST);
      //  request->setDelegate(this);
        //request->send();
    }

    void FirebaseMessagingHelper::onRequestSuccess(int tag, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(k_DEVICE_FCM_TOKEN, _token);
        CCLOG("Firebase Token Uploaded.");

    }
    void FirebaseMessagingHelper::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        CCLOG("Firebase Token Cannot Upload. %s", message.c_str());
    }

    void FirebaseMessagingHelper::initFirebaseCloudMessaging() {
#if (!TARGET_OS_SIMULATOR)
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        auto app = ::firebase::App::GetInstance();
        auto env = cocos2d::JniHelper::getEnv();
        auto activity = cocos2d::JniHelper::getActivity();
        app = ::firebase::App::Create(::firebase::AppOptions(), env, activity);
        ::firebase::messaging::Initialize(*app, &mj::FirebaseMessagingHelper::getInstance());
        ::firebase::analytics::Initialize(*app);

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto app = ::firebase::App::Create(::firebase::AppOptions());
        ::firebase::messaging::Initialize(*app, &mj::FirebaseMessagingHelper::getInstance());
        ::firebase::analytics::Initialize(*app);
        
#endif
        
#endif
        
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsEvent(std::string event_name, std::string parameter_name, std::string parameter_value){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (parameter_name == "") {
            ::firebase::analytics::LogEvent(event_name.c_str());
        }else ::firebase::analytics::LogEvent(event_name.c_str(), parameter_name.c_str(), parameter_value.c_str());
        
#endif
        
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsEvent(std::string event_name, std::string parameter_name, int parameter_value_int){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if (parameter_name == "") {
            ::firebase::analytics::LogEvent(event_name.c_str());
        }else ::firebase::analytics::LogEvent(event_name.c_str(), parameter_name.c_str(), parameter_value_int);
        
#endif
        
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsParametersEvent(std::string event_name, cocos2d::ValueMap dict_parmeters){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto num_value = (int)dict_parmeters.size();
        ::firebase::analytics::Parameter *parameters = new ::firebase::analytics::Parameter[num_value];
        auto index = 0;
//        std::string event_name_convert = event_name;
        for (auto item: dict_parmeters) {
            parameters [index] = ::firebase::analytics::Parameter(item.first.c_str(), item.second.asString());
            index++;
//            event_name_convert += cocos2d::StringUtils::format("_%sis%s", item.first.c_str(),item.second.asString().c_str());
        }
        ::firebase::analytics::LogEvent(event_name.c_str(), parameters, sizeof(parameters) / sizeof(parameters[0]));
        delete[] parameters;
//        ::firebase::analytics::LogEvent(event_name_convert.c_str());
#endif
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsGameEvent(std::string event_name, cocos2d::ValueMap dict_parmeters){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//        auto num_value = (int)dict_parmeters.size();
//        ::firebase::analytics::Parameter *parameters = new ::firebase::analytics::Parameter[num_value];
//        parameters [0] = ::firebase::analytics::Parameter("event_detail", dict_parmeters["event_detail"].asString());
//        parameters [1] = ::firebase::analytics::Parameter("id_story", dict_parmeters["id_story"].asInt());
//        parameters [2] = ::firebase::analytics::Parameter("id_game", dict_parmeters["id_game"].asInt());
//        ::firebase::analytics::LogEvent(event_name.c_str(), parameters, sizeof(parameters) / sizeof(parameters[0]));
//        delete[] parameters;
        
        std::string event_name_convert = cocos2d::StringUtils::format("%s_%s_%d", event_name.c_str(), dict_parmeters["event_detail"].asString().c_str(), dict_parmeters["id_game"].asInt());
        ::firebase::analytics::LogEvent(event_name_convert.c_str());
#endif
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsScene(std::string scene_name, std::string scene_classes){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
        return;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        /*::firebase::analytics::SetCurrentScreen(scene_name.c_str(), scene_classes.c_str())*/;
#endif
    }
    
    void FirebaseMessagingHelper::firebaseAnalyticsSetDeviceID(std::string device_id){
#if (TARGET_OS_SIMULATOR)
        return;
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        ::firebase::analytics::SetUserId(device_id.c_str());
#endif
    }
}

