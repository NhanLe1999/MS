//
//  StoryParentMusicSettings_Landscape.cpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/1/18.
//

#include "StoryParentMusicSettings_Landscape.h"
#include "GlobalFunc.h"
#include "MJDefault.h"
#include "HSLib.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "manager/CleverTapManager.h"

INITIALIZE_READER_INSTANCE(StoryParentMusicSettings_LandscapeReader);

StoryParentMusicSettings_Landscape* StoryParentMusicSettings_Landscape::createPage()
{
    StoryParentMusicSettings_Landscape *p = static_cast<StoryParentMusicSettings_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentMusicSettings_Landscape.csb"));;
    
    if (p) 
	{
        p->didLoadFromCSB();
        return p;
    }
    
    CC_SAFE_DELETE(p);
    return nullptr;
}

void StoryParentMusicSettings_Landscape::didLoadFromCSB()
{
	m_beforeState = MJDEFAULT->getBoolForKey(key_background_music);

    if (!CONFIG_MANAGER->isHomeScreenLandscape())
	{
        this->setContentSize(DIRECTOR->getVisibleSize());
        ui::Helper::doLayout(this);
    }
    
    utils::findChild<Text*>(this, "Text_1")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("background.music.on.off"));
    utils::findChild<Text*>(this, "Text_1")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
    auto is_switched = MJDEFAULT->getBoolForKey(key_background_music);
    auto btn = utils::findChild<Button*>(this, "Button_1");
    btn->loadTextureNormal(StringUtils::format("mjstory/new_ui_vn/btn_switch_%d.png", is_switched));
}

StoryParentMusicSettings_Landscape::~StoryParentMusicSettings_Landscape()
{
	pushCleverTapEvents();
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentMusicSettings_Landscape::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentMusicSettings_Landscape, onSwitchedButton),
        CLICK_MAP(StoryParentMusicSettings_Landscape, onBack),
    };
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
    }
#endif
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

void StoryParentMusicSettings_Landscape::onSwitchedButton(cocos2d::Ref *sender)
{
    auto button = static_cast<cocos2d::ui::Button *>(sender);
    auto is_switched = MJDEFAULT->getBoolForKey(key_background_music);
    
    if(is_switched)
	{
        AUDIO_ENGINE->stopBackgroundMusic();
    }
	else
	{
        if(CONFIG_MANAGER->isVietnameseStories())
		{
            AUDIO_ENGINE->playBackgroundMusic("sounds/bg_music.mp3");
        }
    }
    
    is_switched = !is_switched;
    MJDEFAULT->setBoolForKey(key_background_music, is_switched);
//    is_switched = MJDEFAULT->getBoolForKey(key_background_music);
    CCLOG("autoplay = %d", is_switched);
    button->loadTextureNormal(StringUtils::format("mjstory/new_ui_vn/btn_switch_%d.png", is_switched));
	
}

void StoryParentMusicSettings_Landscape::onBack(Ref *sender)
{
    if (isBack) return;
    isBack = true;
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }
}

void StoryParentMusicSettings_Landscape::pushCleverTapEvents()
{
	try
	{
		if (m_beforeState != MJDEFAULT->getBoolForKey(key_background_music))
		{
			CleverTapManager::GetInstance()->pushEvent("background_music",
			{
				{"view_screen",cocos2d::Value(true)},
				{"turn_on_background_music",cocos2d::Value(MJDEFAULT->getBoolForKey(key_background_music) ? "on" : "off")},
			});
		}
	}
	catch (const std::exception&)
	{

	}
}
