//
//  MSSearchView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/6/17.
//

#include "MSSearchView.h"
#include "StoryDataModel.h"
#include "StoryLanguageManager.h"
#include "MSCollectionStoryItem.h"
#include "MSComplexCollection.h"
#include "StoryInfoCover.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSEventLogger.h"

USING_NS_CC;
INITIALIZE_READER(MSSearchView);

std::once_flag ms_searchview_reader;

MSSearchView * MSSearchView::createView() {
    std::call_once(ms_searchview_reader, []{
        REGISTER_CSB_READER(MSSearchView);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/SearchView.csb");
    auto view = reinterpret_cast<MSSearchView *>(CSLoader::createNode(csb_name));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MSSearchView::didLoadFromCSB() 
{
	m_clickedStoryInfo.story_id = -1;
	m_clickedStoryInfo.name = "unknow";

    auto fake_layout = ui::Layout::create();
    fake_layout->setContentSize(Size(150 + MJ_PLATFORM_CONFIG.getNotchHeight(), 120));
    fake_layout->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
    fake_layout->setPosition(Point(0, this->getContentSize().height));
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
            auto query = std::string(_textfield->getString());
            if (query.empty()) {
                _filtered_stories.clear();
            } else {
                _filtered_stories = StoryDataModel::getInstance()->searchStoryByLanguage(StoryLanguageManager::getCurrentLangId(), query);
            }
            //    CCLOG("result: %d", (int)story.size());
            reloadData();
        }
    });
    
    _collection = nullptr;
    
    
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    
    auto push_clevertap_event = EventListenerCustom::create("push_clevertap_event_for_story_tab", [=](EventCustom *e)
	{
		pushCleverTapEvent();

    });

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(push_clevertap_event, this);

	auto event_should_close_listener = EventListenerCustom::create(key_event_search_page_close, [=](EventCustom *e) {
		this->removeFromParent();
	});
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(event_should_close_listener, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
}

void MSSearchView::onEnter() {
    ui::Layout::onEnter();
    _textfield->attachWithIME();
}

void MSSearchView::setContentSize(const Size & size) {
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

int MSSearchView::numberOfCells() {
    return (int)_filtered_stories.size();
}

int MSSearchView::numberOfColumns() {
    return (int) ((collectionSize().width + 10) / (modelSize().width + 10));
}

ui::Widget * MSSearchView::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    if (_ui_source.empty()) {
        return nullptr;
    }
    auto index = row * numberOfColumns() + col;
    if (_ui_source[index]) {
    } else {
        _ui_source[index] = MSCollectionStoryItem::createItem(_filtered_stories[index]);
        _ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
    }
    
    return _ui_source[index];
}

Size MSSearchView::modelSize() {
    return Size(170, 270) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

Size MSSearchView::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

void MSSearchView::offLoadCellAt(int index) {
    if (index < 0 || index >= _ui_source.size()) {
        return;
    }
    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
    _ui_source[index] = nullptr;
}

void MSSearchView::reloadData() {
    auto label_result = utils::findChild<ui::Text *>(this, "label_no_result");
    label_result->setVisible(numberOfCells() == 0 && !_textfield->getString().empty());
    label_result->setString(LANGUAGE_MANAGER->getDisplayTextByKey("search.no.result.stories"));
	
	if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE)
	{
		label_result->setFontName("fonts/leelawdb.ttf");
	}

    _ui_source.clear();
    _ui_source.resize(_filtered_stories.size(), nullptr);
    
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

#include "MJAPIStoryDetails.h"
void MSSearchView::onSelectCollectionItem(cocos2d::ui::Widget * item) {
	
	m_isItemSelected = true;

    if (!item) {
        return;
    }
    
    auto story_item = dynamic_cast<MSCollectionStoryItem *>(item);
    if (!story_item) {
        return;
    }
    
    auto info = story_item->getStoryInfo();

	m_clickedStoryInfo = info;
	MJDEFAULT->setStringForKey("STORY_START_FROM", "story_search_tab");

    ms::EventLogger::getInstance().logEvent(ms::analytics::kSearchEvent, {
            {kSearchEventTag,cocos2d::Value(_textfield->getString()) },
            {kSearchEventType,cocos2d::Value(kSearchEventTypeStory) },
            {kSearchEventBookID,cocos2d::Value(info.story_id)},
            {kSearchEventItemName, cocos2d::Value(info.name)}
    });

    mj::helper::playTransitionStoryCover(story_item, _collection);
}

void MSSearchView::onExit()
{
	cocos2d::ui::Layout::onExit();
}

void MSSearchView::pushCleverTapEvent()
{
	try
	{
		CleverTapManager::GetInstance()->pushEvent("search_event",
			{
				{"view_screen",cocos2d::Value(true) },
				{"searching_tags",cocos2d::Value(_textfield->getString()) },
				{"choose_story_from_searching_box",cocos2d::Value(true)},
				{"item_id",cocos2d::Value(m_clickedStoryInfo.story_id)},
				{"item_name", cocos2d::Value(m_clickedStoryInfo.name)},
				{"type",cocos2d::Value("story") }
			});
	}
	catch (const std::exception&)
	{

	}
}

void MSSearchView::onBack(cocos2d::Ref * sender) {
    mj::helper::playButtonFX();
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

ui::Widget::ccWidgetClickCallback MSSearchView::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSSearchView, onBack)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSSearchView::hideVirtualKeyboard()
{
    if(_textfield)
    {
        _textfield->didNotSelectSelf();
    }
}
