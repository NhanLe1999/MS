//
//  MSExplorerView_Categories.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/21/17.
//

#include "MSExplorerView_Categories.h"

#include "MSCollectionCategoryItem.h"
#include "MSComplexCollection.h"
#include "MSGradeBox.h"
#include "StoryDataModel.h"
#include "StoryInfoCover.h"
#include "MJDefinitions.h"
#include "MSExploreView.h"

USING_NS_CC;
INITIALIZE_READER(MSExploreView_Categories);

std::once_flag ms_explore_view_categories_reader;
MSExploreView_Categories * MSExploreView_Categories::createView(int i_gradeID) {
    std::call_once(ms_explore_view_categories_reader, [] {
        REGISTER_CSB_READER(MSExploreView_Categories);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/ExploreView_Categories.csb");
    auto view = reinterpret_cast<MSExploreView_Categories *>(CSLoader::createNode(csb_name));
    if (view) {
        view->didLoadFromCSB(i_gradeID);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MSExploreView_Categories::didLoadFromCSB(int i_gradeID)
{
    _categories = StoryDataModel::getInstance()->getCategories(StoryLanguageManager::getCurrentLangId());
    _current_grade_id = i_gradeID;
    _ui_source.resize(_categories.size(), nullptr);
    
    auto title = utils::findChild<ui::Text *>(this, "title");
    
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-Bold.ttf");

    title->setString(LANGUAGE_MANAGER->getDisplayTextByKey("stories.category.title"));
    
    auto dot = utils::findChild(this, "dot");
    auto line = utils::findChild(this, "line");
    
    auto last_x = title->getPositionX() + title->getContentSize().width;
    dot->setPosition(Point(last_x + 50, dot->getPositionY()));
    line->setPosition(Point(dot->getPositionX(), line->getPositionY()));
    line->setContentSize(Size(this->getContentSize().width - last_x - 100, line->getContentSize().height));
    
    _collection = MSComplexCollection::createCollection(this);
    utils::findChild(this, "collection")->addChild(_collection);
    _collection->setDelegate(this);
    
    ui::Helper::doLayout(this);
    
    auto backkey_listener = EventListenerKeyboard::create();
    
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event)
    {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK)
        {
            this->onBack(nullptr);
        }
    };
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
}

void MSExploreView_Categories::setContentSize(const Size & size)
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

void MSExploreView_Categories::runAnimationAppear() {
//    auto saved_position = this->getPosition();
//    this->setPosition(this->getPosition() + Vec2(this->getContentSize().width, 0));
//    this->runAction(EaseQuinticActionOut::create(MoveTo::create(0.5, saved_position)));
//    _collection->runAnimationAppear();
    
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.25));
}

void MSExploreView_Categories::runAnimationDisappear() {
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
//    _collection->runAnimationDisappear();
}

void MSExploreView_Categories::onEnter() {
    ui::Layout::onEnter();
    runAnimationAppear();
}

ui::Widget::ccWidgetClickCallback MSExploreView_Categories::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSExploreView_Categories, onBack)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSExploreView_Categories::onBack(Ref * sender) {
    MJDEFAULT->setStringForKey(key_read_from, "new");
    mj::helper::playButtonFXClose();
    runAnimationDisappear();
}

int MSExploreView_Categories::numberOfCells() {
    return (int)_categories.size();
}

int MSExploreView_Categories::numberOfColumns() {
    return (int) ((collectionSize().width + 10) / (modelSize().width + 10));
}

ui::Widget * MSExploreView_Categories::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    auto index = row * numberOfColumns() + col;
    if (_ui_source.empty()) {
        return nullptr;
    }
    
    if (index < 0 || index >= _ui_source.size()) {
        return nullptr;
    }
    if (_ui_source[index]) {
    } else {
        _ui_source[index] = MSCollectionCategoryItem::createItem(_categories[index]);
    }
    
    return _ui_source[index];
}

Size MSExploreView_Categories::modelSize() {
    return Size(250, 200);
}

Size MSExploreView_Categories::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

void MSExploreView_Categories::offLoadCellAt(int index) {
    if (index < 0 || index >= _ui_source.size()) {
        return;
    }
    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
    _ui_source[index] = nullptr;
}

void MSExploreView_Categories::reloadData() {
    _ui_source.clear();
    _ui_source.resize(_categories.size(), nullptr);
    _collection->reloadData();
}

void MSExploreView_Categories::onSelectCollectionItem(cocos2d::ui::Widget * item) {
    if (!item) {
        return;
    }
    
    auto category_item = dynamic_cast<MSCollectionCategoryItem *>(item);
    if (category_item) {
        mj::helper::playButtonFX();
        auto info = category_item->getCategoryInfo();
        auto stories = StoryDataModel::getInstance()->getStoriesByCategory(info.category_id);
        auto view = MSExploreView::createView(info.content, MSExploreView::CollectionType::SERIES, _current_grade_id, info.category_id);
        view->setContentSize(this->getContentSize());
        ui::Helper::doLayout(view);
        this->addChild(view, 1);
        return;
    }
    
    
}
