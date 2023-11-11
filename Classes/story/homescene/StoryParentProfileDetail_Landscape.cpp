//
//  StoryParentProfileDetail_Landscape.cpp
//  MJStory
//
//  Created by Minh on 08/07/2019.
//

#include "StoryParentProfileDetail_Landscape.h"
#include "APDatabase.h"
#include "StoryParentEditProfile_Landscape.h"
#include  "StoryParentAccountProfile_Landscape.h"
#include "StoryLanguageManager.h"

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"

#include "APProfileNameView.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"

USING_NS_CC;

INITIALIZE_READER(StoryParentProfileDetail_Landscape);

std::once_flag story_parent_profile_detail_reader_flag;

StoryParentProfileDetail_Landscape * StoryParentProfileDetail_Landscape::createPage()
{
	std::call_once(story_parent_profile_detail_reader_flag, [] {
		REGISTER_CSB_READER(StoryParentProfileDetail_Landscape);
	});
	auto node = reinterpret_cast<StoryParentProfileDetail_Landscape *>(CSLoader::createNodeWithVisibleSize("csb/StoryParentProfileDetail_Landscape.csb"));
	if (node) {
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

void StoryParentProfileDetail_Landscape::didLoadFromCSB() {

	_edit_enabled = false;
	m_lv_profile = cocos2d::utils::findChild<ui::ListView*>(this, "lvProfile");
	m_lv_profile->setScrollBarColor(Color3B(255, 255, 255));
	m_lv_profile->setScrollBarAutoHideEnabled(false);
	m_lv_profile->setScrollBarEnabled(true);
	m_lv_profile->removeAllItems();

	auto lv_pos = m_lv_profile->getPosition();
	auto list_width = 0.0f;
	auto list_profile = ap::Database::getInstance().getProfiles();
    auto current_it = 0;
    int i = 0;
	for (auto profile : list_profile)
	{
		auto card_profile = APProfileCard::createCard(profile);
		card_profile->setDelegate(this);
        auto rate_scale = 0.8;
        card_profile->setScale(rate_scale);
		m_lv_profile->pushBackCustomItem(card_profile);
		list_width += card_profile->getContentSize().width*rate_scale + m_lv_profile->getItemsMargin();
        
        if(profile.profile_id == MJDEFAULT->getIntegerForKey(key_current_profile_id)){
            current_it = i;
        }
        i++;
	}
	if (list_width > 0)
	{
		list_width -= m_lv_profile->getItemsMargin();
	}

    m_lv_profile->setContentSize(Size (list_width , m_lv_profile->getContentSize().height));
    
	if (list_profile.size() < 4)
	{
        m_lv_profile->setContentSize(cocos2d::Size(list_width, m_lv_profile->getContentSize().height));
//        m_lv_profile->setAnchorPoint(Point::ANCHOR_MIDDLE);
        m_lv_profile->setPositionX(this->getContentSize().width * 0.5);
        
    } else {
//        m_lv_profile->setScrollBarEnabled(true);
        m_lv_profile->setMagneticType(ui::ListView::MagneticType::CENTER);
        
        m_lv_profile->setCurSelectedIndex(current_it);
    }
    
	if (list_width > this->getContentSize().width)
	{
//        m_lv_profile->setContentSize(cocos2d::Size(list_width - 233, m_lv_profile->getContentSize().height));
	}
	// hide edit button if don't have profile
	if (list_profile.empty())
	{
		utils::findChild(this,"btn_edit")->setVisible(false);
		auto btn_create = utils::findChild<ui::Button*>(this, "btn_create");
		btn_create->setPositionType(PositionType::PERCENT);
		btn_create->setPositionPercent(Vec2(0.5f,0.5f));
	}
    
    // hide create button if user has enough profile
    if(list_profile.size() >= MJDEFAULT->getIntegerForKey(key_max_account_profile)){
        utils::findChild(this,"btn_create")->setVisible(false);
        auto btn_edit = utils::findChild<ui::Button*>(this, "btn_edit");
        btn_edit->setPositionType(PositionType::PERCENT);
        btn_edit->setPositionPercent(Vec2(0.5f,0.5f));
    }
	
}

void StoryParentProfileDetail_Landscape::onEnter()
{
    ui::Layout::onEnter();
    
    ms::EventLogger::getInstance().logEvent("view_settings_profile", {});
    
    std::vector<cocos2d::ui::Text*> textSameFonts =
    {
        utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_edit"), "Text_1_0_0"),
        utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_create"), "Text_1_0_0"),
        utils::findChild<Text*>(this, "Text_72")
    };
    
    for(auto text : textSameFonts)
    {
        if(!text)
        {
            continue;
        }
        
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    }

    utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_edit"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("favorite.edit"));
    utils::findChild<Text*>(utils::findChild<ui::Button*>(this,"btn_create"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.new.profile"));
    
    utils::findChild<Text*>(this, "Text_72")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.back"));
                                                        
}

void StoryParentProfileDetail_Landscape::onExit() {
	ui::Layout::onExit();
}

void StoryParentProfileDetail_Landscape::onNewProfile(Ref * sender)
{
	if (!ap::Database::getInstance().isReachMaxProfile()) // nếu chưa max số profile
	{
        ms::EventLogger::getInstance().logEvent("click_new_profile", {});
		auto profile_name = APProfileNameView::createView();
		profile_name->setDelegate(this);
		Director::getInstance()->getRunningScene()->addChild(profile_name);
	}
	else
	{
        StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.full.profiles"), "OK",LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
	}
}

void StoryParentProfileDetail_Landscape::onEditProfile(cocos2d::Ref * sender)
{

	if (m_delegate != nullptr)
	{
		if (auto delegate = dynamic_cast<StoryParentAccountProfile_Landscape*>(m_delegate))
		{
			delegate->onEditProfile();
		}
	}

    ms::EventLogger::getInstance().logEvent("click_edit_profile", {});
	_edit_enabled = true;
	utils::findChild(this, "btn_edit")->setVisible(false);
	utils::findChild(this, "btn_create")->setVisible(false);
	for (auto profile : m_lv_profile->getItems())
	{
		static_cast<APProfileCard*>(profile)->setCardMode(APProfileCard::APCARD_MODE::EDITABLE);
	}
}

void StoryParentProfileDetail_Landscape::onBack(cocos2d::Ref * sender)
{
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        this->removeFromParent();
    }), NULL));
}

void StoryParentProfileDetail_Landscape::onClose(cocos2d::Ref * sender)
{
	if (_edit_enabled)
	{
		_edit_enabled = false;
		utils::findChild(this, "btn_edit")->setVisible(true);
        
       // hide create button if user has enough profile
        if(ap::Database::getInstance().getProfiles().size() < MJDEFAULT->getIntegerForKey(key_max_account_profile)){
            utils::findChild(this, "btn_create")->setVisible(true);
        }
		
		for (auto profile : m_lv_profile->getItems())
		{
			static_cast<APProfileCard*>(profile)->setCardMode(APProfileCard::APCARD_MODE::NORMAL);
		}
	}
	else
	{
		if (m_delegate != nullptr)
		{
			if (auto delegate = dynamic_cast<StoryParentAccountProfile_Landscape*>(m_delegate))
			{
				delegate->setVisible(true);
			}
		}
		//// create subpage
		//auto page = StoryParentAccountProfile_Landscape::createPage();
		//page->adjustWidth(this->getContentSize().width);
		//this->getParent()->addChild(page);
		this->removeFromParent();
	}

}

void StoryParentProfileDetail_Landscape::onClickProfileIcon(cocos2d::Ref * sender)
{
	auto card = static_cast<APProfileCard*>(sender);
	if (card->getCardMode() == APProfileCard::EDITABLE)
	{
		auto edit_view = StoryParentEditProfile_Landscape::createView(card->getProfile());
		Director::getInstance()->getRunningScene()->addChild(edit_view, INT_MAX - 1);
		auto listener = cocos2d::EventListenerCustom::create("key.event.account.profile.edit.success",
			[this, card](EventCustom * event) {
			auto profile = (ap::Profile *) event->getUserData();
			auto profile_cpy = *profile; // copy value profile get from edit view
			if (profile)
			{
				this->scheduleOnce([profile_cpy, card](float dt) {
					card->setProfile(profile_cpy);
				}, 0.1f,"delay exit");

			}
			cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("key.event.account.profile.edit.success");
		});

		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void StoryParentProfileDetail_Landscape::onProfileCreated(int profile_id)
{
	this->didLoadFromCSB();

    CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN,[=](){
        
        const int k_currentProfileID = MJDEFAULT->getIntegerForKey(key_current_profile_id);

        CleverTapManager::GetInstance()->pushProfileByID(k_currentProfileID,CleverTapManager::PUSH_TYPE::LOGIN,nullptr);
        
    });
}

ui::Widget::ccWidgetClickCallback StoryParentProfileDetail_Landscape::onLocateClickCallback(const std::string & name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(StoryParentProfileDetail_Landscape, onNewProfile),
		CLICK_MAP(StoryParentProfileDetail_Landscape, onEditProfile),
		CLICK_MAP(StoryParentProfileDetail_Landscape, onClose),
        CLICK_MAP(StoryParentProfileDetail_Landscape, onBack)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}

void StoryParentProfileDetail_Landscape::pushCleverTapTrackingData(int profileID)
{
	try
	{
		auto profile = ap::Database::getInstance().getProfileById(profileID);

		std::vector<std::pair<std::string, cocos2d::Value>> eventTrackingProperties =
		{
		  {"view_screen", cocos2d::Value(true)},
		  {"profile_name", cocos2d::Value(profile.name)},
		  {"profile_gender", cocos2d::Value(profile.gender)},
		  {"profile_age", cocos2d::Value(profile.age)},
		};

		CleverTapManager::GetInstance()->pushEvent("create_new_profile", eventTrackingProperties);
	}
	catch (const char * i_errorCode)
	{
		CCLOG("Error: cannot push story tracking data to server !");
	}
}

