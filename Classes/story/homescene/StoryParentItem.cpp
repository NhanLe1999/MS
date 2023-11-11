//
//  StoryParentItem.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryParentItem.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "TruyenTranhPage.h"
#include "MJ_AnimationCache.h"
#include "AppDelegate.h"
#include "StoryDataModel.h"


INITIALIZE_READER_INSTANCE(StoryParentItemReader);

USING_NS_CC;
using namespace ui;

void StoryParentItem::didLoadFromCSB() {
    utils::findChild(this, "arrow")->setColor(Color3B(41, 168, 211));
}
#pragma mark - onEnter
void StoryParentItem::onEnter() {
    Layout::onEnter();
    didLoadFromCSB();
}
void StoryParentItem::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryParentItem::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();

}

void StoryParentItem::setItemWithData(ValueMap map_data){
    data = map_data;
    auto layout_icon = this->getChildByName("layout_icon");
    auto icon = ImageView::create(StringUtils::format("mjstory/parent/%s", data["icon"].asString().c_str()));
    layout_icon->addChild(icon);
    icon->setPosition(layout_icon->getContentSize()/2);
    icon->setScale(0.9);
    icon->setColor(Color3B(41, 168, 211));

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        icon->setRotation(-90);
    }
    layout_icon->getChildByName<Text *>("lb_title")->setString(data["content"].asString());

    utils::findChild(this, "noti_img")->setVisible(false);

    if (data["tag"].asInt() == 101) {
        //notification

        auto list_noti = StoryDataModel::getInstance()->getAllMessage();
        auto num_new_noti = 0;
        for (int i = 0; i < (int) list_noti.size(); i++) {
            if (XHSLib->checkMessageUnRead(list_noti[i].asValueMap()["id"].asInt())) {
                num_new_noti++;
            }
        }

        utils::findChild(this, "noti_img")->setVisible(num_new_noti > 0);
        utils::findChild<ui::Text *>(this, "noti_lb")->setString(StringUtils::toString(num_new_noti));

        auto show_noti = EventListenerCustom::create("mjstory.shownoti", [=](EventCustom *event_custom) {
            int *edata = (int *) event_custom->getUserData();
            utils::findChild(this, "noti_img")->setVisible(true);
            utils::findChild<ui::Text *>(this, "noti_lb")->setString(StringUtils::toString(*edata));
        });
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(show_noti, this);

        auto hide_noti = EventListenerCustom::create("mjstory.hidenoti", [=](EventCustom *event_custom) {
            utils::findChild(this, "noti_img")->setVisible(false);
        });
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(hide_noti, this);
    }
}

#pragma mark - BUTTON
void StoryParentItem::onClick(cocos2d::Ref *sender){
    CCLOG("on click item = %d", data["tag"].asInt());
    AUDIO_ENGINE->playEffect("sounds/tap-mellow.mp3");
    if (this->delegate) {
        this->delegate->onClickItem(data["tag"].asInt());
    }
}

void StoryParentItem::setDelegate(StoryParentItemPortraitDelegate * delegate) {
    this->delegate = delegate;
}



#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryParentItem::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentItem, onClick),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


