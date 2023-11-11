
#include "AppInbox.h"
#include "AppDelegate.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"

#include "MJPlatformConfig.h"
#include "StoryParentNotification_Landscape.h"
#include "CleverTapManager.h"

USING_NS_CC;

namespace mj {
    namespace appinbox {
        StoryNotificationInfo setValueObject(std::string value)
        {
            StoryNotificationInfo info;
            if (value.empty()) {
                return info;
            }

            rapidjson::Document doc;

            doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)value.c_str());

            if (doc.HasParseError()) {
                return info;
            }

            if (!doc.IsObject()) {
                return info;
            }

            if(JSON_SAFE_CHECK(doc, "id", String))
            {
                std::string id = doc["id"].GetString();
                info.id = id;
            }

            if(JSON_SAFE_CHECK(doc, "msg", Object))
            {
              auto msg = doc["msg"].GetObject();

              if(JSON_SAFE_CHECK(msg, "content", Array))
              {
                  auto content = msg["content"].GetArray();
                    if(content.Size() > 0)
                    {
                        if(content[0].IsObject())
                        {
                            auto inforMessage = content[0].GetObject();

                            if(JSON_SAFE_CHECK(inforMessage, "message", Object))
                            {
                                auto message = inforMessage["message"].GetObject();
                                if(JSON_SAFE_CHECK(message, "text", String))
                                {
                                    std::string text = message["text"].GetString();
                                    info.message = text;
                                }
                            }

                            if(JSON_SAFE_CHECK(inforMessage, "title", Object))
                            {
                                auto title = inforMessage["title"].GetObject();
                                if(JSON_SAFE_CHECK(title, "text", String))
                                {
                                    std::string text = title["text"].GetString();
                                    info.title = text;
                                }
                            }

                            if(JSON_SAFE_CHECK(inforMessage, "action", Object))
                            {
                                auto action = inforMessage["action"].GetObject();
                                if(JSON_SAFE_CHECK(action, "url", Object))
                                {
                                    auto url = action["url"].GetObject();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                                    if(JSON_SAFE_CHECK(url, "android", Object))
                                    {
                                        auto android = url["android"].GetObject();
                                        if(JSON_SAFE_CHECK(android, "text", String))
                                        {
                                            auto linkurl = android["text"].GetString();
                                            info.url = linkurl;
                                        }
                                    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
                                    if(JSON_SAFE_CHECK(url, "ios", Object))
                                    {
                                        auto ios = url["ios"].GetObject();
                                        if(JSON_SAFE_CHECK(ios, "text", String))
                                        {
                                            auto linkurl = ios["text"].GetString();
                                            info.url = linkurl;
                                        }
                                    }
#endif
                                }
                            }

                        }
                    }
              }

            }

            if(JSON_SAFE_CHECK(doc, "isRead", Bool))
            {
                info.unread = doc["isRead"].GetBool();
            }

            if(JSON_SAFE_CHECK(doc, "date", Int))
            {
                info.time_epoch = doc["date"].GetInt();
            }

            return info;
        }

        std::vector<StoryNotificationInfo> getMessageAppinbox()
        {
            std::vector<StoryNotificationInfo> message;
            std::vector<std::string> value;
            if (auto delegate = (AppDelegate *)cocos2d::Application::getInstance()) {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
                value = delegate->getDataAppinbox();
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
                value = CleverTapManager::GetInstance()->getValueDateAppInbox();
#endif
                for(int i = 0; i < value.size(); i++)
                {
                    CCLOG("%s_____", value[i].c_str());
                    StoryNotificationInfo info = mj::appinbox::setValueObject(value[i]);
                    info.type = StoryNotificationInfo::TYPE_NOTIFICATION::APPINBOX;
                    message.push_back(info);
                }

                int i = 0;

                while (i < message.size() && value.size() != 0)
                {
                    for(int k = i + 1; k < message.size(); k++)
                    {
                        if(message[i].title == message[k].title && message[i].message == message[k].message)
                        {
                            message.erase(message.begin() + k);
                            k--;
                        }
                    }
                    i++;
                }
            }
            return message;
        }
    }
}

