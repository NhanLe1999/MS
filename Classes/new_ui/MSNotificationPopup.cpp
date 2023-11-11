//
//  MSNotificationPopup.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/26/18.
//

#include "MSNotificationPopup.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(MSNotificationPopup);

std::once_flag ms_notification_info_reader;
MSNotificationPopup * MSNotificationPopup::createPopup(mj::model::NotificationInfo info) {
    std::call_once(ms_notification_info_reader, [] {
        REGISTER_CSB_READER(MSNotificationPopup);
    });
    
    auto popup = reinterpret_cast<MSNotificationPopup *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/NotificationPopup.csb"));
    if (popup) {
        popup->didLoadFromCSB(info);
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

void MSNotificationPopup::show(mj::model::NotificationInfo info) {
    auto popup = MSNotificationPopup::createPopup(info);
    Director::getInstance()->getRunningScene()->addChild(popup, INT_MAX - 1);
}

void MSNotificationPopup::didLoadFromCSB(mj::model::NotificationInfo info)
{
    _info = info;
    
    const std::string k_lang = ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? "th" : ((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)?"vi":"en"));

    const std::string k_title           = info.m_titles.count(k_lang) ? info.m_titles.at(k_lang):"";
    const std::string k_message         = info.m_messages.count(k_lang) ? info.m_messages.at(k_lang):"";

    auto text_title = utils::findChild<ui::Text *>(this, "title");
    text_title->setString(k_title);
    mj::helper::setTextWidth(text_title, 700);
    
    auto split = mj::helper::split(k_message, '|');
    
    auto content = utils::findChild<ui::Text *>(this, "content");
    if (!split.empty()) {
        content->setString(split[0]);
    }
    
    if (split.size() == 2) {
        utils::findChild<ui::Text *>(this, "action")->setString(split[1]);
    } else {
        auto action = LANGUAGE_MANAGER->getDisplayLangId() == 1 ? "Open" : "Mở";
        utils::findChild<ui::Text *>(this, "action")->setString(action);
    }
    
    mj::helper::setTextWidth(content, 700);
    auto text_size = content->getContentSize();
    auto listview = utils::findChild(this, "listview");
    listview->setContentSize(Size(text_size.width, MIN(text_size.height + text_title->getContentSize().height + 20, 400)));
    utils::findChild(this, "background")->setContentSize(listview->getContentSize() + Size(100, 200));
    
    ui::Helper::doLayout(this);
}

void MSNotificationPopup::onEnter() {
    ui::Layout::onEnter();
    utils::findChild(this, "overlay")->runAction(Sequence::create(CallFuncN::create([](Node * n) {
        n->setOpacity(0);
    }), FadeIn::create(0.25), NULL));
    
    auto bg = utils::findChild(this, "background");
    bg->runAction(Sequence::create(CallFuncN::create([](Node * n) {
        n->setPosition(n->getPosition() + Point(0, 1000));
    }), EaseCubicActionOut::create(MoveBy::create(0.5, Point(0, -1000))), NULL));
    
    if (_info.url.empty()) {
        utils::findChild(this, "button_close")->setVisible(false);
    }
}

ui::Widget::ccWidgetClickCallback MSNotificationPopup::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSNotificationPopup, onClose),
        CLICK_MAP(MSNotificationPopup, onOpen)
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}


void MSNotificationPopup::onClose(cocos2d::Ref *sender) {
    MJDEFAULT->setBoolForKey(StringUtils::format("%s%d", key_popup_showed, _info.notification_id), true);
    MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
    utils::findChild(this, "overlay")->runAction(FadeOut::create(0.25));
    auto bg = utils::findChild(this, "background");
    bg->runAction(EaseCubicActionOut::create(MoveTo::create(1, Point(75, 60) / 2)));
    bg->runAction(ScaleTo::create(1, 0.05));
    bg->runAction(Sequence::create(DelayTime::create(0.25), FadeOut::create(0.5), CallFunc::create([] {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("mjstory.animate.popup");
    }), NULL));
    this->runAction(Sequence::create(DelayTime::create(1), RemoveSelf::create(), NULL));
}

void MSNotificationPopup::onOpen(cocos2d::Ref *sender) {
    MJDEFAULT->setBoolForKey(StringUtils::format("%s%d", key_popup_showed, _info.notification_id), true);
    MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
    if (!_info.url.empty()) {
        Application::getInstance()->openURL(_info.url);
    }
    
    onClose(nullptr);
}
