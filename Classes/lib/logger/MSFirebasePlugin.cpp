//
//  MSFirebasePlugin.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/14/17.
//


#include "MSFirebasePlugin.h"
#include "MMFirebaseDatabase.h"


//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)

//#endif

#include "cocos2d.h"

namespace ms {

namespace remoteConfig {
    void Firebase::fetDataRemoteConfigABTesting(std::function<void(std::string)> callbackCallABTesting, std::string type)
    {
        std::function<void()> callbackABTesting = [=] () {
            auto app = ::firebase::App::GetInstance();
            if (!app) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                app = ::firebase::App::Create(::firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    app = ::firebase::App::Create(::firebase::AppOptions());
#else
                auto json = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services.json");
            auto option = firebase::AppOptions::LoadFromJsonConfig(json.c_str());
            app = ::firebase::App::Create(* option);
#endif
            }

            auto remote_config = firebase::remote_config::RemoteConfig::GetInstance(app);
            auto cc = remote_config->GetKeys();

            auto configsetting = firebase::remote_config::ConfigSettings();
            remote_config->SetConfigSettings(configsetting);

            remote_config->FetchAndActivate().OnCompletion([=](const ::firebase::Future<bool>& future) {
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {

                    if(type == "key_is_landing")
                    {
                        std::string valueLanding = "landing";
                        std::string value = remote_config->GetString("key_is_landing");

                        if(value == "landing")
                        {
                            valueLanding = "landing";
                        }else{
                            valueLanding = "iap";
                        }
                        mm::RealtimeDatabase::getInstance().updatevalueUserLandingPage(valueLanding, [=] () {

                        });

                        if(callbackCallABTesting)
                        {
                            callbackCallABTesting(valueLanding);
                        }

                        return;
                    }

                    std::string isTrue = "true";
                    std::string f2p = remote_config->GetString("free_2_paid");

                    if(f2p == "")
                    {
                        isTrue = "false";
                    }
                    std::string defaultClassification = "receipt_2_items_day";
                    if(f2p == "1")
                    {
                        defaultClassification = "receipt_7_stories";
                    }else if(f2p == "2")
                    {
                        defaultClassification = "receipt_7_items";
                    }else if(f2p == "3")
                    {
                        defaultClassification = "receipt_2_items_day";
                    }
                    if(MJDEFAULT->getBoolForKey("key_push_event_AB_testing", false))
                    {
                        mm::RealtimeDatabase::getInstance().updateInformationEventNetworkError(5392196, f2p, defaultClassification, isTrue, isTrue, isTrue, isTrue);
                    }
                    if(callbackCallABTesting)
                    {
                        callbackCallABTesting(defaultClassification);
                    }
                });
                // handle completion
            });
        };

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI
        if(type == "key_is_landing" && MJDEFAULT->getStringForKey(key_country_code, "US") == "VN")
        {
            mm::RealtimeDatabase::getInstance().loadValueUserLandingpage(ap::Database::getInstance().getUserInfo()._id, [=] () {
                auto valueLandingPage = MJDEFAULT->getStringForKey("key_is_landingPage", "");
                if(valueLandingPage != "")
                {
                    if(callbackCallABTesting) callbackCallABTesting("");
                    return;
                }
                if(callbackABTesting) callbackABTesting();

            });
        }else{
#endif
            if(callbackABTesting) callbackABTesting();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI
        }
#endif
    }
}

namespace analytics {
//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    
#pragma mark - Zzz
    std::string Firebase::QR_DEEP_LINK = "";
    std::string Firebase::analyticsId = "";
    Firebase::Firebase() {
        auto app = ::firebase::App::GetInstance();
        if (!app) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            app = ::firebase::App::Create(::firebase::AppOptions(), cocos2d::JniHelper::getEnv(), cocos2d::JniHelper::getActivity());
#else
            auto json = cocos2d::FileUtils::getInstance()->getStringFromFile("google-services.json");
            auto option = firebase::AppOptions::LoadFromJsonConfig(json.c_str());
            app = ::firebase::App::Create(* option);
#endif
        }
        firebase::analytics::Initialize(* app);
        getAnalyticsId();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
        firebase::dynamic_links::Initialize(* app, new DynamicLinksListener());
#endif
        
        auto remote_config = firebase::remote_config::RemoteConfig::GetInstance(app);
        
    }

    void Firebase::getAnalyticsId() {
        firebase::analytics::GetAnalyticsInstanceId().OnCompletion([=](const ::firebase::Future<std::string>& future) {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                if (future.status() == firebase::kFutureStatusComplete) {
                    CCLOG("Analytics Instance ID %s", future.result()->c_str());
                    analyticsId = future.result()->c_str();
                } else {
                    CCLOG("ERROR: Failed to fetch Analytics Instance ID %s (%d)",
                               future.error_message(), future.error());
                }
            });
        });
    }
    void Firebase::setIdentifier(std::string identifier) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        firebase::analytics::SetUserId(identifier.c_str());
#endif//win32
    }
    
    void Firebase::setUserProperties(std::map<std::string, cocos2d::Value> props) {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&& CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
        for (auto p : props) {
            if (p.second.getType() != cocos2d::Value::Type::STRING) {
                CCLOG("only string is supported in user properties");
                continue;
            }
            firebase::analytics::SetUserProperty(p.first.c_str(), p.second.asString().c_str());
        }
#endif//win32
    }
    
    void Firebase::logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> parameters, bool doPushClevertap) {
        try
        {
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32&& CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
            if (name.empty()) {
                return;
            }
            
            if (parameters.empty()) {
                firebase::analytics::LogEvent(name.c_str());
                return;
            }
          
            std::vector<firebase::analytics::Parameter> param;

            auto counter = 0;
            for (auto& p : parameters) {

                auto type = p.second.getType();

                switch (type) {
                case cocos2d::Value::Type::STRING: {
                    auto value = p.second.asString();
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), value.empty()?"empty":value));
                    break;
                }
                case cocos2d::Value::Type::INTEGER:
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), p.second.asInt()));
                    break;

                case cocos2d::Value::Type::DOUBLE:
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), p.second.asDouble()));
                    break;

                case cocos2d::Value::Type::FLOAT:
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), p.second.asFloat()));
                    break;

                case cocos2d::Value::Type::UNSIGNED:
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), p.second.asInt()));
                    break;
                case cocos2d::Value::Type::BOOLEAN:
                    param.push_back(firebase::analytics::Parameter(p.first.c_str(), p.second.asBool()));
                    break;
                default:
                    break;
                }
            }
            
            const firebase::analytics::Parameter* parameter = param.data();
            
            size_t number_of_parameters = param.size();
            
            if(parameter!= nullptr) {
            
                firebase::analytics::LogEvent(name.c_str(), parameter, number_of_parameters);
                
            }
            
            if (doPushClevertap) {
                CleverTapManager::GetInstance()->pushEvent(name, parameters);
            }
#endif//win32
         
        }
        catch (const std::exception&)
        {

        }
    }
}}
