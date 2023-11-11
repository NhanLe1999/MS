//
//  StoryParentNotificationItem_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#ifndef StoryParentNotificationItem_Landscape_hpp
#define StoryParentNotificationItem_Landscape_hpp

#include "cocostudiomacro.h"
#include "../external/json/document.h"
#include "ui/CocosGUI.h"
#include "StoryLanguageManager.h"

using namespace cocos2d;
using namespace ui;

class StoryParentNotification_Landscape;

struct StoryNotificationInfo
{
    enum TYPE_NOTIFICATION
    {
        APPINBOX = 0,
        DEFAULT = 1
    };
    TYPE_NOTIFICATION type;
    std::string message;
    std::string title;
    std::string url;
    long time_epoch;
    int index;
    bool unread;
    std::string id;

    StoryNotificationInfo(): message(""), title(""), url(""), index(0), time_epoch(0), unread(true), id(""), type(TYPE_NOTIFICATION::DEFAULT)
    {

    }
    
    StoryNotificationInfo(rapidjson::Value object)
    {
        auto notification_object = object.GetObject();
        
        const std::string k_title = StringUtils::format("title_%s",((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en")));
        
        const std::string k_message = StringUtils::format("content_%s",((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en")));
        
        title = std::string(notification_object[k_title.c_str()].GetString());
        message = std::string(notification_object[k_message.c_str()].GetString());
        url = std::string(notification_object["url"].GetString());
        time_epoch = std::atol(notification_object["dateinsert"].GetString());
        index = notification_object["id"].GetInt();
        // TODO: get read state from db
        unread = false;
    }
    
    StoryNotificationInfo(ValueMap notification_object)
    {
        const std::string k_title = StringUtils::format("title_%s",((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en")));
        
        const std::string k_message = StringUtils::format("content_%s",((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en")));
        
        title = std::string(notification_object[k_title.c_str()].asString());
        message = std::string(notification_object["content"].asString());
        url = std::string(notification_object["url"].asString());
        time_epoch = std::atol(notification_object["dateinsert"].asString().c_str());
        index = notification_object["id"].asInt();
        // TODO: get read state from db
        unread = false;
    }
};

class StoryParentNotificationItem_Landscape : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    static void initReader();
    CREATE_FUNC(StoryParentNotificationItem_Landscape);
    static StoryParentNotificationItem_Landscape * createItem(StoryNotificationInfo info);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
    void didLoadFromCSB(StoryNotificationInfo info);
    void onOpenUrl(cocos2d::Ref *sender);
    void onClickBackground(cocos2d::Ref *sender);
    void pushTrackingEventViewAndClickAppInbox(int type);
private:
    std::string url;
	StoryNotificationInfo m_info;
	CC_SYNTHESIZE(StoryParentNotification_Landscape *, m_delegateParent, DelegateParent);
};

CREATE_READER_CLASS(StoryParentNotificationItem_LandscapeReader, StoryParentNotificationItem_Landscape);

#endif /* StoryParentNotificationItem_Landscape_hpp */
