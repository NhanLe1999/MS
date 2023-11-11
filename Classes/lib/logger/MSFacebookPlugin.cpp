//
//  MSFacebookPlugin.cpp
//  MonkeyJunior
//
//  Created by To Ngoc Linh on 9/28/18.
//

#include "MSFacebookPlugin.h"

#include "platform/CCPlatformConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"

#define ANDROID_ONLY (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni/JniHelper.h"
#include <jni.h>
#endif

namespace ms { namespace analytics {

    Facebook::Facebook() {
#if ANDROID_ONLY
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.FacebookPlugin", "init");
#endif
    }

    void Facebook::setIdentifier(std::string identifier) {
#if ANDROID_ONLY
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.FacebookPlugin", "setIdentifier", identifier);
#endif
    }

    void Facebook::logEvent(std::string name, std::vector<std::pair<std::string, cocos2d::Value>> parameters, bool doPushClevertap) {
#if ANDROID_ONLY
        rapidjson::Document out;
        rapidjson::Value o(rapidjson::kArrayType);
        rapidjson::Document::AllocatorType & allocator = out.GetAllocator();

        rapidjson::Value f(rapidjson::kObjectType);

        for (auto p : parameters) {
            auto type = p.second.getType();
            rapidjson::Value k;
            k.SetString(rapidjson::StringRef(p.first.c_str()));

            rapidjson::Value v;

            switch (type) {
                case cocos2d::Value::Type::STRING:
                    v.SetString(rapidjson::StringRef(p.second.asString().c_str()));
                    break;

                case cocos2d::Value::Type::INTEGER:
                    v = rapidjson::Value(p.second.asInt());
                    break;

                case cocos2d::Value::Type::DOUBLE:
                    v = rapidjson::Value(p.second.asDouble());
                    break;

                case cocos2d::Value::Type::FLOAT:
                    v = rapidjson::Value(p.second.asDouble());
                    break;

                case cocos2d::Value::Type::UNSIGNED:
                    v = rapidjson::Value(p.second.asUnsignedInt());
                    break;

                default:
                    assert(false && "type is not supported by analytics");
                    break;
            }

            f.AddMember(k, v, allocator);

        }

        rapidjson::StringBuffer rj_buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
        f.Accept(writer);
        //CCLOG("%s: converted \n %s", __PRETTY_FUNCTION__, rj_buffer.GetString());
        auto json_string = rj_buffer.GetString();
        CCLOG("converted \n %s", rj_buffer.GetString());

        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.FacebookPlugin", "logEvent", name, json_string);
#endif

    }

    void Facebook::setUserProperties(std::map<std::string, cocos2d::Value> parameters) {
#if ANDROID_ONLY
        rapidjson::Document out;
        rapidjson::Value o(rapidjson::kArrayType);
        rapidjson::Document::AllocatorType & allocator = out.GetAllocator();

        rapidjson::Value f(rapidjson::kObjectType);

        for (auto p : parameters) {
            auto type = p.second.getType();
            rapidjson::Value k;
            k.SetString(rapidjson::StringRef(p.first.c_str()));

            rapidjson::Value v;

            switch (type) {
                case cocos2d::Value::Type::STRING:
                    v.SetString(rapidjson::StringRef(p.second.asString().c_str()));
                    break;

                case cocos2d::Value::Type::INTEGER:
                    v = rapidjson::Value(p.second.asInt());
                    break;

                case cocos2d::Value::Type::DOUBLE:
                    v = rapidjson::Value(p.second.asDouble());
                    break;

                case cocos2d::Value::Type::FLOAT:
                    v = rapidjson::Value(p.second.asDouble());
                    break;

                case cocos2d::Value::Type::UNSIGNED:
                    v = rapidjson::Value(p.second.asUnsignedInt());
                    break;

                default:
                    assert(false && "type is not supported by analytics");
                    break;
            }

            f.AddMember(k, v, allocator);

        }

        rapidjson::StringBuffer rj_buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
        f.Accept(writer);
        //CCLOG("%s: converted \n %s", __PRETTY_FUNCTION__, rj_buffer.GetString());
        auto json_string = rj_buffer.GetString();
        CCLOG("converted \n %s", rj_buffer.GetString());

        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.FacebookPlugin", "setUserProperties", json_string);
#endif
    }
}}
