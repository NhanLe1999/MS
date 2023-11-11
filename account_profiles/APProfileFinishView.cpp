#include "APProfileFinishView.h"

#include "MSIntro.h"
#include "MSIntroGrade.h"

#include "MJDefault.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#include "MJPlatformConfig.h"
#include "MSHome.h"
#include "MSInstallPromotion.h"
#include "StoryWelcomePopUp.h"

USING_NS_CC;

INITIALIZE_READER(APProfileFinishView);

std::once_flag ap_profile_finish_view_reader_flag;

APProfileFinishView * APProfileFinishView::createView(const std::string & name, const bool is_boy, const int age,
	const std::string & avatar, bool is_new) {
	std::call_once(ap_profile_finish_view_reader_flag, [] {
		REGISTER_CSB_READER(APProfileFinishView);
		});

	auto p = reinterpret_cast<APProfileFinishView*>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileFinish.csb"));
	if (p) {
		p->didLoadFromCSB(name, is_boy, age, avatar,is_new);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

APProfileFinishView * APProfileFinishView::createView(const ap::Profile& profile, bool is_new)
{
	std::call_once(ap_profile_finish_view_reader_flag, []
    {
		REGISTER_CSB_READER(APProfileFinishView);
	});

	auto p = reinterpret_cast<APProfileFinishView*>(CSLoader::createNodeWithVisibleSize("csb/account/ProfileFinish.csb"));
	
    if (p)
    {
		p->didLoadFromCSB(profile.name, profile.gender == ap::Profile::MALE, profile.age, profile.avatar,is_new);
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void APProfileFinishView::didLoadFromCSB(const std::string & name, const bool is_boy, const int age, const std::string & avatar, bool is_new)
{
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        utils::findChild<Layout*>(this, "Panel_1")->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
    }
    
	auto text_welcome = utils::findChild<ui::Text*>(this, "text_welcome");
	auto appname = MJ_PLATFORM_CONFIG.getAppName();
    
    auto welcome = is_new ? StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("key.welcome.to").c_str(), name.c_str(), appname.c_str()):StringUtils::format(LANGUAGE_MANAGER->getDisplayTextByKey("key.welcome.back").c_str(), name.c_str());

    text_welcome->setString(welcome);
    text_welcome->setTextVerticalAlignment(TextVAlignment::CENTER);
    text_welcome->setTextHorizontalAlignment(TextHAlignment::CENTER);
    
    utils::findChild<ui::Text*>(this, "name")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf":"fonts/Montserrat-SemiBold.ttf");
	utils::findChild<ui::Text*>(this, "name")->setString(name);
	utils::findChild<ui::Text*>(this, "age")->setString(StringUtils::format("Age %d", age));
	utils::findChild<ui::ImageView*>(this, "avatar")->loadTexture(avatar);

    if (MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        utils::findChild<ui::Text*>(this, "age")->setVisible(false);
    }
    
	auto screenSize = Director::getInstance()->getVisibleSize();

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
}

void APProfileFinishView::onEnter()
{
	ui::Layout::onEnter();

    this->scheduleOnce([](float dt)
    {
        MJDEFAULT->setBoolForKey(key_onboarding_license, true);
        //MJDEFAULT->setBoolForKey(key_first_api_called, true);
        MJDEFAULT->setBoolForKey(key_logged_in, true);
        //Director::getInstance()->replaceScene(MSIntroGrade::createScene());
        
        auto home = MSHome::createScene();
        
        if (!CONFIG_MANAGER->isAppActive())
        {
            if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false)) {
                if (MSInstallPromotion::shouldShowPopup()) {
                    auto popup = MSInstallPromotion::createView(true);
                    home->addChild(popup);
                }
                else {
					if (!MJDEFAULT->getBoolForKey("is_skip_login", false)  && !MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
                    {
						auto popup = StoryWelcomePopUp::createPage();
                        popup->setName("StoryWelcomePopUp");
						home->addChild(popup);
					}
                }
            }
        }

        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
    }, 1.5f, "delay_finish_login");
}

void APProfileFinishView::onExit()
{
	ui::Layout::onExit();
}

void APProfileFinishView::changeDisplayLanguage()
{
    
}

void APProfileFinishView::onClose(cocos2d::Ref * sender)
{
	this->removeFromParent();
}

ui::Widget::ccWidgetClickCallback APProfileFinishView::onLocateClickCallback(const std::string & callBackName)
{
	return nullptr;
}

