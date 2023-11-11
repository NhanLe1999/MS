//
//  StoryParentPage_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#include "StoryParentPage_Landscape.h"
#include "StoryParentHeader_Landscape.h"
#include "StoryParentSubPage_Landscape.h"
#include "StoryParentLicense_Landscape.h"
#include "StoryLanguageManager.h"
#include "HSLib.h"
#include "MJCalculator.h"
#include "MJDefinitions.h"
#include "MJDebugView.h"
#include "APProfileManagerView.h"
#include "MJPlatformConfig.h"
#include "MJDefinitions.h"
#include "StoryFreeManager.h"
#include "MJHelper.h"
#include "MSEventLogger.h"
#include "APPopup.h"
#include "MSIntro.h"
#include "APAuth.h"
#include "StoryDataModel.h"
#include "HelperManager.h"
#include "APResetPasswordScreen.h"

#define PATH_DELETE_ACCOUNT "https://monkey.edu.vn/deletion-account?app_id=40"

INITIALIZE_READER_INSTANCE(StoryParentPage_LandscapeReader);

#define READING_LEVEL_TAG   301

StoryParentPage_Landscape *StoryParentPage_Landscape::createPage()
{
    auto csb_name = mj::helper::getCSBName("csb/StoryParentPage_Landscape.csb");
    //auto csb_name = mj::helper::getCSBName("csb/new_setting.csb");
    
    auto p = static_cast<StoryParentPage_Landscape *>(cocos2d::CSLoader::createNodeWithVisibleSize(csb_name));
    
    if (p)
    {
        p->didLoadFromCSB();
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

bool StoryParentPage_Landscape::didLoadFromCSB()
{
    _not_select_display_language = true;
    _content    = static_cast<cocos2d::ui::Layout *>(cocos2d::ui::Helper::seekWidgetByName(this, "content"));
    title       = static_cast<Text *>(ui::Helper::seekWidgetByName(this, "title"));
   
    list_section = static_cast<cocos2d::ui::ListView *>(cocos2d::ui::Helper::seekWidgetByName(this, "list_section"));
    classes_name = "parentpage";
    changeTextDisplayLang();
    
    //TODO
    //new menu bg img
    if(CONFIG_MANAGER->isVietnameseStories())
    {
        auto menu_bg_img = static_cast<ImageView *>(ui::Helper::seekWidgetByName(this, "menu_bg_img"));
        menu_bg_img->setContentSize(Size(list_section->getContentSize().width + 30 , this->getContentSize().height + 30));

        if(Director::getInstance()->getVisibleSize().width/Director::getInstance()->getVisibleSize().height >= 19.f/9.0f)
        {
            if(auto home = utils::findChild<ui::Layout*>(this, "home"))
            {
                auto background = Sprite::create("mjstory/new_ui_vn/bg.jpg");
                background->setPosition(home->getContentSize()/2);
                background->setScale(Director::getInstance()->getVisibleSize().width/background->getContentSize().width);
                home->addChild(background,-1);

                home->removeBackGroundImage();
            }
        }
    }

    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *)
    {
        this->changeTextDisplayLang();
    });
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
    
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
//    utils::findChild<ui::Text *>(this, "build_label")->setString(StringUtils::format("Build No. %s", MJ_PLATFORM_CONFIG.getBuildNumber().c_str()));
//    utils::findChild(this, "button_reset")->setVisible(true);
//    utils::findChild(this, "label_reset")->setVisible(true);
//    utils::findChild(this, "button_reset_rate")->setVisible(true);
#else
    
#endif
    
    if ( MJDEFAULT->getBoolForKey(key_is_submit, false))
    {
        this->onSelectItem(SPO_LANGUAGES, "Notifications");
    }
    else
    {
        this->onSelectItem(SPO_NOTIFICATION, "Notifications");
    }
    return true;
}

void StoryParentPage_Landscape::goToLicensePage()
{
	this->onSelectItem(SPO_LICENSE, "License key");
}

void StoryParentPage_Landscape::goToAccountProfilePage()
{
    // trong hàm onSelectItem() thì subpage sẽ được add vào _content, trước khi add thì _content đã gọi hàm removeAllChildren() nên không sợ add nhiều page chồng lên nhau.
    this->onSelectItem(SPO_ACCOUNT_PROFILE, "");
    
    // check điều kiện key_show_parent_lock giống như ở onEnter() của MSTabSubscribe
    if (MJDEFAULT->getBoolForKey(key_show_parent_lock, true))
    {
        MJCalculator::showCalculator(this, nullptr, [=]
        {
            this->onBack(this);
        });
    }
}

void StoryParentPage_Landscape::changeTextDisplayLang()
{
    if(title)
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  title->setFontName("fonts/leelawdb.ttf") : title->setFontName("fonts/Montserrat-Bold.ttf");
        title->setString(LANGUAGE_MANAGER->getTextByKey(StoryLanguageManager::getDisplayLangId(), "settings.title"));
    }

    displayItem();

    if (_not_select_display_language)
    {
        _not_select_display_language = false;
        return;
    }

    auto list_section = utils::findChild<ui::ListView *>(this, "list_section");
    for (auto item : list_section->getItems())
    {
        auto item_casted = dynamic_cast<StoryParentItem_Landscape *>(item);
        if (item_casted)
            item_casted->setSelected(item_casted->getTag() == 201);
    }
}

void StoryParentPage_Landscape::displayItem()
{
    list_section->removeAllChildrenWithCleanup(true);
    auto menu_width = list_section->getContentSize().width;
    list_section->setItemsMargin(2);
    list_section->setScrollBarEnabled(false);

    CCLOG("Parent_plist_file = %s", StringUtils::format(
            "plist/parentitem_%d_%d.plist",
            MJDEFAULT->getIntegerForKey(key_lang_display, 1),
            CONFIG_MANAGER->isLicenceEnable()).c_str());

    auto list_object = cocos2d::FileUtils::getInstance()->getValueVectorFromFile(StringUtils::format(
                    "plist/parentitem_%d_%d.plist",
                    MJDEFAULT->getIntegerForKey(key_lang_display, 1),
                    CONFIG_MANAGER->isLicenceEnable()));

    auto list_height = 0;
    
    auto device_id = std::string("Device ID: ") + MJ_PLATFORM_CONFIG.getDeviceId();
    auto device_header = StoryParentHeader_Landscape::createHeader(StringUtils::format("User ID: %d", MJDEFAULT->getIntegerForKey(key_user_id)), device_id);
    if(!device_header) {
        exit(-1);
    }
    
    device_header->adjustWidth(menu_width);
    list_section->pushBackCustomItem(device_header);
    list_height += device_header->getContentSize().height + list_section->getItemsMargin();
    
    auto packages = StoryDataModel::getInstance()->getLicensePackages();
    if (!packages.empty()) {
        packages.erase(std::remove_if(packages.begin(), packages.end(), [](license::Package l) {
            return (l.time_expiry > 0) && (l.time_expiry < mj::helper::getTime());
        }), packages.end());
    }

    for (auto iterator : list_object)
    {
        ValueMap &section = iterator.asValueMap();
    
        if (section["name"].asString().empty())
        {
            
        }
        else
        {
            auto header = StoryParentHeader_Landscape::createHeader(section["name"].asString());
            header->adjustWidth(menu_width);
            list_section->pushBackCustomItem(header);
            list_height += header->getContentSize().height + list_section->getItemsMargin();
        }

        if (section.count("option") > 0 && section.at("option").getType() == cocos2d::Value::Type::VECTOR)
        {
            ValueVector &options = section.at("option").asValueVector();
            for (auto iterator : options) {
                
                StoryParentItem_Landscape *option;
//                if (iterator.asValueMap()["tag"].asInt() == 202 && CONFIG_MANAGER->isAppActive()) {
//                    option = StoryParentItem_Landscape::createItemBig(iterator.asValueMap());
//                }else {
//                    option = StoryParentItem_Landscape::createItem(iterator.asValueMap());
//                }
                option = StoryParentItem_Landscape::createItem(iterator.asValueMap());
                option -> setContentSize(Size(option->getContentSize().width, option->getContentSize().height - 10));
                
                // hieunt: account v3 đã có nút Account ngang với Settings ở sidebar, nên sẽ bỏ tab ở đây đi
                if (option->getTag() == 216)
                {
                    continue;
                }
                
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
#else
//                release
                if (option->getTag() == 999) {
                    auto value = MJDEFAULT->getIntegerForKey("key.debug.epoch", 0);
                    auto current_time = mj::helper::getTime() / 1000;
                    if (current_time - value >= 3600)
                        continue;
                }
#endif
                if(MJDEFAULT->getBoolForKey(key_is_submit, false) || !MJDEFAULT->getBoolForKey("is_license_feature_enable", false)|| CONFIG_MANAGER->isVietnameseStories())
                {
                    if(option->getTag() == 202)
                    {
                        continue;
                    }
                }
                
                if(option->getTag() == READING_LEVEL_TAG && (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE || LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE))
                {
                    continue;
                }

                if (option->getTag() == 333 && !MJDEFAULT->getBoolForKey("show_deletion_user", false))
                {
                    continue;
                }
               
                if (option->getTag() == 205)
                {
                    if (!CONFIG_MANAGER->isAppActive() || mj::Pay::getInstance().getLicense().empty()) {
                        //nếu app không kích hoạt thì sẽ không hiện quản lý thiết bị
                        continue;
                    }
                    if (packages.size() > 1) {
                        continue;
                    }
                }
                
                if ( MJDEFAULT->getBoolForKey(key_is_submit, false) && option->getTag() == 101)
                {
                    continue;
                }
                
                if (option->getTag() == 215)
                {
                    if (packages.size() <= 1) {
                        continue;
                    }
                }
                
                if (option->getTag() == 205)
                  {
                      if (!CONFIG_MANAGER->isAppActive() || mj::Pay::getInstance().getLicense().empty()) {
                          //nếu app không kích hoạt thì sẽ không hiện quản lý thiết bị
                          continue;
                      }
                      if (packages.size() > 1) {
                          continue;
                      }
                  }

                option->adjustWidth(menu_width);
                option->setDelegate(this);
                //                option->setSelected(true);
                list_section->pushBackCustomItem(option);
                list_height += option->getContentSize().height + list_section->getItemsMargin();
            }
        }
    }

    list_section->setTopPadding(2);

    list_section->setContentSize(cocos2d::Size(list_section->getContentSize().width, list_height - list_section->getItemsMargin() + list_section->getTopPadding()));

    list_section->setAnchorPoint(cocos2d::Point::ANCHOR_TOP_LEFT);
    list_section->setPosition(Point(0, Director::getInstance()->getVisibleSize().height - 80));
}

void StoryParentPage_Landscape::onSelectItem(const int &tag, const std::string &name)
{
    mj::helper::playButtonFX();
    
    CCLOG("Parent select: %s", name.c_str());
    
    auto option_tag = (StoryParentOptionTag) tag;
	
    if (option_tag == SPO_LICENSE && MJDEFAULT->getBoolForKey("is_user_skip", false))
    {
		auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
        
		AccountPopup::show({ LANGUAGE_MANAGER->getDisplayTextByKey("key.register.popup.msg"),
            LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.back"),
            LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title")
        }, NULL, [this]()
        {
			auto waiting_layout = Layout::create();
			waiting_layout->setContentSize(Director::getInstance()->getVisibleSize());
			waiting_layout->setBackGroundColor(Color3B::BLACK);
			waiting_layout->setBackGroundColorType(BackGroundColorType::SOLID);
			waiting_layout->setBackGroundColorOpacity(200);
			waiting_layout->setTouchEnabled(true);
			waiting_layout->setName("waiting_logout");
			Director::getInstance()->getRunningScene()->addChild(waiting_layout);
			auto indicator = cocos2d::ui::ImageView::create("mjstory/indicator.png");
			waiting_layout->addChild(indicator);
			indicator->setPosition(waiting_layout->getContentSize() / 2);
			indicator->setVisible(true);
			indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));
			//reload data
			auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event)
				{
					MJDEFAULT->setBoolForKey(key_onboarding_license, false);
					MJDEFAULT->setBoolForKey(key_logged_in, false);
					//        waiting_layout->removeFromParent();
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
						auto scene = MSIntro::createScene();
						Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));
						});
				});
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
			auto listener_failed = EventListenerCustom::create("event_logout_failed", [this, waiting_layout](EventCustom * event) {
				cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
				waiting_layout->removeFromParent();
				});
			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);
			ap::Auth::signOut();
			});
		return;
	}
    if (option_tag == SPO_TERMS_OF_USE) {
        ms::EventLogger::getInstance().logEvent("view_terms_of_use", {});
        
        if(MJDEFAULT->getBoolForKey(key_is_submit, false))
        {
            cocos2d::Application::getInstance()->openURL(key_terms_of_service_url_submit);
        }else{
            if (CONFIG_MANAGER->isCountryVN()) {
                cocos2d::Application::getInstance()->openURL(key_terms_of_service_url_vn);
            }else if (CONFIG_MANAGER->isCountryThailand()){
                cocos2d::Application::getInstance()->openURL(key_terms_of_service_url_th);
            } else if(CONFIG_MANAGER->isCountryIndonesia())
            {
                cocos2d::Application::getInstance()->openURL(key_terms_of_service_url_id);
            }else{
                cocos2d::Application::getInstance()->openURL(key_terms_of_service_url_en);
            }
        }
        
        return;
    }

    if (option_tag == DELETE_ACCOUNT) {
        //cocos2d::Application::getInstance()->openURL(PATH_DELETE_ACCOUNT);
        onDeleteAccount();
        return;
    }


    if (option_tag == SPO_PRIVACY_POLICY) {
        ms::EventLogger::getInstance().logEvent("view_privacy_policy", {});
		CleverTapManager::GetInstance()->pushEvent("click_privacy_policy ", {});
        
        if(MJDEFAULT->getBoolForKey(key_is_submit, false))
        {
            cocos2d::Application::getInstance()->openURL(key_privacy_policy_url_submit);
        }else{
            if (CONFIG_MANAGER->isCountryVN()) {
                cocos2d::Application::getInstance()->openURL(key_privacy_policy_url_vn);
            }else if (CONFIG_MANAGER->isCountryThailand()){
                cocos2d::Application::getInstance()->openURL(key_privacy_policy_url_th);
            } else if(CONFIG_MANAGER->isCountryIndonesia())
            {
                cocos2d::Application::getInstance()->openURL(key_privacy_policy_url_id);
            }else{
                cocos2d::Application::getInstance()->openURL(key_privacy_policy_url_en);
            }
        }
        
        
        return;
    }

	//if (option_tag == SPO_ACCOUNT_PROFILE)
	//{
	//	auto profile_manager = APProfileManagerView::createView();
	//	Director::getInstance()->getRunningScene()->addChild(profile_manager, INT_MAX - 1);
	//	return;
	//}

    if (option_tag == SPO_DEBUG) {
        auto debug_view = MJDebugView::createView();
        Director::getInstance()->getRunningScene()->addChild(debug_view, INT_MAX - 1);
        return;
    }
    
    if (option_tag == SPO_DEVICE_ID) {
        return;
    }
    
    if (option_tag == SPO_MANAGER_DEVICE){
        this->showListDeviceView();
        return;
    }
    
    if (option_tag == SPO_MANAGE_LICENSES) {
        this->showListLicenseView();
    }
    
    auto list_section = static_cast<cocos2d::ui::ListView *>(cocos2d::ui::Helper::seekWidgetByName(this, "list_section"));
    for (auto item : list_section->getItems()) {
        auto item_casted = dynamic_cast<StoryParentItem_Landscape *>(item);
        if (item_casted)
            item_casted->setSelected(item_casted->getTag() == tag);
    }

    auto subpage = StoryParentSubPage_Landscape::createPage(option_tag);
    
    if (subpage)
    {
        subpage->adjustWidth(_content->getContentSize().width);
        _content->removeAllChildren();
        _content->addChild(subpage);
        
        if(option_tag == SPO_SUPPORT) {
            // màn hình design đang là 700x628, nên hiện giờ chỉ đẹp với màn hình tỉ lệ 4:3, với các màn hình khác, \
            cái giao diện này nhìn hơi xấu, ta sẽ scale nó lại
            //auto width = subpage->getContentSize().width;
            auto width = subpage->getParent()->getContentSize().width;
            auto content = subpage->getChildByName("content");
            if(content) {
                if(width > 666) {
//                    content->setScale(width*0.95/666);
                    content->setScale(1.1f); // scale to lên xíu thôi, scale theo tỉ lệ width thì tràn height đó =.=
                }
            }
        }
    }

	if (option_tag == SPO_ACCOUNT_PROFILE)
	{
        ms::EventLogger::getInstance().logEvent("view_user_options", {});
		auto listener = EventListenerCustom::create("key.event.account.profile.click.add.license", [this](EventCustom*) {
			this->goToLicensePage();
			Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("key.event.account.profile.click.add.license");
		});
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void StoryParentPage_Landscape::onEnter() {
    ui::Layout::onEnter();
    //showCalculator();
}

void StoryParentPage_Landscape::onClose()
{
    Layout::onExit();
    if (MJDEFAULT->getBoolForKey(SHOW_MONKEY_TALKING_KEY, true)) {
        ms::HomeTab preTabUnclock = (ms::HomeTab)MJDEFAULT->getIntegerForKey(PRE_TAB_UNCLOCK_KEY, (int)ms::HomeTab::LESSONS);
        _delegate ? _delegate->onSelectedTab(preTabUnclock, ms::MovingDirection::NONE) : (void) nullptr;
    }
    
    this->removeFromParent();
}

void StoryParentPage_Landscape::showCalculator() {
    if (MJDEFAULT->getBoolForKey(key_show_parent_lock, true))
        MJCalculator::showCalculator(this);
}

ui::Widget::ccWidgetClickCallback StoryParentPage_Landscape::onLocateClickCallback(const std::string &name) {
    if (name == "onBack") {
        return CC_CALLBACK_1(StoryParentPage_Landscape::onBack, this);
    }
    
    if (name == "onResetFree") {
        return CC_CALLBACK_1(StoryParentPage_Landscape::onResetFree, this);
    }
    
    return nullptr;
}

void StoryParentPage_Landscape::onBack(cocos2d::Ref *sender) {
    MJ_PLATFORM_CONFIG.forceDismissKeyboard();
    ms::EventLogger::getInstance().logEvent("exit_settings", {});
    mj::helper::playButtonFXClose();
    mj::PlatformConfig::getInstance().forceDismissKeyboard();
    this->runAction(Sequence::create(FadeOut::create(0.25), CallFunc::create([this] {
        onClose();
    }), NULL));
}

void StoryParentPage_Landscape::onResetFree(cocos2d::Ref * sender) {
    auto tag = (int)static_cast<ui::Button *>(sender)->getTag();
    if (tag == 180){
        StoryFreeManager::getInstance()->setPercentActionFreePlayed(-100);
    }else {
        MJDEFAULT->setIntegerForKey("key_count_to_show", 0);
        MJDEFAULT->setIntegerForKey("key_time_to_show", 0);
        MJDEFAULT->setBoolForKey(key_rate_app, false);
        MJDEFAULT->setBoolForKey(key_rate_app_won, false);
        MJDEFAULT->setIntegerForKey("key_rate_action", 0);
        MJDEFAULT->setIntegerForKey(key_app_firstdate, 1504604738);
    }
}
#include "MJListDeviceView.h"
void StoryParentPage_Landscape::showListDeviceView() {
    auto list_device = LICENSE_INFO_MODEL->getLicenseDeviceInfo();
    auto current_device = atoi(mj::PlatformConfig::getInstance().getDeviceId().c_str());
    auto license_key = LICENSE_INFO_MODEL->getCurrentLicense();
    LICENSE_INFO_MODEL->setInfoVerify(MJDEFAULT->getStringForKey("phone_info"), MJDEFAULT->getStringForKey("mail_info"));
    auto total_active = MJDEFAULT->getStringForKey("total_active");
    auto list_device_view = MJListDeviceView::createView(list_device, current_device, license_key, total_active);
    cocos2d::Director::getInstance()->getRunningScene()->addChild(list_device_view, INT_MAX - 1);
    list_device_view->onShow();
}

#include "MJListLicense.h"
void StoryParentPage_Landscape::showListLicenseView() {
    auto list_license = MJListLicense::createView({});
    Director::getInstance()->getRunningScene()->addChild(list_license, INT_MAX - 10);
}

void StoryParentPage_Landscape::onDeleteAccount()
{
    auto screen = APResetPasswordScreen::createView(APResetPasswordScreen::RESET_TYPE::PHONE, APPopupBase::Flow::NONE,0, APResetPasswordScreen::TYPE_OTP::DELETE_ACCOUNT);
    Director::getInstance()->getRunningScene()->addChild(screen, INT_MAX - 1);
}

void StoryParentPage_Landscape::changeDisplayLanguage()
{

}
