//
//  MSSearchView_Audiobooks_Audiobooks.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 1/8/18.
//

#include "MSSearchView_Audiobooks.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSCollectionAudiobookItem.h"
#include "MSComplexCollection.h"
#include "MSAudiobookPlayer.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"

USING_NS_CC;
INITIALIZE_READER(MSSearchView_Audiobooks);

std::once_flag ms_searchview_audiobooks_reader;

MSSearchView_Audiobooks * MSSearchView_Audiobooks::createView() 
{
    std::call_once(ms_searchview_audiobooks_reader, []{
        REGISTER_CSB_READER(MSSearchView_Audiobooks);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/SearchView_Audiobooks.csb");
    auto view = reinterpret_cast<MSSearchView_Audiobooks *>(CSLoader::createNode(csb_name));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

#include "MJPlatformConfig.h"
void MSSearchView_Audiobooks::didLoadFromCSB() 
{
	m_clickedAudiobookInfo.book_id = -1;
	m_clickedAudiobookInfo.name = "unknow";

    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height - 240));
    fake_layout->setTouchEnabled(true);
    fake_layout->addClickEventListener([this](Ref * sender) {
        this->onBack(nullptr);
    });
    this->addChild(fake_layout);
    
    _textfield = utils::findChild<ui::TextField *>(this, "textfield");
    _textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
    _textfield->setPlaceHolderColor(Color4B(Color3B::WHITE, 0xaa));
    _textfield->setCursorEnabled(true);
    _textfield->setCursorChar('|');
    _textfield->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("search.placeholder"));
    _textfield->addEventListener([=](Ref * sender, ui::TextField::EventType event) {

        if (event == ui::TextField::EventType::INSERT_TEXT || event == ui::TextField::EventType::DELETE_BACKWARD) {
            auto query = _textfield->getString();
            _audiobooks = StoryDataModel::getInstance()->searchAudiobookByLanguage(StoryLanguageManager::getCurrentLangId(), query);
            reloadData();
        }
    });

    _collection = nullptr;
    
	auto push_clevertap_event = EventListenerCustom::create("push_clevertap_event_for_audiobook_tab", [=](EventCustom *e)
	{
		pushCleverTapEvent();

	});

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(push_clevertap_event, this);

    auto event_should_close_listener = EventListenerCustom::create(key_event_search_page_close, [=](EventCustom *e) {
        this->removeFromParent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_should_close_listener, this);
    
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
}

void MSSearchView_Audiobooks::onEnter() {
    ui::Layout::onEnter();
    _textfield->attachWithIME();
}

void MSSearchView_Audiobooks::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    
    auto collection_layout = utils::findChild(this, "collection");
    if (collection_layout) {
        collection_layout->setContentSize(Size(size.width - 100, collection_layout->getContentSize().height));
    }
    
    if (_collection) {
        _collection->setContentSize(Size(size.width - 100, _collection->getContentSize().height));
        //        _collection->reloadData();
        reloadData();
    }
}

int MSSearchView_Audiobooks::numberOfCells() {
    return (int)_audiobooks.size();
}

int MSSearchView_Audiobooks::numberOfColumns() {
    return (int) ((collectionSize().width + 10) / (modelSize().width + 10));
}

ui::Widget * MSSearchView_Audiobooks::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    if (_ui_source.empty()) {
        return nullptr;
    }
    auto index = row * numberOfColumns() + col;
    if (_ui_source[index]) {
    } else {
        _ui_source[index] = MSCollectionAudiobookItem::createThumb(_audiobooks[index]);
        _ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
    }
    
    return _ui_source[index];
}

Size MSSearchView_Audiobooks::modelSize() {
    return Size(240, 270) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

Size MSSearchView_Audiobooks::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

void MSSearchView_Audiobooks::offLoadCellAt(int index) {
    if (index < 0 || index >= _ui_source.size()) {
        return;
    }
    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
    _ui_source[index] = nullptr;
}

void MSSearchView_Audiobooks::reloadData() {
    auto label_result = utils::findChild<ui::Text *>(this, "label_no_result");
    label_result->setVisible(numberOfCells() == 0 && !_textfield->getString().empty());
    label_result->setString(LANGUAGE_MANAGER->getDisplayTextByKey("search.no.result.audiobooks"));
    
	if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
	{
		label_result->setFontName("fonts/leelawdb.ttf");
	}

    _ui_source.clear();
    _ui_source.resize(_audiobooks.size(), nullptr);
    
    if (_collection) {
        _collection->reloadData();
    } else {
        _collection = MSComplexCollection::createCollection(this);
        utils::findChild(this, "collection")->addChild(_collection);
        _collection->setDelegate(this);
        
        ui::Helper::doLayout(this);
        //        _collection->reloadData();
    }
}

#include "MSChapterPrepare.h"
void MSSearchView_Audiobooks::onSelectCollectionItem(cocos2d::ui::Widget * item) {
    if (!item) {
        return;
    }
    
    auto audiobook_item = dynamic_cast<MSCollectionAudiobookItem *>(item);
    if (!audiobook_item) {
        return;
    }
    
	auto info = audiobook_item->getInfo();

	MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", "audiobook_search_tab");
	m_clickedAudiobookInfo = info;

    if (StoryDataModel::getInstance()->isAudiobookChapter(info.book_id)) {
        auto list_chapter = StoryDataModel::getInstance()->getChapterOfAudiobook(info.book_id);
        auto view = MSChapter_Prepare::createView(info, list_chapter);
        view->setContentSize(this->getContentSize());
        ui::Helper::doLayout(view);
        this->addChild(view, 1);
    } else {
        auto collection_wrapper = utils::findChild<ui::Widget *>(this, "collection");
        mj::helper::playTransitionAudiobookCover(audiobook_item, collection_wrapper);
    }

	

    ms::EventLogger::getInstance().logEvent(ms::analytics::kSearchEvent, {
            {kSearchEventTag,cocos2d::Value(_textfield->getString()) },
            {kSearchEventType,cocos2d::Value(kSearchEventTypeAudiobook) },
            {kSearchEventBookID,cocos2d::Value(info.book_id)},
            {kSearchEventItemName, cocos2d::Value(info.name)}
    });
}

void MSSearchView_Audiobooks::onBack(cocos2d::Ref * sender) {\
    mj::helper::playButtonFX();
    this->runAction(Sequence::create(FadeOut::create(0.25f), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

ui::Widget::ccWidgetClickCallback MSSearchView_Audiobooks::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSSearchView_Audiobooks, onBack)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSSearchView_Audiobooks::hideVirtualKeyboard()
{
    if(_textfield)
    {
        _textfield->didNotSelectSelf();
    }
}

void MSSearchView_Audiobooks::pushCleverTapEvent()
{
	try
	{
		CleverTapManager::GetInstance()->pushEvent("search_event",
			{
				{"view_screen",cocos2d::Value(true) },
				{"searching_tags",cocos2d::Value(_textfield->getString()) },
				{"choose_story_from_searching_box",cocos2d::Value(true)},
				{"item_id",cocos2d::Value(m_clickedAudiobookInfo.book_id)},
				{"item_name", cocos2d::Value(m_clickedAudiobookInfo.name)},
				{"type",cocos2d::Value(kSearchEventTypeAudiobook) }
			});
	}
	catch (const std::exception&)
	{

	}
}