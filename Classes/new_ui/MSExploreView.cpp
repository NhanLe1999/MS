//
//  MSExploreView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/10/17.
//

#include "MSExploreView.h"
#include "MSCollectionStoryItem.h"
#include "MSComplexCollection.h"
#include "MSGradeBox.h"
#include "APProfileManager.h"
#include "StoryDataModel.h"
#include "StoryInfoCover.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSTabFavourite.h"
#include "MJ_PageLoadResource.h"
#include "manager/CleverTapManager.h"
#include "MJPlatformConfig.h"
#include "MJAPIStoryDetails.h"
#include "MSPopularSearchItem.h"
#include "MJWorksheetNoResult.h"
#include "MSCacheManager.h"

USING_NS_CC;
INITIALIZE_READER(MSExploreView);

MSExploreView::~MSExploreView()
{
    _textFieldListener = nullptr;
    hideVirtualKeyboard();
    pushCleverTapTrackingData();
}

std::once_flag ms_explore_view_reader;

MSExploreView * MSExploreView::createView(std::string name, MSExploreView::CollectionType i_collectionType, int i_gradeID, int i_categoryID)
{
    std::string readFrom = MJDEFAULT->getStringForKey(key_read_from);
    if (readFrom != "see more" && readFrom != "Library")
    {
        MJDEFAULT->setStringForKey(key_read_from, "categories");
        MJDEFAULT->deleteValueForKey("key_read_from_the_second");
    }
    std::call_once(ms_explore_view_reader, []
    {
        REGISTER_CSB_READER(MSExploreView);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Popular_Stories.csb");//ExploreView.csb
    
    auto view = reinterpret_cast<MSExploreView *>(CSLoader::createNode(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB(name, i_collectionType, i_gradeID, i_categoryID);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MSExploreView::setTitle(const std::string& str_title)
{
    _title = str_title;
    if(auto title = utils::findChild<ui::Text *>(this, "title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-Bold.ttf");
        
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(str_title));
    }
    
    setContentSize(this->getContentSize());
}

void MSExploreView::hideSearchBar() {
    m_isSearchEnabled = false;
    
    if (auto searchBar = cocos2d::utils::findChild(this, "Searchfield_layout"))
    {
        searchBar->setVisible(false);
    }
    
    if (auto titleBar = cocos2d::utils::findChild(this, "Title_layout"))
    {
        titleBar->setVisible(true);
    }
}

void MSExploreView::hideVirtualKeyboard() {
    if (m_searchText) {
        m_searchText->didNotSelectSelf();
    }
}

void MSExploreView::searchText()
{
        if (!m_searchText) {
            return;
        }

        m_curSearchType = MSExploreView::CurrentSearchType::ORGANIC;

        /*
        // original search text
        auto originalInput = m_searchText->getString();
        // left trim
        m_searchText->setString(mj::helper::str_ltrim(originalInput));
        // cut 1 whitespace on the right if contain 2 whitespaces
        if (originalInput.length() > 1 && originalInput.substr(originalInput.length()-2, 2).compare("  ") == 0) {
            m_searchText->setString(originalInput.substr(0, originalInput.length()-1));
        }
         */

        auto query = m_searchText->getString();
        if(!m_popularSearchTemplate)
        {
            return;
        }

        if (query.empty())
        {
            _filtered_stories.clear();

            m_popularSearchTemplate->setVisible(true);
            m_button_clear_search->setVisible(false);

            return;
        }

        m_popularSearchTemplate->setVisible(false);
        m_button_clear_search->setVisible(true);

        if (query.empty()) {
            _filtered_stories.clear();
        }
        else {
            _filtered_stories = StoryDataModel::getInstance()->searchStoryByLanguage(StoryLanguageManager::getCurrentLangId(), query, _gradebox ? _gradebox->getSelectedGradeId() : -1);
        }

        this->hidePanelNoResult();

        reloadData();

        if (this->numberOfCells() == 0)
        {
            this->showPanelNoResult(query);
        }
}

void MSExploreView::changeDisplayLanguage()
{
}

void MSExploreView::didLoadFromCSB(std::string name, MSExploreView::CollectionType i_collectionType, int i_gradeID, int i_categoryID)
{
    mj::helper::SetFpsByNumberFrames(6);
    std::string readFrom = MJDEFAULT->getStringForKey(key_read_from);
    m_collectionType = i_collectionType;
    m_categoryID = i_categoryID;
    
    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender) {
        this->onBack(nullptr);
    });
    fake_layout->setName("fake_layout");
    this->addChild(fake_layout);
    
    _title = name;
    setTitle(_title);

    m_scope = ms::GradeManager::getInstance().getScope();
    
    auto gradebox_placeholder = utils::findChild(this, "gradebox");
    if(!gradebox_placeholder) {
        exit(-1);
    }
    
    _gradebox = MSGradeBox::createBox(false,i_gradeID);
    _gradebox->setPosition(gradebox_placeholder->getPosition());
    _gradebox->setAnchorPoint(gradebox_placeholder->getAnchorPoint());
    gradebox_placeholder->getParent()->addChild(_gradebox);
    gradebox_placeholder->removeFromParent();
    _gradebox->setDelegate(this);
    _gradebox->setName("gradebox");
    _gradebox->setEnableInteraction(false);
    
    _collection = MSComplexCollection::createCollection(this);
    utils::findChild(this, "collection")->addChild(_collection);
    _collection->setDelegate(this);
    
    m_button_clear_search = cocos2d::utils::findChild<ui::Button *>(this, "Button_clear");
    m_button_clear_search->setVisible(false);

    m_searchText = utils::findChild<ui::TextField*>(this, "text");
    m_searchText->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");
    m_searchText->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.material.placeholder.search.story"));
    m_searchText->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    m_searchText->setTextColor(Color4B::BLACK);
    m_searchText->setCursorEnabled(true);
    m_searchText->setCursorChar('|');
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    registerPressCtrlEventForTextField(m_searchText, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    m_searchText->addEventListener([=](Ref * sender, ui::TextField::EventType event) {
        
        if(_textFieldListener) {
            
            _textFieldListener(sender,event);
        }

    });
    
    ui::Helper::doLayout(this);
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);
        
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto push_clevertap_event = EventListenerCustom::create("push_clevertap_event_for_story_tab", [this](EventCustom *e) {
        this->pushCleverTapEvent_searchEvent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(push_clevertap_event, this);
    
    onReceivedEventGradeChanged(i_gradeID);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->setTitle(_title);
    }), this);
    
    auto event_should_close_listener = EventListenerCustom::create(key_event_search_page_close, [=](EventCustom *e) {
        this->removeFromParent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_should_close_listener, this);
}

void MSExploreView::setContentSize(const Size & size)
{
    ui::Layout::setContentSize(size);
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    auto title = utils::findChild<ui::Text *>(this, "title");
    
    if (dot && line && title) {
    auto last_x = title->getPositionX() + title->getContentSize().width;
        dot->setPosition(Point(last_x + 50, dot->getPositionY()));
        line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
        line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    }
    
    auto gradebox = utils::findChild(this, "gradebox");
    if (gradebox) {
        gradebox->setPosition(Point(size.width - gradebox->getContentSize().width - 25, gradebox->getPositionY()));
    }
    
    auto collection_layout = utils::findChild(this, "collection");
    if (collection_layout) {
        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));
    }
    
    if (_collection) {
        _collection->setContentSize(Size(size.width - 100, _collection->getContentSize().height));
        reloadData();
    }
}

void MSExploreView::runAnimationAppear()
{
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.3f));
}

void MSExploreView::runAnimationDisappear()
{
    _collection->runAnimationDisappear();
    this->runAction(Sequence::create(DelayTime::create(0.3f), FadeOut::create(0.5), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
}

void MSExploreView::onEnter()
{
    ui::Layout::onEnter();

    _textFieldListener = [this](Ref* sender, ui::TextField::EventType event) {

        if (m_searchText->getString().length() > 0)
        {
            MJDEFAULT->setStringForKey(key_read_from, "search bar");
            MJDEFAULT->deleteValueForKey("key_read_from_the_second");
        }
        if (m_searchText->getString().length() <= 0)
        {
            MJDEFAULT->setStringForKey(key_read_from, MJDEFAULT->getStringForKey(key_read_from));
        }

        switch (event)
        {
        case TextField::EventType::ATTACH_WITH_IME:
                if(!m_searchText)
                {
                    break;
                }
            if (m_searchText->getString().empty()) {
                initPopularSearch();
                if(!m_popularSearchTemplate || !m_button_clear_search)
                {
                    break;
                }
                m_popularSearchTemplate->setVisible(true);
                m_button_clear_search->setVisible(false);
            }
            m_searchText->setTextColor(Color4B::BLACK);
            break;
        case TextField::EventType::DETACH_WITH_IME:
            {
                if(!m_popularSearchTemplate || !m_searchText) break;
                if (!m_popularSearchTemplate->isVisible() && m_searchText->getString().length() && this->numberOfCells() > 0) {
                    m_popularSearchTemplate->setRecentSearches(m_searchText->getString());
                }
                m_searchText->setTextColor(Color4B::BLACK);
                break;
            }
        case TextField::EventType::INSERT_TEXT:
        {
            searchText();
        }
        case TextField::EventType::DELETE_BACKWARD:
        {
            searchText();
        }
        break;
        default:
            break;
        }

    };

    
    this->scheduleOnce([](float)
    {
        mj::helper::purgeTextureData();
    }, 0.25, "delay_purge_cached");
    
    runAnimationAppear();
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_delete_story", [=](EventCustom * e)
    {
        auto book = * reinterpret_cast<mj::model::StoryInfo *>(e->getUserData());
        CCLOG("removing book %s", book.name.c_str());
        
        
        if (MSTabFavourite::is_tab_downloaded) {
            CCLOG("remove downloaded stories");
            mj::PageLoadResource::deleteCacheOfStory(book.unique_id);
        } else {
            CCLOG("remove favourite stories");
            StoryDataModel::getInstance()->removeFavouriteStoryByLanguage(book.unique_id, book.lang_id);
        }
        
        auto it = std::find_if(_filtered_stories.begin(), _filtered_stories.end(), [book](mj::model::StoryInfo a) {
            return a.unique_id == book.unique_id;
        });
        
        if (it == _filtered_stories.end()) {
            return;
        }
        
        auto d = (int)std::distance(_filtered_stories.begin(), it);
        this->removeCellAtPosition(d);
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
    }), this);
}

void MSExploreView::onSelectSearchField() {
     
}

void MSExploreView::initPopularSearch()
{
	if (m_popularSearchTemplate) {
		return;
	}

	if (auto collection = utils::findChild(this, "collection")) {

		m_popularSearchTemplate = PopularSearchTemplate::createTemplate(PopularSearchTemplate::Type::Story);

		m_popularSearchTemplate->ignoreContentAdaptWithSize(false);

		m_popularSearchTemplate->setContentSize(collection->getContentSize());

		m_popularSearchTemplate->reloadData();

		m_popularSearchTemplate->setVisible(false);

		collection->addChild(m_popularSearchTemplate);

		m_popularSearchTemplate->setRecentItemClickCallback([=](Ref* sender) {

			if (auto button = dynamic_cast<ui::Button *>(sender)) {

				auto keyword = button->getTitleText();
				m_searchText->setString(keyword);
                if (keyword.length() > 0)
                {
                    MJDEFAULT->setStringForKey(key_read_from, "search bar");
                }
                m_curSearchType = MSExploreView::CurrentSearchType::RECENT;
                m_button_clear_search->setVisible(true);
				m_popularSearchTemplate->setVisible(false);
				_filtered_stories = StoryDataModel::getInstance()->searchStoryByLanguage(StoryLanguageManager::getCurrentLangId(), keyword, _gradebox ? _gradebox->getSelectedGradeId() : -1);
				this->reloadData();
                
                if (_filtered_stories.size()) {
                    m_popularSearchTemplate->setRecentSearches(keyword);
                }
			}
		});

		m_popularSearchTemplate->setPopularItemClickCallback([=](Ref* sender) {

			if (auto popItem = dynamic_cast<MJPopularSearchItem*>(sender)) {

				auto keyword = popItem->getKeyword();
				m_searchText->setString(keyword);
                MJDEFAULT->setStringForKey(key_read_from, "search bar");
                searchText();
                m_curSearchType = MSExploreView::CurrentSearchType::POPULAR;
				m_searchText->setTextColor(Color4B::BLACK);
				_filtered_stories = StoryDataModel::getInstance()->searchStoryByLanguage(StoryLanguageManager::getCurrentLangId(), keyword, _gradebox ? _gradebox->getSelectedGradeId() : -1);
                m_button_clear_search->setVisible(true);
				m_popularSearchTemplate->setVisible(false);
				this->reloadData();
                
                if (_filtered_stories.size()) {
                    m_popularSearchTemplate->setRecentSearches(keyword);
                }
			}
		});
	}
}

ui::Widget::ccWidgetClickCallback MSExploreView::onLocateClickCallback(const std::string & callback_name)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(MSExploreView, onBack),
        CLICK_MAP(MSExploreView, onClearSearch),
        CLICK_MAP(MSExploreView, onEdit)
    };
    
    auto backkey_listener = EventListenerKeyboard::create();
    
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event)
    {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    
    if (function_map.count(callback_name) > 0)
    {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSExploreView::onBack(Ref * sender)
{
    mj::helper::SetFpsByNumberFrames(10);
    ms::CacheManager::getInstance().clearListStoryInfo();
    if (MJDEFAULT->getStringForKey(key_read_from) != "Library")
    {
        MJDEFAULT->setStringForKey(key_read_from, "new");
    }
    mj::helper::playButtonFXClose();
    
    if (m_isSearchEnabled == false)
    {
        // code me here
    }
    else if (m_popularSearchTemplate && m_popularSearchTemplate->isVisible())
    {
        // đang ở màn hình popular searches --> quay lại màn hình chính
        onReceivedEventGradeChanged(_gradebox->getSelectedGradeId());
		m_popularSearchTemplate->setVisible(false);
        this->hidePanelNoResult();
		return;
        
    }
    else if (m_popularSearchTemplate && !m_searchText->getString().empty())
    {
        // đang ở màn hình search --> quay lại màn hình popular searches
        if (m_searchText->getString().length() && _filtered_stories.size())
        {
            m_popularSearchTemplate->setRecentSearches(m_searchText->getString());
        }
        m_popularSearchTemplate->setVisible(true);
        m_popularSearchTemplate->reloadData();
        m_button_clear_search->setVisible(false);
        m_searchText->setString("");
        hideVirtualKeyboard();
        hidePanelNoResult();
        return;
    }

    this->getParent()->enumerateChildren("//.*", [](Node * c)
    {
        c->resume();
        return false;
    });
    
    this->runAnimationDisappear();
}

void MSExploreView::onClearSearch(Ref* sender) {
    
    if (m_searchText->getString().length() && _filtered_stories.size()) {
        m_popularSearchTemplate->setRecentSearches(m_searchText->getString());
        m_popularSearchTemplate->reloadData();
    }
    
    m_button_clear_search->setVisible(false);
    
    m_searchText->setString("");
    
    m_popularSearchTemplate->setVisible(true);
    
    this->hideVirtualKeyboard();
    this->hidePanelNoResult();
}

int MSExploreView::numberOfCells()
{
    return (int)_filtered_stories.size();
}

int MSExploreView::numberOfColumns()
{
    auto nr = (int) ((collectionSize().width + 10) / (modelSize().width + 10));
    return nr;
}

ui::Widget * MSExploreView::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned)
{
    auto index = row * numberOfColumns() + col;
    
    if (_ui_source.empty())
    {
        return nullptr;
    }
    
    if (index < 0 || index >= _ui_source.size())
    {
        return nullptr;
    }
    
    if (_ui_source[index])
    {
        
    }
    else
    {
        _ui_source[index] = MSCollectionStoryItem::createItem(_filtered_stories[index]);
        _ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        if (_is_editing)
        {
            reinterpret_cast<MSCollectionStoryItem *>(_ui_source[index])->enableDeleteMode();
        }
    }
    
    return _ui_source[index];
}

Size MSExploreView::modelSize()
{
    return Size(170, 270) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

Size MSExploreView::collectionSize()
{
    return utils::findChild(this, "collection")->getContentSize();
}

void MSExploreView::offLoadCellAt(int index)
{
    if (index < 0 || index >= _ui_source.size())
    {
        return;
    }
    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
    _ui_source[index] = nullptr;
}

void MSExploreView::reloadData()
{
    _ui_source.clear();
    _ui_source.resize(_filtered_stories.size(), nullptr);
    _collection->reloadData();
}

void MSExploreView::removeCellAtPosition(int index)
{
    if (_ui_source.size() <= index)
    {
        return;
    }
    
    auto item = _ui_source[index];
    
    if (item)
    {
        for (auto i = index + 1; i < _ui_source.size(); ++i)
        {
            if (_ui_source[i] && _ui_source[i - 1])
            {
                _ui_source[i]->runAction(MoveTo::create(0.25, _ui_source[i - 1]->getPosition()));
            }
        }
        
        item->removeFromParent();
        _ui_source.erase(_ui_source.begin() + index);
        _filtered_stories.erase(_filtered_stories.begin() + index);
    }
}

void MSExploreView::onSelectGradeBox()
{
    
}

void MSExploreView::onDeselectGradeBox(int grade_id)
{

}
void MSExploreView::onSelectCollectionItem(cocos2d::ui::Widget * item)
{
    if (!item)
    {
        return;
    }
    
    auto story_item = dynamic_cast<MSCollectionStoryItem *>(item);
    
    if (!story_item)
    {
        return;
    }
    
	auto info = story_item->getStoryInfo();
    
    MJDEFAULT->setStringForKey("STORY_START_FROM", "story_search_tab");

    m_clickedStoryInfo  = info; // bị trùng chức năng với mấy biến ở dưới nè, giữ lại thằng này, cơ mà đang không rảnh để sửa
	m_itemName			= info.name;
	m_isItemSelected	= true;
	m_itemID			= info.story_id;

    auto storyinfocover = static_cast<StoryInfoCover *>(CSLoader::createNode("csb/StoryInfoCover.csb"));
        StoryInfoObj obj;
        obj.name = info.name;
        obj.mostpopular = info.total_words;
        obj.readinglevel = info.levels[0];
        obj.languageid = info.lang_id;
        obj.storyid = info.story_id;
        obj.newest = info.unique_id;
        obj.total_page = info.total_page;
        obj.word_count = info.total_words;
        obj.lexile = info.lexile;
        obj._id = info.unique_id;
        obj._storyid = info.story_id;
        obj._version = info.version;
        obj.downloaded = false;
        obj.thumbnail = info.thumbnail_url;
        obj.written = info.author;
        obj.design = info.designer;
        obj.image = info.thumbnail_url;
        storyinfocover->initWithData(obj, false);
    
    
    auto collection_wrapper = utils::findChild<ui::Widget *>(this, "collection");

    
    mj::helper::playTransitionStoryCover(story_item, collection_wrapper, -1,INT_MAX-1);
}

void MSExploreView::onReceivedEventGradeChanged(int grade_id)
{
    // hàm này có chức năng load/reset dữ liệu
    if (m_collectionType == CollectionType::SERIES) {
        if (m_categoryID > 0) {
            _filtered_stories = StoryDataModel::getInstance()->getStoriesByCategory(m_categoryID, m_searchText->getString(), _gradebox ? _gradebox->getSelectedGradeId() : -1);
        }
    } else if (m_collectionType == CollectionType::READED) {
        _filtered_stories.clear();
        int grade_id = _gradebox ? _gradebox->getSelectedGradeId() : -1;
        auto downloadedStories = PROFILE_MANAGER.getInstance().getAllReadStories(MJDEFAULT->getIntegerForKey(key_current_profile_id,-1));
        for (auto idx : downloadedStories) {
            auto story = StoryDataModel::getInstance()->getStoryById(idx);
            if (story.story_id != 0) {  // kiểm tra story_id != 0 thì mới push_back
                if (grade_id == -1 || story.levels.front() == grade_id) {
                    if(_filtered_stories.size() >= 50) return;
                    _filtered_stories.push_back(story);
                }
            }
        }
    } else if (m_collectionType == CollectionType::DOWNLOADED) {
        _filtered_stories.clear();
        auto downloaded = StoryDataModel::getInstance()->getAllStoryDownloaded();
        for (auto idx : downloaded) {
            auto story = StoryDataModel::getInstance()->getStoryById(idx);
            _filtered_stories.push_back(story);
        }
    } else if (m_collectionType == CollectionType::FAVOURITE) {
        // nothing now
    } else {
        _filtered_stories = StoryDataModel::getInstance()->searchStoryByLanguage(StoryLanguageManager::getCurrentLangId(), m_searchText->getString(), _gradebox ? _gradebox->getSelectedGradeId() : -1);
    }

    if (m_collectionType != CollectionType::READED)
    {
        std::stable_sort(std::begin(_filtered_stories), std::end(_filtered_stories), [](mj::model::StoryInfo a, mj::model::StoryInfo b) {
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
    }
    
    reloadData();
    
    // check xem có đang search và không có kết quả không
    auto gradeID = _gradebox ? _gradebox->getSelectedGradeId() : -1;
    std::string nameGrade = _gradebox->getGradeName((int)gradeID);
    if (m_searchText->getString().length() == 0 && _filtered_stories.size() == 0) {
        this->showPanelNoResult(m_searchText->getString(), nameGrade);
    } else {
        if (_ui_source.size() == 0)
        {
            this->showPanelNoResult(m_searchText->getString(), nameGrade);
        }
        else
        {
            this->hidePanelNoResult();
        }
    }
}

void MSExploreView::pushCleverTapTrackingData()
{
	if (this->_tag == 113115)
	{
		CCLOG("this catagory !");
		return;
	}

	try
	{
		CleverTapManager::GetInstance()->pushEvent("click_see_more_button",
			{
				{"button_name",cocos2d::Value(MJDEFAULT->getStringForKey("see_more_button_name", "unknow"))},
				{"category_name",cocos2d::Value(MJDEFAULT->getStringForKey("see_more_category_name", "unknow"))},
				{"select_item",cocos2d::Value(m_isItemSelected)},
				{"item_id",cocos2d::Value(m_itemID)},
				{"item_name",cocos2d::Value(m_itemName)},
				{"feature_name",cocos2d::Value(MJDEFAULT->getStringForKey("see_more_feature_name", "unknow"))},
			});
	}
	catch (const std::exception&)
	{

	}
}

void MSExploreView::onEdit(cocos2d::Ref *sender)
{
    if (!_is_editing)
    {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionStoryItem *>(item);
                casted->enableDeleteMode();
            }
        }
        if(auto labelEdit = utils::findChild<ui::Text *>(this, "label_edit")){
            labelEdit->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favorite.done"));
        }
        
    }
    else
    {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionStoryItem *>(item);
                casted->disableDeleteMode();
            }
        }
        utils::findChild<cocos2d::ui::Text*>(this, "label_edit")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_saved_title_edit));
    }
    _is_editing = !_is_editing;
}

void MSExploreView::enableEditMode(std::string str)
{
    _saved_title_edit = str;
    if (auto fake = utils::findChild(this, "fake_layout"))
    {
        fake->setPosition(Point(0, this->getContentSize().height - 360));
    }
    
    auto bt_edit = utils::findChild(this, "button_edit");
    
    if (bt_edit)
    {
        bt_edit->setVisible(true);
        utils::findChild<cocos2d::ui::Text*>(this, "label_edit")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_saved_title_edit));
    }
    
    auto gradebox = utils::findChild(this, "gradebox");
    
    if (gradebox)
    {
        gradebox->setVisible(false);
    }
    
    onEdit(nullptr);
}

void MSExploreView::onExit()
{
    ui::Layout::onExit();
 
    _textFieldListener = nullptr;
    
    Director::getInstance()->getScheduler()->schedule([](float)
    {
        mj::helper::purgeTextureData();
    }, Application::getInstance(), 0, 0, 1.25, false, "delay_purge_cached_2");
    
}

void MSExploreView::showPanelNoResult(const std::string& keyword, std::string gradeName)
{
    if (_panel_noResult == nullptr)
    {
        if (auto collection = cocos2d::utils::findChild(this, "collection"))
        {
            _panel_noResult = MJWorksheetNoResult::createView(keyword, gradeName);
            _panel_noResult->setAnchorPoint(Point::ANCHOR_MIDDLE);
            _panel_noResult->setPosition(Vec2(Director::getInstance()->getRunningScene()->getContentSize()/2));
            _panel_noResult->setContentSize(Director::getInstance()->getRunningScene()->getContentSize());
            _panel_noResult->setVisible(!MJDEFAULT->getBoolForKey(key_click_on_delete_stories));
            if (gradeName != LANGUAGE_MANAGER->getDisplayTextByKey("key.all.grades"))
            {
                _panel_noResult->setVisible(true);
            }
            ui::Helper::doLayout(_panel_noResult);
            collection->addChild(_panel_noResult);
        }
    }
    _panel_noResult->setKeyword(keyword);
    _panel_noResult->stopAllActions();
    _panel_noResult->setOpacity(0);
    _panel_noResult->runAction(FadeIn::create(0.25f));
}

void MSExploreView::hidePanelNoResult()
{
    if (_panel_noResult)
    {
        _panel_noResult->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([=](){
            _panel_noResult->removeFromParent();
            _panel_noResult = nullptr;
        }), NULL));
    }
}

void MSExploreView::pushCleverTapEvent_searchEvent()
{
    try
    {
        if (m_searchText->getString().empty()) {
            return;
        }
        
        std::string searchTypeStr = "organic";
        if (m_curSearchType == MSExploreView::CurrentSearchType::RECENT) {
            searchTypeStr = "recent";
        } else if (m_curSearchType == MSExploreView::CurrentSearchType::POPULAR) {
            searchTypeStr = "popular";
        } else {
            // nothing here
        }
        
        CleverTapManager::GetInstance()->pushEvent("search_event",
        {
            {"searching_tags",cocos2d::Value(m_searchText->getString()) },
            {"choose_item",cocos2d::Value(true)},
            {"item_id",cocos2d::Value(m_clickedStoryInfo.story_id)},
            {"item_name", cocos2d::Value(m_clickedStoryInfo.name)},
            {"type",cocos2d::Value("story") },
            {"search_type",cocos2d::Value(searchTypeStr) }
        });
    }
    catch (const std::exception&)
    {
    }
}
