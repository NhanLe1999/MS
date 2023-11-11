//
//  MSExploreView_Audiobooks.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/27/17.
//

#include "MSExploreView_Audiobooks.h"

#include "MSCollectionAudiobookItem.h"
#include "MSComplexCollection.h"
#include "MSGradeBox.h"
#include "APProfileManager.h"
#include "StoryDataModel.h"
#include "StoryInfoCover.h"
#include "MJDefinitions.h"
#include "MSExploreView.h"
#include "MSAudiobookPlayer.h"
#include "MJPlatformConfig.h"
#include "MSTabFavourite.h"
#include "MJ_PageLoadResource.h"
#include "manager/CleverTapManager.h"
#include "MJPlatformConfig.h"
#include "MSPopularSearchItem.h"
#include "MJWorksheetNoResult.h"
#include "MSCacheManager.h"

USING_NS_CC;
INITIALIZE_READER(MSExploreView_Audiobooks);

std::once_flag ms_explore_view_audiobooks_reader;

MSExploreView_Audiobooks * MSExploreView_Audiobooks::createView(std::string name, MSExploreView_Audiobooks::CollectionType i_collectionType, int i_gradeId, const mj::model::AudiobookSeriesInfo& i_seriesInfo)
{
    std::call_once(ms_explore_view_audiobooks_reader, [] {
        REGISTER_CSB_READER(MSExploreView_Audiobooks);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/ExploreView_Audiobooks.csb");
    
    auto view = reinterpret_cast<MSExploreView_Audiobooks *>(CSLoader::createNode(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB(name, i_collectionType, i_gradeId, i_seriesInfo);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    
    return nullptr;
}

void MSExploreView_Audiobooks::setTitle(const std::string& str_title)
{
    _title = str_title;
    if(auto title = utils::findChild<ui::Text *>(this, "title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-Bold.ttf");

        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(str_title));
    }
    setContentSize(this->getContentSize());
}

void MSExploreView_Audiobooks::hideSearchBar() {
    m_isSearchEnabled = false;
    
    if (auto searchBar = cocos2d::utils::findChild(this, "searchField"))
    {
        searchBar->setVisible(false);
    }
    
    if (auto titleBar = cocos2d::utils::findChild(this, "Title_layout"))
    {
        titleBar->setVisible(true);
    }
}

void MSExploreView_Audiobooks::hideVirtualKeyboard() {
    if (textField) {
        textField->didNotSelectSelf();
    }
}

void MSExploreView_Audiobooks::searchText()
{
        if (!textField) {
            return;
        }

        m_curSearchType = MSExploreView_Audiobooks::CurrentSearchType::ORGANIC;

        auto query = textField->getString();

        if (query.empty())
        {
            _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), query, (_gradebox ? _gradebox->getSelectedGradeId() : -1));

            m_popularSearchTemplate->setVisible(true);
            m_button_clear_search->setVisible(false);

            return;
        }

        m_popularSearchTemplate->setVisible(false);
        m_button_clear_search->setVisible(true);

        if (query.empty()) {
            _filtered_audiobooks.clear();
        }
        else {
            _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), query, (_gradebox ? _gradebox->getSelectedGradeId() : -1));
        }

        this->hidePanelNoResult();

        reloadData();

        if (this->numberOfCells() == 0)
        {
            this->showPanelNoResult(query);
        }
}

void MSExploreView_Audiobooks::didLoadFromCSB(std::string name, MSExploreView_Audiobooks::CollectionType i_collectionType, int i_gradeId, const mj::model::AudiobookSeriesInfo& i_seriesInfo)
{
    mj::helper::SetFpsByNumberFrames(6);
    _readFrom = MJDEFAULT->getStringForKey(key_read_from_of_audiobook);
    m_collectionType = i_collectionType;
    m_seriesInfo = i_seriesInfo;
    
    m_button_clear_search = cocos2d::utils::findChild<ui::Button *>(this, "eraseButton");
    m_button_clear_search->setVisible(false);
    
    textField = utils::findChild<ui::TextField*>(this, "TextField_2");
    textField->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");
    textField->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.material.placeholder.search.audiobook"));
    textField->setPlaceHolderColor(Color4B(0, 0, 0, 64));
    textField->setTextColor(Color4B::BLACK);
    textField->setCursorEnabled(true);
    textField->setCursorChar('|');
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    registerPressCtrlEventForTextField(textField, this);
#endif // (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	textField->addEventListener([=](Ref * sender, ui::TextField::EventType event) {
        if (textField->getString().length() > 0)
        {
            MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "search bar");
        }

        if (textField->getString().length() <= 0)
        {
            MJDEFAULT->setStringForKey(key_read_from_of_audiobook, _readFrom);
        }

		switch (event)
		{
		case TextField::EventType::ATTACH_WITH_IME:
        {
            if (!textField)
            {
                break;
            }
            if (textField->getString().empty()) {
                initPopularSearch();
                if (m_popularSearchTemplate && m_button_clear_search)
                {
                    m_popularSearchTemplate->setVisible(true);
                    m_button_clear_search->setVisible(false);
                }
            }
            textField->setTextColor(Color4B::BLACK);
            break;
        }
		case TextField::EventType::DETACH_WITH_IME:
            if (m_popularSearchTemplate && textField)
            {
                if (!m_popularSearchTemplate->isVisible() && textField->getString().length() && this->numberOfCells() > 0) {
                    m_popularSearchTemplate->setRecentSearches(textField->getString());
                }
                textField->setTextColor(Color4B::BLACK);
            }
			break;
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

	});


    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height - 240));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender) {
        this->onBack(nullptr);
    });
    this->addChild(fake_layout);

    _current_grade_id = i_gradeId;
    _title = name;
    
    m_scope = ms::GradeManager::Scope::AUDIOBOOKS;

    utils::findChild(this, "button_play_all")->setCascadeColorEnabled(false);
    
    utils::findChild(this, "button_edit")->setVisible(false);
    
    setTitle(_title);
    
    auto gradebox_placeholder = utils::findChild(this, "gradebox");
    _gradebox = MSGradeBox::createBox(false,i_gradeId);
    _gradebox->setPosition(gradebox_placeholder->getPosition());
    _gradebox->setAnchorPoint(gradebox_placeholder->getAnchorPoint());
    gradebox_placeholder->getParent()->addChild(_gradebox);
    gradebox_placeholder->removeFromParent();
    _gradebox->setDelegate(this);
    _gradebox->setName("gradebox");
    _gradebox->setEnableInteraction(false);
    
    auto title_play_all = utils::findChild<ui::Text*>(this, "title_play_all");

    if (title_play_all)
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? title_play_all->setFontName("fonts/leelawdb.ttf") : title_play_all->setFontName("fonts/Montserrat-SemiBold.ttf");

        title_play_all->setString(LANGUAGE_MANAGER->getDisplayTextByKey("collection.play.all"));
        title_play_all->getParent()->setContentSize(Size(title_play_all->getContentSize().width + 50, title_play_all->getParent()->getContentSize().height));
        ui::Helper::doLayout(title_play_all->getParent());
    }

    _collection = MSComplexCollection::createCollection(this);
    utils::findChild(this, "collection")->addChild(_collection);
    _collection->setDelegate(this);
    
    ui::Helper::doLayout(this);
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }

//        auto grade_id = reinterpret_cast<int *>(e->getUserData());
//        this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    onReceivedEventGradeChanged(i_gradeId);
    
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->setTitle(_title);
    }), this);
    
    auto push_clevertap_event = EventListenerCustom::create("push_clevertap_event_for_audiobook_tab", [this](EventCustom *e) {
        this->pushCleverTapEvent_searchEvent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(push_clevertap_event, this);
    
    auto event_should_close_listener = EventListenerCustom::create(key_event_search_page_close, [=](EventCustom *e) {
        this->removeFromParent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_should_close_listener, this);
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    utils::findChild(this, "button_play_all")->setVisible(false);
#endif
    
}

void MSExploreView_Audiobooks::setContentSize(const Size & size)
{
    auto visiable = Director::getInstance()->getVisibleSize();
    ui::Layout::setContentSize(visiable);
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    auto title = utils::findChild<ui::Text *>(this, "title");
    auto titleLayout = utils::findChild<ui::Layout*>(this, "Title_layout");
    if (dot && line && title && titleLayout) {
        titleLayout->setContentSize(Size(visiable.width, titleLayout->getContentSize().height));
        auto last_x = title->getPositionX() + title->getContentSize().width;
        dot->setPosition(Point(last_x + 25, dot->getPositionY()));
        line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
        line->setContentSize(Size(visiable.width - last_x - 100, line->getContentSize().height));

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

void MSExploreView_Audiobooks::runAnimationAppear() {
    
    this->setCascadeOpacityEnabled(true);
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.3f));
}

void MSExploreView_Audiobooks::runAnimationDisappear() {
    this->runAction(Sequence::create(FadeOut::create(0.3f), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
}

MSExploreView_Audiobooks::~MSExploreView_Audiobooks()
{
    hideVirtualKeyboard();
	pushCleverTapTrackingData();
}

void MSExploreView_Audiobooks::onEnter() 
{
    cocos2d::ui::Layout::onEnter();
    reloadData();
    runAnimationAppear();
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_delete_audiobook", [=](EventCustom * e) {
        auto book = * reinterpret_cast<mj::model::AudiobookInfo *>(e->getUserData());
        
        if (MSTabFavourite::is_tab_downloaded) {
            CCLOG("remove downloaded audiobooks");
            mj::PageLoadResource::deleteCacheOfAudiobook(book.book_id);
        } else {
            CCLOG("remove favourite audiobooks");
            StoryDataModel::getInstance()->removeFavouriteAudiobookByLanguage(book.book_id, book.lang_id);
        }
        
        auto it = std::find_if(_filtered_audiobooks.begin(), _filtered_audiobooks.end(), [book](mj::model::AudiobookInfo a) {
            return a.book_id == book.book_id;
        });
        
        if (it == _filtered_audiobooks.end()) {
            return;
        }
        
        auto d = (int)std::distance(_filtered_audiobooks.begin(), it);
        this->removeCellAtPosition(d);
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
    }), this);
}

ui::Widget::ccWidgetClickCallback MSExploreView_Audiobooks::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSExploreView_Audiobooks, onBack),
        CLICK_MAP(MSExploreView_Audiobooks, onEdit),
        CLICK_MAP(MSExploreView_Audiobooks, onPlayAll),
        CLICK_MAP(MSExploreView_Audiobooks, clearTextFeild),
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSExploreView_Audiobooks::onBack(Ref * sender) {

    ms::CacheManager::getInstance().clearListAudiobookInfo();

    MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "new");

    mj::helper::playButtonFXClose();

    if (m_isSearchEnabled == false) {
        // code me here
    } else if (m_popularSearchTemplate && m_popularSearchTemplate->isVisible()) {
        // đang ở màn hình popular searches --> quay lại màn hình chính
        onReceivedEventGradeChanged(_gradebox->getSelectedGradeId());
		m_popularSearchTemplate->setVisible(false);
        this->hidePanelNoResult();
		return;
        
    } else if (m_popularSearchTemplate && !textField->getString().empty()) {
        // đang ở màn hình search --> quay lại màn hình popular saerches
        if (textField->getString().length() && _filtered_audiobooks.size()) {
            m_popularSearchTemplate->setRecentSearches(textField->getString());
        }
        m_popularSearchTemplate->setVisible(true);
        m_popularSearchTemplate->reloadData();
        m_button_clear_search->setVisible(false);
        textField->setString("");
        hideVirtualKeyboard();
        hidePanelNoResult();
        return;
    }

    runAnimationDisappear();
}
void MSExploreView_Audiobooks::clearTextFeild(Ref* sender) {
    if (textField->getString().length() && _filtered_audiobooks.size()) {
        m_popularSearchTemplate->setRecentSearches(textField->getString());
        m_popularSearchTemplate->reloadData();
    }
    
    _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), "", (_gradebox ? _gradebox->getSelectedGradeId() : -1));
    
    m_button_clear_search->setVisible(false);
    textField->setString("");
    m_popularSearchTemplate->setVisible(true);
    this->hideVirtualKeyboard();
    this->hidePanelNoResult();
}
#include "MSPlaylistPrepare.h"
void MSExploreView_Audiobooks::onPlayAll(cocos2d::Ref *sender) {
    if (!CONFIG_MANAGER->isAppActive()) {
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("sub.popup.feature.free"), "OK", "");
        return;
    }

    ms::GradeManager::getInstance().setScope(m_scope);
    auto explorer = MSPlaylist_Prepare::createView(_title, _series_name, _filtered_audiobooks, _current_grade_id);
    explorer->setContentSize(this->getContentSize());
    ui::Helper::doLayout(explorer);
    this->addChild(explorer, 1);
}

int MSExploreView_Audiobooks::numberOfCells() {
    return (int)_filtered_audiobooks.size();
}

int MSExploreView_Audiobooks::numberOfColumns() {
    return (int) ((collectionSize().width + 10) / (modelSize().width + 10));
}

ui::Widget * MSExploreView_Audiobooks::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    auto index = row * numberOfColumns() + col;
    if (_ui_source.empty()) {
        return nullptr;
    }
    
    if (index < 0 || index >= _ui_source.size()) {
        return nullptr;
    }
    if (_ui_source[index]) {
    } else {
        _ui_source[index] = MSCollectionAudiobookItem::createThumb(_filtered_audiobooks[index]);
        _ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        if (_is_editing) {
            reinterpret_cast<MSCollectionAudiobookItem *>(_ui_source[index])->enableDeleteMode();
        }
    }
    
    return _ui_source[index];
}

void MSExploreView_Audiobooks::removeCellAtPosition(int index) {
    if (_ui_source.size() <= index) {
        return;
    }
    
    auto item = _ui_source[index];
    if (item) {
        for (auto i = index + 1; i < _ui_source.size(); ++i) {
            if (_ui_source[i]) {
                _ui_source[i]->runAction(MoveTo::create(0.25, _ui_source[i - 1]->getPosition()));
            }
        }
        
        item->removeFromParent();
        _ui_source.erase(_ui_source.begin() + index);
        _filtered_audiobooks.erase(_filtered_audiobooks.begin() + index);
    }
}

Size MSExploreView_Audiobooks::modelSize() {
    return Size(240, 270) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

Size MSExploreView_Audiobooks::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

void MSExploreView_Audiobooks::offLoadCellAt(int index) {
    if (index < 0 || index >= _ui_source.size()) {
        return;
    }
    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
    _ui_source[index] = nullptr;
}

void MSExploreView_Audiobooks::reloadData() {
    _ui_source.clear();
    _ui_source.resize(_filtered_audiobooks.size(), nullptr);
    _collection->reloadData();
}

#include "MSChapterPrepare.h"
void MSExploreView_Audiobooks::onSelectCollectionItem(cocos2d::ui::Widget * item) {
    if (!item) {
        return;
    }
    
    auto audiobook_item = dynamic_cast<MSCollectionAudiobookItem *>(item);
    
    if (audiobook_item) 
	{
        auto info = audiobook_item->getInfo();
        
        MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "audiobook_search_tab");

        m_clickedAudiobookInfo = info; // bị trùng chức năng với mấy biến ở dưới nè, giữ lại thằng này, cơ mà đang không rảnh để sửa
		m_itemName = info.name;//for clevertap tracking.
		m_isItemSelected = true;//for clevertap tracking.
		m_itemID = info.book_id;//for clevertap tracking.

        if (StoryDataModel::getInstance()->isAudiobookChapter(info.book_id)) 
		{
            auto list_chapter = StoryDataModel::getInstance()->getChapterOfAudiobook(info.book_id);
            auto view = MSChapter_Prepare::createView(info, list_chapter);
            view->setContentSize(this->getContentSize());
            ui::Helper::doLayout(view);
            this->addChild(view, 1);
        } 
		else 
		{
            auto bb = audiobook_item->getBoundingBox();
            auto initial_position = bb.origin + bb.size / 2;
            initial_position = audiobook_item->getParent()->convertToWorldSpace(initial_position);
            
            auto collection_wrapper = utils::findChild<ui::Widget *>(this, "collection");
            mj::helper::playTransitionAudiobookCover(audiobook_item, collection_wrapper, 1);
        }
    }
}

void MSExploreView_Audiobooks::onEdit(cocos2d::Ref *sender) {
    if (!_is_editing) {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionAudiobookItem *>(item);
                casted->enableDeleteMode();
            }
        }
//        utils::findChild<ui::Text *>(this, "label_edit")->setString("Done");
        utils::findChild<ui::Text *>(this, "label_edit")->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favorite.done"));
    } else {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionAudiobookItem *>(item);
                casted->disableDeleteMode();
            }
        }
//        utils::findChild<ui::Text *>(this, "label_edit")->setString("Edit");
        utils::findChild<ui::Text *>(this, "label_edit")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_saved_edit_title));
    }
    _is_editing = !_is_editing;
}

void MSExploreView_Audiobooks::enableEditMode(std::string str) {
    _saved_edit_title = str;
    auto fake = utils::findChild(this, "fake_layout");
    if (fake)
        fake->setPosition(Point(0, this->getContentSize().height - 360));
    
    auto bt_edit = utils::findChild(this, "button_edit");
    if (bt_edit) {
        bt_edit->setVisible(true);
        utils::findChild<ui::Text *>(this, "label_edit")->setString(LANGUAGE_MANAGER->getDisplayTextByKey(_saved_edit_title));
    }
    auto gradebox = utils::findChild(this, "gradebox");
    if (gradebox) {
        gradebox->setVisible(false);
    }
    
    onEdit(nullptr);
}

void MSExploreView_Audiobooks::onReceivedEventGradeChanged(int grade_id) {
    // hàm này có chức năng load/reset dữ liệu
    if (m_collectionType == CollectionType::SERIES) {
        if (m_seriesInfo.books.size()) {
            _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookFromVector(StoryDataModel::getInstance()->getAllAudiobooksByBookIDs(m_seriesInfo.books),
                                                                                            textField->getString(),
                                                                                            _gradebox->getSelectedGradeId());
        }
    } else if (m_collectionType == CollectionType::READED) {
        _filtered_audiobooks.clear();
        auto readAudiobooks = PROFILE_MANAGER.getInstance().getAllReadAudiobooks(MJDEFAULT->getIntegerForKey(key_current_profile_id,-1));
        for(auto audioID : readAudiobooks) {
            auto audiobook = StoryDataModel::getInstance()->getAudiobook(audioID);
            if (audiobook.book_id != 0) {
                _filtered_audiobooks.push_back(audiobook);
            }
        }
        _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookFromVector(_filtered_audiobooks,
                                                                                        textField->getString(),
                                                                                        _gradebox->getSelectedGradeId());
    } else if (m_collectionType == CollectionType::DOWNLOADED) {
        _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookFromVector(StoryDataModel::getInstance()->getAllAudiobooksDownloaded(),
                                                                                        textField->getString(),
                                                                                        _gradebox->getSelectedGradeId());
    } else if (m_collectionType == CollectionType::FAVOURITE) {
        // nothing now
    } else {
        _filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(),
                                                                                        textField->getString(),
                                                                                        _gradebox->getSelectedGradeId());
    }
    
    if (m_collectionType != CollectionType::READED)
    {
        std::stable_sort(_filtered_audiobooks.begin(), _filtered_audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool {
            if (a.date_publish != b.date_publish) {
                return a.date_publish > b.date_publish;
            }

            if (a.quality != b.quality) {
                return a.quality > b.quality;
            }

            if (a.read != b.read) {
                return (int)a.read < (int)b.read;
            }

            return a.name.compare(b.name) < 0;
            });
    }

    _current_grade_id = grade_id;
    
    reloadData();
    
    // check xem có đang search và không có kết quả không
    if (textField->getString().length() && _filtered_audiobooks.size() == 0) {
        this->showPanelNoResult(textField->getString());
    } else {
        this->hidePanelNoResult();
    }
}

void MSExploreView_Audiobooks::pushCleverTapTrackingData()
{
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

void MSExploreView_Audiobooks::onSelectGradeBox() 
{
    
}

void MSExploreView_Audiobooks::onDeselectGradeBox(int grade_id) {
    
}

void MSExploreView_Audiobooks::onExit() {
    ui::Layout::onExit();
    mj::helper::SetFpsByNumberFrames(10);
    Director::getInstance()->getScheduler()->schedule([](float) {
        mj::helper::purgeTextureData();
    }, Application::getInstance(), 0, 0, 1.25, false, "delay_purge_cached_2");
}

void MSExploreView_Audiobooks::enablePlayAll(bool is_enabled) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
    is_enabled = false;
#endif
    utils::findChild(this, "button_play_all")->setVisible(is_enabled);
}

void MSExploreView_Audiobooks::initPopularSearch()
{
	if (m_popularSearchTemplate) {
		return;
	}

	if (auto collection = utils::findChild(this, "collection")) {

		m_popularSearchTemplate = PopularSearchTemplate::createTemplate(PopularSearchTemplate::Type::Audiobook);

		m_popularSearchTemplate->ignoreContentAdaptWithSize(false);

		m_popularSearchTemplate->setContentSize(collection->getContentSize());

		m_popularSearchTemplate->reloadData();

		m_popularSearchTemplate->setVisible(false);

		collection->addChild(m_popularSearchTemplate);

		m_popularSearchTemplate->setRecentItemClickCallback([=](Ref* sender) {

			if (auto button = dynamic_cast<ui::Button *>(sender)) {

				auto keyword = button->getTitleText();
				textField->setString(keyword);
                if (keyword.length() > 0)
                {
                    MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "search bar");
                }
                m_curSearchType = MSExploreView_Audiobooks::CurrentSearchType::RECENT;
                m_button_clear_search->setVisible(true);
				m_popularSearchTemplate->setVisible(false);
				_filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), keyword, (_gradebox ? _gradebox->getSelectedGradeId() : -1));
				this->reloadData();
                
                if (_filtered_audiobooks.size()) {
                    m_popularSearchTemplate->setRecentSearches(keyword);
                }
			}
		});

		m_popularSearchTemplate->setPopularItemClickCallback([=](Ref* sender) {

			if (auto popItem = dynamic_cast<MJPopularSearchItem*>(sender)) {

				auto keyword = popItem->getKeyword();
				textField->setString(keyword);
                searchText();
                MJDEFAULT->setStringForKey(key_read_from_of_audiobook, "search bar");
                m_curSearchType = MSExploreView_Audiobooks::CurrentSearchType::POPULAR;
				textField->setTextColor(Color4B::BLACK);
				_filtered_audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), keyword, (_gradebox ? _gradebox->getSelectedGradeId() : -1));
                m_button_clear_search->setVisible(true);
				m_popularSearchTemplate->setVisible(false);
				this->reloadData();
                
                if (_filtered_audiobooks.size()) {
                    m_popularSearchTemplate->setRecentSearches(keyword);
                }
			}
		});
	}
}

void MSExploreView_Audiobooks::showPanelNoResult(const std::string& keyword)
{
    if (_panel_noResult == nullptr)
    {
        if (auto collection = cocos2d::utils::findChild(this, "collection"))
        {
            _panel_noResult = MJWorksheetNoResult::createView(keyword);
            _panel_noResult->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
            _panel_noResult->setPosition(Point(collection->getContentSize().width/2, 0));
            _panel_noResult->setContentSize(collection->getContentSize());
            ui::Helper::doLayout(_panel_noResult);
            collection->addChild(_panel_noResult);
        }
    }
    _panel_noResult->setKeyword(keyword);
    _panel_noResult->stopAllActions();
    _panel_noResult->setOpacity(0);
    _panel_noResult->runAction(FadeIn::create(0.25f));
}

void MSExploreView_Audiobooks::hidePanelNoResult()
{
    if (_panel_noResult)
    {
        _panel_noResult->runAction(Sequence::create(FadeOut::create(0.2f), CallFunc::create([=](){
            _panel_noResult->removeFromParent();
            _panel_noResult = nullptr;
        }), NULL));
    }
}

void MSExploreView_Audiobooks::changeDisplayLanguage()
{

}

void MSExploreView_Audiobooks::pushCleverTapEvent_searchEvent()
{
    try
    {
        if (textField->getString().empty()) {
            return;
        }
        
        std::string searchTypeStr = "organic";
        if (m_curSearchType == MSExploreView_Audiobooks::CurrentSearchType::RECENT) {
            searchTypeStr = "recent";
        } else if (m_curSearchType == MSExploreView_Audiobooks::CurrentSearchType::POPULAR) {
            searchTypeStr = "popular";
        } else {
            // nothing here
        }
        
        CleverTapManager::GetInstance()->pushEvent("search_event",
        {
            {"searching_tags",cocos2d::Value(textField->getString()) },
            {"choose_item",cocos2d::Value(true)},
            {"item_id",cocos2d::Value(m_clickedAudiobookInfo.book_id)},
            {"item_name", cocos2d::Value(m_clickedAudiobookInfo.name)},
            {"type",cocos2d::Value("audiobook") },
            {"search_type",cocos2d::Value(searchTypeStr) }
        });
    }
    catch (const std::exception&)
    {
    }
}
