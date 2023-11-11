//
//  MSExploreView_Lessons_Lessons.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/29/18.
//

#include "MSExploreView_Lessons.h"
#include "MSCollectionLessonItem.h"
#include "MSComplexCollection.h"
#include "MSGradeBox.h"
#include "StoryDataModel.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSTabFavourite.h"
#include "MJ_PageLoadResource.h"
#include "MJPlatformConfig.h"
#include "MSLessonPreview.h"

USING_NS_CC;
INITIALIZE_READER(MSExploreView_Lessons);

std::once_flag ms_explore_view_lessons_reader;
MSExploreView_Lessons * MSExploreView_Lessons::createView(std::string name, std::vector<mj::model::LessonInfo> lessons)
{
    std::call_once(ms_explore_view_lessons_reader, [] {
        REGISTER_CSB_READER(MSExploreView_Lessons);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/ExploreView_Lessons.csb");
    
    auto view = reinterpret_cast<MSExploreView_Lessons *>(CSLoader::createNode(csb_name));
    
    if (view)
    {
        view->didLoadFromCSB(name, lessons);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}
void MSExploreView_Lessons::onLearnLesson(mj::model::LessonInfo lesson)
{
    auto preview = MSLessonPreview::createView(lesson);
    preview->setContentSize(this->getContentSize());
    ui::Helper::doLayout(preview);
    preview->setName("lesson_preview");
    this->addChild(preview, 100);
    preview->setOpacity(0);
    preview->runAction(FadeIn::create(0.25));
}
void MSExploreView_Lessons::onTryToLearnLockedLessons(mj::model::LessonInfo lesson)
{
    CCLOG("??");
}
void MSExploreView_Lessons::setName(std::string name)
{
    if(auto title = utils::findChild<ui::Text *>(this, "title"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-Bold.ttf");
        
        title->setString(LANGUAGE_MANAGER->getDisplayTextByKey(name));
    }
    
    setContentSize(this->getContentSize());
}

void MSExploreView_Lessons::didLoadFromCSB(std::string name, std::vector<mj::model::LessonInfo> lessons) {
    mj::helper::SetFpsByNumberFrames(6);
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
    
    _lessons = lessons;
    
    _filtered_lessons = lessons;
    
    _ui_source.resize(_filtered_lessons.size(), nullptr);
    _name = name;
    
    setName(_name);

    m_scope = ms::GradeManager::getInstance().getScope();

    auto title = utils::findChild<ui::Text *>(this, "title");
    
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
    
    auto listener = EventListenerCustom::create(key_event_grade_changed, [this](EventCustom * e) {

        std::string* data = reinterpret_cast<std::string*>(e->getUserData());

        auto dataVec = ms::GradeManager::getInstance().getSubStringsInSeparatedVerticalBarString(*data);

        //e->userData() = [current scope]|[current grade id] , ex : 1|1
        if(dataVec.size() >= 2 && static_cast<ms::GradeManager::Scope>(std::stoi(dataVec[1])) == m_scope)
        {
            this->onReceivedEventGradeChanged(std::stoi(dataVec[0]));
        }

        //auto grade_id = reinterpret_cast<int *>(e->getUserData());
        //this->onReceivedEventGradeChanged(* grade_id);
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    onReceivedEventGradeChanged(ms::GradeManager::getInstance().getCurrentGradeId());
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->setName(_name);
    }), this);
}

void MSExploreView_Lessons::setContentSize(const Size & size) {
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

void MSExploreView_Lessons::runAnimationAppear() {
    //    auto saved_position = this->getPosition();
    //    this->setPosition(this->getPosition() + Vec2(this->getContentSize().width, 0));
    //    this->runAction(EaseQuinticActionOut::create(MoveTo::create(0.5, saved_position)));
    //    _collection->runAnimationAppear();
    
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.5));
}

void MSExploreView_Lessons::runAnimationDisappear() {
//    _collection->runAnimationDisappear();
    this->runAction(Sequence::create(DelayTime::create(0.25), FadeOut::create(0.5), CallFunc::create([=] {
        this->removeFromParent();
    }), NULL));
}

void MSExploreView_Lessons::onEnter() {
    ui::Layout::onEnter();
    this->scheduleOnce([](float) {
        mj::helper::purgeTextureData();
    }, 0.25, "delay_purge_cached");
    runAnimationAppear();
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_delete_lesson", [=](EventCustom * e) {
        auto book = * reinterpret_cast<mj::model::LessonInfo *>(e->getUserData());
        CCLOG("removing book %s", book.name.c_str());
        
        
        if (MSTabFavourite::is_tab_downloaded) {
            CCLOG("remove downloaded stories");
            StoryDataModel::getInstance()->removeActivitiesOfLesson(book.lesson_id);
            mj::PageLoadResource::deleteCacheOfStory(book.lesson_id);
        } else {
//            CCLOG("remove favourite stories");
//            StoryDataModel::getInstance()->removeFavouriteStoryByLanguage(book.unique_id, book.lang_id);
        }
        
        auto it = std::find_if(_filtered_lessons.begin(), _filtered_lessons.end(), [book](mj::model::LessonInfo a) {
            return a.lesson_id == book.lesson_id;
        });
        
        if (it == _filtered_lessons.end()) {
            return;
        }
        
        auto d = (int)std::distance(_filtered_lessons.begin(), it);
        this->removeCellAtPosition(d);
        
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_refresh_downloaded_favourite);
    }), this);
}

ui::Widget::ccWidgetClickCallback MSExploreView_Lessons::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSExploreView_Lessons, onBack),
        CLICK_MAP(MSExploreView_Lessons, onEdit),
    };
    
    auto backkey_listener = EventListenerKeyboard::create();
    backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
        if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
            this->onBack(nullptr);
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSExploreView_Lessons::onBack(Ref * sender) {
    mj::helper::playButtonFXClose();
    mj::helper::SetFpsByNumberFrames(10);
    this->getParent()->enumerateChildren("//.*", [](Node * c) {
        c->resume();
        return false;
    });
    
    this->runAnimationDisappear();
}

int MSExploreView_Lessons::numberOfCells() {
    return (int)_filtered_lessons.size();
}

int MSExploreView_Lessons::numberOfColumns() {
    auto nr = (int) ((collectionSize().width + 10) / (modelSize().width + 10));
    return nr;
}

ui::Widget * MSExploreView_Lessons::cellForPosition(int row, int col, std::vector<int> listIdOfLessonsLearned) {
    auto index = row * numberOfColumns() + col;
    if (_ui_source.empty()) {
        return nullptr;
    }
    
    if (index < 0 || index >= _ui_source.size()) {
        return nullptr;
    }
    if (_ui_source[index]) {
    } else {
        auto ui = MSCollectionLessonItem::createItem(_filtered_lessons[index]);
        ui->setDelegate(this);
        //ui->setDisplayMode();
        if(auto parent = dynamic_cast<ms::delegate::CollectionLessonItem*>(this->getParent()))
        {
            ui->setDelegate(parent);
        }
        _ui_source[index] = ui;
        _ui_source[index]->setScale(MJ_PLATFORM_CONFIG.getItemScaleFactor());
        if (_is_editing) {
            reinterpret_cast<MSCollectionLessonItem *>(_ui_source[index])->enableDeleteMode();
        }
    }
    
    return _ui_source[index];
}

Size MSExploreView_Lessons::modelSize() {
    return Size(190, 340) * MJ_PLATFORM_CONFIG.getItemScaleFactor();
}

Size MSExploreView_Lessons::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}

void MSExploreView_Lessons::offLoadCellAt(int index) {
    if (index < 0 || index >= _ui_source.size()) {
        return;
    }
//    _ui_source[index] ? _ui_source[index]->removeFromParent() : (void) nullptr;
//    _ui_source[index] = nullptr;
}

void MSExploreView_Lessons::reloadData() {
    _ui_source.clear();
    _ui_source.resize(_filtered_lessons.size(), nullptr);
    _collection->reloadData();
}

void MSExploreView_Lessons::removeCellAtPosition(int index) {
    if (_ui_source.size() <= index) {
        return;
    }
    
    auto item = _ui_source[index];
    if (item) {
        for (auto i = index + 1; i < _ui_source.size(); ++i) {
            if (_ui_source[i] && _ui_source[i - 1]) {
                _ui_source[i]->runAction(MoveTo::create(0.25, _ui_source[i - 1]->getPosition()));
            }
        }
        
        item->removeFromParent();
        _ui_source.erase(_ui_source.begin() + index);
        _filtered_lessons.erase(_filtered_lessons.begin() + index);
        //        auto updated = _collection->updatePadding();
    }
}

void MSExploreView_Lessons::onSelectGradeBox() {
    
}

void MSExploreView_Lessons::onDeselectGradeBox(int grade_id) {
    
}

void MSExploreView_Lessons::onSelectCollectionItem(cocos2d::ui::Widget * item)
{
    auto lesson_item = dynamic_cast<MSCollectionLessonItem *>(item);
    
    if (lesson_item)
    {
        mj::helper::playButtonFX();
        auto preview = MSLessonPreview::createView(lesson_item->getInfo());
        preview->setName("lesson_preview");
        preview->setContentSize(this->getContentSize());
//        if (auto sidebar = cocos2d::utils::findChild(Director::getInstance()->getRunningScene(), "msSideBar")) {
//            // Cho màn Lesson Preview full màn hình
//            sidebar->setVisible(false); // hide the sidebar
//            preview->setContentSize(this->getContentSize() + Size(sidebar->getContentSize().width, 0)); // reset ContentSize
//            preview->setPosition(Point(-sidebar->getContentSize().width, 0)); // reset Position
//        }
        ui::Helper::doLayout(preview);
        Director::getInstance()->getRunningScene()->addChild(preview, 100);
        preview->setOpacity(0);
        preview->runAction(FadeIn::create(0.25));
    }
}

void MSExploreView_Lessons::onReceivedEventGradeChanged(int grade_id) {
//    _filtered_stories.clear();
//    if (grade_id == -1) {
//        // all grades
//        _filtered_stories = _stories;
//    } else {
//        for (auto i = 0; i < _stories.size(); ++i) {
//            if (_stories[i].levels[0] == grade_id) {
//                //            auto level_info = StoryDataModel::getInstance()->getLevelInfoById(_stories[i].levels[0], 1);
//                //            if (level_info.grade_id == grade_id) {
//                _filtered_stories.push_back(_stories[i]);
//            }
//        }
//    }
//    reloadData();
}

void MSExploreView_Lessons::onEdit(cocos2d::Ref *sender) {
    if (!_is_editing) {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionLessonItem *>(item);
                casted->enableDeleteMode();
            }
        }
    } else {
        for (auto item : _ui_source) {
            if (item) {
                auto casted = reinterpret_cast<MSCollectionLessonItem *>(item);
                casted->disableDeleteMode();
            }
        }
    }
    _is_editing = !_is_editing;
}

void MSExploreView_Lessons::enableEditMode(std::string str) {
    _saved_title_edit = str;
    auto fake = utils::findChild(this, "fake_layout");
    if (fake)
        fake->setPosition(Point(0, this->getContentSize().height - 360));
    
}

void MSExploreView_Lessons::onExit() {
    ui::Layout::onExit();
    Director::getInstance()->getScheduler()->schedule([](float) {
        mj::helper::purgeTextureData();
    }, Application::getInstance(), 0, 0, 1.25, false, "delay_purge_cached_2");
}
