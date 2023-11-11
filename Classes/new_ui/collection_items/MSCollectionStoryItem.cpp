//
//  MSCollectionStoryItem.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "MSCollectionStoryItem.h"
#include "RemoteImageView.h"
#include "StoryDataModel.h"
#include "MSGradeManager.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"

USING_NS_CC;
INITIALIZE_READER(MSCollectionStoryItem);

cocos2d::Data collection_item_node_data;

std::once_flag ms_collection_story_item_reader;
MSCollectionStoryItem * MSCollectionStoryItem::createItem(mj::model::StoryInfo info) {
    std::call_once(ms_collection_story_item_reader, [] {
        REGISTER_CSB_READER(MSCollectionStoryItem);
        collection_item_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/StoryItem.csb");
    });
    
    auto item = reinterpret_cast<MSCollectionStoryItem *>(CSLoader::createNode(collection_item_node_data));
    if (item) {
        item->didLoadFromCSB(info);
        return item;
    }
    
    CC_SAFE_DELETE(item);
    return nullptr;
}

void MSCollectionStoryItem::didLoadFromCSB(mj::model::StoryInfo info)
{
    _info = info;
    
    utils::findChild<ui::Text *>(this, "placeholder")->setString(info.name);
    
    auto remoteview = RemoteImageView::createImage(_info.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
        if (is_success) {
            this->getChildByName("image")->setVisible(false);
            utils::findChild(this, "placeholder")->runAction(FadeOut::create(0.25));
            img->setContentSize(Size(168, 230));
            img->setOpacity(0);
            img->runAction(FadeIn::create(0.25));
        }
    }, RemoteImageView::Type::STORIES);

   // remoteview->setName("thumb");
    this->getChildByName("clip")->addChild(remoteview);
    
    //// <
    //remoteview->setPosition(Point(this->getContentSize().width / 2, 118));
    remoteview->setAnchorPoint(Vec2(0.5, 0));
    remoteview->setPosition(Vec2(this->getContentSize().width / 2, 0));
    //// >

    remoteview->setContentSize(Size(168, 230));
    
//    auto mask_sprite = Scale9Sprite::create("mjstory/new_ui/image_item_mask.png");
//    mask_sprite->setCapInsets(Rect(24, 24, 2, 2));
//    mask_sprite->setContentSize(Size(168, 230));
//    auto mask = ClippingNode::create(mask_sprite);
//    mask->setAlphaThreshold(0);
//    mask->setPosition(Point(this->getContentSize().width / 2, 118));
//    this->addChild(mask);
//    mask->addChild(remoteview);
//    mask->setCascadeOpacityEnabled(true);
    
    auto button_delete = ui::Button::create("mjstory/new_ui/button_delete_book.png");
    button_delete->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    button_delete->setPosition(Point(170, 250));
    button_delete->addClickEventListener([=](Ref *) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_delete_story", &_info);
    });
    button_delete->setVisible(false);
    button_delete->setOpacity(0);
    button_delete->setName("button_delete");
    this->addChild(button_delete, 3);
    
    icon_new = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "icon_new");
    
    if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE)
    {
        icon_new->loadTexture("mjstory/home/storyitem_iconnew_vi.png");
    }
    
    icon_read = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "icon_read");
    icon_read->setVisible(false);
    icon_new->setVisible(false);
    
    if ((int)_info.is_read >= 1)
    {
        icon_read->setVisible(true);
    }
    else
    {
        if (MJDEFAULT->getBoolForKey(key_time_mark_new_available))
        {
            auto time_mark_new = MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_time_mark_new, MJDEFAULT->getIntegerForKey(key_current_lang, 1)));
            
            if (_info.date_published > time_mark_new)
            {
                icon_new->setVisible(true);
            }
        }
        else
        {
            auto current_date = LTDate(MJDEFAULT->getIntegerForKey(key_time_curent));
            current_date = current_date + (- 1209600);
            auto story_date_compare = current_date.year*10000 + current_date.month * 100 + current_date.day;
            auto date_convert = LTDate(_info.date_published);
            auto story_date_sort = date_convert.year * 10000 + date_convert.month * 100 + date_convert.day;
            
            if (story_date_sort > story_date_compare)
            {
                icon_new->setVisible(true);
            }
        }
    }
    
    this->setCascadeOpacityEnabled(true);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        if(icon_new)
        {
            icon_new->loadTexture(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE ? "mjstory/home/storyitem_iconnew_vi.png" : "mjstory/home/storyitem_iconnew.png");
        }
        
    }), this);
}

void MSCollectionStoryItem::disableNewRead() {
    icon_new = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "icon_new");
    icon_read = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "icon_read");
    
    icon_read->setVisible(false);
    icon_new->setVisible(false);
    
    utils::findChild(this, "bar")->setVisible(false);
    utils::findChild<ui::Layout *>(this, "clip")->setClippingEnabled(false);
}

void MSCollectionStoryItem::onEnter() {
    ui::Layout::onEnter();

    if (!_info.levels.empty())
    {
        auto level_info = StoryDataModel::getInstance()->getLevelInfoById(_info.levels[0], _info.lang_id);

        if (auto level = utils::findChild<ui::Text*>(this, "lb_level"))
        {
            level->setString(StringUtils::format("%s", level_info.name.c_str()));
        }
    }

    auto barNode = utils::findChild(this, "bar");
    if (barNode) {
        barNode->setLocalZOrder(2);
    }
   
}

ui::Widget * MSCollectionStoryItem::createCloneInstance() {
    return new MSCollectionStoryItem();
}

void MSCollectionStoryItem::copySpecialProperties(cocos2d::ui::Widget* model) {
    ui::Layout::copySpecialProperties(model);
    auto casted = dynamic_cast<MSCollectionStoryItem *>(model);
    this->_info = casted->_info;
}

void MSCollectionStoryItem::enableDeleteMode() {
    for (auto c : this->getChildren()) {
        if (c->getName() != "button_delete")
            c->setOpacity(0x77);
    }
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setOpacity(0xff);
    button_delete->setVisible(true);
    button_delete->runAction(FadeIn::create(0.1));
}

void MSCollectionStoryItem::disableDeleteMode() {
    for (auto c : this->getChildren()) {
        c->setOpacity(0xff);
    }
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setVisible(false);
}
