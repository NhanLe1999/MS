#include "StoryParentAccountProfile_Landscape.h"
#include "APDatabase.h"
#include "APUpdateUserInfoAPI.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJHelper.h"
#include "StoryAlert.h"
#include "LicenseInfoModel.h"
#include "StoryConfigManager.h"
#include "StoryParentProfileDetail_Landscape.h"
#include "StoryParentEditUserInfo_Landscape.h"
#include "MJPay.h"
#include "StoryLanguageManager.h"
#include "LTDate.h"
#include "manager/GettingUserInfoManager.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "MSEventLogger.h"
#include "manager/GettingUserInfoManager.h"
#include "manager/RatingManager.h"
#include "APPopup.h"
#include "MJ_PageLoadResource.h"

USING_NS_CC;

INITIALIZE_READER(StoryParentAccountProfile_Landscape);

#define MAX_AVATAR_SHOW 4

StoryParentAccountProfile_Landscape * StoryParentAccountProfile_Landscape::createPage()
{
	StoryParentAccountProfile_Landscape * p;

	auto csb_name = mj::helper::getCSBName("csb/StoryParentAccountProfile_Landscape.csb");
	p = static_cast<StoryParentAccountProfile_Landscape *>(cocos2d::CSLoader::createNode(csb_name));

	if (p)
	{
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;

}


StoryParentAccountProfile_Landscape::~StoryParentAccountProfile_Landscape()
{
	if (edit_profile || edit_user_info || detail_profile || log_out || upgrade_account) 
	{
		try
		{
			CleverTapManager::GetInstance()->pushEvent("click_user_account", {
                
					{"view_screen",cocos2d::Value(true)},
					{"edit_profile",cocos2d::Value(edit_profile)},
					{"edit_user_info",cocos2d::Value(edit_user_info)},
					{"detail_profile",cocos2d::Value(detail_profile)},
					{"log_out",cocos2d::Value(log_out)},
					{"upgrade_account",cocos2d::Value(upgrade_account)},
				});
		}
		catch (const std::exception&)
		{

		}
	}
}

void StoryParentAccountProfile_Landscape::didLoadFromCSB()
{
	loadLicenseInfo();
	loadUserInfo();
	loadProfileInfo();
}

void StoryParentAccountProfile_Landscape::onEnter()
{
	ui::Layout::onEnter();
	this->onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
}

void StoryParentAccountProfile_Landscape::onChangeDisplayLanguage(int lang_id)
{
	//label
    
    if(auto text =utils::findChild<Text*>(this, "Text_1"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-Regular.ttf");

        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.account.info"));
    }
    
    if(auto text =utils::findChild<Text*>(this, "lbl_thong_tin_tai_khoan"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-Regular.ttf");

        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.account.info.user"));
    }
                        
    if(auto text =utils::findChild<Text*>(this, "lbl_quan_ly_ho_so"))
    {
        (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  text->setFontName("fonts/leelawdb.ttf") : text->setFontName("fonts/Montserrat-Regular.ttf");

        text->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.parent.account.info.profile"));
    }
    
    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  txt_name->setFontName("fonts/leelawdb.ttf") : txt_name->setFontName("fonts/Montserrat-Regular.ttf");

    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  txt_email->setFontName("fonts/leelawdb.ttf") : txt_email->setFontName("fonts/Montserrat-Regular.ttf");

    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  txt_phone->setFontName("fonts/leelawdb.ttf") : txt_phone->setFontName("fonts/Montserrat-Regular.ttf");

    (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  txt_address->setFontName("fonts/leelawdb.ttf") : txt_address->setFontName("fonts/Montserrat-Regular.ttf");

	if (txt_name->getString() == "")
    {
		txt_name->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.adding.your.name"));
	}
    
	if (txt_email->getString() == "")
    {
		txt_email->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.adding.your.email"));
	}
    
	if (txt_phone->getString() == "")
    {
        txt_phone->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.adding.your.phone"));
	}
    
	if (txt_address->getString() == "")
    {
		txt_address->setPlaceHolder(LANGUAGE_MANAGER->getDisplayTextByKey("key.adding.your.address"));
	}

    utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn__add_license"), "Text_1_0_0")->setFontName((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_edit"), "Text_1_0_0")->setFontName((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");

    utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_detail"), "Text_1_0_0")->setFontName((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    
    utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_sign_out"), "Text_1_0_0")->setFontName((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?"fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
    
    //
	utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn__add_license"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("pay.notice.detail.other"));
	utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_edit"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.btn.edit"));
	utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_detail"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.detail"));
	utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_sign_out"), "Text_1_0_0")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("key.logout"));
    
    //
    if (MJDEFAULT->getBoolForKey("is_user_skip",false))
    {
        if(auto btnSignOutText = utils::findChild<Text*>(utils::findChild<ui::Button*>(this, "btn_sign_out"), "Text_1_0_0"))
        {
            (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ?  btnSignOutText->setFontName("fonts/leelawdb.ttf") : btnSignOutText->setFontName("fonts/Montserrat-Regular.ttf");

            btnSignOutText->setString(LANGUAGE_MANAGER->getDisplayTextByKey("ap_register_button_title"));
        }
    }
	
    if (CONFIG_MANAGER->isAppVNStories())
	{
		txt_name->setPlaceHolderColor(cocos2d::Color3B::WHITE);
		txt_email->setPlaceHolderColor(cocos2d::Color3B::WHITE);
		txt_phone->setPlaceHolderColor(cocos2d::Color3B::WHITE);
		txt_address->setPlaceHolderColor(cocos2d::Color3B::WHITE);
	}
}

void StoryParentAccountProfile_Landscape::onExit()
{
	Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("key.event.click.add.license");
	ui::Layout::onExit();
}

void StoryParentAccountProfile_Landscape::loadUserInfo()
{
	// Load Thông tin tài khoản
	auto user_info = ap::Database::getInstance().getUserInfo();
    
	txt_name = utils::findChild<ui::TextField*>(this, "txt_name");
	txt_email = utils::findChild<ui::TextField*>(this, "txt_email");
	txt_phone = utils::findChild<ui::TextField*>(this, "txt_phone");
	txt_address = utils::findChild<ui::TextField*>(this, "txt_address");

	/*txt_name->setCursorEnabled(true);
	txt_name->setCursorChar('|');*/
	txt_name->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	//txt_email->setCursorEnabled(true);
	//txt_email->setCursorChar('|');
	txt_email->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	//txt_phone->setCursorEnabled(true);
	//txt_phone->setCursorChar('|');
	txt_phone->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	//txt_address->setCursorEnabled(true); 
	//txt_address->setCursorChar('|');
	txt_address->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));

	txt_name->setString(user_info._name);
	txt_email->setString(user_info._mail);
	txt_phone->setString(user_info._phone);
	txt_address->setString(user_info._address);
    
    txt_name->setTouchEnabled(false);
    txt_email->setTouchEnabled(false);
    txt_phone->setTouchEnabled(false);
    txt_address->setTouchEnabled(false);
}

void StoryParentAccountProfile_Landscape::loadLicenseInfo()
{
	auto lang_id = LANGUAGE_MANAGER->getDisplayLangId();
	
    auto text_details = static_cast<cocos2d::ui::Text *>(cocos2d::ui::Helper::seekWidgetByName(this, "lbl_expiry_date"));
    
    text_details->setFontName((LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ) ? "fonts/leelawdb.ttf":"fonts/Montserrat-Regular.ttf");
	
    auto expire_time = mj::Pay::getInstance().getTimeExpire();
	
    if (mj::Pay::getInstance().isActive())
	{
		if (expire_time == -1)
        {
            utils::findChild<ui::Button*>(this, "btn__add_license")->setVisible(false);
			text_details->setString(cocos2d::StringUtils::format("%s: %s",
				LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(),
				LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.lifetime").c_str()
			));
		}
		else
        {
			auto expire_date = LTDate(expire_time);
			text_details->setString(cocos2d::StringUtils::format("%s: %s", LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), expire_date.toPrettyString().c_str()));
		}
	}
	else
	{
		text_details->setString(cocos2d::StringUtils::format("%s: %s",
                                                             LANGUAGE_MANAGER->getTextByKey(lang_id, "parent.license.expire").c_str(), LANGUAGE_MANAGER->getDisplayTextByKey("key.trial").c_str()));
	}
    
    if(MJDEFAULT->getBoolForKey(key_is_submit, false) || !MJDEFAULT->getBoolForKey("is_license_feature_enable", false)|| CONFIG_MANAGER->isVietnameseStories())
    {
        utils::findChild<ui::Button*>(this, "btn__add_license")->setVisible(false);
    }
}

void StoryParentAccountProfile_Landscape::loadProfileInfo()
{
	auto lv_avatar = utils::findChild<ui::ListView*>(this, "lv_avatar");
	lv_avatar->setScrollBarEnabled(false);

	auto list_profile = ap::Database::getInstance().getProfiles();
	lv_avatar->removeAllItems();
	int num_avatar = std::min(MAX_AVATAR_SHOW, (int)list_profile.size());
	float size_width = 0.0f;
	for (int i = 0; i < num_avatar; ++i)
	{
		auto path_avatar = list_profile[i].avatar;
		auto image = ui::ImageView::create(path_avatar);
		image->ignoreContentAdaptWithSize(false);
		image->setContentSize(Size(80, 80));
		lv_avatar->pushBackCustomItem(image);
		size_width += image->getContentSize().width + lv_avatar->getItemsMargin();
	}
	lv_avatar->setContentSize(Size(size_width, lv_avatar->getContentSize().height));
}

void StoryParentAccountProfile_Landscape::onClickAddLicense(cocos2d::Ref * sender)
{
	if (MJDEFAULT->getBoolForKey("is_user_skip", false)) {
		auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
		AccountPopup::show({ is_vn ? "Ba mẹ vui lòng đăng ký tài khoản để sử dụng tính năng này" : "Please register an account to access this function",is_vn ? "Quay lại" : "Back",is_vn ? "Đăng ký" : "Register" }, NULL, [this]() {
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
	}
	else
    {
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.event.account.profile.click.add.license");
	}
}

void StoryParentAccountProfile_Landscape::onClickEditProfile(cocos2d::Ref * sender)
{
    Button* btn = (Button*)sender;
    btn->setTouchEnabled(false);
    this->scheduleOnce([=](float)
    {
        btn->setTouchEnabled(true);
    }, 1.f, "reset t ");
	if (MJDEFAULT->getBoolForKey("is_user_skip", false)) {
        bool is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
		AccountPopup::show({ is_vn ? "Ba mẹ vui lòng đăng ký tài khoản để sử dụng tính năng này" : "Please register an account to access this function",is_vn ? "Quay lại" : "Back",is_vn ? "Đăng ký" : "Register"  },NULL, [this]() {
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
	}
	else 
	{
	
		GettingUserInfoManager::GetInstance()->showGettingUserInfoPopup(false,false);
		//auto edit_view = StoryParentEditUserInfo_Landscape::createView();
		//Director::getInstance()->getRunningScene()->addChild(edit_view, INT_MAX - 1);
		auto listener = cocos2d::EventListenerCustom::create("key.event.account.profile.edit.user.info.success",
			[this](EventCustom * event) 
			{
				edit_user_info = true;

				this->scheduleOnce([=](float)
					{
						this->loadUserInfo();
					}, 0.25f, "delay close edit popup");

				cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("key.event.account.profile.edit.user.info.success");
			});

		cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void StoryParentAccountProfile_Landscape::onClickDetailProfile(cocos2d::Ref * sender)
{
	detail_profile = true;

	auto profile_detail = StoryParentProfileDetail_Landscape::createPage();
	profile_detail->setParentDelegate(this);
	profile_detail->adjustWidth(this->getContentSize().width);
	this->getParent()->addChild(profile_detail, INT_MAX - 1);

	this->setVisible(false);
}

void StoryParentAccountProfile_Landscape::onClickSignOut(cocos2d::Ref * sender)
{
	log_out = true;
	
    ms::EventLogger::getInstance().logEvent("click_signout", {});
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

	ap::Auth::signOut();
	//reload data

	auto listener = EventListenerCustom::create("event_logout_successfully", [this, waiting_layout](EventCustom * event)
	{
	
       ms::EventLogger::getInstance().logEvent("event_signout_done", {}); cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_successfully");
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
         ms::EventLogger::getInstance().logEvent("event_signout_fail", {});
		cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_logout_failed");
		waiting_layout->removeFromParent();

	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_failed, this);

	//delete unused data key.
	GettingUserInfoManager::GetInstance()->deleteData();
	RatingManager::GetInstance()->deleteData();
    
    mj::PageLoadResource::getInstance().deleteCacheAllStory();
    mj::PageLoadResource::getInstance().deleteCacheAllAudiobooks();
    mj::PageLoadResource::getInstance().deleteCacheAllLessons();
    FileUtils::getInstance()->purgeCachedEntries();
}

void StoryParentAccountProfile_Landscape::onClose(cocos2d::Ref * sender)
{
	this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentAccountProfile_Landscape::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(StoryParentAccountProfile_Landscape, onClickAddLicense),
		CLICK_MAP(StoryParentAccountProfile_Landscape, onClickEditProfile),
		CLICK_MAP(StoryParentAccountProfile_Landscape, onClickDetailProfile),
		CLICK_MAP(StoryParentAccountProfile_Landscape, onClickSignOut),
		CLICK_MAP(StoryParentAccountProfile_Landscape, onClose)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}

void StoryParentAccountProfile_Landscape::onEditProfile()
{
	edit_profile = true;
}
