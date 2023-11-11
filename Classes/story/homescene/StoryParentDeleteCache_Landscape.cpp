//
//  StoryParentDeleteCache_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/10/17.
//

#include "StoryParentDeleteCache_Landscape.h"
#include "MJ_PageLoadResource.h"
#include "StoryLanguageManager.h"
#include "StoryConfigManager.h"
#include "Key_Default_Model.h"
#include "MJDefault.h"
#include "MJHelper.h"
#include "MJDefinitions.h"
#include "manager/CleverTapManager.h"

#define event_query_folder_size "system.query.folder_size"
#define event_query_audiobook_size "system.query.audiobook_size"
#define event_query_cached_size "system.query.cached_size"

USING_NS_CC;
INITIALIZE_READER_INSTANCE(StoryParentDeleteCache_LandscapeReader);

StoryParentDeleteCache_Landscape * StoryParentDeleteCache_Landscape::createPage() {
    StoryParentDeleteCache_Landscape * page;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        page = static_cast<StoryParentDeleteCache_Landscape *>(CSLoader::createNode("csb/StoryParentDeleteCache_Landscape.csb"));
    } else {
        page = static_cast<StoryParentDeleteCache_Landscape *>(CSLoader::createNodeWithVisibleSize("csb/StoryParentDeleteCache.csb"));
    }

    if (page) {
        page->didLoadFromCSB();
        return page;
    }

    CC_SAFE_DELETE(page);
    return nullptr;
}

StoryParentDeleteCache_Landscape::~StoryParentDeleteCache_Landscape()
{
	
}

void StoryParentDeleteCache_Landscape::didLoadFromCSB()
{
    _stories_size = _audiobooks_size = _cached_size = 0;
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);

    utils::findChild<ui::Text *>(this, "label_size")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Black.ttf");
    utils::findChild<ui::Text *>(this, "label_size")->setString("--- MB");

    auto text_confirm = utils::findChild<ui::Text *>(this, "label_confirm");
    text_confirm->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, text_confirm->getString()));

    auto text_info = utils::findChild<ui::Text *>(this, "label_info");
    text_info->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
    text_info->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, text_info->getString()));

    auto text_action = utils::findChild<ui::Text *>(this, "parent.delete_cache.action");
    text_action->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf");
    text_action->setString(LANGUAGE_MANAGER->getTextByKey(lang_id, text_action->getName()));
    
    auto text_warn = utils::findChild<ui::Text *>(this, "parent.delete_cache.warn");
    text_warn->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-MediumItalic.ttf");
    text_warn->setString(LANGUAGE_MANAGER->getDisplayTextByKey("parent.delete_cache.warn"));
    
    utils::findChild<ui::Text *>(this, "label_audiobooks")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf");
    utils::findChild<ui::Text *>(this, "label_stories")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf");

    utils::findChild<ui::Text *>(this, "label_stories")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("intro.load.stories"));
    utils::findChild<ui::Text *>(this, "label_stories")->setFontSize(22);
    utils::findChild<ui::Text *>(this, "label_audiobooks")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("sub.text.detail2"));
    
    utils::findChild<ui::Text *>(this, "label_details_single")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<ui::Text *>(this, "label_details_single")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("parent.delete.cache.single.info"));

    utils::findChild<ui::Text *>(this, "label_info_single")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    utils::findChild<ui::Text *>(this, "label_info_single")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("parent.delete.cache.single"));

    utils::findChild<ui::Text *>(this, "label_info_all")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");
    utils::findChild<ui::Text *>(this, "label_info_all")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("parent.delete.cache.all"));

    auto lv = utils::findChild<ui::ListView *>(this, "listview");
    lv->setScrollBarEnabled(false);
    auto w = 0.f;
    for (auto c : lv->getItems()) {
        w += c->getContentSize().width;
    }
    w += lv->getItemsMargin();
    lv->setContentSize(Size(w, lv->getContentSize().height));
    lv->setPosition(Point(lv->getParent()->getContentSize().width / 2, 180));
    
    auto listener_get_folder_size = EventListenerCustom::create(event_query_folder_size, [=](EventCustom * e) {
        _stories_size = * static_cast<long *>(e->getUserData());
        utils::findChild<ui::Text *>(this, "label_size")->setString(StringUtils::format("%.2f MB", (_stories_size + _audiobooks_size + _cached_size) / 1000000.f));
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_get_folder_size, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_query_audiobook_size, [=](EventCustom * e) {
        _audiobooks_size = * static_cast<long *>(e->getUserData());
        utils::findChild<ui::Text *>(this, "label_size")->setString(StringUtils::format("%.2f MB", (_stories_size + _audiobooks_size + _cached_size) / 1000000.f));
    }), this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(event_query_cached_size, [=](EventCustom * e) {
        _cached_size = * static_cast<long *>(e->getUserData());
        utils::findChild<ui::Text *>(this, "label_size")->setString(StringUtils::format("%.2f MB", (_stories_size + _audiobooks_size + _cached_size) / 1000000.f));
    }), this);

    startQueryFolderSize();
}

void StoryParentDeleteCache_Landscape::onShow(){
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->setPositionY(768);
        root_layout->runAction(MoveBy::create(0.25f, Vec2(0, -768)));
        static_cast<Text *>(ui::Helper::seekWidgetByName(root_layout, "title_portrait"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "delete_cache"));
    }
}

void StoryParentDeleteCache_Landscape::startQueryFolderSize() {
    auto resource_folder = FileUtils::getInstance()->getWritablePath() + "mjstory";
    mj::helper::getFolderSize(resource_folder, event_query_folder_size);
    
    auto audiobook_folder = FileUtils::getInstance()->getWritablePath() + "audiobooks";
    mj::helper::getFolderSize(audiobook_folder, event_query_audiobook_size);
    
    auto cached_folder = FileUtils::getInstance()->getWritablePath() + "cached";
    mj::helper::getFolderSize(cached_folder, event_query_cached_size);
}

ui::Widget::ccWidgetClickCallback StoryParentDeleteCache_Landscape::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentDeleteCache_Landscape, onBack),
        CLICK_MAP(StoryParentDeleteCache_Landscape, onOK),
        CLICK_MAP(StoryParentDeleteCache_Landscape, onDeleteStory),
        CLICK_MAP(StoryParentDeleteCache_Landscape, onDeleteAudiobook),
    };
    
//    if (callback_name == "onBack") {
//        return CC_CALLBACK_1(StoryParentDeleteCache_Landscape::onBack, this);
//    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (callback_name == "onBack") {
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onBack(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
        
        return CC_CALLBACK_1(StoryParentDeleteCache_Landscape::onBack, this);
    }
#endif
    
    if (function_map.count(callback_name) > 0) {
        return function_map[callback_name];
    }
    return nullptr;
}

#include "MSHome.h"
#include "MSIntroGrade.h"
void StoryParentDeleteCache_Landscape::onOK(cocos2d::Ref *sender) 
{
	m_deletingProperty = "all";
	pushCleverTapTrackingData();

    mj::helper::playButtonFX();
    mj::PageLoadResource::getInstance().deleteCacheAllStory();
    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
    FileUtils::getInstance()->purgeCachedEntries();
    
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25, MSIntroGrade::createScene()));
}

void StoryParentDeleteCache_Landscape::onBack(Ref *sender){
    if (_is_back) return;
    _is_back = true;
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }
}

#include "MSExploreView.h"
#include "MSExploreView_Audiobooks.h"
#include "MSGradeManager.h"

void StoryParentDeleteCache_Landscape::onDeleteStory(cocos2d::Ref *sender) 
{
    MJDEFAULT->setBoolForKey(key_click_on_delete_stories, true);
	m_deletingProperty = "story";
	pushCleverTapTrackingData();

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);
    auto saved = ms::GradeManager::getInstance().getCurrentGradeId();
    ms::GradeManager::getInstance().setCurrentGradeId(-1);
    
    std::string title = LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "downloaded.stories");
    auto explorer = MSExploreView::createView(title, MSExploreView::CollectionType::DOWNLOADED, ms::GradeManager::getInstance().getCurrentGradeId());
    explorer->setContentSize(Director::getInstance()->getVisibleSize());
    explorer->enableEditMode("downloaded.edit");
    ui::Helper::doLayout(explorer);
    Director::getInstance()->getRunningScene()->addChild(explorer, 1);
    ms::GradeManager::getInstance().setCurrentGradeId(saved);
}

void StoryParentDeleteCache_Landscape::onDeleteAudiobook(cocos2d::Ref *sender) {

	m_deletingProperty = "audiobook";
	pushCleverTapTrackingData();

    ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::AUDIOBOOKS);
    auto saved = ms::GradeManager::getInstance().getCurrentGradeId();
    ms::GradeManager::getInstance().setCurrentGradeId(-1);
    
    std::string title = LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "downloaded.audiobooks");
    auto explorer = MSExploreView_Audiobooks::createView(title, MSExploreView_Audiobooks::CollectionType::DOWNLOADED, ms::GradeManager::getInstance().getCurrentGradeId());
    explorer->setContentSize(Director::getInstance()->getVisibleSize());
    explorer->enableEditMode("downloaded.edit");
    explorer->enablePlayAll(false);
    ui::Helper::doLayout(explorer);
    Director::getInstance()->getRunningScene()->addChild(explorer, 1);
    ms::GradeManager::getInstance().setCurrentGradeId(saved);
}

void StoryParentDeleteCache_Landscape::pushCleverTapTrackingData()
{
	try
	{
		CleverTapManager::GetInstance()->pushEvent("delete_cache",
		{
			{"view_screen",cocos2d::Value(true)},
			{"delete_properties",cocos2d::Value(m_deletingProperty)},
		});
	}
	catch (const std::exception&)
	{

	}

}
