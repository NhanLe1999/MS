//
//  StoryParentNotificationItem_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#include "StoryParentNotification_Landscape.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "HSLib.h"
#include "CleverTapManager.h"

INITIALIZE_READER_INSTANCE(StoryParentNotificationItem_LandscapeReader);

StoryParentNotificationItem_Landscape * StoryParentNotificationItem_Landscape::createItem(StoryNotificationInfo info)
{
    auto csb_name = XHSLib->checkMessageUnRead(info.index) ? "csb/StoryParentNotificationItemNew_Landscape.csb" : "csb/StoryParentNotificationItem_Landscape.csb";
    
    auto p = static_cast<StoryParentNotificationItem_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    
    if (p)
    {
        p->didLoadFromCSB(info);
        return p;
    }

    CC_SAFE_DELETE(p);
    
    return nullptr;
}

void StoryParentNotificationItem_Landscape::didLoadFromCSB(StoryNotificationInfo info) 
{
	m_info = info;
    pushTrackingEventViewAndClickAppInbox(0);
    auto background     = static_cast<cocos2d::ui::ImageView *>(cocos2d::ui::Helper::seekWidgetByName(this, "background"));
    auto text_title     = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "title"));
    auto text_message   = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "message"));
    auto text_date      = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "date"));
    auto title_btopen   = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "title_btopen"));
        
    LANGUAGE_MANAGER->setFontByLang({
        text_title
    }, StoryLanguageManager::TextType::BOLD);
    
    LANGUAGE_MANAGER->setFontByLang({
        title_btopen,
        text_message,
    }, StoryLanguageManager::TextType::REGULAR);

    title_btopen->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "noti_open"));
    
    
    if (info.message.find("|") != std::string::npos) {
        
        auto split = mj::helper::split(info.message, '|');
        
        info.message = split[0];
        
        if (split.size() > 1)
        {
            title_btopen->setString(split[1]);
        }
    }

    text_title->setString(info.title);
    text_message->setString(info.message);
    text_date->setString(mj::helper::epochTimeToText(info.time_epoch));
    
    if(LANGUAGE_MANAGER->getDisplayLangId() != 1)
    {
        text_date->setString(LTDate(info.time_epoch).toPrettyString());
    }
    else
    {
        text_date->setString(LTDate(info.time_epoch).toPrettyStringGB());
    }

    text_title->ignoreContentAdaptWithSize(false);
    text_title->setTextAreaSize(cocos2d::Size(text_title->getContentSize().width, 0));
    text_title->setContentSize(text_title->getVirtualRendererSize());

    text_message->ignoreContentAdaptWithSize(false);
    text_message->setTextAreaSize(cocos2d::Size(text_message->getContentSize().width, 0));
    text_message->setContentSize(text_message->getVirtualRendererSize());

    auto height = text_message->getContentSize().height + text_title->getContentSize().height + text_date->getContentSize().height + 30 + 10 + 50;
    this->setContentSize(cocos2d::Size(this->getContentSize().width, height));

    auto top_position = height - 30;

    text_title->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE_TOP);
    text_title->setPosition(cocos2d::Point(background->getContentSize().width / 2, top_position));
    top_position -= (text_title->getContentSize().height + 25);

    text_message->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE_TOP);
    text_message->setPosition(cocos2d::Point(background->getContentSize().width / 2, top_position));
    top_position -= (text_message->getContentSize().height + 25);

    text_date->setAnchorPoint(cocos2d::Point::ANCHOR_TOP_LEFT);
    text_date->setPosition(cocos2d::Point(25, top_position));
    top_position -= (text_date->getContentSize().height + 10);
    
    url = info.url;
    
    if (url == "")
    {
        static_cast<cocos2d::ui::Button *>(cocos2d::ui::Helper::seekWidgetByName(this, "Button_1"))->setVisible(false);//bt_open
    }
    
    cocos2d::ui::Helper::doLayout(this);
}

void StoryParentNotificationItem_Landscape::onOpenUrl(cocos2d::Ref *sender){

	if (m_delegateParent != nullptr) {
		m_delegateParent->onNotificationItemClicked(m_info);
	}
    pushTrackingEventViewAndClickAppInbox(1);
    if(!url.empty()) {
        Application::getInstance()->openURL(url);
    }
}

void StoryParentNotificationItem_Landscape::onClickBackground(cocos2d::Ref *sender){

    onOpenUrl(sender);
}

void StoryParentNotificationItem_Landscape::pushTrackingEventViewAndClickAppInbox(int type)
{
    if(m_info.type == StoryNotificationInfo::TYPE_NOTIFICATION::APPINBOX)
    {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.AppActivity", "pushInboxNotificationViewedEvent", m_info.id, type);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        CleverTapManager::GetInstance()->pushInboxNotificationViewedEvent(m_info.id, type);
#endif
    }
}

Widget::ccWidgetClickCallback StoryParentNotificationItem_Landscape::onLocateClickCallback(const std::string &callback_name)
{
    std::map<std::string, Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(StoryParentNotificationItem_Landscape, onOpenUrl),
        CLICK_MAP(StoryParentNotificationItem_Landscape, onClickBackground),
    };
    
    if (function_map.count(callback_name) > 0)
    {
        return function_map[callback_name];
    }
    
    return nullptr;
}
