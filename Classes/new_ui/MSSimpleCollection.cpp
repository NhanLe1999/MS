//
//  MSSimpleCollection.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "MSSimpleCollection.h"
#include "MSCollectionStoryItem.h"
#include "MSCollectionAudiobookItem.h"
#include "MSCollectionCategoryItem.h"
#include "MSCollectionDelegate.h"
#include "StoryLanguageManager.h"
#include "StoryDataModel.h"
#include "MJPlatformConfig.h"
#include "manager/CleverTapManager.h"
#include "APProfileManager.h"

#include "MSLessonManager.h"
#include "MSLessonAISpeakingManager.h"

#include "MSCollectionLessonItem.h"
//#include "MSCollectionAiSpeakingLessonItem.h"

#include "MSCollectionLessonItemDelegate.h"
//#include "MSCollectionAISpeakingLessonItemDelegate.h"
#define LIMIT_MAX_ITEM 7

USING_NS_CC;
INITIALIZE_READER(MSSimpleCollection);

std::once_flag ms_simple_collection_reader;

MSSimpleCollection * MSSimpleCollection::createCollection(std::string title, std::vector<cocos2d::ui::Widget *> items)
{
    std::call_once(ms_simple_collection_reader, []
    {
        REGISTER_CSB_READER(MSSimpleCollection);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/SimpleCollection.csb");
    
    auto collection = reinterpret_cast<MSSimpleCollection *>(CSLoader::createNode(csb_name));
    
    if (collection)
    {
        collection->didLoadFromCSB(title, items);
        return collection;
    }
    
    CC_SAFE_DELETE(collection);
    
    return nullptr;
}

MSSimpleCollection * MSSimpleCollection::createCategoriesCollection()
{
    std::vector<cocos2d::ui::Widget *> category_items;
    
    auto categories = StoryDataModel::getInstance()->getCategories(StoryLanguageManager::getCurrentLangId());
    
    for (auto category : categories)
    {
        category_items.push_back(MSCollectionCategoryItem::createItem(category));
    }
    
    auto collection = MSSimpleCollection::createCollection("stories.category.title", category_items);
    collection->setLeftPadding(25);
    collection->setCategories(categories);
    collection->setCollectionType(CollectionType::CATEGORY);
    collection->disableMore();
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createAudiobookCategoriesCollection()
{
	std::vector<cocos2d::ui::Widget *> category_items;

	auto categories = StoryDataModel::getInstance()->getAudiobookSeries();

	for (auto category : categories)
	{
		category_items.push_back(MSCollectionCategoryItem::createItem(category));
	}

	auto collection = MSSimpleCollection::createCollection("stories.genre.title", category_items);
	collection->setLeftPadding(25);
	collection->setAudiobookCategories(categories);
	collection->setCollectionType(CollectionType::AUDIOBOOK_CATEGORY);
	collection->disableMore();
	return collection;
}

MSSimpleCollection * MSSimpleCollection::createPopularStoriesCollection(int grade_id)
{
    auto popular_stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());

    std::stable_sort(std::begin(popular_stories), std::end(popular_stories), [](const mj::model::StoryInfo & a, const mj::model::StoryInfo & b)
    {
        if (a.quality != b.quality)
        {
            return a.quality > b.quality;
        }
        
        if (a.is_read != b.is_read)
        {
            return (int)a.is_read < (int)b.is_read;
        }
        
        if (a.date_published != b.date_published)
        {
            return a.date_published > b.date_published;
        }
        
        return false;
    });
    
    std::vector<cocos2d::ui::Widget *> ui_popular_stories;
    std::vector<mj::model::StoryInfo> stories;
    
    auto counter = 0;
    for (auto story : popular_stories)
    {
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        if (grade_id == -1)
        {
            CCLOG("MSSimpleCollection::createPopularStoriesCollection| grade_id == -1 MSCollectionStoryItem::createItem:%s",story.name.c_str());
            counter++;
            ui_popular_stories.push_back(MSCollectionStoryItem::createItem(story));
            stories.push_back(story);
        }
        else
        {
            if (story.levels[0] == grade_id)
            {
                
                CCLOG("MSSimpleCollection::createPopularStoriesCollection| story.levels[0] == grade_id MSCollectionStoryItem::createItem:%s",story.name.c_str());

                //            auto level_info = StoryDataModel::getInstance()->getLevelInfoById(_stories[i].levels[0], 1);
                //            if (level_info.grade_id == grade_id) {
                counter++;
                ui_popular_stories.push_back(MSCollectionStoryItem::createItem(story));
                stories.push_back(story);
            }
        }
    }
    
    auto collection = MSSimpleCollection::createCollection("stories.popular.title", ui_popular_stories);
    collection->setStories(stories);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::STORY);
    collection->setCollectionOrder(CollectionOrder::POPULAR);
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createNewestStoriesCollection(int grade_id)
{
    auto new_stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    
    std::stable_sort(std::begin(new_stories), std::end(new_stories), [](const mj::model::StoryInfo & a, const mj::model::StoryInfo & b)
    {
        if (a.quality != b.quality)
        {
            return a.quality > b.quality;
        }
        
        if (a.is_read != b.is_read)
        {
            return (int)a.is_read < (int)b.is_read;
        }
        
        if (a.date_published != b.date_published)
        {
            return a.date_published > b.date_published;
        }
        
        return false;
    });
    
    std::vector<mj::model::StoryInfo> top_10_popular;
    int idx = 0;
    
	int limit_item = LIMIT_MAX_ITEM;
    
	if (limit_item > (int)new_stories.size())
    {
		limit_item = (int)new_stories.size();
	}
    
    if (grade_id == -1)
    {
        top_10_popular = std::vector<mj::model::StoryInfo>(new_stories.begin(), new_stories.begin() + limit_item);
    }
    else
    {
        while (top_10_popular.size() < limit_item && idx < new_stories.size())
        {
            auto story = new_stories[idx];
            
            if (!story.levels.empty() && story.levels[0] == grade_id)
            {
                top_10_popular.push_back(story);
            }
            
            idx++;
        }
    }
    
    std::stable_sort(std::begin(new_stories), std::end(new_stories), [](const mj::model::StoryInfo & a, const mj::model::StoryInfo & b)
    {
        if (a.is_read != b.is_read)
        {
            return (int)a.is_read < (int)b.is_read;
        }
        
        if (a.date_published != b.date_published)
        {
            return a.date_published > b.date_published;
        }
        
        if (a.quality != b.quality)
        {
            return a.quality > b.quality;
        }
        
        return false;
    });
    
    std::vector<cocos2d::ui::Widget *> ui_new_stories;
    std::vector<mj::model::StoryInfo> stories;
    
    auto counter = 0;
    
    for (auto story : new_stories)
    {
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        
        if (std::find_if(std::begin(top_10_popular), std::end(top_10_popular), [story](mj::model::StoryInfo i)
        {
            return i.unique_id == story.unique_id;
        }) != std::end(top_10_popular))
        {
            continue;
        }
        
        if (grade_id == -1)
        {
            ui_new_stories.push_back(MSCollectionStoryItem::createItem(story));
            stories.push_back(story);
            counter++;
        }
        else
        {
            if (story.levels[0] == grade_id)
            {
                ui_new_stories.push_back(MSCollectionStoryItem::createItem(story));
                stories.push_back(story);
                counter++;
            }
        }
    }
    
    if (counter == 0)
    {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("stories.newstories.title", ui_new_stories);
    collection->setStories(stories);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::STORY);
    collection->setCollectionOrder(CollectionOrder::NEW);
    
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createFavouriteStoriesCollection(int grade_id)
{
    MJDEFAULT->deleteValueForKey(key_click_on_delete_stories);
    auto fav_stories = StoryDataModel::getInstance()->getFavouriteStoriesByLanguage(StoryLanguageManager::getCurrentLangId());
    
    std::vector<cocos2d::ui::Widget *> ui_new_stories;
    std::vector<mj::model::StoryInfo> stories;
    
    auto counter = 0;
    
    for (auto story : fav_stories)
    {
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        counter++;
        
        if (grade_id == -1)
        {
            ui_new_stories.push_back(MSCollectionStoryItem::createItem(story));
            stories.push_back(story);
        }
        else
        {
            if (!story.levels.empty() && story.levels[0] == grade_id)
            {
                ui_new_stories.push_back(MSCollectionStoryItem::createItem(story));
                stories.push_back(story);
            }
        }
    }
    
    if (stories.empty())
    {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("favorite.stories", ui_new_stories);
    collection->setStories(fav_stories);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::STORY);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    
    if (stories.size() < LIMIT_MAX_ITEM)
    {
        collection->disableMore();
    }
    
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createDownloadedStoriesCollection(int grade_id)
{
    auto downloadedStories = PROFILE_MANAGER.getInstance().getAllReadStories(MJDEFAULT->getIntegerForKey(key_current_profile_id,-1));
    
    //StoryDataModel::getInstance()->getAllStoryDownloaded();
    std::vector<mj::model::StoryInfo> stories;
    std::vector<cocos2d::ui::Widget *> ui_new_stories;

    auto counter = 0;
    
    for (auto idx : downloadedStories)
    {
        auto story = StoryDataModel::getInstance()->getStoryById(idx);
        
        if (story.story_id == 0)
        {
            continue;
        }
        
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        
        counter++;
        
        if (grade_id == -1)
        {
            stories.push_back(story);
        }
        else
        {
            if (!story.levels.empty() && story.levels[0] == grade_id)
            {
                stories.push_back(story);
            }
        }
    }
    
    if (stories.empty())
    {
        return nullptr;
    }
    
    stories.clear();
    
    for (auto idx : downloadedStories)
    {
        auto story = StoryDataModel::getInstance()->getStoryById(idx);
        if (story.story_id != 0) {  // kiểm tra story_id != 0 thì mới push_back
            stories.push_back(story);
        }
    }
    
    //std::stable_sort(std::begin(stories), std::end(stories), [](const mj::model::StoryInfo & a, const mj::model::StoryInfo & b)
    //{
    //    if (a.quality != b.quality)
    //    {
    //        return a.quality > b.quality;
    //    }
    //    
    //    if (a.is_read != b.is_read) {
    //        return (int)a.is_read < (int)b.is_read;
    //    }
    //    
    //    if (a.date_published != b.date_published)
    //    {
    //        return a.date_published > b.date_published;
    //    }
    //    
    //    return false;
    //});
    
    for (int i=0; i< MIN(LIMIT_MAX_ITEM, (int)stories.size()); i++) {
        ui_new_stories.push_back(MSCollectionStoryItem::createItem(stories.at(i)));
    }
    
    auto collection = MSSimpleCollection::createCollection("downloaded.stories", ui_new_stories);
    collection->setStories(stories);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::STORY);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    
    if (stories.size() < LIMIT_MAX_ITEM)
    {
        collection->disableMore();
    }
    
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createDownloadedAudiobooksCollection(int grade_id)
{
    auto readAudiobooks = PROFILE_MANAGER.getInstance().getAllReadAudiobooks(MJDEFAULT->getIntegerForKey(key_current_profile_id,-1));
    
    std::vector<mj::model::AudiobookInfo> downloadedAudiobooks;

    for(auto audioID :readAudiobooks)
    {
        auto audiobook = StoryDataModel::getInstance()->getAudiobook(audioID);

        if(audiobook.book_id != 0)
        {
            downloadedAudiobooks.push_back(audiobook);
        }
    }

  /*  std::stable_sort(std::begin(downloadedAudiobooks), std::end(downloadedAudiobooks), [](const mj::model::AudiobookInfo & a, const mj::model::AudiobookInfo & b) {
        
        if (a.date_publish != b.date_publish)
        {
            return a.date_publish > b.date_publish;
        }
        
        if (a.quality != b.quality)
        {
            return a.quality > b.quality;
        }
        
        return false;
    });*/
    
    
    //auto downloaded =StoryDataModel::getInstance()->getAllAudiobooksDownloaded();
    std::vector<mj::model::AudiobookInfo> audiobooks;
    std::vector<cocos2d::ui::Widget *> ui_downloaded_audiobooks;
    
    auto counter = 0;
    
    for (auto info : downloadedAudiobooks)
    {
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        
        counter++;
        
        if (grade_id == -1)
        {
            ui_downloaded_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(info));
            audiobooks.push_back(info);
        }
        else
        {
            if (info.grade_id == grade_id)
            {
                ui_downloaded_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(info));
                audiobooks.push_back(info);
            }
        }
    }
    
    if (audiobooks.empty())
    {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("downloaded.audiobooks", ui_downloaded_audiobooks);
    collection->setAudiobooks(downloadedAudiobooks);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::AUDIOBOOK);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    
    if (audiobooks.size() < LIMIT_MAX_ITEM)
    {
        collection->disableMore();
    }
    
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createRecentStoriesCollection(int grade_id)
{
    auto recent_stories = StoryDataModel::getInstance()->getRecentStoriesByLanguage(StoryLanguageManager::getCurrentLangId());
    
    if (recent_stories.empty())
    {
        return nullptr;
    }
    
    auto counter = 0;
    
    std::vector<mj::model::StoryInfo> stories;
    std::vector<cocos2d::ui::Widget *> ui_source;
    
    for (auto story : recent_stories)
    {
        if (story.story_id == 0)
        {
            continue;
        }
        
        if (counter == LIMIT_MAX_ITEM)
        {
            break;
        }
        counter++;
        
        ui_source.push_back(MSCollectionStoryItem::createItem(story));
        stories.push_back(story);
    }
    
    auto collection = MSSimpleCollection::createCollection("Recent Stories", ui_source);
    collection->setStories(stories);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::STORY);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    collection->setStories(stories);
    
    if (stories.size() < LIMIT_MAX_ITEM)
    {
        collection->disableMore();
    }
    
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createFavouriteAudiobooksCollection(int grade_id)
{
    auto audiobooks = StoryDataModel::getInstance()->getFavouriteAudiobooksByLanguage(StoryLanguageManager::getCurrentLangId());
    
    std::vector<ui::Widget *> ui_audiobooks;
    
    for (auto audiobook : audiobooks)
    {
        ui_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
    }
    
    if (audiobooks.empty())
    {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("favorite.audiobooks", ui_audiobooks);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    
    if (audiobooks.size() < LIMIT_MAX_ITEM)
    {
        collection->disableMore();
    }
    
    collection->setCollectionType(CollectionType::AUDIOBOOK);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    collection->setAudiobooks(audiobooks);
    
    return collection;
}

void MSSimpleCollection::didLoadFromCSB(std::string title, std::vector<cocos2d::ui::Widget *> items)
{
    _collection_type = CollectionType::STORY;
    _collection_order = CollectionOrder::OTHER;
    _delegate = nullptr;
    _items = items;
    this->setClippingEnabled(false);
    _list = this->getChildByName<ui::ListView *>("list");
    _list->setPropagateTouchEvents(true);
    _list->setSwallowTouches(false);
    _list->setScrollBarEnabled(false);
    _title = title;
    
//    _list->addEventListener(CC_CALLBACK_2(MSSimpleCollection::onSelectItem, this));
    this->setTouchEnabled(true);
    this->addTouchEventListener([=](Ref * sender, ui::Widget::TouchEventType event)
    {
        switch (event)
        {
            case ui::Widget::TouchEventType::BEGAN:
            {
                _time_touch_start = utils::getTimeInMilliseconds();
                 _position_touch_start = this->getTouchBeganPosition();
                 
                 break;
            }
            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED:
            {
                auto current_time = utils::getTimeInMilliseconds();
                auto current_position = this->getTouchEndPosition();
                if (current_position.fuzzyEquals(_position_touch_start, 20) && current_time - _time_touch_start < 500)
                {
                    CCLOG("Clicked!!!!");
                    this->processClickEvent(_position_touch_start);
                }
                
                break;
            }
                
            default:
                break;
        }
    });
    
    auto text_title = utils::findChild<ui::Text *>(this, "title");
    text_title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_title));
    utils::findChild<ui::Text *>(this, "more_title")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "stories.more"));

    auto d = 0.f;
    for (auto item : items)
    {
        if (!item)
            continue;
        item->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        _list->addChild(item);
        d += item->getContentSize().width * (MJ_PLATFORM_CONFIG.getItemScaleFactor() - 1.f);
    }
    
    auto more = this->getChildByName<ui::Widget *>("panel_more");
    more->removeFromParentAndCleanup(false);
    
    if (!items.empty() && items[0])
    {
        more->setContentSize(Size(more->getContentSize().width, items[0]->getContentSize().height * items[0]->getScaleY()));
        ui::Helper::doLayout(more);
        auto button_more = more->getChildByName("button_more");
        button_more->setScale(items[0]->getScale());
    }
    _list->addChild(more);
    
    _list->setRightPadding(d + 25);
    
    if (items.empty())
    {
        return;
    }
    if (items.front())
    {
        _list->setContentSize(Size(_list->getContentSize().width, items.front()->getBoundingBox().size.height));
        this->setContentSize(Size(this->getContentSize().width, _list->getContentSize().height + 165));

        auto background = utils::findChild(this, "background");
        background->setContentSize(Size(this->getContentSize().width, _list->getContentSize().height + 135));
    }

 
    auto more_fixed = utils::findChild(this, "button_more_fixed");
    more_fixed->setCascadeColorEnabled(false);
    
    auto play_all = utils::findChild(this, "button_play_all");
    if (play_all)
        play_all->setCascadeColorEnabled(false);

    ui::Helper::doLayout(this);

    changeTextDisplayLang(_title);

//    _list->ScrollView::addEventListener([this](Ref* ref, ScrollView::EventType eventType) {
//
//        ListView* listView = dynamic_cast<ListView*>(ref);
//
//        if(listView == nullptr || eventType != ScrollView::EventType::CONTAINER_MOVED)
//        {
//            return;
//        }
//
//        auto left = listView->getLeftmostItemInCurrentView();
//        auto right = listView->getRightmostItemInCurrentView();
//        //auto top = listView->getTopmostItemInCurrentView();
//        //auto bottom = listView->getBottommostItemInCurrentView();
//        //auto center = listView->getCenterItemInCurrentView();
//
//        for (size_t i = 0; i < listView->getChildrenCount(); i++)
//        {
//            listView->getChildren().at(i)->setVisible(false);
//        }
//
//        for (size_t i = listView->getIndex(left); i <= listView->getIndex(right); i++)
//        {
//            if (auto node = listView->getItem(i))
//            {
//                node->setVisible(true);
//            }
//        }
//
//    });
//  _list->scrollToLeft(0.1f,false);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [=](EventCustom *)
    {
        this->changeTextDisplayLang(_title);
    }), this);
}

void MSSimpleCollection::disableMore() {
    utils::findChild(this, "button_more_fixed")->setVisible(false);
    this->setContentSize(Size(this->getContentSize().width, _list->getContentSize().height + 130));
    auto background = utils::findChild(this, "background");
    background->setContentSize(Size(this->getContentSize().width, _list->getContentSize().height + 100));
    _list->removeLastItem();
    
    ui::Helper::doLayout(this);
}

#include "StoryConfigManager.h"
void MSSimpleCollection::enablePlaylist() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    utils::findChild(this, "button_play_all")->setVisible(true);
    if(!CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild(this, "dot")->setVisible(true);
        utils::findChild(this, "line")->setVisible(true);
    }
#endif
}

void MSSimpleCollection::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    
    if (_list) {
        _list->setContentSize(Size(size.width - 40, _list->getContentSize().height));
//        _list->setRightPadding(25);
    }
    
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    auto title = utils::findChild<ui::Text *>(this, "title");
    
    if (dot && line && title) {
        auto last_x = title->getPositionX() + title->getContentSize().width;
        dot->setPosition(Point(last_x + 25, dot->getPositionY()));
        line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
        line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    }
    
    auto more = utils::findChild(this, "button_more_fixed");
    if (more) {
        more->setPosition(Point(size.width / 2, more->getPositionY()));
    }
    
    auto play_all = utils::findChild(this, "button_play_all");
    if (play_all) {
        //play_all->setPosition(Point(size.width - 15, play_all->getPositionY()));
        play_all->setPosition(Point(size.width - 15, play_all->getPositionY()) + Vec2(-40,0));//fix to fit oppo screen
    }
    
    auto background = utils::findChild(this, "background");
    if (background) {
        background->setContentSize(Size(size.width, background->getContentSize().height));
    }
}

void MSSimpleCollection::setLeftPadding(float padding) {
    _list->setLeftPadding(padding);
}

void MSSimpleCollection::setChildMargin(float margin) {
    _list->setItemsMargin(margin);
}

void MSSimpleCollection::onSelectItem(Ref * sender, ui::ListView::EventType type) {
    CCLOG("Select item:");
}

void MSSimpleCollection::processClickEvent(cocos2d::Point touch_position) {
//    if (_list->isScrolling() || _list->isAutoScrolling()) {
//        return;
//    }

	if (_collection_type == CollectionType::CATEGORY )
	{
		MJDEFAULT->setStringForKey("STORY_START_FROM", "category");
	}
	else if (_collection_type == CollectionType::AUDIOBOOK_CATEGORY)
	{
		MJDEFAULT->setStringForKey("STORY_START_FROM", "audiobook_category");
	}
	else
	{
		switch (_collection_order)
		{
		case MSSimpleCollection::POPULAR:
			if (_collection_type == CollectionType::STORY || _collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey(StringUtils::format("%s_START_FROM", (_collection_type == CollectionType::STORY) ? "STORY" : "AUDIOBOOK"), "popular");
			}
			break;
		case MSSimpleCollection::NEW:
			if (_collection_type == CollectionType::STORY || _collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey(StringUtils::format("%s_START_FROM", (_collection_type == CollectionType::STORY) ? "STORY" : "AUDIOBOOK"), "new");
			}
			break;
		case MSSimpleCollection::MORAL_STORIE:
			if (_collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "moral_stories");
			}
			break;
		case MSSimpleCollection::WORLD_OF_TALE:
			if (_collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "world_of_tales");
			}
			break;
		case MSSimpleCollection::FAMOUS_FABLE:
			if (_collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "famous_fables");
			}
			break;
		case MSSimpleCollection::POEM:
			if (_collection_type == CollectionType::AUDIOBOOK)
			{
				MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "poems");
			}
			break;
		case MSSimpleCollection::OTHER:
			break;
		default:
			break;
		}
	}

    auto children = _list->getItems();
    for (auto c : children) {
        auto bb = c->getBoundingBox();
        bb.origin = c->getParent()->convertToWorldSpace(bb.origin);
        if (bb.containsPoint(touch_position)) {
            _delegate ? _delegate->onSelectCollectionItem(c) : (void) nullptr;
        }
    }
}

void MSSimpleCollection::onMoreDefault()
{
    this->onMore(this);
}

void MSSimpleCollection::onMore(cocos2d::Ref *sender) 
{
	std::string featureName = "unknow";
	
	std::string catalogName = "unknow";

	switch (_collection_type)
	{
	case MSSimpleCollection::STORY:
		featureName = "story";
		break;
	case MSSimpleCollection::CATEGORY:
		featureName = "category";
		break;
	case MSSimpleCollection::AUDIOBOOK:
		featureName = "audiobook";
		break;
	case MSSimpleCollection::LESSON:
		featureName = "lesson";
		break;
	default:
		break;
	}

	switch (_collection_order)
	{
	case MSSimpleCollection::POPULAR:
		catalogName = "popular";
		break;
	case MSSimpleCollection::NEW:
		catalogName = "new";
		break;
	case MSSimpleCollection::MORAL_STORIE:
		catalogName = "moral_stories";
		break;
	case MSSimpleCollection::WORLD_OF_TALE:
		catalogName = "word_of_tales";
		break;
	case MSSimpleCollection::FAMOUS_FABLE:
		catalogName = "famous_fables";
		break;
	case MSSimpleCollection::POEM:
		catalogName = "famous_poems";
		break;
	case MSSimpleCollection::OTHER:
		break;
	default:
		break;
	}

	std::string buttonName = "";

	if (auto button = dynamic_cast<Node*>(sender))
	{
		buttonName = (button->getName() == "button_more_fixed" ? "want_to_see_more":"plus_button");
	}

	MJDEFAULT->setStringForKey("see_more_button_name", buttonName);//for clever tap data tracking.
	MJDEFAULT->setStringForKey("see_more_category_name", catalogName);//for clever tap data tracking.
	MJDEFAULT->setStringForKey("see_more_feature_name", featureName);//for clever tap data tracking.

    mj::helper::playButtonFX();

    if (_delegate) 
	{
        _delegate->onSelectMore(this);
    }
}

void MSSimpleCollection::onPlayAll(cocos2d::Ref *sender) {
    MJDEFAULT->setBoolForKey(key_audio_bool_pause, true);
    mj::helper::playButtonFX();
    if (_delegate) {
        _delegate->onPlayAll(this);
    }
}

std::string MSSimpleCollection::getTitle() {
    return _title;
}

void MSSimpleCollection::changeTextDisplayLang(std::string title)
{
    _title = title;
    
    auto display_text = LANGUAGE_MANAGER->getDisplayTextByKey(_title);
    
    if(auto title = utils::findChild<ui::Text *>(this, "title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-SemiBold.ttf");

       title->setString(display_text);
        
    }
    
    //currently, cannot see this button
    if(auto moreTitle = utils::findChild<ui::Text *>(this, "more_title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  moreTitle->setFontName("fonts/leelawdb.ttf") : moreTitle->setFontName("fonts/Montserrat-SemiBold.ttf");

       moreTitle->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "stories.more"));
    }
    
    auto title_more = utils::findChild<ui::Text *>(this, "title_more");
    
    if (title_more)
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title_more->setFontName("fonts/leelawdb.ttf") : title_more->setFontName("fonts/Montserrat-SemiBold.ttf");

        title_more->setString(LANGUAGE_MANAGER->getDisplayTextByKey("collection.more"));
    }
    
    auto title_play_all = utils::findChild<ui::Text *>(this, "title_play_all");
    
    if (title_play_all)
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title_play_all->setFontName("fonts/leelawdb.ttf") : title_play_all->setFontName("fonts/Montserrat-SemiBold.ttf");

        title_play_all->setString(LANGUAGE_MANAGER->getDisplayTextByKey("collection.play.all"));
        title_play_all->getParent()->setContentSize(Size(title_play_all->getContentSize().width + 50, title_play_all->getParent()->getContentSize().height));
        ui::Helper::doLayout(title_play_all->getParent());
    }
    
    this->setContentSize(this->getContentSize());
}

ui::Widget::ccWidgetClickCallback MSSimpleCollection::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onMore") {
        return CC_CALLBACK_1(MSSimpleCollection::onMore, this);
    }
    
    if (callback_name == "onPlayAll") {
        return CC_CALLBACK_1(MSSimpleCollection::onPlayAll, this);
    }
    
    return nullptr;
}

void MSSimpleCollection::addItems(std::vector<cocos2d::ui::Widget *> items) {
    if (!_list) {
        return;
    }
    for (auto item : items) {
        item->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        _list->insertCustomItem(item, 0);
    }
}


MSSimpleCollection * MSSimpleCollection::createPopularAudiobooksCollection(int grade_id) {
    auto popular_audioboks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    
    std::stable_sort(std::begin(popular_audioboks), std::end(popular_audioboks), [](const mj::model::AudiobookInfo & a, const mj::model::AudiobookInfo & b) {
        if (a.quality != b.quality) {
            return a.quality > b.quality;
        }
        
        if (a.read != b.read) {
            return (int)a.read < (int)b.read;
        }
        
        if (a.date_publish != b.date_publish) {
            return a.date_publish > b.date_publish;
        }
        return false;
    });
    
    std::vector<cocos2d::ui::Widget *> ui_popular_audiobooks;
    std::vector<mj::model::AudiobookInfo> audiobooks;
    
    auto counter = 0;
    for (auto audiobook : popular_audioboks) {
        if (counter == LIMIT_MAX_ITEM) {
            break;
        }
        if (grade_id == -1) { //grade_id == -1
            counter++;
            ui_popular_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
            audiobooks.push_back(audiobook);
        } else {
            if (audiobook.grade_id == grade_id) {
                counter++;
                ui_popular_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
                audiobooks.push_back(audiobook);
            }
        }
    }
    
    if (audiobooks.empty()) {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("audiobooks.popular.title", ui_popular_audiobooks);
    collection->setAudiobooks(popular_audioboks);
    if (audiobooks.size() < LIMIT_MAX_ITEM) {
        collection->disableMore();
    }
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::AUDIOBOOK);
    collection->setCollectionOrder(CollectionOrder::POPULAR);
    collection->setSeriesName("series_popular_audiobooks");
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createNewestAudiobooksCollection(int grade_id) {
    auto popular_audioboks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
    
    std::stable_sort(std::begin(popular_audioboks), std::end(popular_audioboks), [](const mj::model::AudiobookInfo & a, const mj::model::AudiobookInfo & b) {
        if (a.date_publish != b.date_publish) {
            return a.date_publish > b.date_publish;
        }

        if (a.quality != b.quality) {
            return a.quality > b.quality;
        }
        
        if (a.read != b.read) {
            return (int)a.read < (int)b.read;
        }
        
        return false;
    });
    
    std::vector<mj::model::AudiobookInfo> top_10_popular;
    int idx = 0;
    if (grade_id == -1) {
        auto limit = min(LIMIT_MAX_ITEM, (int) popular_audioboks.size());
        top_10_popular = std::vector<mj::model::AudiobookInfo>(popular_audioboks.begin(), popular_audioboks.begin() + limit);
    } else {
        while (top_10_popular.size() < LIMIT_MAX_ITEM && idx < popular_audioboks.size()) {
            auto audiobook = popular_audioboks[idx];
            if (audiobook.grade_id == grade_id) {
                top_10_popular.push_back(audiobook);
            }
            idx++;
        }
    }
    
    std::vector<cocos2d::ui::Widget *> ui_new_audiobooks;
    std::vector<mj::model::AudiobookInfo> audiobooks;
    
    auto counter = 0;
    
    for (auto audiobook : popular_audioboks) {
        if (counter == LIMIT_MAX_ITEM) {
            break;
        }
        
        if (grade_id == -1) {
            ui_new_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
            audiobooks.push_back(audiobook);
            counter++;
        } else {
            if (audiobook.grade_id == grade_id) {
                ui_new_audiobooks.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
                audiobooks.push_back(audiobook);
                counter++;
            }
        }
    }
    
    if (audiobooks.empty()) {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("audiobooks.newest.title", ui_new_audiobooks);
    collection->setAudiobooks(audiobooks);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    if (audiobooks.size() < LIMIT_MAX_ITEM) {
        collection->disableMore();
    }
    collection->setCollectionType(CollectionType::AUDIOBOOK);
    collection->setCollectionOrder(CollectionOrder::NEW);
    collection->setSeriesName("series_newest_audiobooks");
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createChaptersCollection(int grade_id) {
    auto chapters = StoryDataModel::getInstance()->getAudiobookChapters();
    
    std::stable_sort(std::begin(chapters), std::end(chapters), [](const mj::model::AudiobookInfo & a, const mj::model::AudiobookInfo & b) {
        if (a.quality != b.quality) {
            return a.quality > b.quality;
        }
        
        if (a.read != b.read) {
            return (int)a.read < (int)b.read;
        }
        
        if (a.date_publish != b.date_publish) {
            return a.date_publish > b.date_publish;
        }
        return false;
    });
    
    std::vector<cocos2d::ui::Widget *> ui_chapters;
    std::vector<mj::model::AudiobookInfo> audiobooks;
    
    auto counter = 0;
    
    for (auto audiobook : chapters) {
        if (counter == LIMIT_MAX_ITEM) {
            break;
        }
        
        if (grade_id == -1) {
            ui_chapters.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
            audiobooks.push_back(audiobook);
            counter++;
        } else {
            if (audiobook.grade_id == grade_id) {
                ui_chapters.push_back(MSCollectionAudiobookItem::createThumb(audiobook));
                audiobooks.push_back(audiobook);
                counter++;
            }
        }
    }
    
    if (audiobooks.size() < 1) {
        return nullptr;
    }
    
    auto collection = MSSimpleCollection::createCollection("audiobook.novels", ui_chapters);
    collection->setAudiobooks(audiobooks);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    if (audiobooks.size() < LIMIT_MAX_ITEM) {
        collection->disableMore();
    }
    collection->setCollectionType(CollectionType::AUDIOBOOK);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    collection->setSeriesName("Chapters");
    return collection;
}

MSSimpleCollection * MSSimpleCollection::createDownloadedLessonsCollection(cocos2d::Node* i_node)
{
    auto lessons = ms::LessonManager::getInstance().getDownloadedLessons();
    if (lessons.empty()) {
        return nullptr;
    }
    
    auto counter = 0;
    std::vector<cocos2d::ui::Widget *> ui_source;
    for (auto lesson : lessons) {
        if (counter == LIMIT_MAX_ITEM) {
            break;
        }
        counter++;
        auto ui = MSCollectionLessonItem::createItem(lesson);
        //ui->setDisplayMode();
        
        if(auto parent = dynamic_cast<ms::delegate::CollectionLessonItem*>(i_node))
        {
            ui->setDelegate(parent);
        }
        
        ui_source.push_back(ui);
    }
    
    auto collection = MSSimpleCollection::createCollection("Recent Stories", ui_source);
    collection->setLessons(lessons);
    collection->setLeftPadding(25);
    collection->setChildMargin(25);
    collection->setCollectionType(CollectionType::LESSON);
    collection->setCollectionOrder(CollectionOrder::OTHER);
    
    if (lessons.size() < LIMIT_MAX_ITEM) {
        collection->disableMore();
    }
    
    return collection;
}

MSSimpleCollection* MSSimpleCollection::createDownloadedAISpeakingLessonsCollection(cocos2d::Node* i_node)
{
    
    return nullptr;
    
//    auto lessons = ms::LessonAISpeakingManager::getInstance().getDownloadedLessons();
//    if (lessons.empty()) {
//        return nullptr;
//    }
//
//    auto counter = 0;
//    std::vector<cocos2d::ui::Widget*> ui_source;
//    for (auto lesson : lessons) {
//        if (counter == LIMIT_MAX_ITEM) {
//            break;
//        }
//        counter++;
//        auto ui = MSCollectionAiSpeakingLessonItem::createItem(lesson);
//        //ui->setDisplayMode();
//
//        if (auto parent = dynamic_cast<ms::delegate::CollectionAISpeakingLessonItem*>(i_node))
//        {
//            ui->setDelegate(parent);
//        }
//
//        ui_source.push_back(ui);
//    }
//
//    auto collection = MSSimpleCollection::createCollection("Recent Stories", ui_source);
//    collection->setAISpekingLessons(lessons);
//    collection->setLeftPadding(25);
//    collection->setChildMargin(25);
//    collection->setCollectionType(CollectionType::AI_SPEAKING);
//    collection->setCollectionOrder(CollectionOrder::OTHER);
//
//    if (lessons.size() < LIMIT_MAX_ITEM) {
//        collection->disableMore();
//    }
//
//    return collection;
}
