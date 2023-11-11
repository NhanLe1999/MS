//
//  MSTabStories.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#include "MSTabStories.h"
#include "MSSearchbar.h"
#include "MSSimpleCollection.h"

#include "StoryDataModel.h"
#include "MSCollectionCategoryItem.h"
#include "MSCollectionStoryItem.h"
#include "MSFeaturedStories.h"

#include "MSExploreView.h"
#include "MSExplorerView_Categories.h"
#include "MSSearchView.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "StoryInfoCover.h"
#include "MSActivityEnd.h"
#include "platform/CCPlatformConfig.h"

#include "MSGradeBox.h"
#include "MSHome.h"

USING_NS_CC;
INITIALIZE_READER(MSTabStories);

std::once_flag ms_tab_stories_reader;
MSTabStories * MSTabStories::createTab(MSHome * home) {
    std::call_once(ms_tab_stories_reader, [] {
        REGISTER_CSB_READER(MSTabStories);
    });
     auto csb_name = mj::helper::getCSBName("csb/new_ui/new_stories.csb");
    auto tab = reinterpret_cast<MSTabStories *>(CSLoader::createNode(csb_name));
    if (tab) {
        tab->didLoadFromCSB(home);
        return tab;
    }
    
    CC_SAFE_DELETE(tab);
    return nullptr;
}

void MSTabStories::didLoadFromCSB(MSHome * home)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    /*auto bt = ui::Button::create("mjstory/new_ui/screen_close.png");
    bt->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    bt->setPosition(this->getContentSize());
    bt->setName("screen_close");
    bt->addClickEventListener([](Ref *) {
        exit(0);
    });
    this->addChild(bt);*/
#endif
    MJDEFAULT->setStringForKey(key_read_from, "new");
    MJDEFAULT->deleteValueForKey(key_click_on_delete_stories);
    MJDEFAULT->deleteValueForKey("key_read_from_the_second");
    _mshome = home;
	_recent_stories_collection = nullptr;
    _new_stories_collection = nullptr;
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
    m_scope = ms::GradeManager::Scope::STORIES;
    this->setClippingEnabled(false);
    _current_grade_id = ms::GradeManager::getInstance().getCurrentGradeId();
    _featured = nullptr;

    _content = utils::findChild<ui::ListView *>(this, "content");
    _content->setScrollBarEnabled(false);
    _content->setSwallowTouches(false);
    
    auto gradebox = MSGradeBox::createBox(true,_current_grade_id);
    gradebox->setName("story_gradebox");
    gradebox->setAnchorPoint(Vec2(0, 0));
    gradebox->setPosition(Vec2(0, 0));
    auto gradebox_placeholder = utils::findChild<ui::Layout *>(this, "gradebox");
    gradebox_placeholder->addChild(gradebox);
 
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {
        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }
       // auto grade_id = reinterpret_cast<int *>(e->getUserData());
      //  this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto event_listener = EventListenerCustom::create(key_event_language_changed, [this](EventCustom * e) {
        this->reloadData();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_listener, this);
    
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    //    _content->scrollToItem(1, Point::ANCHOR_MIDDLE_TOP, Point::ANCHOR_MIDDLE_TOP, 2);
}

void MSTabStories::onEnter() 
{
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
    ui::Layout::onEnter();
   // this->schedule(schedule_selector(MSTabStories::updateSearchbarPosition), 1.0 / 60);
    
    reloadData();	
}

void MSTabStories::reloadData() 
{
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);

//    _featured ? _featured->removeFromParent() : (void) nullptr;
//    _featured = nullptr;
//    _featured = MSFeaturedStories::createView();
//    _featured->setContentSize(Size(this->getContentSize().width, _featured->getContentSize().height));
//    this->addChild(_featured);
//    while (_content->getItems().size() > 2)
//    {
//        _content->removeItem(2);
//    }
    
    
    if(_content)
    {
        _content->removeAllItems();
    }


    _popular_stories_collection = MSSimpleCollection::createPopularStoriesCollection(_current_grade_id);
    _popular_stories_collection->setContentSize(Size(this->getContentSize().width, _popular_stories_collection->getContentSize().height));
    _popular_stories_collection->setDelegate(this);
    _content->addChild(_popular_stories_collection);

//    _new_stories_collection = MSSimpleCollection::createNewestStoriesCollection(_current_grade_id);
//    if (_new_stories_collection) {
//        _new_stories_collection->setContentSize(Size(this->getContentSize().width, _popular_stories_collection->getContentSize().height));
//        _new_stories_collection->setDelegate(this);
//        _content->addChild(_new_stories_collection);
//    }
//
    _categories_collection = MSSimpleCollection::createCategoriesCollection();
    _categories_collection->setContentSize(Size(this->getContentSize().width, _categories_collection->getContentSize().height));
    _categories_collection->setDelegate(this);
    _content->addChild(_categories_collection);

    _content->jumpToTop();
    
    changeTextDisplayLang();
}

void MSTabStories::setContentSize(const Size & size) 
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    if (!_content) 
	{
        return;
    }
    
//    if(auto feature = _content->getChildByName("featured_placeholder"))
//    {
//        //feature->setContentSize(Size(size.width, feature->getContentSize().height));
//        //ui::Helper::doLayout((ui::Widget *)feature);
//        //feature
//    }

	if (auto bt = this->getChildByName("screen_close"))
	{
		bt->setPosition(size);
	}
}

void MSTabStories::updateSearchbarPosition(float dt) 
{
    if(_content)
    {
        auto top = _content->getTopmostItemInCurrentView();
        auto bottom = _content->getBottommostItemInCurrentView();

        for (size_t i = 0; i < _content->getChildrenCount(); i++)
        {
            _content->getChildren().at(i)->setVisible(false);
        }

        for (size_t i = _content->getIndex(top); i <= _content->getIndex(bottom); i++)
        {
            if (auto node = _content->getItem(i))
            {
                node->setVisible(true);
            }
        }
    }
    
    

    if (!_featured) 
	{
        return;
    }

//    auto feature_hook = utils::findChild(this, "featured_placeholder");
//    auto feature_position = feature_hook->getParent()->convertToWorldSpace(feature_hook->getPosition());
//    feature_position = _featured->getParent()->convertToNodeSpace(feature_position);
//    _featured->setPosition(feature_position);
    
}

void MSTabStories::onSelectGrade() 
{
//    auto searchbar_hook = utils::findChild(this, "searchbar_hook");
//    auto boundingbox = searchbar_hook->getBoundingBox();
//    boundingbox.origin = searchbar_hook->getParent()->convertToWorldSpace(boundingbox.origin);
//    if (boundingbox.getMaxY() < Director::getInstance()->getVisibleSize().height) {
//       // _content->scrollToItem(1, Point::ANCHOR_MIDDLE_TOP, Point::ANCHOR_MIDDLE_TOP);
//    }
}

void MSTabStories::onDeselectGrade(int grade_id) 
{
//    CCLOG("Select grade in tab stories: %d", grade_id);
//    _current_grade_id = grade_id;
//    reloadData();
   // _content->jumpToItem(1, Point::ANCHOR_MIDDLE_TOP, Point::ANCHOR_MIDDLE_TOP);
}

void MSTabStories::onSelectSearch() 
{
//    CCLOG("querying: %lld", utils::getTimeInMilliseconds());
//    auto stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(StoryLanguageManager::getCurrentLangId());
//    CCLOG("queried %lld", utils::getTimeInMilliseconds());
//    auto explore_view = MSExploreView::createView("All Stories", stories);
//    this->addChild(explore_view);
    
    auto search_view = MSSearchView::createView();
    search_view->setContentSize(this->getContentSize());
    search_view->setName("search_view");
    ui::Helper::doLayout(search_view);
    this->addChild(search_view, 1);
}

void MSTabStories::propagateClickEventToCollection(Point touch_position) {
    auto children = _content->getItems();
    for (auto c : children) {
        if (dynamic_cast<MSSimpleCollection *>(c)) {
            auto bb = c->getBoundingBox();
            bb.origin = c->getParent()->convertToWorldSpace(bb.origin);
            if (bb.containsPoint(touch_position)) {
                CCLOG("Touched collection");
                reinterpret_cast<MSSimpleCollection *>(c)->processClickEvent(touch_position);
            }
        }
    }
}

#include "MJAPIStoryDetails.h"
void MSTabStories::onSelectCollectionItem(ui::Widget * item) {
    if (!item) {
        return;
    }
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);

    auto story_item = dynamic_cast<MSCollectionStoryItem *>(item);
    if (story_item) {

        MJDEFAULT->setStringForKey("STORY_START_FROM","story_tab");

        mj::helper::playTransitionStoryCover(story_item, _content->getInnerContainer());
       
        return;
    }
    
    auto category_item = dynamic_cast<MSCollectionCategoryItem *>(item);
    if (category_item) {
        mj::helper::playButtonFX();
        auto info = category_item->getCategoryInfo();
        
        auto view = MSExploreView::createView(info.content, MSExploreView::CollectionType::SERIES, _current_grade_id, info.category_id);
        view->setTag(113115);
//        view->setContentSize(this->getContentSize());
//        ui::Helper::doLayout(view);
//        this->addChild(view, 1);
        if (auto mshome = (MSHome *)Director::getInstance()->getRunningScene()->getChildByName(MSHOME_NAME)) {
            view->setContentSize(mshome->getContentSize());
            ui::Helper::doLayout(view);
            mshome->addChild(view, kMSHomeZOrder_contentFull);
        }
        return;
    }
}

void MSTabStories::onReceivedEventGradeChanged(int grade_id) {
    _current_grade_id = grade_id;
    reloadData();
}

void MSTabStories::changeTextDisplayLang()
{
    _categories_collection->changeTextDisplayLang("stories.category.title");
    _popular_stories_collection->changeTextDisplayLang("stories.popular.title");
    _new_stories_collection ? _new_stories_collection->changeTextDisplayLang("stories.newstories.title") : (void) nullptr;
    if (_recent_stories_collection)
        _recent_stories_collection->changeTextDisplayLang("stories.recent.title");
}

void MSTabStories::goToPopularStories()
{
    if (_popular_stories_collection)
    {
        _popular_stories_collection->onMoreDefault();
    }
}

void MSTabStories::goToCategory(int catId)
{
    if (_categories_collection)
    {
        auto items = _categories_collection->getCollectionItems();
        
        for (auto item : items)
        {
            if (catId == ((MSCollectionCategoryItem *)item)->getCategoryInfo().category_id)
            {
                this->onSelectCollectionItem(item);
                return;
            }
        }
    }
}

void MSTabStories::onSelectMore(MSSimpleCollection *sender) 
{
    mj::helper::playButtonFX();
    MJDEFAULT->setStringForKey(key_read_from, "see more");
    MJDEFAULT->deleteValueForKey("key_read_from_the_second");

    auto type = sender->getCollectionType();

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);

    if (type == MSSimpleCollection::CollectionType::CATEGORY) 
	{
        auto explorer = MSExploreView_Categories::createView(_current_grade_id);
        explorer->setContentSize(this->getContentSize());
        explorer->setName("explorer");
        ui::Helper::doLayout(explorer);
        if (_mshome) {
            _mshome->addChild(explorer, kMSHomeZOrder_contentFull);
        }
    }
    
    std::vector<mj::model::StoryInfo> stories;
    if (type == MSSimpleCollection::CollectionType::STORY) 
	{
        if (sender->getCollectionOrder() != MSSimpleCollection::CollectionOrder::OTHER) {
            stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
            
            if (sender->getCollectionOrder() == MSSimpleCollection::CollectionOrder::POPULAR) {
                std::stable_sort(std::begin(stories), std::end(stories), [](mj::model::StoryInfo a, mj::model::StoryInfo b) {
                    if (a.quality != b.quality) {
                        return a.quality > b.quality;
                    }
                    
                    if (a.is_read != b.is_read) {
                        return (int)a.is_read < (int)b.is_read;
                    }
                    
                    if (a.date_published != b.date_published) {
                        return a.date_published > b.date_published;
                    }
                    return false;
                });
            } else if (sender->getCollectionOrder() == MSSimpleCollection::CollectionOrder::NEW) {
                std::stable_sort(std::begin(stories), std::end(stories), [](mj::model::StoryInfo a, mj::model::StoryInfo b) {
                    if (a.date_published != b.date_published) {
                        return a.date_published > b.date_published;
                    }
                    
                    if (a.quality != b.quality) {
                        return a.quality > b.quality;
                    }
                    
                    if (a.is_read != b.is_read) {
                        return (int)a.is_read < (int)b.is_read;
                    }
                    
                    return false;
                });
            }
        } else {
            stories = sender->getStories();
        }
        
//        this->enumerateChildren("//.*", [](Node * c) {
//            c->pause();
//            return false;
//        });
        
        auto explorer = MSExploreView::createView(sender->getTitle(), MSExploreView::CollectionType::NONE, _current_grade_id);
        explorer->setContentSize(Director::getInstance()->getVisibleSize());
        explorer->setTag(113114);// cannot use setName method , because MSExploreView override this method , now I don't have time to fix this
        ui::Helper::doLayout(explorer);
        if (_mshome) {
            _mshome->addChild(explorer, kMSHomeZOrder_contentFull);
        }
    }
}

void MSTabStories::clearUnusedContents()
{
    if(auto msSearchView = dynamic_cast<MSSearchView*>(this->getChildByName("search_view")))
    {
        msSearchView->hideVirtualKeyboard();
        msSearchView->removeFromParent();
    }

    if(auto msExploreView = this->getChildByTag(113114))
    {
        msExploreView->removeFromParent();
    }

    if(auto msExploreView = this->getChildByTag(113115))
    {
        msExploreView->removeFromParent();
    }

    if(auto msExploreView = this->getChildByName("explorer"))
    {
        msExploreView->removeFromParent();
    }
}

cocos2d::Vec2 MSTabStories::getGradePosition()
{
    auto panel_2 = utils::findChild(this, "Panel_2");
    
    if(auto gradeBox = utils::findChild(this, "gradebox"))
    {
        auto pos = gradeBox->getParent()->convertToWorldSpace(gradeBox->getPosition());
        
        return Vec2(pos.x-gradeBox->getContentSize().width/2,Director::getInstance()->getVisibleSize().height - panel_2->getContentSize().height*0.5f);//_searchbar->getParent()->convertToWorldSpace(_searchbar->getPosition());
    }
    
    return cocos2d::Vec2();
}

cocos2d::Size MSTabStories::getGradeSize()
{
    if(auto gradeBox = utils::findChild(this, "gradebox"))
    {       
       return gradeBox->getContentSize();
    }
    
    return cocos2d::Size();
}

void MSTabStories::setLevelInfoIOfStories(int levelId, int langId)
{

}

mj::model::LevelInfo MSTabStories::getLevelInfo(int levelId, int langId)
{
    std::string key = std::to_string(levelId) + std::to_string(langId);

    if (_levelInfoOfStoryItem.count(key) > 0)
    {
        return _levelInfoOfStoryItem.at(key);
    }

    mj::model::LevelInfo levelInfo;

    levelInfo = StoryDataModel::getInstance()->getLevelInfoById(levelId, langId);
    _levelInfoOfStoryItem.insert({ key, levelInfo });
    return levelInfo;
}
