//
//  StoryParentReadingLevel_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/15/17.
//
//

#include "StoryParentReadingLevel_Landscape.h"
#include "../external/json/prettywriter.h"
#include <iostream>
#include "WebService.h"
#include "NativeBridge.h"
#include "HSLib.h"
#include "StoryDataModel.h"
#include "manager/CleverTapManager.h"

INITIALIZE_READER_INSTANCE(StoryParentReadingLevel_LandscapeReader);

StoryParentReadingLevel_Landscape * StoryParentReadingLevel_Landscape::createPage() {
    auto csb_name = CONFIG_MANAGER->isHomeScreenLandscape() ?
            "csb/StoryParentReadingLevel_Landscape.csb" :
            "csb/StoryParentReadingLevel.csb";
    StoryParentReadingLevel_Landscape * p;
    if (CONFIG_MANAGER->isHomeScreenLandscape()) {
        p = static_cast<StoryParentReadingLevel_Landscape *>(cocos2d::CSLoader::createNode(csb_name));
    } else {
        p = static_cast<StoryParentReadingLevel_Landscape *>(cocos2d::CSLoader::createNodeWithVisibleSize(csb_name));
    }

    if (p) {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

StoryParentReadingLevel_Landscape::~StoryParentReadingLevel_Landscape()
{
	
}

void StoryParentReadingLevel_Landscape::didLoadFromCSB() 
{
    StoryParentSubPage_Landscape::didLoadFromCSB();

	m_isPushEventEnable = false;

	grade_container = MJGradeInfoContainer::createView();
    auto placeholder = utils::findChild(this, "placeholder");
    grade_container->setName("grade_container");
    grade_container->setPosition(placeholder->getContentSize() / 2);
    grade_container->setAnchorPoint(Point::ANCHOR_MIDDLE);
	grade_container->setParentDelegate(this);
    placeholder->addChild(grade_container);
    ui::Helper::doLayout(this);
    grade_container->setContentSize(placeholder->getContentSize());
    ui::Helper::doLayout(grade_container);
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentReadingLevel_Landscape::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onBack") {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //back key listener for android
        auto backkey_listener = EventListenerKeyboard::create();
        backkey_listener->onKeyReleased = [=](EventKeyboard::KeyCode key_code, Event * event) {
            if (key_code == EventKeyboard::KeyCode::KEY_BACK) {
                this->onBack(nullptr);
            }
        };
        this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(backkey_listener, this);
#endif
        

        return CC_CALLBACK_1(StoryParentReadingLevel_Landscape::onBack, this);
    }
    return nullptr;
}

void StoryParentReadingLevel_Landscape::onBack(cocos2d::Ref *) {
    this->runAction(Sequence::create(MoveBy::create(0.25, Point(0, Director::getInstance()->getVisibleSize().height)), CallFunc::create([=] {
        this->removeFromParent();
    }), nullptr));
}

void StoryParentReadingLevel_Landscape::onEnter() {
    Layout::onEnter();
    auto grade_container = utils::findChild<MJGradeInfoContainer *>(this, "grade_container");
    auto placeholder = utils::findChild(this, "placeholder");

    grade_container->setContentSize(placeholder->getContentSize());
    grade_container->setPosition(placeholder->getContentSize() / 2);
    ui::Helper::doLayout(grade_container);
    grade_container->selectGrade(1);
    
    if (!CONFIG_MANAGER->isHomeScreenLandscape()) {
        this->setPosition(this->getPosition() + Point(0, Director::getInstance()->getVisibleSize().height));
        this->runAction(MoveBy::create(0.25, Point(0, -Director::getInstance()->getVisibleSize().height)));
    }
}

void StoryParentReadingLevel_Landscape::onSelectTab(MJGradeInfoTab * selected_tab, mj::model::GradeInfo info)
{
	if (m_isPushEventEnable)
	{
		pushCleverTapEvents();
	}
}

void StoryParentReadingLevel_Landscape::pushCleverTapEvents()
{
	try
	{
		std::string lvProperties = (grade_container != nullptr ? grade_container->getCurrentGrade() : "");

		CleverTapManager::GetInstance()->pushEvent("click_reading_levels",
			{
				{"view_screen",cocos2d::Value(true)},
				{"level_properties",cocos2d::Value(lvProperties)},
			});
	}
	catch (const std::exception&)
	{

	}
}