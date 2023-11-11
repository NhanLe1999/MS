//
//  StoryParentItem_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#include "StoryParentItem_Landscape.h"
#include "HSAudioEngine.h"
#include "StoryDataModel.h"
#include "HSLib.h"
#include "MJPlatformConfig.h"
#include "MJPay.h"
#include "StoryConfigManager.h"
#include "AppInbox.h"

USING_NS_CC;

INITIALIZE_READER_INSTANCE(StoryParentItem_LandscapeReader);

StoryParentItem_Landscape * StoryParentItem_Landscape::createItem(const cocos2d::ValueMap & data) {
    
    auto csb_name = mj::helper::getCSBName("csb/StoryParentItem_Landscape.csb");
    auto p = static_cast<StoryParentItem_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    if (p) {
        p->didLoadFromCSB(data);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

StoryParentItem_Landscape * StoryParentItem_Landscape::createItemBig(const cocos2d::ValueMap & data) {
    auto p = static_cast<StoryParentItem_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentItem_Landscape_big.csb"));
    if (p) {
        p->didLoadFromCSB(data, true);
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void StoryParentItem_Landscape::didLoadFromCSB(const cocos2d::ValueMap &data, bool is_big_item)
{
    this->is_big_item = is_big_item;
    _delegate = nullptr;

    auto layout_icon = this->getChildByName("layout_icon");
    auto icon = cocos2d::ui::ImageView::create(cocos2d::StringUtils::format("mjstory/parent/%s", data.at("icon").asString().c_str()));
    layout_icon->addChild(icon);
    icon->setScale(0.7);
//    icon->setColor(Color3B(41, 168, 211));
    icon->setPosition(Vec2(layout_icon->getContentSize().width/2+10, layout_icon->getContentSize().height/2));
    icon->setName("img_icon");
    
    auto label = utils::findChild<ui::Text *>(this, "lb_title");
    label->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
    label->setString(data.at("content").asString());


    _item_name = data.at("content").asString();
    _tag = data.at("tag").asInt();

    this->setName(_item_name);
    this->setTag(_tag);
    
    if (!is_big_item){
        utils::findChild(this, "noti_img")->setVisible(false);
    }

    if (_tag == 101) {
        //notification

        auto list_noti = StoryDataModel::getInstance()->getAllMessage();
        auto num_new_noti = 0;
        for (int i = 0; i < (int) list_noti.size(); i++) {
            if (XHSLib->checkMessageUnRead(list_noti[i].asValueMap()["id"].asInt())) {
                num_new_noti++;
            }
        }

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID ||  CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        auto message = mj::appinbox::getMessageAppinbox();
        num_new_noti = num_new_noti + message.size();
#endif
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

    _original_size = this->getContentSize();
    
    if (is_big_item){
        int lang_id = LANGUAGE_MANAGER->getDisplayLangId();
        auto text_license = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "license_lb"));
        text_license->setString(mj::Pay::getInstance().getLicense());
        auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "expire_lb"));
        auto time_expire = mj::Pay::getInstance().getTimeExpire();
        if (time_expire == -1) {
            text_details->setString(cocos2d::StringUtils::format("%s",LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()));
        } else {
            auto expire_date = LTDate(time_expire);
            text_details->setString(cocos2d::StringUtils::format("%s", expire_date.toPrettyString().c_str()));
        }
    }
    
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentItem_Landscape::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onClickItem") {
        return CC_CALLBACK_1(StoryParentItem_Landscape::onClickItem, this);
    }

    return nullptr;
}

void StoryParentItem_Landscape::onClickItem(cocos2d::Ref *sender) {
    AUDIO_ENGINE->playEffect("sounds/tap-mellow.mp3");
    if (_delegate) {
        _delegate->onSelectItem(_tag, _item_name);
    }
}

void StoryParentItem_Landscape::adjustWidth(float width) {
    this->setContentSize(cocos2d::Size(width, this->getContentSize().height));
    cocos2d::ui::Helper::doLayout(this);
    _original_size = this->getContentSize();
    
    auto layout_icon = this->getChildByName("layout_icon");
    layout_icon->setPosition(layout_icon->getPosition() + Vec2(MJ_PLATFORM_CONFIG.getNotchHeight(), 0));
}

void StoryParentItem_Landscape::setSelected(bool isSelected) {
    auto label = utils::findChild<ui::Text *>(this, "lb_title");
    auto icon = utils::findChild(this, "img_icon");
    auto arrow = utils::findChild(this, "arrow");
    
    if(CONFIG_MANAGER->isVietnameseStories()){
        auto choosen_img = utils::findChild(this, "choosen_img");
        if (isSelected) {
            choosen_img->setVisible(true);
        } else {
            choosen_img->setVisible(false);
        }

    }else{
        if (isSelected) {
            label->setTextColor(Color4B::BLACK);
            icon->setColor(Color3B::BLACK);
            arrow->setColor(Color3B::BLACK);
        } else {
            label->setTextColor(Color4B(41, 168, 211, 255));
            icon->setColor(Color3B(41, 168, 211));
            arrow->setColor(Color3B(41, 168, 211));
        }
    }
   
    cocos2d::ui::Helper::doLayout(this);
}
