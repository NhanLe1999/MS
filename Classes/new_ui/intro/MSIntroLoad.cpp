//
//  MSIntroLoad.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/15/17.
//

#include "MSIntroLoad.h"
#include "MJAPIRefresh.h"
#include "MJAPI_Audiobook_Zip.h"
#include "MJAPI_Story_Zip.h"
#include "MJAPI_AppInfo_Zip.h"
#include "MJAPI_AISpeakingLessonList_Zip.h"
#include "MJAPI_AISpeakingTopicList_Zip.h"
#include "MJAPI_AISpeakingCommonInfoList_Zip.h"
#include "MJAPIRegisterDevice.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "StoryDataModel.h"
#include "StoryFreeManager.h"
#include "MJAPIVerifyApple.h"
#include "NativeBridge.h"
#include "MJFirebaseListener.h"
#include "MSHome.h"
#include "MSDownloader.h"
#include "MJAPIGetLinkDownload.h"
#include "StoryInfoCover.h"
#include "MSGradeManager.h"
#include "MJPay.h"
#include "GAFWrapper.h"
#include "MSEventLogger.h"
#include "StoryWelcomePopup.h"
#include "StoryData.h"
#define DELAY_BETWEEN_API_CALL 0.1
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "../proj.win32/Win32DeviceInfo.h"
#endif
#include "MJPlatformConfig.h"
#include "StoryLanguageManager.h"
#include "MSGradeManager.h"

#include "APLoginView.h"
#include "APWaitView.h"
#include "MSIntroLicense.h"
#include "APProfilesListAPI.h"

#include "ThumbQueue.h"
#include "MJMailIntro.h"
#include "APLoadUpdateAPI.h"
#include "APDatabase.h"
#include "APProfileManager.h"
#include "MSInstallPromotion.h"
#include "APChooseProfileView.h"
#include "APProfileFinishView.h"
#include "APUpdateUserInfoAPI.h"
#include "MMFirebaseDatabase.h"
#include "APLoginScreen.h"
#include "MSLanguageSelection.h"
#include "StoryFreeManager.h"
#include "APDatabase.h"
#include "MSFirebasePlugin.h"
#include "HelperManager.h"
#include "MJAPI_Material_Zip.h"
#include "MJAPI_FirstInstall_Thumb_Zip.h"
#include"MJAPI_FirstInstallManager.h"
#include "MJAPI_ActivitiesList_Zip.h"
#include "MJAPI_Categories_Zip .h"
#include "MJAPI_GameList_Zip.h"
#include "MJAPI_LessonList_Zip.h"
#include "MJAPI_LessonInterative_Zip.h"
#include "MJAPI_DecodeObjectJsonInfo.h"
#include "GettingStartedScene.h"
#include "SplashScene.h"
#include "F2PManager.h"
#include "MSFreeContentPage.h"
#include "NameSetupScene.h"
USING_NS_CC;
INITIALIZE_READER(MSIntroLoad);
#define MAX_UPDATE_VERSION 100
#define MJ_CALLBACK_4(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

std::once_flag ms_intro_load_reader;

constexpr auto k_tag_free_3_day = 30;

const std::unordered_map<std::string, float> K_PERCENTS =
{
	{"onRegisterAPICalled"		,8.0f},
	{"onFisrtInstallCall"		,25.0f},
	{"onCheckUpdateApp"		,8.0f},
	{"onLoadUpdateInfoAPICalled"		,25.0f},
	{"onThumbResourceCalled"	,30.0f},
	{"onAppInfoAPICalled"		,35.0f},
	{"onAllAudiobooksCalled"	,40.0f},
	{"onHomeAPICalled"			,45.0f},
	{"onCheckUpdateCatagoriesAPICalled"	,50.0f},
	{"onCheckUpdateGameListAPICalled"	,55.0f},
	{"onCheckUpdateLessonListAPICalled"	,60.0f},
	{"onAISpeakingTopicCalled"	,65.0f},
	{"onAISpeakingCommonCalled"	,67.0f},
	{"onAISpeakingLessonCalled"	,70.0f},
	{"onCommonResourceCalled"   ,75.0f},
	{"onMaterialAPICalled"      ,80.0f},
	{"onLoadUpdateAPICalled"	,85.0f},
	{"onProfileListAPICalled"	,90.0f},
	{"onAPIDone", 95.0f},

};
Scene* MSIntroLoad::createScene()
{
    auto scene = Scene::create();

    if (scene)
    {
        auto view = MSIntroLoad::createView();
        view->setName("IntroLoadScene");
        scene->addChild(view);
    }
    else
    {
        CC_SAFE_DELETE(scene);
    }

    return scene;
}

MSIntroLoad* MSIntroLoad::createView() {
	std::call_once(ms_intro_load_reader, [] {
		REGISTER_CSB_READER(MSIntroLoad);
		});

	auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Intro_Load.csb");
	auto node = reinterpret_cast<MSIntroLoad*>(CSLoader::createNodeWithVisibleSize(csb_name));
	if (node) {
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

void MSIntroLoad::didLoadFromCSB() {
    this->setName(MSINTROLOAD_VIEW);
	if (CONFIG_MANAGER->isVietnameseStories()) {
		auto logo = utils::findChild<Sprite*>(this, "logo");
		logo->setTexture("mjstory/monkey_logo_vi.png");
	}
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
		mm::RealtimeDatabase::getInstance().loadKeyNewLibApp(USER_ID_CONFIG);
		});

	_read_sample = false;
	_show_license = false;
	_sample_id = 0;
	_loading_bar = utils::findChild<ui::LoadingBar*>(this, "progress");
	this->setPercent(0);

	PROFILE_MANAGER.initProfile(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0));

	_is_api_required = MJDEFAULT->getBoolForKey(key_first_api_called, true);
	_is_api_done = false;
	_is_onboarded = MJDEFAULT->getBoolForKey(key_onboarding_part_1, false);

	// logo
	auto logo = utils::findChild(this, "logo");
	logo->setPosition(logo->getPosition() + Point(0, 30));
	logo->setVisible(true);

	const char* logoImage = "mjstory/new_ui/image_appdrawer_stories_logo_mask.png";
	if (CONFIG_MANAGER->isVietnameseStories()) {
		logoImage = "mjstory/new_ui_vn/image_appdrawer_stories_logo_mask.png";
	}
	auto logo_mask = ImageView::create(logoImage);
	logo_mask->setPosition(Point(logo->getContentSize().width / 2, logo->getContentSize().height / 2 - 30));
	logo->addChild(logo_mask, -1);

	_show_sample = false;

	utils::findChild(this, "button_skip")->setVisible(false);

	changeLanguage();

	_monkey = GAFWrapper::createGAF("gaf/monkey_load/monkey_load.gaf");
	utils::findChild(this, "loading_bar")->addChild(_monkey);
	_monkey->play(true);
	_monkey->setPosition(Point(100, 100));
	_monkey->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
		this->schedule(schedule_selector(MSIntroLoad::updateProgressBar));
	});


	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(key_event_intro_completed, [=](EventCustom*) {
		_is_onboarded = true;
		if (_is_api_done)
			this->onAPIDone();
		}), this);

	ms::GradeManager::getInstance().setScope(ms::GradeManager::Scope::STORIES);

	auto grade_id = ms::GradeManager::getInstance().getCurrentGradeId();
	if (grade_id == 0) {
		ms::GradeManager::getInstance().setInitialGrade("All Grades");
	}


	auto e = EventListenerCustom::create("event_login_successfully_when_delete_account", [=](EventCustom*)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				scheduleOnce([=](float)
					{
						this->onCommonResourceCalled(true); // call loadupdates
					}, DELAY_BETWEEN_API_CALL, "delay api");

				Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("event_login_successfully_when_delete_account");

			});
	});

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);


}

void MSIntroLoad::updateProgressBar(float dt) {
	auto percent = _loading_bar->getPercent();
	auto bb = _loading_bar->getBoundingBox();
	_monkey->setPosition(Point(percent * bb.size.width / 100, 26));
}

void MSIntroLoad::changeLanguage() {
	this->enumerateChildren("//.*", [](Node* child) {
		auto t = dynamic_cast<ui::Text*>(child);
		if (!t) {
			return false;
		}

		auto str = LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), t->getName());
		if (str == t->getName()) {
			return false;
		}

		t->setString(str);

		return false;
		});

	auto text_skip = utils::findChild(this, "intro.load.skip");
	auto underline_skip = utils::findChild(this, "underline_skip");
	underline_skip->setContentSize(Size(text_skip->getContentSize().width, 1));

	auto text_restore = utils::findChild(this, "intro.load.restore");
	auto underline_restore = utils::findChild(this, "underline_restore");
	underline_restore->setContentSize(Size(text_restore->getContentSize().width, 1));
}

void MSIntroLoad::onEnter()
{
	ui::Layout::onEnter();

	_is_api_required = MJDEFAULT->getBoolForKey(key_first_api_called, true);
	bool keyCheckIsFirstInstal = MJDEFAULT->getBoolForKey(key_check_is_first_install_is_finished, false);
	ms::EventLogger::getInstance().logEvent("enter_intro_screen", {

		{"is_reuquired",cocos2d::Value(_is_api_required)}

		});

	auto content = this->getChildByName("content");
	auto delay = 0.f;
	auto counter = 0;
	for (auto c : content->getChildren()) {
		c->setPosition(c->getPosition() + Vec2(1000, 0));
		auto saved_opacity = c->getOpacity();
		c->setOpacity(0);
		c->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseQuinticActionOut::create(MoveBy::create(.5f, Vec2(-1000, 0))), FadeTo::create(.5f, saved_opacity), nullptr), nullptr));
		delay += .1f + counter++ * 0.01;
	}

	this->scheduleOnce([=](float dt)
		{
			std::string pathfirstInstall = MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, "");
			std::vector<mj::model::LessonInfo> lesson = StoryDataModel::getInstance()->getAllLessonsByLanguage(StoryLanguageManager::getCurrentLangId());
			std::vector<mj::model::StoryInfo> stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(StoryLanguageManager::getCurrentLangId());

			std::string currVerion = MJ_PLATFORM_CONFIG.getAppVersion();
			bool isCheckVersion = CompareVersionApp(currVerion);
			bool isCall = MJDEFAULT->getBoolForKey("key_isNewVersionCall" + currVerion, true);

			if (!keyCheckIsFirstInstal || pathfirstInstall.empty() || stories.empty() || lesson.empty() || isCall || CompareVersionApp(currVerion))
			{
				MJDEFAULT->setBoolForKey(key_checkIsLoadUpdate, false);
				MJDEFAULT->setBoolForKey("key_isNewVersionCall" + currVerion, false);
				mj::RegisterAPI::call(CC_CALLBACK_1(MSIntroLoad::onRegisterAPICalled, this));
			}
			else
			{
				NativeBridge::setCrashlyticsKeyValue(key_crashlytics_device, MJDEFAULT->getStringForKey(key_device_identifier));
				ms::EventLogger::getInstance().logEvent(ms::analytics::kEventAPIAppInfo, {
					{"is_reuquired",cocos2d::Value(_is_api_required)}
					});
				onCheckUpdateApp(true);
			}

		}, 0.5f, "delay_call_api");

	std::vector<string> list_gafname = {
		"gaf/maybay2/maybay2.gaf",
		"gaf/la_co/la_co.gaf",
		"gaf/phao_giay/phao_giay.gaf",
		"gaf/phao_giay_goc_trai/phao_giay_goc_trai.gaf",
		"gaf/phao_hoa/phao_hoa.gaf" };

	HSLib::anims.clear();

	for (int i = 0; i < (int)list_gafname.size(); i++) {
		HSLib::anims.push_back(list_gafname[i]);
	}
}

void MSIntroLoad::onExit() {
	mj::helper::deleteWaitView();
	ui::Layout::onExit();

}

void MSIntroLoad::reloadIntro() {
	// đoạn này giống trong hàm onEnter()
	bool keyCheckIsFirstInstal = MJDEFAULT->getBoolForKey(key_check_is_first_install_is_finished, false);

	if (keyCheckIsFirstInstal)
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				mj::RegisterAPI::call(CC_CALLBACK_1(MSIntroLoad::onRegisterAPICalled, this));
			});
	}
	else
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
			{
				mj::api::AppInfoZip::call(MJ_CALLBACK_4(MSIntroLoad::onAppInfoAPICalled, this));
			});
	}
}

ui::Widget::ccWidgetClickCallback MSIntroLoad::onLocateClickCallback(const std::string& callback_name) {
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(MSIntroLoad, onSampleStory),
		CLICK_MAP(MSIntroLoad, onSkip),
		CLICK_MAP(MSIntroLoad, onRestore)
	};

	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}

	return nullptr;
}

void MSIntroLoad::setPercent(double percent) {

	if (!_loading_bar)
	{
		return;
	}
	auto current = _loading_bar->getPercent();
	if (percent > 0 && percent < current)
		return;
	if (percent >= 100)
		percent = 100;
	_loading_bar->stopAllActions();

	_loading_bar->runAction(ActionFloat::create(0.2, current, percent, [this](float v)
		{
			double whole, fraction;

			fraction = std::modf(v, &whole);

			if (auto text = utils::findChild<ui::Text*>(this, "intro.load.progress"))
			{
				text->setString("Loading...." + (fraction > 0.0 ? StringUtils::format("%.02f %%", v) : StringUtils::format("%d %%", (int)v)));
			}

			this->_loading_bar->setPercent(v);
		}));
}

void MSIntroLoad::onRegisterAPICalled(bool is_success) {
	//    MJ_PLATFORM_CONFIG.setDeviceID(std::string("6576762"));
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

			HelperManager::GetInstance()->parseCountryCodesInfomationFromJson();


			if (is_success) {
				#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_PLATFORM_HUAWEI)
				CleverTapManager::GetInstance()->loginDummyProfileForWinAndMac();
				#endif
				NativeBridge::setCrashlyticsKeyValue(key_crashlytics_device, MJDEFAULT->getStringForKey(key_device_identifier));
				mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsSetDeviceID(MJDEFAULT->getStringForKey(key_device_identifier));

				this->setPercent(K_PERCENTS.at("onRegisterAPICalled"));
				//check first install path exits
				std::string pathfirstInstall = MJDEFAULT->getStringForKey(PATH_FIRST_INSTALL_KEY, "");
				if(pathfirstInstall != "")
					onFisrtInstallCall(true);
				else
				{
					// need call register location again to update first install path
					onNetworkError(kMSIntroLoadAlertTag_ApiRegister);
				}

				//mj::api::AppInfoZip::call(MJ_CALLBACK_4(MSIntroLoad::onAppInfoAPICalled, this));

				if (CONFIG_MANAGER->isTestActVmonkey()) {
					this->onTestActVMonkey();
				}

				this->addTextDeviceID();
			}
			else {
				onNetworkError(kMSIntroLoadAlertTag_ApiRegister);
			}
		});

}
void MSIntroLoad::onFisrtInstallCall(bool is_success)
{
	ms::EventLogger::getInstance().logEvent("onFisrtInstallCall", {
	{"status",cocos2d::Value(is_success)}
		});
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			if (is_success) {

				NativeBridge::setCrashlyticsKeyValue(key_crashlytics_device, MJDEFAULT->getStringForKey(key_device_identifier));
				mj::FirebaseMessagingHelper::getInstance().firebaseAnalyticsSetDeviceID(MJDEFAULT->getStringForKey(key_device_identifier));

				this->setPercent(K_PERCENTS.at("onFisrtInstallCall"));

				mj::api::MJAPIFirstInstallManager::call(CC_CALLBACK_1(MSIntroLoad::onThumbResourceCalled, this));

				if (CONFIG_MANAGER->isTestActVmonkey()) {
					this->onTestActVMonkey();
				}

				this->addTextDeviceID();
			}
			else {
				onNetworkError(kMSIntroLoadAlertTag_ApiFisrtInstall);
			}
		});

}
void MSIntroLoad::onCheckUpdateApp(bool is_success)
{
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_PLATFORM_HUAWEI)
		CleverTapManager::GetInstance()->loginDummyProfileForWinAndMac();
	#endif
	if (is_success)
	{
		MJDEFAULT->setBoolForKey(key_checkIsLoadUpdate, true);
		bool checkToken = ap::Database::getInstance().getAccessToken().length() > 0 && (MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false));
		if (checkToken) {
			MJDEFAULT->deleteValueForKey("logged_in_skip");
		

			if (!MJDEFAULT->getBoolForKey("CL_Loggin_Called", false))
			{
				CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
			}
		
            if(NativeBridge::networkAvaiable()) {
              
                ap::api::LoadUpdate::call(CC_CALLBACK_2(MSIntroLoad::onLoadUpdateInfoAPICalled, this));
            }
            else
            {
                bool isUpdate3Days = FREE_STORY_MANAGER->isUpdate3Days();
                bool loggedIn = MJDEFAULT->getBoolForKey(key_logged_in, false);
                bool update_3_day_key = MJDEFAULT->getBoolForKey("key_update_3_day");
                
                if (isUpdate3Days && loggedIn && !update_3_day_key)
                {
                    Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
                        {
                            MJDEFAULT->setBoolForKey("key_update_3_day", true);
                            _storyAlert = StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.time.update"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "network_cancel"), "", false, 28, this, k_tag_free_3_day);
                        });
                }
				onLoadUpdateInfoAPICalled(true, "");
            }

		}
		else
		{
			onAPIDone();
		}
	}
}

void MSIntroLoad::onThumbResourceCalled(bool is_success) {
	_v_server_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1);
	_v_server_story = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, -1);
	_v_server_material = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, -1);
	_v_server_lesson = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer, -1);
	_v_server_ai_speaking_topic = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, -1);
	_v_server_ai_speaking_items = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, -1);

	_v_local_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, -1);
	_v_local_story = MJDEFAULT->getIntegerForKey(key_api_version_story_local, -1);
	_v_local_material = MJDEFAULT->getIntegerForKey(key_api_version_material_local, -1);
	_v_local_lesson = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_local, -1);
	_v_local_ai_speaking_topic = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_local, -1);
	_v_local_ai_speaking_items = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_local, -1);

	this->scheduleOnce([=](float) {

		if (is_success) {

			this->setPercent(K_PERCENTS.at("onThumbResourceCalled"));

			MJDEFAULT->setIntegerForKey(key_api_version_story_local, _v_server_story);
			std::vector <mj::api::ThumbDownloadInfo*> thumbDownloadInfos;

			std::string download_url_thumb_story = mj::helper::getUrlFromDomain(MJDEFAULT->getStringForKey(THUMB_STORY_URL_KEY, ""), mj::UrlType::MSURL_MEDIA);
			std::string download_url_thumb_audio = mj::helper::getUrlFromDomain(MJDEFAULT->getStringForKey(THUMB_AUDIO_URL_KEY, ""), mj::UrlType::MSURL_MEDIA);
			std::string download_url_thumb_lesson = mj::helper::getUrlFromDomain(MJDEFAULT->getStringForKey(THUMB_LESSON_URL_KEY, ""), mj::UrlType::MSURL_MEDIA);
			std::string download_url_thumb_other = mj::helper::getUrlFromDomain(MJDEFAULT->getStringForKey(THUMB_OTHER_URL_KEY, ""), mj::UrlType::MSURL_MEDIA);

			std::vector <std::string> download_url_thumbs;
			download_url_thumbs.push_back(download_url_thumb_story);
			download_url_thumbs.push_back(download_url_thumb_audio);
			download_url_thumbs.push_back(download_url_thumb_lesson);
			download_url_thumbs.push_back(download_url_thumb_other);
			for (std::string download_url_thumb: download_url_thumbs)
			{
				if (mj::helper::isValidURL(download_url_thumb)) {
					mj::api::ThumbDownloadInfo* thumbDownloadInfo = new mj::api::ThumbDownloadInfo{};
					thumbDownloadInfo->downloadUrl = download_url_thumb;
					thumbDownloadInfo->countRetryDownload = 0;
					thumbDownloadInfos.push_back(thumbDownloadInfo);
				}
			}
			mj::api::FisrtInstallThumb::call(thumbDownloadInfos, [=](bool isSuccess, int, int, int) {

				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

					if (auto thisInstance = dynamic_cast<MSIntroLoad*>(utils::findChild(Director::getInstance()->getRunningScene(), "loadview"))) {

						thisInstance->onFisrtInstallThumbCalled(isSuccess);

					}

					});


				}, [=](float i_percent) {


					Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {

						if (auto thisInstance = dynamic_cast<MSIntroLoad*>(utils::findChild(Director::getInstance()->getRunningScene(), "loadview"))) {

							double downloadPercent = K_PERCENTS.at("onThumbResourceCalled") + i_percent * (K_PERCENTS.at("onAppInfoAPICalled") - K_PERCENTS.at("onThumbResourceCalled")) / 100.f;
							thisInstance->setPercent(downloadPercent);
						}

						});

				}, RemoteImageView::Type::OTHER);
		}
		else
		{
			onNetworkError(kMSIntroLoadAlertTag_ApiFisrtInstall);
		}
		}, DELAY_BETWEEN_API_CALL, "onThumbResourceCalled");
}

void MSIntroLoad::onCheckUpdateGameListAPICalled(bool is_success)
{
	ms::EventLogger::getInstance().logEvent("onCheckUpdateGameListAPICalled", {
	{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_lesson_list = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_local, -1);
		auto v_server_lesson_list = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onCheckUpdateGameListAPICalled"));
			// TODO: call API MaterialZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_lesson_list <= 0 || v_client_lesson_list < v_server_lesson_list) {
				mj::api::LessonListZip::call(CC_CALLBACK_1(MSIntroLoad::onCheckUpdateLessonListAPICalled, this));

			}
			else {
				this->onCheckUpdateLessonListAPICalled(true);
			}
		}
		else
		{
			this->onCheckUpdateLessonListAPICalled(true);
			//onNetworkError();// kMSIntroLoadAlertTag_ApiGameListZip); //if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "onCheckUpdateGameListAPICalled");
}

void MSIntroLoad::onCheckUpdateCatagoriesAPICalled(bool is_success)
{
	ms::EventLogger::getInstance().logEvent("onCheckUpdateCatagoriesAPICalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_game_list = MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, -1);
		auto v_server_game_list = MJDEFAULT->getIntegerForKey(key_api_version_all_games_fromServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onCheckUpdateCatagoriesAPICalled"));
			// TODO: call API MaterialZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_game_list <= 0 || v_client_game_list < v_server_game_list) {
				mj::api::GameListZip::call(CC_CALLBACK_1(MSIntroLoad::onCheckUpdateGameListAPICalled, this));
			}
			else {
				this->onCheckUpdateGameListAPICalled(true);
			}
		}
		else
		{
			this->onCheckUpdateGameListAPICalled(true);
			//onNetworkError();//kMSIntroLoadAlertTag_ApiCatagoriesZip); if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "onCheckUpdateCatagoriesAPICalled");
}

void MSIntroLoad::onCheckUpdateLessonListAPICalled(bool is_success)
{

	// after call LessonList api, call AISpeaking Topic api
	ms::EventLogger::getInstance().logEvent("onCheckUpdateLessonListAPICalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_ai_speaking_topic_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_local, -1);
		auto v_server_ai_speaking_topic_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onCheckUpdateLessonListAPICalled"));
			// TODO: call API MJAPIAISpeakingTopicListZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_ai_speaking_topic_list <= 0 || v_client_ai_speaking_topic_list < v_server_ai_speaking_topic_list) {
				mj::api::MJAPIAISpeakingTopicListZip::call(CC_CALLBACK_1(MSIntroLoad::onAISpeakingTopicCalled, this));

			}
			else {
				this->onAISpeakingTopicCalled(true);
			}
		}
		else
		{
			this->onAISpeakingTopicCalled(true);
			//onNetworkError();// kMSIntroLoadAlertTag_ApiGameListZip); //if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "onCheckUpdateLessonListAPICalled");
}

void MSIntroLoad::onFisrtInstallThumbCalled(bool is_success)
{
	if (!_checkOntimeAPIDone)
	{
		onMaterialAPICalled(is_success);
		_checkOntimeAPIDone = true;
	}
	else
	{
		return;
	}
}

void MSIntroLoad::onAppInfoAPICalled(bool is_success, int v_server_audio, int v_server_story, int v_server_material) {

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			if (is_success) {

				this->setPercent(K_PERCENTS.at("onAppInfoAPICalled"));
				if (v_server_audio != -1 && v_server_story != -1 && v_server_material != -1)
				{
					_v_server_audio = v_server_audio;
					_v_server_story = v_server_story;
					_v_server_material = v_server_material;
				}
				else
				{
					_v_server_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1);
					_v_server_story = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, -1);
					_v_server_material = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, -1);
				}

				auto v_client_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, -1);
				_v_server_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1);

				if (v_client_audio < _v_server_audio) {
					ms::EventLogger::getInstance().logEvent(ms::analytics::kEventAPIAllAudiobooks, {});
					mj::api::AudiobookZip::call(CC_CALLBACK_1(MSIntroLoad::onAllAudiobooksCalled, this));
				}
				else
				{
					onAllAudiobooksCalled(true);
				}
			}
			else {
				bool keyCheckIsFirstInstal = MJDEFAULT->getBoolForKey(key_check_is_first_install_is_finished, false);
				if (keyCheckIsFirstInstal) {

					onNetworkError();
				}
				else {
					onCommonResourceCalled(true);
				}
			}
		});
}

void MSIntroLoad::onAllAudiobooksCalled(bool is_success) {
	this->scheduleOnce([this, is_success](float) {
		if (is_success) {
			this->setPercent(K_PERCENTS.at("onAllAudiobooksCalled"));

			auto v_client_story = MJDEFAULT->getIntegerForKey(key_api_version_story_local, -1);
			_v_server_story = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, -1);
			if (v_client_story < _v_server_story) {
				ms::EventLogger::getInstance().logEvent(ms::analytics::kEventAPIHome, {});
				mj::api::StoryZip::call(CC_CALLBACK_1(MSIntroLoad::onHomeAPICalled, this));
				//mj::api::LessonInterativeZip::call(nullptr);
				// call api Lessoninteractive::call in here
			}
			else {
				onHomeAPICalled(true);
			}

		}
		else {
			//onNetworkError();// kMSIntroLoadAlertTag_ApiAllAudiobookZip);  if request false, only show alert and doesn't call again
			onHomeAPICalled(true);
		}
		}, DELAY_BETWEEN_API_CALL, "delay_allaudiobooks_next");
}

void MSIntroLoad::onHomeAPICalled(bool is_success) {
	ms::EventLogger::getInstance().logEvent("onHomeAPICalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_categories = MJDEFAULT->getIntegerForKey(key_api_version_categories_local, -1);
		auto v_server_categories = MJDEFAULT->getIntegerForKey(key_api_version_categories_fromServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onHomeAPICalled"));
			// TODO: call API MaterialZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_categories <= 0 || v_client_categories < v_server_categories) {
				mj::api::CategoriesZip::call(CC_CALLBACK_1(MSIntroLoad::onCheckUpdateCatagoriesAPICalled, this));
			}
			else {
				this->onCheckUpdateCatagoriesAPICalled(true);
			}
		}
		else
		{
			//onNetworkError();// kMSIntroLoadAlertTag_ApiStoryZip); if request false, only show alert and doesn't call again
			this->onCheckUpdateCatagoriesAPICalled(true);
		}
		}, DELAY_BETWEEN_API_CALL, "delay_home_next");
}



void MSIntroLoad::onCommonResourceCalled(bool is_success) {
	ms::EventLogger::getInstance().logEvent("onCommonResourceCalled", {
		{"status",cocos2d::Value(is_success)}
		});
	bool isF2pSkipUser = F2P_MANAGER->isSkipUser();
	HelperManager::GetInstance()->addSpriteFramesWithFile("spritesheet/flags.plist", "spritesheet/flags.png");

	HelperManager::GetInstance()->parseCountryCodesInfomationFromJson();

	// nếu là MS thì gọi api material
	this->scheduleOnce([=](float) {
		auto v_client_material = MJDEFAULT->getIntegerForKey(key_api_version_material_local, -1);
		auto v_server_material = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, -1);

		if (is_success) {
			this->setPercent(K_PERCENTS.at("onCommonResourceCalled"));

			MJDEFAULT->setIntegerForKey(key_common_res_version_current, MJDEFAULT->getIntegerForKey(key_common_res_version_request));

			// TODO: call API MaterialZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_material <= 0 || v_client_material < v_server_material) {
				mj::api::MaterialZip::call(CC_CALLBACK_1(MSIntroLoad::onMaterialAPICalled, this));
			}
			else {
				this->onMaterialAPICalled(true);
			}
		}
		else {
			this->onMaterialAPICalled(true);
			//onNetworkError();// kMSIntroLoadAlertTag_ApiLessonListZip); //if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "delay_commonRes_next");

}

void MSIntroLoad::onMaterialAPICalled(bool is_success) {
	ms::EventLogger::getInstance().logEvent("onMaterialAPICalled", {
		{"status",cocos2d::Value(is_success)}
		});

	/*
	 * nội dung hàm này phải gần giống với đoạn gọi scheduleOnce để call api LoadUpdate trong hàm onCommonResourceCalled()
	 */
	this->scheduleOnce([=](float) {
		if (is_success) {
			this->setPercent(K_PERCENTS.at("onMaterialAPICalled"));

			MJDEFAULT->setIntegerForKey(key_api_version_material_local, _v_server_material);

			if (ap::Database::getInstance().getAccessToken().length() > 0 && (MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false))) {
				MJDEFAULT->deleteValueForKey("logged_in_skip");
				if (!MJDEFAULT->getBoolForKey("CL_Loggin_Called", false))
				{
					CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
				}
				bool checkIsLoadUpdate = MJDEFAULT->getBoolForKey(key_checkIsLoadUpdate, false);
				if (!checkIsLoadUpdate)
					ap::api::LoadUpdate::call(CC_CALLBACK_2(MSIntroLoad::onLoadUpdateAPICalled, this));
				else
				{
					this->onLoadUpdateAPICalled(true, "");
				}
			}
			else
			{
				this->onLoadUpdateAPICalled(true, "");
			}
		}
		else {
			this->onLoadUpdateAPICalled(true, "");
			//onNetworkError();// kMSIntroLoadAlertTag_ApiMaterial); if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "delay_material_next");
}

void MSIntroLoad::onAISpeakingLessonCalled(bool is_success)
{
	// after call LessonList api, call AISpeaking Topic api
	ms::EventLogger::getInstance().logEvent("onAISpeakingLessonCalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
			if (is_success) {

				this->setPercent(K_PERCENTS.at("onAISpeakingLessonCalled"));
				onCommonResourceCalled(is_success);
			}
			else
			{
				onCommonResourceCalled(is_success);
				//onNetworkError();// kMSIntroLoadAlertTag_ApiGameListZip); //if request false, only show alert and doesn't call again
			}
		}, DELAY_BETWEEN_API_CALL, "onAISpeakingLessonCalled");
}

void MSIntroLoad::onAISpeakingTopicCalled(bool is_success)
{
	// after call LessonList api, call AISpeaking Topic api
	ms::EventLogger::getInstance().logEvent("onAISpeakingTopicCalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_ai_speaking_common_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_common_list_local, -1);
		auto v_server_ai_speaking_common_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_common_list_fromJsonServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onAISpeakingTopicCalled"));
			// TODO: call API MJAPIAISpeakingCommonListZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_ai_speaking_common_list <= 0 || v_client_ai_speaking_common_list < v_server_ai_speaking_common_list) {
				mj::api::MJAPIAISpeakingCommonListZip::call(CC_CALLBACK_1(MSIntroLoad::onAISpeakingCommonCalled, this));

			}
			else {
				this->onAISpeakingCommonCalled(true);
			}
		}
		else
		{
			this->onAISpeakingCommonCalled(true);
			//onNetworkError();// kMSIntroLoadAlertTag_ApiGameListZip); //if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "onAISpeakingTopicCalled");
}

void MSIntroLoad::onAISpeakingCommonCalled(bool is_success)
{
	onCommonResourceCalled(true);
	return;
	// after call LessonList api, call AISpeaking Topic api
	ms::EventLogger::getInstance().logEvent("onAISpeakingCommonCalled", {
		{"status",cocos2d::Value(is_success)}
		});

	this->scheduleOnce([=](float) {
		auto v_client_ai_speaking_item_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_local, -1);
		auto v_server_ai_speaking_item_list = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, -1);
		if (is_success) {

			this->setPercent(K_PERCENTS.at("onAISpeakingCommonCalled"));
			// TODO: call API MJAPIAISpeakingItemsListZip
			// Điều kiện để gọi api này là:
			//      - version lưu ở local <= 0 (tức chưa gọi thành công api lần nào) hoặc
			//      - version lưu ở local < version mà appinfo trả về

			if (v_client_ai_speaking_item_list <= 0 || v_client_ai_speaking_item_list < v_server_ai_speaking_item_list) {
				mj::api::MJAPIAISpeakingLessonListZip::call(CC_CALLBACK_1(MSIntroLoad::onAISpeakingLessonCalled, this));

			}
			else {
				this->onAISpeakingLessonCalled(true);
			}
		}
		else
		{
			this->onAISpeakingLessonCalled(true);
			//onNetworkError();// kMSIntroLoadAlertTag_ApiGameListZip); //if request false, only show alert and doesn't call again
		}
		}, DELAY_BETWEEN_API_CALL, "onAISpeakingCommonCalled");
}

void MSIntroLoad::onCallRequestFalseApiAgain(ApiTag apiCalltag)
{
	switch (apiCalltag)
	{
	case kMSIntroLoadAlertTag_ApiRegister:
	{
		onEnter();
		break;
	}
	case kMSIntroLoadAlertTag_ApiRequired:
	{
		break;
	}
	case kMSIntroLoadAlertTag_ApiMaterial:
	{
		auto v_client_material = MJDEFAULT->getIntegerForKey(key_api_version_material_local, -1);
		auto v_server_material = MJDEFAULT->getIntegerForKey(key_api_version_material_fromServer, -1);
		if (v_client_material <= 0 || v_client_material < v_server_material) {
			mj::api::MaterialZip::call(CC_CALLBACK_1(MSIntroLoad::onMaterialAPICalled, this));
		}
		else {
			this->onMaterialAPICalled(true);
		}
		break;
	}
	case kMSIntroLoadAlertTag_ApiFisrtInstall:
	{
		mj::RegisterAPI::call(CC_CALLBACK_1(MSIntroLoad::onRegisterAPICalled, this));
		break;
	}
	case kMSIntroLoadAlertTag_ApiProfileList:
	{
		onLoadUpdateAPICalled(true, "");
		break;
	}
	case kMSIntroLoadAlertTag_APIAISpeakingTopicsZip:
	{
		onCheckUpdateLessonListAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_APIAISpeakingItemsZip:
	{
		onAISpeakingTopicCalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiLoadUpdateInfo:
	{
		onCheckUpdateApp(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiAllAudiobookZip:
	{
		onLoadUpdateInfoAPICalled(true, "");
		break;
	}
	case kMSIntroLoadAlertTag_ApiStoryZip:
	{
		onAllAudiobooksCalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiCatagoriesZip:
	{
		onHomeAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiGameListZip:
	{
		onCheckUpdateCatagoriesAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiLessonListZip:
	{
		onCheckUpdateGameListAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiLoadUpdate:
	{
		onMaterialAPICalled(true);
		break;
	}
	default:
		break;
	}
}

void MSIntroLoad::onSkipRequestFalseApi(ApiTag apiCalltag)
{
	switch (apiCalltag)
	{

	case kMSIntroLoadAlertTag_ApiRegister:  // need allway call registerAPI again if request register api  is false
	{
		onEnter();
		break;
	}
	case kMSIntroLoadAlertTag_ApiRequired:
	{
		break;
	}
	case kMSIntroLoadAlertTag_ApiMaterial:
	{
		this->onMaterialAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiFisrtInstall:
	{
		onFisrtInstallThumbCalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiProfileList:
	{
		onProfileListAPICalled(true, "");
		break;
	}
	case kMSIntroLoadAlertTag_ApiLoadUpdateInfo:
	{
		onLoadUpdateInfoAPICalled(true, "");
		break;
	}
	case kMSIntroLoadAlertTag_ApiAllAudiobookZip:
	{
		onAllAudiobooksCalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiStoryZip:
	{	onHomeAPICalled(true);
	break;
	}
	case kMSIntroLoadAlertTag_ApiCatagoriesZip:
	{
		onCheckUpdateCatagoriesAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiGameListZip:
	{
		onCheckUpdateGameListAPICalled(true);
		break;
	}
	case kMSIntroLoadAlertTag_ApiLessonListZip:
	{
		onCommonResourceCalled(true);
		break;
	}

	default:
		break;
	}
}

void MSIntroLoad::onLoadUpdateAPICalled(bool is_success, std::string msg) {

	ms::EventLogger::getInstance().logEvent(ms::analytics::kEventLoadUpdate, {});
    
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]() {
		mj::helper::deleteWaitView();
		if (is_success) {
			this->setPercent(K_PERCENTS.at("onLoadUpdateAPICalled"));
            
            auto f2pCheckExecution = [=]() -> bool {
        
                 if(APPURCHASE_MANAGER->isFreeUser() && !F2P_MANAGER->getKeyGotoF2PGetStarted()) {

					goToF2PFlow();
                    return true;
                }
                
                return false;

            };
            
			if (ap::Database::getInstance().getAccessToken().length() > 0 && (MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false))) {
				MJDEFAULT->deleteValueForKey("logged_in_skip");
				int user_info_version_local = MJDEFAULT->getIntegerForKey(key_api_version_app_info_local,0);
				int user_info_version_from_server = MJDEFAULT->getIntegerForKey(key_api_version_app_info_from_server, 0);
				if (user_info_version_from_server <= 0)
				{
					ap::api::LoadUpdate::call(CC_CALLBACK_2(MSIntroLoad::onLoadUpdateAPICalled, this));
					return;
				}
                if(!f2pCheckExecution())
                {
                    ap::api::ProfilesList::call(CC_CALLBACK_2(MSIntroLoad::onProfileListAPICalled, this));
                }
			}
			else
			{
                if(!f2pCheckExecution())
                {
                    this->onProfileListAPICalled(true, "");
                }
			}
		}
		else
		{
			if (MJDEFAULT->getBoolForKey("require_load_update", true))
			{
				if (msg.empty())
				{
					onNetworkError(kMSIntroLoadAlertTag_ApiLoadUpdate);
				}
				else
				{
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
						{
							_storyAlert = StoryAlert::showAlert(msg, LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "network_ok"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "network_cancel"), false, 28, this);
						});
				}
			}
			else
			{
				if (msg.empty() && FREE_STORY_MANAGER->isUpdate3Days() && MJDEFAULT->getBoolForKey(key_logged_in, false) && !MJDEFAULT->getBoolForKey("key_update_3_day"))
				{
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
						{
							MJDEFAULT->setBoolForKey("key_update_3_day", true);
							_storyAlert = StoryAlert::showAlert(LANGUAGE_MANAGER->getDisplayTextByKey("key.account.time.update"), LANGUAGE_MANAGER->getTextObjectWithKeyOfClass("popup", "network_cancel"), "", false, 28, this, k_tag_free_3_day);
						});

				}
				else
				{
					onAPIDone();
				}
			}
		}
		});

}

void MSIntroLoad::onLoadUpdateInfoAPICalled(bool is_success, std::string message)
{
	int v_server_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1);
	int v_server_story = MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, -1);
	int v_server_lesson = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer, -1);
	int v_server_ai_speaking_topics = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, -1);
	int v_server_ai_speaking_items = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, -1);

	int v_fisrt_install_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_fisrt_install, -1);
	int v_fisrt_install_story = MJDEFAULT->getIntegerForKey(key_api_version_story_fisrt_install, -1);
	int v_fisrt_install_lesson = MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fisrt_install, -1);
	int v_fisrt_install_ai_speaking_topics = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fisrt_install, -1);
	int v_fisrt_install_ai_speaking_items = MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fisrt_install, -1);


	if (v_server_audio > v_fisrt_install_audio + MAX_UPDATE_VERSION ||
		v_server_story > v_fisrt_install_story + MAX_UPDATE_VERSION ||
		v_server_lesson > v_fisrt_install_lesson + MAX_UPDATE_VERSION||
		v_server_ai_speaking_topics > v_fisrt_install_ai_speaking_topics + MAX_UPDATE_VERSION||
		v_server_ai_speaking_items > v_fisrt_install_ai_speaking_items + MAX_UPDATE_VERSION
		)
	{
        //MJDEFAULT->getBoolForKey("key_isNewVersionCall" + currVerion, false);
		MJDEFAULT->setBoolForKey(key_check_is_first_install_is_finished, false);
		//if the updated version is counted from the first install >  MAX_UPDATE_VERSION, we are need call firstinstall to download thumb zip
		if (v_server_audio > v_fisrt_install_audio)
		{
			MJDEFAULT->setIntegerForKey(key_api_version_audio_fisrt_install, MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1));
		}
		if (v_server_story > v_fisrt_install_story)
		{
			MJDEFAULT->setIntegerForKey(key_api_version_story_fisrt_install, MJDEFAULT->getIntegerForKey(key_api_version_story_v2_fromJsonServer, -1));
		}
		if (v_server_lesson > v_fisrt_install_lesson)
		{
			MJDEFAULT->setIntegerForKey(key_api_all_lessons_version_fisrt_install, MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_fromServer, -1));
		}
		if (v_server_ai_speaking_items > v_fisrt_install_ai_speaking_items)
		{
			MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_lesson_list_fisrt_install, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_lesson_list_fromJsonServer, -1));
		}
		if (v_server_ai_speaking_topics > v_fisrt_install_ai_speaking_topics)
		{
			MJDEFAULT->setIntegerForKey(key_api_version_ai_speaking_topic_fisrt_install, MJDEFAULT->getIntegerForKey(key_api_version_ai_speaking_topic_fromJsonServer, -1));
		}
		mj::RegisterAPI::call(CC_CALLBACK_1(MSIntroLoad::onRegisterAPICalled, this));
		return;
	}
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			if (is_success) {

				this->setPercent(K_PERCENTS.at("onLoadUpdateInfoAPICalled"));

				auto v_client_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, -1);
				_v_server_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_v2_fromServer, -1);

				if (v_client_audio < _v_server_audio) {
					ms::EventLogger::getInstance().logEvent(ms::analytics::kEventAPIAllAudiobooks, {});
					mj::api::AudiobookZip::call(CC_CALLBACK_1(MSIntroLoad::onAllAudiobooksCalled, this));
				}
				else
				{
					onAllAudiobooksCalled(true);
				}
			}
			else {
				//onNetworkError();// kMSIntroLoadAlertTag_ApiLoadUpdateInfo);
				onAllAudiobooksCalled(true);
			}
		});
}

bool MSIntroLoad::CompareVersionApp(std::string currVerion)
{
    auto listCurrentVersion = mj::helper::splitString(currVerion, '.');
    std::string newestVersion = MJDEFAULT->getStringForKey(key_ver_newest_version, "0");
    auto listNewVersion = mj::helper::splitString(newestVersion, '.');

    int version = 0;

    if (listCurrentVersion.size() >0)
    {
        int index = std::pow(10, listCurrentVersion.size());

        for (int i = 0; i < listCurrentVersion.size(); i++)
        {
            version = version + std::atoi(listCurrentVersion[i].c_str()) * index;
            index = index / 10;
        }
    }

    int verNew = 0;

    if (listNewVersion.size() > 0)
    {
        int index = std::pow(10, listNewVersion.size());

        for (int i = 0; i < listNewVersion.size(); i++)
        {
            verNew = verNew + std::atoi(listNewVersion[i].c_str()) * index;
            index = index / 10;
        }
    }
    return version > verNew;
}

void MSIntroLoad::onProfileListAPICalled(bool is_success, std::string message)
{
	this->scheduleOnce([=](float) {
        ms::EventLogger::getInstance().logEvent(ms::analytics::kEventProfileList, {
            {"status",cocos2d::Value(is_success)},
            });
        this->setPercent(K_PERCENTS.at("onProfileListAPICalled"));
        this->onAPIDone();
		}, DELAY_BETWEEN_API_CALL, "delay_refresh_next");
}

void MSIntroLoad::onNetworkError(int tag)
{
	ms::EventLogger::getInstance().logEvent("onNetworkError", {
		{"tag",cocos2d::Value(tag)}
		});

	MJDEFAULT->setStringForKey(key_live_in_screen, "onboarding");

	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{
			auto csb_name = mj::helper::getCSBName("csb/StoryNetworkDown.csb");

			auto alert = static_cast<StoryAlert*>(CSLoader::createNode(csb_name));
			alert->setNetworkDownContent(false, true, false);

			if (MJDEFAULT->getStringForKey(key_device_identifier, "").empty())
			{
				alert->show();
			}
			else
			{
				alert->show(Director::getInstance()->getRunningScene(), 5);
			}
			alert->setTag(tag);
			alert->setDelegate(this);
		});
}

void MSIntroLoad::onAlertCancel(Ref* sender)
{

	// hieunt: nếu alert ở bước call api Material
	if (auto alert = static_cast<StoryAlert*>(sender))
	{
		// skip call api again if use choose cancel button
		ApiTag apiCallTag = (ApiTag)alert->getTag();

		// call again falsed api
		onCallRequestFalseApiAgain(apiCallTag);
	}

	// code cũ
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
		{

			ms::EventLogger::getInstance().logEvent("onAlertCancel", {
				{"tag",cocos2d::Value("api_register")}
				});

		});
}

void MSIntroLoad::onAlertOK(Ref* sender)
{
	if (auto alert = static_cast<StoryAlert*>(sender))
	{
		int apiCallTag = alert->getTag();
		// after 3 days user not openapp, call Loadupdate api 
		ap::api::LoadUpdate::call(CC_CALLBACK_2(MSIntroLoad::onLoadUpdateInfoAPICalled, this));
		// call again falsed api
		// 
		//onCallRequestFalseApiAgain((ApiTag)apiCallTag);

	}
	// code cũ
	MJDEFAULT->setIntegerForKey(key_api_version, 0);

	if (auto node = dynamic_cast<Node*>(sender))
	{
		if (node->getTag() == k_tag_free_3_day) // check popup 3 day
		{
			ms::EventLogger::getInstance().logEvent("onAlertOK", {
				{"tag",cocos2d::Value("free_3_day")}
				});

			_is_api_required = true;
			//MJDEFAULT->deleteValueForKey(key_first_api_called);
		}
	}
}

void MSIntroLoad::preloadThumb()
{
	CRASH_AUTO_LOG;

	std::vector<std::string> thumbs;
	auto features = StoryDataModel::getInstance()->getFeaturedStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
	for (auto f : features) {
		thumbs.push_back(f.second);
	}

	for (auto t : thumbs) {
		if (FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + mj::helper::getFilenameFromURL(t))) {
			continue;
		}

		ms::utils::ThumbQueue::getInstance().enqueue(t, nullptr, RemoteImageView::Type::OTHER);
	}

	thumbs.clear();

	auto popular_stories = StoryDataModel::getInstance()->getAllStoriesByLanguage(LANGUAGE_MANAGER->getCurrentLangId());
	std::stable_sort(std::begin(popular_stories), std::end(popular_stories), [](mj::model::StoryInfo a, mj::model::StoryInfo b) {
		if (a.quality != b.quality) {
			return a.quality > b.quality;
		}

		if (a.is_read != b.is_read) {
			return (int)a.is_read < (int)b.is_read;
		}

		if (a.date_published != b.date_published) {
			return a.date_published > b.date_published;
		}

		return false;
		});

	if (!popular_stories.empty()) {
		for (auto i = 0; i < MIN((int)popular_stories.size(), 8); ++i) {
			thumbs.push_back(popular_stories[i].thumbnail_url);
		}
	}

	std::stable_sort(std::begin(popular_stories), std::end(popular_stories), [](mj::model::StoryInfo a, mj::model::StoryInfo b) {
		if (a.quality != b.quality) {
			return a.quality > b.quality;
		}

		if (a.is_read != b.is_read) {
			return (int)a.is_read < (int)b.is_read;
		}

		if (a.date_published != b.date_published) {
			return a.date_published > b.date_published;
		}

		return false;
		});

	auto counter = 0;
	auto idx = 0;
	while (counter < 8 && idx < (int)popular_stories.size()) {
		auto url = popular_stories[idx].thumbnail_url;
		if (!mj::helper::isContain(thumbs, url)) {
			counter++;
			thumbs.push_back(url);
		}
		idx++;
	}

	for (auto t : thumbs) {
		if (FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->getWritablePath() + IMAGE_CACHE_PATH + mj::helper::getFilenameFromURL(t))) {
			continue;
		}

		ms::utils::ThumbQueue::getInstance().enqueue(t, nullptr, RemoteImageView::Type::STORIES, true);
		
	}
}


void MSIntroLoad::goHome()
{
	if (!MJDEFAULT->getBoolForKey("CL_Loggin_Called", false))
	{
		try
		{
			CleverTapManager::GetInstance()->pushProfile(CleverTapManager::PUSH_TYPE::UPDATE, nullptr);

			CleverTapManager::GetInstance()->pushEvent("event_login", {
				{"view_screen",cocos2d::Value(true)},
				{"login_type",cocos2d::Value(MJDEFAULT->getStringForKey("ap_login_type","reloginwithcurrenttoken"))},
				{"login_successfull",cocos2d::Value(true)},
				{"is_upgrade",cocos2d::Value(false)},
				{"forgot_password",cocos2d::Value(MJDEFAULT->getStringForKey("CL_FORGOT_PASSWORD","not_yet"))},
				});

		}
		catch (const std::exception&)
		{

		}
	}

	if (!MJDEFAULT->getBoolForKey("key_profile_welcome_back", false))
	{
		MJDEFAULT->setBoolForKey("key_profile_welcome_back", true);
		auto finish_view = APProfileFinishView::createView(ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id)), false);
		Director::getInstance()->getRunningScene()->addChild(finish_view, INT_MAX - 1);
		return;
	};

	auto home = MSHome::createScene();
	if (!CONFIG_MANAGER->isAppActive()) {
		if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false)) {
			if (MSInstallPromotion::shouldShowPopup()) {
				auto popup = MSInstallPromotion::createView(true);
				home->addChild(popup);
			}
			else {
				if (!MJDEFAULT->getBoolForKey("is_user_skip", false)) {
					//auto popup = StoryWelcomePopUp::createPage();
					//popup->setName("StoryWelcomePopUp");
					//home->addChild(popup);
				}
			}
		}
	}

	MJDEFAULT->deleteValueForKey("CL_Loggin_Called");

	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));

}

void MSIntroLoad::onNextScene() {

	MJDEFAULT->setIntegerForKey(key_call_welcome_popup, CONFIG_MANAGER->isAppActive() ? 3 : 0);

	//if (MJDEFAULT->getIntegerForKey(key_current_profile_id, 0) == 0 || ap::Database::getInstance().getProfileById(MJDEFAULT->getIntegerForKey(key_current_profile_id, 0)).profile_id == -1) 
	{
		auto choose_profile = APChooseProfileView::createView();
		choose_profile->setName("choose_profile_view");
		choose_profile->hideCreateNewProfilesButton();
		Director::getInstance()->getRunningScene()->addChild(choose_profile, INT_MAX - 1);


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		CCLOG("ms::analytics::Firebase::QR_DEEP_LINK :%s", ms::analytics::Firebase::QR_DEEP_LINK.c_str());
		if (ms::analytics::Firebase::QR_DEEP_LINK.empty())
		{
			const std::string k_pendingDeeplink = cocos2d::JniHelper::callStaticStringMethod("org.cocos2dx.cpp.AppActivity", "getPendingDeeplink");

			if (!k_pendingDeeplink.empty()) {
				ms::analytics::Firebase::QR_DEEP_LINK = k_pendingDeeplink;
			}
		}
#endif

		if (!ms::analytics::Firebase::QR_DEEP_LINK.empty())
		{
			if (ms::analytics::Firebase::QR_DEEP_LINK.find("newprofile") != std::string::npos)
			{
				choose_profile->onNewProfile(nullptr);
				ms::analytics::Firebase::QR_DEEP_LINK = "newprofile";
			}

			if (ms::analytics::Firebase::QR_DEEP_LINK.find("chooseprofile") != std::string::npos)
			{
				ms::analytics::Firebase::QR_DEEP_LINK = "";
			}
		}

		return;
	}

	MJDEFAULT->deleteValueForKey("is_link_account_processing");
    int currentProfileId = MJDEFAULT->getIntegerForKey(key_current_profile_id);
    if( currentProfileId >0)
    {
        PROFILE_MANAGER.getInstance().switchProfile(currentProfileId);
    }
	PROFILE_MANAGER.clearDataModel();
	PROFILE_MANAGER.getInstance().loadProcess();

	if ((MJDEFAULT->getBoolForKey(key_Actived, false) && !MJDEFAULT->getBoolForKey("check_is_free", false)) || MJDEFAULT->getBoolForKey(key_is_submit) || !MJDEFAULT->getBoolForKey("is_license_feature_enable", false) || CONFIG_MANAGER->isVietnameseStories()) {
		goHome();
	}
	else {
		ms::EventLogger::getInstance().logEvent("view_license_intro", {});
		auto license_confirm = APWaitView::createView("Bạn đã có MÃ KÍCH HOẠT chưa ?", true, false);
		license_confirm->setVisible(this->isVisible());
		license_confirm->setName("loadview");
		license_confirm->setOnConfirmCallback([=](bool confirm)
			{
				//                MJDEFAULT->setBoolForKey(key_onboarding_license, true);
				Director::getInstance()->getRunningScene()->removeChildByName("loadview");
				Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
					{
						if (confirm)
						{
							ms::EventLogger::getInstance().logEvent("click_license_confirm", {});
							auto intro_license = MSIntroLicense::showPopup();
							intro_license->setOnLicenseCallback([=](bool success)
								{
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

void MSIntroLoad::onAPIDone()
{
	CRASH_AUTO_LOG;

	setPercent(K_PERCENTS.at("onAPIDone"));
	preloadThumb();
	_is_api_done = true;
	_is_api_required = false;

	ms::utils::Downloader::getInstance().init();

	MJDEFAULT->setBoolForKey(key_first_api_called, false);

	//đã load xong api, và đã onboard part 1 xong




	auto intro_onboard = Director::getInstance()->getRunningScene()->getChildByName("intro_onboard");
	bool isF2pSkipUser = F2P_MANAGER->isSkipUser();
    
	if (isF2pSkipUser || MJDEFAULT->getBoolForKey("on_register_success_f2p", false)||
		F2P_MANAGER->isCanOpenMergeProfilePage()|| !F2P_MANAGER->getKeyGotoF2PGetStarted()||
		F2P_MANAGER->isMegreSkipAccount())
	{
        goToF2PFlow();
	}
	else
	{
		goToNormalFlowAfterAPIDone();
	}


}

void MSIntroLoad::addTextDeviceID() {
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
		this->removeChildByName("text_device_id");
		auto id = MJ_PLATFORM_CONFIG.getDeviceId();
		auto text = ui::Text::create(StringUtils::format("Device ID: %s", id.c_str()), "fonts/Montserrat-SemiBold.ttf", 35);
		text->setName("text_device_id");
		text->setFontSize(20);
		text->setTextColor(Color4B::BLACK);
		text->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
		text->setPosition(Point(Director::getInstance()->getVisibleSize().width - 20 - MJ_PLATFORM_CONFIG.getNotchHeight(), Director::getInstance()->getVisibleSize().height - 20));
		text->setOpacity(0x99);
		this->addChild(text, INT_MAX - 1);
		});
}

void MSIntroLoad::updateProfileInClevertab(int idProfile)
{
    if(idProfile != 0)
    {
        CleverTapManager::GetInstance()->pushProfileByID(idProfile,CleverTapManager::PUSH_TYPE::LOGIN,nullptr);
    }
}

void MSIntroLoad::goToF2PFlow()
{
	
	int profileId = PROFILE_MANAGER.getCurrentProfileId();
	bool isRegistered = MJDEFAULT->getBoolForKey("on_register_success_f2p", false);
	auto userInfo = ap::Database::getInstance().getUserInfo();

	bool is_logon = ((MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false)) && !ap::Database::getInstance().getAccessToken().empty());

    if (F2P_MANAGER->isCanOpenMergeProfilePage())
    {
        auto scene = Merge1Scene::createScene();
        updateProfileInClevertab(userInfo._id);
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
        return;
    }
    if(F2P_MANAGER->isMegreSkipAccount())
    {
        this->runAction(Sequence::create(DelayTime::create(0.2f),
            CallFunc::create([=] {
            updateProfileInClevertab(userInfo._id);
            auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::Merge1Skip, ms::Authen::StartRegisterFrom::Default);
            loginView->setVisible(this->isVisible());
            loginView->setName("loginView");
            Director::getInstance()->getRunningScene()->addChild(loginView, INT_MAX-2);

                }),
            NULL));
    }
	else if (is_logon && !F2P_MANAGER->isSkipUser())
	{
		goToNormalFlowAfterAPIDone();
	}
	else if ((profileId == 0 && !isRegistered) || !F2P_MANAGER->getKeyGotoF2PGetStarted())// || userInfo._id == 0)
	{
		/*
		* userId = 0, bieu thi cho viec user chua dang nhap/ dang ki
		*/
        this->runAction(Sequence::create(DelayTime::create(0.2f),
            CallFunc::create([=] {
            updateProfileInClevertab(userInfo._id);
            auto scene = GettingStartedScene::createScene();
            Director::getInstance()->replaceScene(TransitionSlideInR::create(0.02, scene));
                }),
            NULL));
	}
	else if(userInfo._id != 0 && profileId != 0)

	{
        updateProfileInClevertab(userInfo._id);
        CleverTapManager::GetInstance()->pushProfileByID(PROFILE_MANAGER.getCurrentProfileId(),CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
		if (APPURCHASE_MANAGER->isSkipUser())
		{
			auto scene = MSFreeContentPage::createScene();
			Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
		}
		else if(APPURCHASE_MANAGER->getTypeUser() == IAP::user::TYPE_USERS::USER_EXPRIED)
		{
			MJDEFAULT->deleteValueForKey("on_register_success_f2p");
			MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::TEN_ITEM_FREE);
			auto scene = MSFreeContentPage::createScene();
			Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
		}
        else
        {
            goHome();
        }
	}
	else if (isRegistered && is_logon) {
        updateProfileInClevertab(userInfo._id);
		auto scene = NameSetupScene::createScene();
		Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
	}
	else
	{
		goToNormalFlowAfterAPIDone();
	}


}

void MSIntroLoad::goToNormalFlowAfterAPIDone()
{
	bool is_logon = ((MJDEFAULT->getBoolForKey(key_logged_in, false) || MJDEFAULT->getBoolForKey("logged_in_skip", false)) && !ap::Database::getInstance().getAccessToken().empty());
	if (is_logon) {
		MJDEFAULT->deleteValueForKey("logged_in_skip");
		MJDEFAULT->setBoolForKey("require_load_update", false);
		auto content = this->getChildByName("content");
		auto delay = 0.f;
		auto counter = 0;
		for (auto c : content->getChildren()) {
			c->runAction(Sequence::create(DelayTime::create(delay), Spawn::create(EaseQuinticActionIn::create(MoveBy::create(.5f, Vec2(-1000, 0))), FadeOut::create(.5f), nullptr), nullptr));
			delay += .1f - counter++ * 0.01;
		}


		auto lang_id = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
		if (MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_time_mark_new, lang_id)) == 0) {
			auto list_data_stories = StoryDataModel::getInstance()->getAllStories(MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_level_choose, lang_id), 0), lang_id, "date_publish DESC, read, quality DESC");
			auto max_index_new = min(19, (int)list_data_stories.size() - 1);
			if (max_index_new >= 0) {
				auto time_mark_new = list_data_stories[max_index_new].asValueMap()["date_publish"].asInt();
				MJDEFAULT->setIntegerForKey(StringUtils::format("%s%d", key_time_mark_new, lang_id), time_mark_new);
				list_data_stories.clear();
			}
		}

		if (MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_time_mark_new_AUDIO, lang_id), 0) == 0) {
			auto audiobooks = StoryDataModel::getInstance()->getAllAudiobooksByLanguage(StoryLanguageManager::getCurrentLangId());
			//sort audiobooks
			if (!audiobooks.empty()) {
				std::sort(audiobooks.begin(), audiobooks.end(), [=](mj::model::AudiobookInfo a, mj::model::AudiobookInfo b) -> bool {
					if (a.date_publish != b.date_publish) {
						return a.date_publish > b.date_publish;
					}
					else {
						if (a.read != b.read) {
							return a.read < b.read;
						}
						else return a.quality > b.quality;
					}
					});

				auto max_index_new = min(19, (int)audiobooks.size() - 1);
				if (max_index_new >= 0) {
					auto time_mark_new = audiobooks[max_index_new].date_publish;
					MJDEFAULT->setIntegerForKey(StringUtils::format("%s%d", key_time_mark_new_AUDIO, lang_id), time_mark_new);
					audiobooks.clear();
				}
			}
		}
		Director::getInstance()->getRunningScene()->scheduleOnce([this](float) {
			if (CONFIG_MANAGER->isAppActive() && !FREE_STORY_MANAGER->isFirst2Days()) {
				onNextScene();

			}
			else {
				this->runAction(Sequence::create(FadeOut::create(0.25), Hide::create(), nullptr));

				if (0 && FREE_STORY_MANAGER->isFirst2Days() && !MJDEFAULT->getBoolForKey("key.show.2.days.free.welcome")) {
					MJDEFAULT->setBoolForKey("key.show.2.days.free.welcome", true);
					auto csb_name = mj::helper::getCSBName("csb/new_ui/intro/Intro_Welcome.csb");
					auto free_text = CSLoader::createNode(csb_name);
					utils::findChild<ui::Text*>(free_text, "title")->setString(LANGUAGE_MANAGER->getDisplayTextByKey("free_welcome"));
					this->getParent()->addChild(free_text);
					free_text->setOpacity(0);
					//TODO: căn lại free welcome
					free_text->setAnchorPoint(Point::ANCHOR_MIDDLE);
					free_text->setPosition(Director::getInstance()->getVisibleSize() / 2);
					free_text->runAction(Sequence::create(FadeIn::create(0.25), DelayTime::create(3.5), CallFunc::create([this] {
						onNextScene();
						}), NULL));
				}
				else {
					onNextScene();
				}
			}
			}, delay + .5f, "schedule_next_page");
	}
	else
	{
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(1.0f), CallFunc::create([=]() {

			auto loginView = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN, ms::Authen::StartLoginFrom::MSIntroLoad, ms::Authen::StartRegisterFrom::Default);
			loginView->setOnLoginCallback([this] {

				});

			auto e = EventListenerCustom::create("onCommonResourceCalled_done", [=](EventCustom*)
				{
					Director::getInstance()->getScheduler()->performFunctionInCocosThread([=]
						{
							scheduleOnce([=](float)
				
								{
									this->onCommonResourceCalled(true); // call loadupdates
								}, DELAY_BETWEEN_API_CALL, "delay api");

						});
				});

			Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(e, this);

			loginView->setVisible(this->isVisible());
			loginView->setName("loginView");
			Director::getInstance()->getRunningScene()->addChild(loginView);
			})));
	}
	setPercent(100);
	MJDEFAULT->deleteValueForKey("on_register_success_f2p");
}

void MSIntroLoad::onSkip(cocos2d::Ref* sender) {
	_show_sample = false;
	_read_sample = false;
	onAPIDone();
}

void MSIntroLoad::onSampleStory(cocos2d::Ref* sender) {
	_read_sample = true;
	//    onAPIDone();

	auto lang_id = MJDEFAULT->getIntegerForKey(key_current_lang, 1);
	if (MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_time_mark_new, lang_id)) == 0) {
		auto list_data_stories = StoryDataModel::getInstance()->getAllStories(MJDEFAULT->getIntegerForKey(StringUtils::format("%s%d", key_level_choose, lang_id), 0), lang_id, "date_publish DESC, read, quality DESC");
		auto max_index_new = min(19, (int)list_data_stories.size() - 1);
		if (max_index_new >= 0) {
			auto time_mark_new = list_data_stories[max_index_new].asValueMap()["date_publish"].asInt();
			MJDEFAULT->setIntegerForKey(StringUtils::format("%s%d", key_time_mark_new, lang_id), time_mark_new);
			list_data_stories.clear();
		}
	}

	MJDEFAULT->setIntegerForKey(key_call_welcome_popup, CONFIG_MANAGER->isAppActive() ? 3 : 0);

	auto home = MSHome::createScene();
	if (!CONFIG_MANAGER->isAppActive()) {
		if (MSInstallPromotion::shouldShowPopup()) {
			auto popup = MSInstallPromotion::createView(true);
			home->addChild(popup);
		}
		else {
			if (!MJDEFAULT->getBoolForKey("is_user_skip", false)) {
				auto popup = StoryWelcomePopUp::createPage();
				home->addChild(popup);
			}
		}
	}

	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
}

void MSIntroLoad::onRestore(cocos2d::Ref* sender) {
	if (CONFIG_MANAGER->isCountryVN()) {
		_show_license = true;
		onAPIDone();
	}
	else {
		mj::Pay::getInstance().restore();
	}
}

#define key_api_all_lessons_version_vm "key.api.all.lessons.vm.ver"
void MSIntroLoad::onTestActVMonkey() {

	auto game_ver = MJDEFAULT->getIntegerForKey(key_api_version_all_games_local);
	auto vm_lesson_ver = MJDEFAULT->getStringForKey(key_api_all_lessons_version_vm, "0");

	auto clone_request = new mj::Request(mj::Request::ActionType::APP_INFO_V2, {
		{ "version_game_number", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, 0)) },
		{ "version_lesson_number", MJDEFAULT->getStringForKey(key_api_all_lessons_version_vm, "0") },
		{ "api_ver_audio_book", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0))},
		{ "api_ver_story", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0))}
		});

	auto url = clone_request->getURL();
	url = mj::helper::replace_string(url, "app_id=40", "app_id=51");
	if (url.find("http://<new_platform>") != std::string::npos) {
		if (CONFIG_MANAGER->isProductEnvironment()) {
			url = mj::helper::replace_string(url, "http://<new_platform>", "https://api.daybehoc.com");
		}
		else if (CONFIG_MANAGER->isDevEnvironment()) {
			url = mj::helper::replace_string(url, "http://<new_platform>", "https://api.dev.monkeyuni.net");
		}
		else {
			url = mj::helper::replace_string(url, "http://<new_platform>", "https://api.monkeyuni.net");
		}
	}
	cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
	request->setUrl(url);
	request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
	request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
		if (response->getResponseCode() != 200) {
			// failed
			return;
		}

		auto buffer = *response->getResponseData();
		auto data = std::string(buffer.begin(), buffer.end());

		auto unzip_json = mj::helper::unzip_json(data);
		if (!unzip_json) {
			return;
		}
		rapidjson::Document doc;
		doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char*)unzip_json);

		if (doc.HasParseError()) {
			return;
		}

		if (!doc.IsObject()) {
			return;
		}

		auto map_response = doc.GetObject();
		if (JSON_SAFE_CHECK(map_response, "game_list", Object)) {
			mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(1, map_response["game_list"].GetObject());
		}

		std::vector<int> const vmonkey_grade_id = {
			498, 499, 502,507,509
		};

		if (JSON_SAFE_CHECK(map_response, "categories_list", Array)) {
			mj::api::MJAPIDecodeObjectJsonInfo::onDecodeCategoriesInfoData(1, map_response["categories_list"].GetObject());
		}

		if (JSON_SAFE_CHECK(map_response, "lesson_list", Object)) {
			mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLessonListData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Default, map_response["lesson_list"].GetObject());
		}
		});

	cocos2d::network::HttpClient::getInstance()->send(request);
	request->release();
}

MSIntroLoad::~MSIntroLoad() {

	MJDEFAULT->deleteValueForKey(key_live_in_screen);
}
