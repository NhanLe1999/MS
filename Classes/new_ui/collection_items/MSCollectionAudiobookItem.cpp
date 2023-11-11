//
//  MSCollectionAudiobookItem.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#include "MSCollectionAudiobookItem.h"
#include "RemoteImageView.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"

USING_NS_CC;
INITIALIZE_READER(MSCollectionAudiobookItem);

std::once_flag ms_collection_audiobook_item_reader;
cocos2d::Data ms_collection_audiobook_item_node_data;
cocos2d::Data ms_collection_audiobook_chapter_item_node_data;

MSCollectionAudiobookItem * MSCollectionAudiobookItem::createThumb(mj::model::AudiobookInfo info)
{
    std::call_once(ms_collection_audiobook_item_reader, []
    {
        REGISTER_CSB_READER(MSCollectionAudiobookItem);
        ms_collection_audiobook_item_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/AudiobookItem.csb");
        ms_collection_audiobook_chapter_item_node_data = FileUtils::getInstance()->getDataFromFile("csb/new_ui/collection_items/AudiobookItemChapter.csb");
    });

    MSCollectionAudiobookItem * item = nullptr;
    
    if (StoryDataModel::getInstance()->isAudiobookChapter(info.book_id))
    {
        item = reinterpret_cast<MSCollectionAudiobookItem *>(CSLoader::createNode(ms_collection_audiobook_chapter_item_node_data));
    }
    else
    {
        item = reinterpret_cast<MSCollectionAudiobookItem *>(CSLoader::createNode(ms_collection_audiobook_item_node_data));
    }
    
    if (item)
    {
        item->didLoadFromCSB(info);
        return item;
    }

    CC_SAFE_DELETE(item);
    
    return nullptr;
}

void MSCollectionAudiobookItem::didLoadFromCSB(mj::model::AudiobookInfo info)
{
    _info = info;

    utils::findChild<ui::Text *>(this, "placeholder")->setString(info.name);
    utils::findChild(this, "tag")->setLocalZOrder(1);

    auto duration_in_minute = floor(_info.duration / 60000.f);
    
    auto duration_in_second = _info.duration / 1000.f - duration_in_minute * 60;
    
    if (duration_in_second >= 30)
    {
        duration_in_minute += 1;
    }
    
    auto thumb_size = Size(235, 230) * 0.93;
    auto thumb_position = Point(this->getContentSize().width / 2, 113);
    std::string text_extra = StringUtils::format("%0.0f %s", duration_in_minute, LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "audiobooks.duration").c_str());
    
    if (StoryDataModel::getInstance()->isAudiobookChapter(_info.book_id))
    {
        thumb_size = Size(209, 196);
        thumb_position = Point(125.5, 129);
        text_extra = StringUtils::format("%lu %s", StoryDataModel::getInstance()->getChapterOfAudiobook(info.book_id).size(), LANGUAGE_MANAGER->getDisplayTextByKey("audiobook.chapters").c_str());
    }

    if(auto text_duration = utils::findChild<ui::Text *>(this, "duration"))
    {
       (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text_duration->setFontName("fonts/leelawdb.ttf") : text_duration->setFontName("fonts/Montserrat-SemiBold.ttf");
        
        text_duration->setString(text_extra);
    }

    auto remoteview = RemoteImageView::createImage(_info.thumbnail_url, [=](RemoteImageView * img, bool is_success)
    {
        auto image_fade_in = utils::findChild(this, "image");
        if (is_success) {
            img->setContentSize(thumb_size);
            img->setOpacity(0);
            this->runAction(Spawn::create(CallFunc::create([=] {
                image_fade_in->runAction(FadeOut::create(0.25));
                }), CallFunc::create([=] {
                    img->runAction(FadeIn::create(0.25));
                    }), nullptr));
            
        }
    }, RemoteImageView::Type::AUDIOBOOKS);

    remoteview->setPosition(thumb_position);
  //  remoteview->setName("thumb");
    remoteview->setContentSize(thumb_size);
    this->getChildByName("clip")->addChild(remoteview);
    
    icon_new = utils::findChild<ui::ImageView *>(this, "icon_new");
    
    if (LANGUAGE_MANAGER->getDisplayLangId()== LANGUAGE_VN_CODE)
    {
        icon_new->loadTexture("mjstory/home/storyitem_iconnew_vi.png");
    }
    
    icon_read = utils::findChild<ui::ImageView *>(this, "icon_read");
    
    if (_info.read >= 1)
    {
        icon_read->setVisible(true);
    }
    else
    {
        //        CCLOG("book_id = %d", _info.book_id);
        if (MJDEFAULT->getBoolForKey(key_time_mark_new_AUDIO_available))
        {
            auto time_mark_new = MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_time_mark_new_AUDIO, MJDEFAULT->getIntegerForKey(key_current_lang, 1)));
            
            if (_info.date_publish > time_mark_new)
            {
                icon_new->setVisible(true);
            }
        }
        else
        {
            auto current_date = LTDate(MJDEFAULT->getIntegerForKey(key_time_curent));
            current_date = current_date + (-1209600);
            auto story_date_compare = current_date.year*10000 + current_date.month*100 + current_date.day;
            auto date_convert = LTDate(_info.date_publish);
            auto story_date_sort = date_convert.year*10000+date_convert.month*100+date_convert.day;
            
            if (story_date_sort > story_date_compare) {
                icon_new->setVisible(true);
            }
        }
    }
    
    auto button_delete = ui::Button::create("mjstory/new_ui/button_delete_book.png");
    button_delete->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    button_delete->setPosition(Point(230, 257));
    button_delete->addClickEventListener([=](Ref *) {
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_delete_audiobook", &_info);
    });
    button_delete->setVisible(false);
    button_delete->setOpacity(0);
    button_delete->setName("button_delete");
    this->addChild(button_delete, 1);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [=](EventCustom *)
    {
        if(icon_new)
        {
            icon_new->loadTexture(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_VN_CODE ? "mjstory/home/storyitem_iconnew_vi.png" : "mjstory/home/storyitem_iconnew.png");
        }
        
    }), this);
    
}

ui::Widget * MSCollectionAudiobookItem::createCloneInstance()
{
    return new MSCollectionAudiobookItem();
}

void MSCollectionAudiobookItem::copySpecialProperties(cocos2d::ui::Widget* model) {
    ui::Layout::copySpecialProperties(model);
    auto casted = dynamic_cast<MSCollectionAudiobookItem *>(model);
    this->_info = casted->_info;
}

ui::Widget::ccWidgetClickCallback MSCollectionAudiobookItem::onLocateClickCallback(const std::string & callback_name) {
    if (callback_name == "onSelect") {
        return CC_CALLBACK_1(MSCollectionAudiobookItem::onSelect, this);
    }
    
    return nullptr;
}


void MSCollectionAudiobookItem::onSelect(Ref * sender) {
    CCLOG("Select audiobook");
}

void MSCollectionAudiobookItem::enableDeleteMode() {
    for (auto c : this->getChildren()) {
        if (c->getName() != "button_delete")
            c->setOpacity(0x77);
    }
    
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setVisible(true);
    button_delete->runAction(FadeIn::create(0.1));
}

void MSCollectionAudiobookItem::disableDeleteMode() {
    for (auto c : this->getChildren()) {
        c->setOpacity(0xff);
    }
    auto button_delete = utils::findChild(this, "button_delete");
    button_delete->setVisible(false);
    button_delete->runAction(FadeIn::create(0.1));
}

void MSCollectionAudiobookItem::onExit() {
    ui::Layout::onExit();
}

void MSCollectionAudiobookItem::disableNewRead() {
    utils::findChild(this, "tag")->setVisible(false);
    this->getChildByName<ui::Layout *>("clip")->setClippingEnabled(false);
}
