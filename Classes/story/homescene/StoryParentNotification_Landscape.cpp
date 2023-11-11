//
//  StoryParentNotification_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#include "StoryParentNotification_Landscape.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "StoryDataModel.h"
#include "MJDefault.h"
#include "../external/json/prettywriter.h"
#include "HSLib.h"
#include "StoryFreeManager.h"
#include "APDatabase.h"
#include "manager/CleverTapManager.h"
#include "MSInstallPromotion.h"
#include "MJPay.h"
#include "GroupPromoNotification.h"

#include "AppDelegate.h"
#include "AppInbox.h"
INITIALIZE_READER_INSTANCE(StoryParentNotification_LandscapeReader);

//
// Coding Convention
// Please follow here: https://docs.google.com/document/d/1w63J5DSRezBUxIktcyTvB4w90MhrtU-1FiXhTrq--TI/edit?usp=sharing
//

StoryParentNotification_Landscape * StoryParentNotification_Landscape::createPage()
{
	auto csb_name = mj::helper::getCSBName("csb/StoryParentNotification_Landscape.csb");
    
	auto p = static_cast<StoryParentNotification_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    
	if (p) 
	{
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

StoryParentNotification_Landscape::~StoryParentNotification_Landscape()
{
	//write your code here if you need
}

void StoryParentNotification_Landscape::didLoadFromCSB() 
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||  CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    _notifications_info = mj::appinbox::getMessageAppinbox();
#endif
    StoryParentSubPage_Landscape::didLoadFromCSB();
    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.hidenoti");
    auto listview = static_cast<cocos2d::ui::ListView *>(cocos2d::ui::Helper::seekWidgetByName(this, "listview_notification"));
    listview->setScrollBarEnabled(false);

//    _request = new mj::Request(mj::Request::ActionType::NOTIFICATION);
//    _request->setDelegate(this);
//    _request->send();
    
    
    ValueVector list_message = StoryDataModel::getInstance()->getAllMessage();
    ValueMap map_noti;
    map_noti["data"] = Value(list_message);
    auto buffer_string = JsonDump::getInstance()->MapToString(map_noti);
//    CCLOG("buffer_string = %s", buffer_string.c_str());
    rapidjson::Document doc;
    doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
    
    if (doc.HasParseError()) {
        return;
    }
    
    rapidjson::StringBuffer rj_buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
    doc.Accept(writer);
    
    if (doc.HasMember("data")) {
        if (doc["data"].IsArray()) {
            auto notifications = doc["data"].GetArray();
            for (auto &notification : notifications) {
                auto notification_object = notification.GetObject();
                StoryNotificationInfo info(notification_object);
                info.type = StoryNotificationInfo::TYPE_NOTIFICATION::DEFAULT;
                _notifications_info.push_back(info);
            }
            
            loadNotification();
        }
    }
    
    
}

void StoryParentNotification_Landscape::addMessageFreeReadStory()
{
    auto subscribe_type = (int) CONFIG_MANAGER->isAppActive();
        
    auto lang_display = MJDEFAULT->getIntegerForKey(key_lang_display);
    
    auto value_expire = MJDEFAULT->getIntegerForKey(key_freetime_expire);
    auto value_current = MJDEFAULT->getIntegerForKey(key_time_curent);
    value_current = std::max(value_current, int(utils::getTimeInMilliseconds() / 1000));
    
    auto time_expire = LTDate(value_expire);
    auto time_current = LTDate(value_current);
    auto value_rate_expire = MJDEFAULT->getIntegerForKey(rate_time_expire);
    if (value_current < value_rate_expire) {
        auto value_conlai = value_rate_expire - value_current;
        auto date_conlai = value_conlai/86400 + 1;
        CCLOG("value_conlai = %d", date_conlai);
        
        StoryNotificationInfo info;
        info.title = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.title");
        info.message = StringUtils::format("%s %d %s",LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content1").c_str(), date_conlai, LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content4").c_str());
        
        info.time_epoch = value_current;
        info.index = std::atoi(StringUtils::format("%d%d%d", time_current.year, time_current.month, time_current.day).c_str());
        _notifications_info.insert(_notifications_info.begin(), info);
        return;
    }
    
    if (subscribe_type && (!FREE_STORY_MANAGER->isFirst2Days())) return;
    
    if (value_current < value_rate_expire)
    {
        auto value_conlai = value_rate_expire - value_current;
        auto date_conlai = value_conlai/86400 + 1;
        CCLOG("value_conlai = %d", date_conlai);
        
        StoryNotificationInfo info;
        info.title = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.title");
        info.message = StringUtils::format("%s %d %s",LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content1").c_str(), date_conlai, LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content4").c_str());
        
        info.time_epoch = value_current;
        info.index = std::atoi(StringUtils::format("%d%d%d", time_current.year, time_current.month, time_current.day).c_str());
        _notifications_info.insert(_notifications_info.begin(), info);
        
    }
    else if (FREE_STORY_MANAGER->isFirst2Days())
    {
        auto value_conlai = time_expire-time_current;
        auto date_conlai = value_conlai/86400 + 1 - 5;
        CCLOG("value_conlai = %d", date_conlai);
        
        StoryNotificationInfo info;
        info.title = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.title");
        info.message = StringUtils::format("%s %d %s",LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content1").c_str(), date_conlai, LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content4").c_str());
        
        info.time_epoch = value_current;
        info.index = std::atoi(StringUtils::format("%d%d%d", time_current.year, time_current.month, time_current.day).c_str());
        _notifications_info.insert(_notifications_info.begin(), info);
    }
    else if (FREE_STORY_MANAGER->checkFreeTimeAvailable())
    {
        auto value_conlai = time_expire-time_current;
        auto date_conlai = value_conlai/86400 + 1;
        CCLOG("value_conlai = %d", date_conlai);
        
        StoryNotificationInfo info;
        info.title = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.title");
        info.message = StringUtils::format("%s %d %s",LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content1").c_str(), date_conlai, LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content2").c_str());
        
        info.time_epoch = value_current;
        info.index = std::atoi(StringUtils::format("%d%d%d", time_current.year, time_current.month, time_current.day).c_str());
        _notifications_info.insert(_notifications_info.begin(), info);
    }
    else if (!CONFIG_MANAGER->isAppActive())
    {
        StoryNotificationInfo info;
        info.title = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.title");
        info.message = LANGUAGE_MANAGER->getTextByKey(lang_display, "notifree.content3");

        info.time_epoch = value_current;
        info.index = std::atoi(StringUtils::format("%d%d%d", time_expire.year, time_expire.month, time_expire.day).c_str());
        _notifications_info.insert(_notifications_info.begin(), info);
    }
}

void StoryParentNotification_Landscape::getDataInAppInbox()
{

}

void StoryParentNotification_Landscape::onRequestSuccess(int tag, rapidjson::Value data)
{
    auto notifications = data.GetArray();
    
    for (auto &notification : notifications)
    {
        auto notification_object = notification.GetObject();
        StoryNotificationInfo info(notification_object);
        _notifications_info.push_back(info);
    }

    StoryNotificationInfo info;
    info.title = "This is a very long title of a normal notification. Should it be handled?";
    info.message = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor"
                    "incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation"
                    "ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit"
                    "in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat"
                    "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    info.time_epoch = 0;
    info.unread = true;
    _notifications_info.insert(_notifications_info.begin(), info);

    loadNotification();
}

void StoryParentNotification_Landscape::onRequestFailed(int tag, int error, std::string message, rapidjson::Value data) {
    
}


void StoryParentNotification_Landscape::loadNotification()
{
    addMessageFreeReadStory();

    auto listview = static_cast<cocos2d::ui::ListView *>(cocos2d::ui::Helper::seekWidgetByName(this, "listview_notification"));
    
    auto item = GroupPromoNotification::createView();
	item->setDelegateParent(this);
    
    if(CONFIG_MANAGER->isCountryVN() && LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)
    {
        listview->pushBackCustomItem(item);
    }
        
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        if (!mj::Pay::getInstance().isActive() && MJDEFAULT->getStringForKey("key.install.voucher").empty() && MSInstallPromotion::shouldShowNotification()) {
            auto item = MSInstallPromotion::createView();
            listview->pushBackCustomItem(item);
            utils::findChild(item, "btclose")->setVisible(false);
        }
    }

    ValueVector list_read;
    
    std::sort(_notifications_info.begin(),_notifications_info.end(),[](StoryNotificationInfo a,StoryNotificationInfo b){
        return a.time_epoch > b.time_epoch;
        
    });
    
    
    for (auto info:_notifications_info) {
        
        if(info.message.empty() && info.title.empty()) {
            continue;
        }
        
        auto item = StoryParentNotificationItem_Landscape::createItem(info);
		item->setDelegateParent(this);
        listview->pushBackCustomItem(item);
        list_read.push_back(Value(info.index));
    }
    
    XHSLib->addMessageRead(list_read);
}

void StoryParentNotification_Landscape::onNotificationItemClicked(StoryNotificationInfo i_info)
{
	m_messageTitle = i_info.title;

	pushCleverTapEvents();
}

void StoryParentNotification_Landscape::pushCleverTapEvents()
{
	try
	{
		mj::helper::removeCharsFromString(m_messageTitle, "\"");// remove special character "

		std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =
		{
		  {"view_screen", cocos2d::Value(true)},
		  {"message_name", cocos2d::Value(m_messageTitle)},
		};

		CleverTapManager::GetInstance()->pushEvent("click_inapp_message", eventTrackingProperties);
	}
	catch (const char * i_errorCode)
	{
		CCLOG("Error: cannot push story tracking data to server !");
	}
}
