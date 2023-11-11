//
//  MSTabFavourite.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/26/17.
//

#include "MSTabFavourite.h"
#include "MSSimpleCollection.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"

#include "MSCollectionStoryItem.h"
#include "MSCollectionAudiobookItem.h"
#include "MSCollectionLessonItem.h"

#include "StoryInfoCover.h"
#include "MSAudiobookPlayer.h"
#include "MSExploreView.h"
#include "MSExploreView_Audiobooks.h"
#include "MSExploreView_Lessons.h"
//#include "MSExploreViewAISpeaking.h"
#include "MSGradeManager.h"
#include "MSLessonPreview.h"
#include "MJAPIStoryDetails.h"
#include "MSGradeManager.h"
#include "MSActivityWrapper.h"
#include "MSActivityEnd.h"
#include "MSHome.h"
#include "MSTabMaterial.h"

USING_NS_CC;
INITIALIZE_READER(MSTabFavourite);

std::once_flag ms_tab_favourite_reader;

bool MSTabFavourite::is_tab_downloaded = true;

MSTabFavourite * MSTabFavourite::createTab()
{
    std::call_once(ms_tab_favourite_reader, []
    {
        REGISTER_CSB_READER(MSTabFavourite);
    });
    
    auto csb_name = mj::helper::getCSBName("csb/new_ui/Favourite.csb");
    auto tab = reinterpret_cast<MSTabFavourite *>(CSLoader::createNode(csb_name));
    if (tab) {
        tab->didLoadFromCSB();
        return tab; 
    }
    
    CC_SAFE_DELETE(tab);
    return nullptr;
}

void MSTabFavourite::didLoadFromCSB()
{ 
    _downloaded_audiobooks = _downloaded_lessons = _downloaded_stories = _favourite_stories = _favourite_audiobooks = nullptr;
    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
//    auto content = utils::findChild<ui::ListView *>(this, "content");
//    content->setScrollBarEnabled(false);
    
    _button_favourite = utils::findChild<ui::Layout *>(this, "button_favourite");
    _button_downloaded = utils::findChild<ui::Layout *>(this, "button_downloaded");
    
    //reloadData();
    
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_reload_favourite");
//    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("event_reload_favourite", [this](EventCustom *) {
//        this->reloadData();
//    }), this);
    
    onDownloaded(nullptr);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_grade_changed, [this](EventCustom *) {
        this->reloadData();
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->reloadData();
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_refresh_downloaded_favourite, [this](EventCustom *) {
        this->reloadData();
    }), this);
}

void MSTabFavourite::onEnter() {
    ui::Layout::onEnter();
    //mj::helper::showWaitView();
// this will call from key_event_refresh_downloaded_favourite or  didLoadFromCSB()
    reloadData();
    //this->scheduleOnce([=](float dt) {
    //    mj::helper::deleteWaitView();
    //    }, 2.0f, "waiting_hide_waitingView");
    setContentSize(this->getContentSize());
}

void MSTabFavourite::reloadData()
{
    this->scheduleOnce([=](float dt) {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
            reloadData_main();
            });
        }, 0.1f, "reloadData");
}

void MSTabFavourite::reloadData_main()
{
    if (auto label = _button_favourite->getChildByName<ui::Text*>("label"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? label->setFontName("fonts/leelawdb.ttf") : label->setFontName("fonts/Montserrat-Bold.ttf");

        label->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favourite.tab.favourite"));
    }

    if (auto label = _button_downloaded->getChildByName<ui::Text*>("label"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? label->setFontName("fonts/leelawdb.ttf") : label->setFontName("fonts/Montserrat-Bold.ttf");

        label->setString(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favourite.tab.downloaded"));
    }

    auto content = utils::findChild<ui::ListView*>(this, "content_favourite");
    content->removeAllItems();
    content->setScrollBarEnabled(false);

    _favourite_stories = MSSimpleCollection::createFavouriteStoriesCollection(-1);
    if (_favourite_stories)
    {
        _favourite_stories->setDelegate(this);
        _favourite_stories->changeTextDisplayLang(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favorite.stories"));
        content->pushBackCustomItem(_favourite_stories);
    }

    this->scheduleOnce([=](float dt) {
         _favourite_audiobooks = MSSimpleCollection::createFavouriteAudiobooksCollection(-1);
            if (_favourite_audiobooks)
            {
                _favourite_audiobooks->setDelegate(this);
                _favourite_audiobooks->changeTextDisplayLang(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "favorite.audiobook"));
                content->pushBackCustomItem(_favourite_audiobooks);
            }
        }, 0.3f, "createFavouriteAudiobooksCollection");


    //    if (_favourite_stories->getStories().empty()) {
    //        _favourite_stories->setVisible(false);
    //    }
    //    if (_favourite_audiobooks->getAudiobooks().empty()) {
    //        _favourite_audiobooks->setVisible(false);
    //    }


    content = utils::findChild<ui::ListView*>(this, "content_downloaded");
    content->removeAllItems();
    content->setScrollBarEnabled(false);

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
    _downloaded_stories = MSSimpleCollection::createDownloadedStoriesCollection(-1);
    if (_downloaded_stories)
    {
        _downloaded_stories->setDelegate(this);
        content->pushBackCustomItem(_downloaded_stories);
        _downloaded_stories->changeTextDisplayLang(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "learn.stories"));
    }

    this->scheduleOnce([=](float dt) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
        _downloaded_audiobooks = MSSimpleCollection::createDownloadedAudiobooksCollection(-1);
        if (_downloaded_audiobooks)
        {
            _downloaded_audiobooks->setDelegate(this);
            _downloaded_audiobooks->changeTextDisplayLang(LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "learn.audiobooks"));
            content->pushBackCustomItem(_downloaded_audiobooks);
        }
    }, 0.5f, "createDownloadedAudiobooksCollection");


    this->scheduleOnce([=](float dt) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
        _downloaded_lessons = MSSimpleCollection::createDownloadedLessonsCollection(this);
        if (_downloaded_lessons)
        {
            _downloaded_lessons->setDelegate(this);
            _downloaded_lessons->changeTextDisplayLang(LANGUAGE_MANAGER->getDisplayTextByKey("key.material.lesson.learned"));
            content->pushBackCustomItem(_downloaded_lessons);
        }
    }, 0.5f, "createDownloadedLessonsCollection");

    this->scheduleOnce([=](float dt) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AISPEAKING);
        _downloaded_ai_speaking_lessons = MSSimpleCollection::createDownloadedAISpeakingLessonsCollection(this);
        if (_downloaded_ai_speaking_lessons)
        {
            _downloaded_ai_speaking_lessons->setDelegate(this);
            _downloaded_ai_speaking_lessons->changeTextDisplayLang(LANGUAGE_MANAGER->getDisplayTextByKey("downloaded.aiSpeakingLessons"));
            //content->pushBackCustomItem(_downloaded_ai_speaking_lessons);
        }
    }, 1.5f, "createDownloadedAISpeakingLessonsCollection");

    if (auto content_empty = utils::findChild<ui::Text*>(this, "content_empty"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? content_empty->setFontName("fonts/leelawdb.ttf") : content_empty->setFontName("fonts/Montserrat-SemiBold.ttf");

        content_empty->setString(LANGUAGE_MANAGER->getDisplayTextByKey("downloaded.empty"));

        content_empty->setVisible(is_tab_downloaded ? (!_downloaded_stories && !_downloaded_audiobooks && !_downloaded_lessons && !_downloaded_ai_speaking_lessons) : (!_favourite_stories && !_favourite_audiobooks));
    }
    this->scheduleOnce([=](float dt) {
        setContentSize(this->getContentSize());
    }, 0.5f, "set_content");

}

void MSTabFavourite::setContentSize(const Size & size)
{
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);

    auto content_downloaded = utils::findChild(this, "content_downloaded");
    auto content_favourite = utils::findChild(this, "content_favourite");
    if (content_downloaded) {
        content_downloaded->setContentSize(Size(size.width, content_downloaded->getContentSize().height));
    }
    
    if (content_favourite) {
        content_favourite->setContentSize(Size(size.width, content_favourite->getContentSize().height));
    }
    
    if (_favourite_stories)
        _favourite_stories->setContentSize(Size(size.width, _favourite_stories->getContentSize().height));
    
    if (_favourite_audiobooks)
        _favourite_audiobooks->setContentSize(Size(size.width, _favourite_audiobooks->getContentSize().height));
    
    if (_downloaded_stories) {
        _downloaded_stories->setContentSize(Size(size.width, _downloaded_stories->getContentSize().height));
    }
    
    if (_downloaded_audiobooks) {
        _downloaded_audiobooks->setContentSize(Size(size.width, _downloaded_audiobooks->getContentSize().height));
    }
    
    if (_downloaded_lessons) {
        _downloaded_lessons->setContentSize(Size(size.width, _downloaded_lessons->getContentSize().height));
    }
}

void MSTabFavourite::onSelectCollectionItem(ui::Widget * item)
{
    if (!item)
    {
        return;
    }
    
    auto story_item = dynamic_cast<MSCollectionStoryItem *>(item);
    
    if (story_item)
    {
        auto content = utils::findChild<ui::Widget *>(this, "content");
        
        auto info = story_item->getStoryInfo();
        
//        MJDEFAULT->setStringForKey("STORY_START_FROM",is_tab_downloaded ? "downloaded":"favourite");
        MJDEFAULT->setStringForKey("STORY_START_FROM",is_tab_downloaded ? "library":"favourite");
        mj::helper::playTransitionStoryCover(story_item, content);
    }
    
    auto audiobook_item = dynamic_cast<MSCollectionAudiobookItem *>(item);
    
    if (audiobook_item)
    {
        auto content = utils::findChild<ui::Widget *>(this, "content");
        mj::helper::playTransitionAudiobookCover(audiobook_item, content);
        MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", is_tab_downloaded ? "library" : "favourite");
//        MJDEFAULT->setStringForKey("AUDIOBOOK_START_FROM", is_tab_downloaded ? "downloaded" : "favourite");
    }
    
    auto lesson_item = dynamic_cast<MSCollectionLessonItem *>(item);
    
    if (lesson_item)
    {
        mj::helper::playButtonFX();
        auto preview = MSLessonPreview::createView(lesson_item->getInfo());
        preview->setName("lesson_preview");
        preview->setPlayFrom("library");
        if (auto mshome = (MSHome *)Director::getInstance()->getRunningScene()->getChildByName(MSHOME_NAME)) {
            preview->setContentSize(mshome->getContentSize());
            ui::Helper::doLayout(preview);
            mshome->addChild(preview);
        }
        preview->setOpacity(0);
        preview->runAction(FadeIn::create(0.25));
    }
}

void MSTabFavourite::onSelectMore(MSSimpleCollection * sender)
{
    auto type = sender->getCollectionType();
    
    auto is_downloaded = utils::findChild(this, "content_downloaded")->isVisible();
    
    if (type == MSSimpleCollection::CollectionType::STORY) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
        auto saved = ms::GradeManager::getInstance().getCurrentGradeId();
        ms::GradeManager::getInstance().setCurrentGradeId(-1);
        
        std::string title = "favorite.stories";
        if (is_downloaded) {
            title = "learn.stories";
        }
        auto explorer = MSExploreView::createView(title, MSExploreView::CollectionType::READED, ms::GradeManager::getInstance().getCurrentGradeId());
        explorer->setContentSize(Director::getInstance()->getVisibleSize());
        ui::Helper::doLayout(explorer);
        explorer->hideSearchBar();
        Director::getInstance()->getRunningScene()->addChild(explorer, kMSHomeZOrder_contentFull);
        ms::GradeManager::getInstance().setCurrentGradeId(saved);
    }
    
    if (type == MSSimpleCollection::CollectionType::AUDIOBOOK) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
        auto saved = ms::GradeManager::getInstance().getCurrentGradeId();
        ms::GradeManager::getInstance().setCurrentGradeId(-1);
        
        std::string title = "favorite.audiobook";
        if (is_downloaded) {
            title = "learn.audiobooks";
        }
        // nên sửa CollectionType ở MSSimpleCollection khớp với từng hàm create
        auto explorer = MSExploreView_Audiobooks::createView(title, MSExploreView_Audiobooks::CollectionType::READED, ms::GradeManager::getInstance().getCurrentGradeId());
        explorer->hideSearchBar();
        explorer->setContentSize(Director::getInstance()->getVisibleSize());
        if (is_downloaded) {
            explorer->enablePlayAll(false);
        }
        ui::Helper::doLayout(explorer);
        Director::getInstance()->getRunningScene()->addChild(explorer, kMSHomeZOrder_contentFull);
        ms::GradeManager::getInstance().setCurrentGradeId(saved);
    }
    
    if (type == MSSimpleCollection::CollectionType::LESSON) {
        ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::LESSONS);
        auto saved = ms::GradeManager::getInstance().getCurrentGradeId();
        ms::GradeManager::getInstance().setCurrentGradeId(-1);

        std::string title = "favorite.lesson";
        if (is_downloaded) {
            title = "key.material.lesson.learned";
        }
        auto explorer = MSExploreView_Lessons::createView("key.material.lesson.learned", sender->getLessons());
        explorer->setContentSize(Director::getInstance()->getVisibleSize());
        ui::Helper::doLayout(explorer);
        Director::getInstance()->getRunningScene()->addChild(explorer, kMSHomeZOrder_contentFull);
    }
//    if (type == MSSimpleCollection::CollectionType::AI_SPEAKING) {
//        auto explorer = MSExploreViewAISpeaking::createView("downloaded.aiSpeakingLessons", sender->getAISpekingLessons());
//        explorer->setContentSize(Director::getInstance()->getVisibleSize());
//        explorer->enableEditMode("downloaded.edit");
//        ui::Helper::doLayout(explorer);
//        Director::getInstance()->getRunningScene()->addChild(explorer, kMSHomeZOrder_contentFull);
//    }
}

void MSTabFavourite::onDownloaded(cocos2d::Ref *sender) {
    mj::helper::playButtonFX();
    is_tab_downloaded = true;
    utils::findChild(this, "content_downloaded")->stopAllActions();
    utils::findChild(this, "content_favourite")->stopAllActions();
    utils::findChild(this, "content_downloaded")->runAction(Sequence::create(CallFuncN::create([](Node * n) {
        n->setOpacity(0);
        n->setVisible(true);
    }), FadeIn::create(0.5), nullptr));
    utils::findChild(this, "content_favourite")->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) {
        n->setVisible(false);
    }), nullptr));
    _button_downloaded->getChildByName("outline")->setVisible(false);
    _button_downloaded->getChildByName("highlight")->setVisible(true);
    _button_downloaded->setBackGroundColor(Color3B::WHITE);
    
    _button_favourite->getChildByName("outline")->setVisible(true);
    _button_favourite->getChildByName("highlight")->setVisible(false);
    _button_favourite->setBackGroundColor(mj::helper::colorFromHex(0xf4f5f6));
    
    auto content_empty = utils::findChild<ui::Text *>(this, "content_empty");
    content_empty->setVisible(is_tab_downloaded? (!_downloaded_stories && !_downloaded_audiobooks && !_downloaded_lessons) :(!_favourite_stories && !_favourite_audiobooks)) ;
    content_empty->setString(LANGUAGE_MANAGER->getDisplayTextByKey("downloaded.empty"));
}

void MSTabFavourite::onFavourite(cocos2d::Ref *sender) {
    mj::helper::playButtonFX();
    is_tab_downloaded = false;
    utils::findChild(this, "content_downloaded")->stopAllActions();
    utils::findChild(this, "content_favourite")->stopAllActions();
    utils::findChild(this, "content_favourite")->runAction(Sequence::create(CallFuncN::create([](Node * n) {
        n->setOpacity(0);
        n->setVisible(true);
    }), FadeIn::create(0.5), nullptr));
    utils::findChild(this, "content_downloaded")->runAction(Sequence::create(FadeOut::create(0.5), CallFuncN::create([](Node * n) {
        n->setVisible(false);
    }), nullptr));
    _button_downloaded->getChildByName("outline")->setVisible(true);
    _button_downloaded->getChildByName("highlight")->setVisible(false);
    _button_downloaded->setBackGroundColor(mj::helper::colorFromHex(0xf4f5f6));
    
    _button_favourite->getChildByName("outline")->setVisible(false);
    _button_favourite->getChildByName("highlight")->setVisible(true);
    _button_favourite->setBackGroundColor(Color3B::WHITE);
    
    auto content_empty = utils::findChild<ui::Text *>(this, "content_empty");
    content_empty->setVisible(is_tab_downloaded? (!_downloaded_stories && !_downloaded_audiobooks && !_downloaded_lessons) :(!_favourite_stories && !_favourite_audiobooks)) ;
    content_empty->setString(LANGUAGE_MANAGER->getDisplayTextByKey("favourite.empty"));
}



void MSTabFavourite::onLearnLesson(mj::model::LessonInfo lesson)
{
    auto preview = MSLessonPreview::createView(lesson);
    preview->setName("lesson_preview");
    if (auto mshome = (MSHome *)Director::getInstance()->getRunningScene()->getChildByName(MSHOME_NAME)) {
        preview->setContentSize(mshome->getContentSize());
        ui::Helper::doLayout(preview);
        mshome->addChild(preview, kMSHomeZOrder_contentFull);
    }
    preview->setOpacity(0);
    preview->runAction(FadeIn::create(0.25));
}

void MSTabFavourite::onTryToLearnLockedLessons(mj::model::LessonInfo lesson)
{
    
}

ui::Widget::ccWidgetClickCallback MSTabFavourite::onLocateClickCallback(const std::string & name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSTabFavourite, onFavourite),
        CLICK_MAP(MSTabFavourite, onDownloaded)
    };
    
    if (function_map.count(name) > 0) {
        return function_map.at(name);
    }
    
    return nullptr;
}
