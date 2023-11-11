//
//  APChooseProfileView.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#include "APChooseProfileView.h"
#include "APDatabase.h"
#include "APProfileEditView.h"
#include "MSIntro.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "APProfileManager.h"
#include "StoryConfigManager.h"
#include "APProfileNameView.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"
#include "MSHome.h"
#include "MSInstallPromotion.h"
#include "StoryWelcomePopUp.h"
#include "MSEventLogger.h"
#include "MSIntroLicense.h"
#include "APWaitView.h"
#include "APProfileFinishView.h"
#include "APDatabase.h"
#include "MMFirebaseDatabase.h"
#include "APUpdateUserInfoAPI.h"
#include "APProfileFinishView.h"
#include "MJCalculator.h"
#include "MSFirebasePlugin.h"

USING_NS_CC;

INITIALIZE_READER(APChooseProfileView);

std::once_flag ap_choose_profile_view_reader_flag;

APChooseProfileView * APChooseProfileView::createView()
{
	std::call_once(ap_choose_profile_view_reader_flag, []
    {
		REGISTER_CSB_READER(APChooseProfileView);
	});
    
	auto node = reinterpret_cast<APChooseProfileView *>(CSLoader::createNodeWithVisibleSize("csb/account/ChooseProfile.csb"));
    
	if (node)
    {
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
    
	return nullptr;
}

void APChooseProfileView::didLoadFromCSB()
{
	if (CONFIG_MANAGER->isVietnameseStories())
    {
		utils::findChild<Layout*>(this, "Panel_1")->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
	}

	utils::findChild<Layout*>(this, "Panel_4")->setVisible(false);
	utils::findChild<Layout*>(this, "Panel_4")->setTouchEnabled(false);

	_edit_enabled = false;
	m_lv_profile = cocos2d::utils::findChild<ui::ListView*>(this, "lvProfile");
	m_lv_profile->setScrollBarColor(Color3B(255, 255, 255));
	m_lv_profile->setScrollBarAutoHideEnabled(false);
	m_lv_profile->setScrollBarEnabled(true);
	m_lv_profile->removeAllItems();
	auto list_width = 0;

	auto list_profile = ap::Database::getInstance().getProfiles();
	for (auto profile : list_profile)
	{
		auto card_profile = APProfileCard::createCard(profile);
		card_profile->setDelegate(this);
		m_lv_profile->pushBackCustomItem(card_profile);
		list_width += card_profile->getContentSize().width + m_lv_profile->getItemsMargin();
	}
	if (list_width > 0)
	{
		list_width -= m_lv_profile->getItemsMargin();
	}
	if (list_profile.size() < 4)
	{
		m_lv_profile->setContentSize(cocos2d::Size(list_width, m_lv_profile->getContentSize().height));
	}

	auto btn = utils::findChild<ui::Button*>(this, "btn_create");
    
	if (auto text = utils::findChild<ui::Text*>(btn, "Text_1_0_0"))
	{
		text->setTextColor(CONFIG_MANAGER->isVietnameseStories() ? Color4B(28, 34, 96,255) : Color4B(34, 187, 231,255));
    }
    
	if (list_profile.size() >= MJDEFAULT->getIntegerForKey(key_max_account_profile))
    {
		btn->setVisible(false);
	}
    
    auto screenSize = Director::getInstance()->getVisibleSize();

    auto origin     = Director::getInstance()->getVisibleOrigin();

    const float k_currScreenRatio = (screenSize.width / screenSize.height);

    if(k_currScreenRatio >= 19.f/9.f) //20/9,21/9
    {
        if(auto bg =  utils::findChild<Layout*>(this, "Panel_1"))
        {
            bg->setIgnoreAnchorPointForPosition(false);
            bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            bg->setScale((1.1f*screenSize.width)/bg->getContentSize().width);//110%
            bg->setPosition(bg->getParent()->getContentSize()/2);
        }
    }
    
    if(auto button = utils::findChild<ui::Button*>(this, "button_lock"))
    {
        button->setIgnoreAnchorPointForPosition(false);
        button->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        button->setPosition(Vec2(screenSize.width - button->getContentSize().width,+ button->getContentSize().height));
    }
}

void APChooseProfileView::onEnter()
{
	ui::Layout::onEnter();
	changeDisplayLanguage();
	if (m_lv_profile->getItems().size() == 0)
	{
		this->scheduleOnce([=](float)
		{
			onNewProfile(this);
		}, 0.1f, "delay on enter");
	}
}

void APChooseProfileView::onExit()
{
	ui::Layout::onExit();
}

void APChooseProfileView::changeDisplayLanguage()
{
    if(auto text = utils::findChild<Text*>(this, "Text_1"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.account.profile.title"));
    }
    
    if(auto text = utils::findChild<Text*>(utils::findChild<Button*>(this, "btn_create"),"Text_1_0_0"))
    {
        text->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE?"fonts/leelawdb.ttf":"fonts/Montserrat-Medium.ttf");
        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.onboard.account.new.profile"));
    }
    
    if(auto text = utils::findChild<Text*>(utils::findChild(this, "button_lock"),"sidebar.des.parents"))
    {
        LANGUAGE_MANAGER->setFontByLang(text,StoryLanguageManager::TextType::BOLD);

        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.suggest.adding.profile"));
    }
}

void APChooseProfileView::onNewProfile(Ref * sender)
{
	if (!ap::Database::getInstance().isReachMaxProfile()) // nếu chưa max số profile
	{
        MJDEFAULT->setBoolForKey("will_create_new_profile",ap::Database::getInstance().getTotalProfile() <= 0);

		auto profileName = APProfileNameView::createView();//kien
        profileName->setEnableCloseButton(ap::Database::getInstance().getTotalProfile() >= 1);
		profileName->setDelegate(this);
		this->addChild(profileName);
	}
	else
	{
		StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.full.profiles"), "OK",LANGUAGE_MANAGER->getDisplayTextByKey("story.download.failure.close"));
	}
}

void APChooseProfileView::onClose(cocos2d::Ref * sender)
{
	if (_edit_enabled)
	{
		_edit_enabled = false;
		utils::findChild(this, "btn_create")->setVisible(true);
		for (auto profile : m_lv_profile->getItems())
		{
			static_cast<APProfileCard*>(profile)->setCardMode(APProfileCard::APCARD_MODE::NORMAL);
		}
	}
	else
	{
		this->removeFromParent();
	}

}

void APChooseProfileView::onLock(cocos2d::Ref *sender)
{
    auto items = m_lv_profile->getItems();
    
    if(!items.empty())
    {
        MJDEFAULT->setBoolForKey("key_parent_in_choose_view_button_clicked", true);
        
        onClickProfileCard(items.front());
    }
}

void APChooseProfileView::onClickProfileCard(cocos2d::Ref * sender)
{
    showWaittingView();
    this->scheduleOnce([=](float)
    {
        auto card = static_cast<APProfileCard*>(sender);
        onProfileChoosed(card->getProfile().profile_id, false);
    }, 0.2f, "delay on enter");
}

void APChooseProfileView::onProfileCreated(int profile_id)
{
	onProfileChoosed(profile_id,true);
}

void APChooseProfileView::goHome()
{
    if (!MJDEFAULT->getBoolForKey("key_profile_welcome_back", false) && !MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
    {
        MJDEFAULT->setBoolForKey("key_profile_welcome_back", true);
        removeWaittingView();
        auto finish_view = APProfileFinishView::createView(ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id)), _is_welcome_new);
        Director::getInstance()->getRunningScene()->addChild(finish_view, INT_MAX - 1);
        return;
    };
    auto home = MSHome::createScene();
    
    if (!CONFIG_MANAGER->isAppActive())
    {
        if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false))
        {
            if (MSInstallPromotion::shouldShowPopup())
            {
                auto popup = MSInstallPromotion::createView(true);
                home->addChild(popup);
            }
            else
            {
				if (!MJDEFAULT->getBoolForKey("is_skip_login", false) && !MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
                {
					auto popup = StoryWelcomePopUp::createPage();
                    popup->setName("StoryWelcomePopUp");
					home->addChild(popup);
				}
            }
        }
    }

    if(MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
    {
        MJDEFAULT->setBoolForKey("key_parent_in_choose_view_button_clicked", false);
        
        if (auto homeView = (MSHome*)home->getChildByName(MSHOME_NAME))
        {
            auto waitView = APWaitView::createView(std::string(""), false, true);
            waitView->setName("waiting_view");
            home->addChild(waitView,INT_MAX-1);
            
            homeView->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=](){
                
                MJDEFAULT->setBoolForKey(key_show_parent_lock, false);

                waitView->removeFromParent();
                homeView->clearLayouts();
                homeView->goToParent();
                
            })));
        }
    }
    
    // nếu vào qua deep link
    if (_is_welcome_new && ms::analytics::Firebase::QR_DEEP_LINK.find("newprofile") != std::string::npos)
    {
        if (auto homeView = (MSHome *)home->getChildByName(MSHOME_NAME))
        {
            homeView->goToAccountProfileDetailLayout();
        }
    }

    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
}

void APChooseProfileView::onProfileChoosed(int profile_id,bool is_new )
{
    
    _is_welcome_new = is_new;
	PROFILE_MANAGER.getInstance().switchProfile(profile_id);
	if (MJDEFAULT->getBoolForKey(key_first_time_created_profile, true)) // convert profile at first time
	{
		MJDEFAULT->setBoolForKey(key_first_time_created_profile, false);
		PROFILE_MANAGER.loadDataFromMJDefault();
        MJDEFAULT->setBoolForKey(key_data_converted, true);
    }
    
	PROFILE_MANAGER.clearDataModel();
	PROFILE_MANAGER.getInstance().loadProcess();

	auto user_info = ap::Database::getInstance().getUserInfo();

    if((MJDEFAULT->getBoolForKey(key_Actived, false) && !MJDEFAULT->getBoolForKey("check_is_free", false))
       || MJDEFAULT->getBoolForKey(key_is_submit)
       || !MJDEFAULT->getBoolForKey("is_license_feature_enable", false)
       || CONFIG_MANAGER->isVietnameseStories())
    {
            goHome();
    }
    else
    {
        ms::EventLogger::getInstance().logEvent("view_license_intro", {});
        auto license_confirm = APWaitView::createView("Bạn đã có MÃ KÍCH HOẠT chưa ?", true, false);
        license_confirm->setVisible(this->isVisible());
        license_confirm->setName("loadview");
        license_confirm->setOnConfirmCallback([=](bool confirm) {
            
            Director::getInstance()->getRunningScene()->removeChildByName("loadview");
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]  {
                
                if (confirm)
                {
                    ms::EventLogger::getInstance().logEvent("click_license_confirm", {});
                    auto intro_license = MSIntroLicense::showPopup();
                    intro_license->setOnLicenseCallback([=](bool success) {
                        goHome();
                    });
                    
                    Director::getInstance()->getRunningScene()->addChild(intro_license);
                }
                else
                {
                    ms::EventLogger::getInstance().logEvent("click_license_skip", {});
                    goHome();
                }
            });
            
        });
        Director::getInstance()->getRunningScene()->addChild(license_confirm);
    }
}

ui::Widget::ccWidgetClickCallback APChooseProfileView::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
		CLICK_MAP(APChooseProfileView, onNewProfile),
        CLICK_MAP(APChooseProfileView, onLock),        
		CLICK_MAP(APChooseProfileView, onClose)
	};

	if (function_map.find(name) != function_map.end())
    {
		return function_map.at(name);
	}

	return nullptr;
}
void APChooseProfileView::showWaittingView()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
    {
        auto waitView = APWaitView::createView(std::string(""), false, true);
        waitView->setName("waiting_view");
        Director::getInstance()->getRunningScene()->addChild(waitView,INT_MAX-1);
    });
}
void APChooseProfileView::removeWaittingView()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
    {
        if (auto waitView = Director::getInstance()->getRunningScene()->getChildByName("waiting_view"))
        {
            waitView->removeFromParent();
        }
    });
}

void APChooseProfileView::hideCreateNewProfilesButton()
{
    if(auto hideCreateNewProfilesButtonPanel = utils::findChild(utils::findChild(this, "Panel_1"), "Panel_3"))
    {
        hideCreateNewProfilesButtonPanel->setVisible(false);
    }
    
    if(m_lv_profile)
    {
        m_lv_profile->setScale(1.1f);//110%.
    }
}




