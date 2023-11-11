//
//  StoryParentLanguages_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/14/17.
//
//

#include "StoryParentLanguages_Landscape.h"
#include "GlobalFunc.h"
#include "MJDefault.h"
#include "HSLib.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "manager/CleverTapManager.h"
#include "APUpdateUserInfoAPI.h"
#include "APDatabase.h"
#include "APWaitView.h"

INITIALIZE_READER_INSTANCE(StoryParentLanguages_LandscapeReader);

StoryParentLanguages_Landscape * StoryParentLanguages_Landscape::createPage() 
{
    StoryParentLanguages_Landscape *p;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        auto csb_name = mj::helper::getCSBName("csb/StoryParentLanguages_Landscape.csb");
        //auto csb_name = "csb/StoryParentLanguages_Landscape.csb";
        p = static_cast<StoryParentLanguages_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    }else p = static_cast<StoryParentLanguages_Landscape *>(cocos2d::CSLoader::createNode("csb/StoryParentLanguages.csb"));
    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

StoryParentLanguages_Landscape::~StoryParentLanguages_Landscape()
{
	pushCleverTapEvent();
}

void StoryParentLanguages_Landscape::didLoadFromCSB()
{
	beforeLanguageID = MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);

    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        this->setContentSize(DIRECTOR->getVisibleSize());
        ui::Helper::doLayout(this);
    }
    
    changeDisplayLanguage();
}

void StoryParentLanguages_Landscape::changeDisplayLanguage()
{
    current_lang = LANGUAGE_MANAGER->getDisplayLangId() ;
    auto language_name = StringUtils::toString(current_lang);
    auto content = dynamic_cast<cocos2d::ui::Layout *>(ui::Helper::seekWidgetByName(this,"content"));
    //this->getChildByName<cocos2d::ui::Layout *>("content");
    
	auto title = content->getChildByName<cocos2d::ui::Text *>("title");
    
	if (title) 
	{
        title->setFontName(current_lang == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Bold.ttf");

        title->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "language_title"));
    }
    
	for (auto item : content->getChildren()) 
	{
        auto button = dynamic_cast<cocos2d::ui::Button *>(item);
        
		if (button) 
		{
            if (button->getName() == language_name) 
			{
                button->setOpacity(255);
            } 
			else 
			{
                button->setOpacity(90);
            }
            
            auto lang_id = atoi(button->getName().c_str());
            
            auto text_lang = dynamic_cast<cocos2d::ui::Text*>(ui::Helper::seekWidgetByName(this, StringUtils::format("text%d", lang_id)));
            
            if (text_lang)
            {
                text_lang->setFontName(lang_id == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Medium.ttf");
				text_lang->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", StringUtils::format("language%d", lang_id)));

				if (auto parent = dynamic_cast<ui::ImageView*>(text_lang->getParent()))
				{
					if (text_lang->getContentSize().width > parent->getContentSize().width)
					{
						text_lang->setScale(parent->getContentSize().width / text_lang->getContentSize().width);
					}
				}
            }
        }
    }
    
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title_portrait"))->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("parentpage", "language_title_portrait"));
    }
    
    if (CONFIG_MANAGER->isVietnameseStories()) {
        title->setTextColor(Color4B::WHITE);
    }
}

void StoryParentLanguages_Landscape::onShow(){
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->setPositionY(768);
        root_layout->runAction(MoveBy::create(0.25f, Vec2(0, -768)));
    }
}


cocos2d::ui::Widget::ccWidgetClickCallback StoryParentLanguages_Landscape::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, cocos2d::ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryParentLanguages_Landscape, onSelectLanguage),
        CLICK_MAP(StoryParentLanguages_Landscape, onBack),
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

void StoryParentLanguages_Landscape::onSelectLanguage(cocos2d::Ref *sender) 
{
    mj::helper::playButtonFX();
    
    auto wait_view = APWaitView::createView(std::string(""), false, true);
    
    Director::getInstance()->getRunningScene()->addChild(wait_view,INT_MAX - 1);
    
    auto itemchoose_name = static_cast<cocos2d::Node *>(sender)->getName();
    auto content = dynamic_cast<cocos2d::ui::Layout *>(ui::Helper::seekWidgetByName(this,"content"));

    for (auto item : content->getChildren()) 
	{
        auto button = dynamic_cast<cocos2d::ui::Button *>(item);
        if (button)
		{
            if (button->getName() == itemchoose_name) 
			{
                button->setOpacity(255);
                auto lang_id = atoi(itemchoose_name.c_str());
                
                if (lang_id != current_lang) 
				{
                    //change display language
                
					MJDEFAULT->setIntegerForKey(key_lang_display, lang_id);
                    LANGUAGE_MANAGER->updateLanguageWithID(lang_id);
                    
                    changeDisplayLanguage();
                    
                    cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_change_displaylang);
                    
                    mm::RealtimeDatabase::getInstance().updateUserSettings(ap::Database::getInstance().getUserInfo()._id);
                    //ap::api::UpdateUserInfo::call(ap::Database::getInstance().getUserInfo());
                }
                
                CCLOG("lang_id = %d", lang_id);
            } 
			else 
			{
                button->setOpacity(90);
            }
        }
    }
    
    Director::getInstance()->getRunningScene()->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=](){
        
        wait_view->removeFromParent();

    })));

}

void StoryParentLanguages_Landscape::onBack(Ref *sender){
    if (isBack) return;
    isBack = true;
    auto root_layout = static_cast<Layout *>(ui::Helper::seekWidgetByName(this, "root_layout"));
    if (root_layout) {
        root_layout->runAction(Sequence::create(MoveBy::create(0.25f, Vec2(0, 768)), CallFunc::create([=]{
            this->removeFromParent();
        }), NULL));
    }
}

void StoryParentLanguages_Landscape::pushCleverTapEvent()
{
	try
	{
		if (beforeLanguageID != MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE))
		{
			CleverTapManager::GetInstance()->pushEvent("click_display_language",
				{
					{"view_screen",cocos2d::Value(true)},
					{"language",cocos2d::Value(MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE) == LANGUAGE_EN_CODE ? "en" : "vi")},
				});
		}
	}
	catch (const std::exception&)
	{

	}
}
