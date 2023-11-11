
//
//  APLoadUpdateAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#include "APLoadUpdateAPI.h"
#include "APStoreErrorAPI.h"

#include "APDatabase.h"
#include "cocos2d.h"
#include "MJPlatformConfig.h"

#include "MJDefault.h"
#include "MJPay.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryFreeManager.h"
#include "MJStoryInfoModel.h"
#include "StoryDataModel.h"

#include "MSEventLogger.h"

#include "HSLib.h"
#include "JsonDump.h"
//#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/prettywriter.h"
#include <iostream>

#include "APProfileManager.h"
#include "manager/RatingManager.h"
#include "MMFirebaseDatabase.h"
#define key_last_update_id "key_last_update_id"

namespace ap {
    namespace api {
        void LoadUpdate::call(bool skip_update_info, const std::function<void (bool, std::string)> &callback) {
            api::APIConfig config = api::APIConfig();
            /*config.app_version = cocos2d::Application::getInstance()->getVersion();
            config.app_id = 40;
            config.env = api::Environment::DEV;
            config.language = api::Language::EN_US;
            config.operation_system = "mac";
            config.device_id = MJ_PLATFORM_CONFIG.getDeviceId();
            config.device_info = MJ_PLATFORM_CONFIG.getDeviceInfo();
            config.device_type = api::DeviceResolution::HIGH_DEFINITION;*/
            ap::api::Base::setGlobalConfiguration(config);
            auto api_call = new LoadUpdate(skip_update_info, callback);
            api_call->send();
        }
    
        void LoadUpdate::send() const {
            if (_request) {
                _request->send();
            }
        }
    
        LoadUpdate::LoadUpdate(bool skip_update_info, const std::function<void(bool,std::string)> & callback) : _callback(callback), _skip_update_info(skip_update_info) {
            _request = new Base(api::PlatformType::NEWV2, "account/load-update", {
               
            }, api::RequestType::GET);
            _request->enableLog();
            _request->setDelegate(this);
			this->retain();
        }
    
        void LoadUpdate::onRequestError(int tag, int error_code, const std::string &message,  const rapidjson::Value &data) {
            StoreError::call(_request->getURL(), message, "account/load-update", _request->getParam());
            if (_callback) {
                _callback(false, message);
            }
			this->autorelease();
        }
        
        void LoadUpdate::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {
            updateInfo(tag, data);

//            if (_callback) {
//                _callback(true, "");
//            }
//
//            delete this;
        }
        
        void LoadUpdate::updateInfo(int tag, const rapidjson::Value &data) {
            auto map_response = data.GetObject();
		
            if (map_response.HasMember("is_submit")) {
                 MJDEFAULT->setBoolForKey(key_is_submit, map_response["is_submit"].GetBool());
            }
            
            if (map_response.HasMember("show_learn_to_spell")) {
                 MJDEFAULT->setBoolForKey("show_hoc_van", map_response["show_learn_to_spell"].GetBool());
            }
            
			if (JSON_SAFE_CHECK(map_response, "show_feature_license", Bool)) 
			{
				MJDEFAULT->setBoolForKey("is_license_feature_enable", map_response["show_feature_license"].GetBool());
			}
            
            if (map_response.HasMember("current_time")) {
                if (!CONFIG_MANAGER->isAppActive()) {
                    if (!FREE_STORY_MANAGER->compareFreeTimeRead(MJDEFAULT->getIntegerForKey(key_time_curent), map_response["current_time"].GetInt())) {
                        FREE_STORY_MANAGER->map_story_recent.clear();
                    }
                }
                MJDEFAULT->setIntegerForKey(key_time_curent, map_response["current_time"].GetInt());
                
                if (CONFIG_MANAGER->isTestFreeTimeFake()) {
                    FREE_STORY_MANAGER->freetime_fake_count++;
                    if (MJDEFAULT->getIntegerForKey(key_app_firstdate, 0) != 0) {
                        auto date_current = LTDate(map_response["current_time"].GetInt());
                        date_current = date_current + FREE_STORY_MANAGER->freetime_fake_count*24*3600;
                        auto t = date_current.toTimeStruct();
                        MJDEFAULT->setIntegerForKey(key_time_fake, (int)mktime(&t));
                        MJDEFAULT->setIntegerForKey(key_app_firstdate, 1504604738);
                        FREE_STORY_MANAGER->map_story_recent.clear();
                    }
                    else {
                        MJDEFAULT->setIntegerForKey(key_time_fake, map_response["current_time"].GetInt());
                    }
                }
            }
            
            if (map_response.HasMember("limit_seconds_offline")) {
                int time_offline_limit = map_response["limit_seconds_offline"].GetInt();
                MJDEFAULT->setIntegerForKey(key_time_offline_limit, time_offline_limit);
            }
            // check theo time expire
            bool check_time = false;

			if (JSON_SAFE_CHECK(map_response["purchased"], "time_expire", Int) &&
				JSON_SAFE_CHECK(map_response,"current_time", Int))
			{
				auto time_expire	= map_response["purchased"]["time_expire"].GetInt();
				auto current_time	= map_response["current_time"].GetInt();

				if (time_expire > current_time) check_time = true;
			}
            
            bool isFreeUse = map_response["purchased"]["is_free"].GetBool();
            MJDEFAULT->setBoolForKey("check_is_free", isFreeUse);
            if (check_time) isFreeUse = false;
            MJDEFAULT->setBoolForKey(key_isFreeUse, isFreeUse);
            
            bool isActive = map_response["purchased"]["is_active"].GetBool() & !isFreeUse;
            if (check_time) isActive = true;
            MJDEFAULT->setBoolForKey(key_Actived, isActive);
            if (!isActive) {
                if (map_response["purchased"].HasMember("time_expire")) {
                    MJDEFAULT->setIntegerForKey(key_freetime_expire, map_response["purchased"]["time_expire"].GetInt());
                }
                
                if (MJDEFAULT->getIntegerForKey(key_app_firstdate, 0) == 0) {
                    if (map_response.HasMember("day_readed")) {
                        if (!map_response["day_readed"].IsNull() && map_response["day_readed"].IsString()) {
                            std::string day_readed = map_response["day_readed"].GetString();
                            if (day_readed != std::string("")) {
                                auto story_lang_id = std::atoi(day_readed.c_str());
                                if (story_lang_id > 0) {
                                    auto time_current_day = MJDEFAULT->getIntegerForKey(key_time_curent);
                                    auto date_current = LTDate(time_current_day);
                                    auto key = cocos2d::StringUtils::format("%s_%d_%d_%d", key_freetime_read, date_current.year, date_current.month, date_current.day);
                                    MJDEFAULT->setIntegerForKey(key, story_lang_id);
                                    FREE_STORY_MANAGER->setPercentActionFreePlayed(100);
                                }
                            }
                        }
                    }
                    MJDEFAULT->setIntegerForKey(key_app_firstdate, MJDEFAULT->getIntegerForKey(key_time_curent));
                }
                
                mj::Pay::getInstance().forceOverrideLicense("", 0);
                mj::Pay::getInstance().updateActiveStatus(true, false);
            } else {
                mj::Pay::getInstance().updateActiveStatus(true, true);
//                MJDEFAULT->setBoolForKey(key_isFreeUse, false);
            }
            
            if (JSON_SAFE_CHECK(map_response, "purchased", Object))
            {
                auto map_purchased = map_response["purchased"].GetObject();

                if (JSON_SAFE_CHECK(map_purchased,"time_activated",Int))
                {
                    auto time =  map_purchased["time_activated"].GetInt();
                    
                    MJDEFAULT->setIntegerForKey("key_time_activated", time);
                }
                
                if (JSON_SAFE_CHECK(map_purchased,"time_purchased",Int))
                {
                    auto time =  map_purchased["time_purchased"].GetInt();
                    
                    MJDEFAULT->setIntegerForKey("key_time_purchased", time);
                }
            }
            
            ms::EventLogger::getInstance().setUserProperties("purchased", isActive ? "yes" : "no");
            
            std::string license = "";
            if (map_response.HasMember("licence") && map_response["licence"].IsString()) {
                license = map_response["licence"].GetString();
            }
            auto time_expire = 0;
            if (map_response["purchased"].HasMember("time_expire") && map_response["purchased"]["time_expire"].IsInt()) {
                time_expire = map_response["purchased"]["time_expire"].GetInt();
            }
            
            if (time_expire == 0 && isActive) {
                time_expire = -1;
            }
            
            MJDEFAULT->setIntegerForKey("key_time_expire", time_expire);

            mj::Pay::getInstance().forceOverrideLicense(license, time_expire);
            
            if (JSON_SAFE_CHECK(map_response,"is_licence",Bool))
			{
                MJDEFAULT->setBoolForKey(key_is_licence, map_response["is_licence"].GetBool());
            }
            
            if (JSON_SAFE_CHECK(map_response,"country_code",String))
			{
                MJDEFAULT->setStringForKey(key_country_code, map_response["country_code"].GetString());
            }
            
			if (JSON_SAFE_CHECK(map_response, "time_update", String))
			{
                auto time_update_app = map_response["time_update"].GetString();
            }
            
            if (JSON_SAFE_CHECK(map_response,"version_update",String)) 
			{
                auto ver_update_app = map_response["version_update"].GetString();
                MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
            }
            
            if (JSON_SAFE_CHECK(map_response,"version_required",String))
			{
                auto version_required = map_response["version_required"].GetString();
                MJDEFAULT->setStringForKey(key_version_required, version_required);
            }
            
            if (JSON_SAFE_CHECK(map_response,"update",Object))
			{
                const rapidjson::Value& obj_update = map_response["update"];
                
                if(JSON_SAFE_CHECK(obj_update,"id",Int))
                {
                    auto update_id = obj_update["id"].GetInt();
                    // reset time update skip when retrive new update
                    if(MJDEFAULT->getIntegerForKey(key_last_update_id) != update_id)
                    {
                        MJDEFAULT->setIntegerForKey(key_last_update_id,update_id);
                        MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                    }
                }
                
                auto status_update = JSON_SAFE_CHECK(obj_update, "status", String) ? std::string(obj_update["status"].GetString()):"";

                if (status_update != "")
				{
                    if (status_update == "require")
					{
                        MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                    }
                    
                    MJDEFAULT->setBoolForKey(key_update_show, true);
                    
                    rapidjson::StringBuffer rj_buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(rj_buffer);
                    obj_update.Accept(writer);
                    auto json_string = std::string(rj_buffer.GetString());
                    
                    MJDEFAULT->setStringForKey(key_update_content, json_string);
                }
                
				MJDEFAULT->setStringForKey(key_ver_update_app, JSON_SAFE_CHECK(obj_update, "version_update", String) ? obj_update["version_update"].GetString() : "0");

				MJDEFAULT->setStringForKey(key_version_required, JSON_SAFE_CHECK(obj_update, "version_required", String) ? obj_update["version_required"].GetString() : "0");
            } 
			else 
			{
                MJDEFAULT->setStringForKey(key_ver_update_app, "0");
                MJDEFAULT->setStringForKey(key_version_required, "0");
                MJDEFAULT->setStringForKey(key_update_content, "");
            }
            
            auto time_popup = 0;
            auto id_popup = 0;
            
            if (JSON_SAFE_CHECK(map_response, "show_pay", Bool))
			{
                MJDEFAULT->setBoolForKey("key.default.show.payment", map_response["show_pay"].GetBool());
            }
            
            if (JSON_SAFE_CHECK(map_response,"pay_use_url",String))
            {
                MJDEFAULT->setStringForKey("pay_use_url", map_response["pay_use_url"].GetString());
            }
            
            if (JSON_SAFE_CHECK(map_response, "domain_download", Object))
			{
                auto block = map_response["domain_download"].GetObject();

                if (JSON_SAFE_CHECK(block, "story", String))
				{
                    MJDEFAULT->setStringForKey(key_alternative_domain_story, block["story"].GetString());
                }
                
                if (JSON_SAFE_CHECK(block, "audio_book", String))
				{
                    MJDEFAULT->setStringForKey(key_alternative_domain_audio, block["audio_book"].GetString());
                }
                
                if (JSON_SAFE_CHECK(block, "lesson", String))
				{
                    MJDEFAULT->setStringForKey(key_alternative_domain_lesson, block["lesson"].GetString());
                }
            }
            
            if (JSON_SAFE_CHECK(map_response, "notify", Array))
            {
                std::vector<mj::model::NotificationInfo> notifications;
                auto noti_info = map_response["notify"].GetArray();
                for (auto i = 0; i < noti_info.Size(); i++)
                {
                    if (noti_info[i].IsObject())
                    {
                        
                        auto noti_o = noti_info[i].GetObject();
                        
                        if (JSON_SAFE_CHECK(noti_o, "popup", Int))
                        {
                            if (noti_o["popup"].GetInt() == 1) {
                                if (JSON_SAFE_CHECK(noti_o, "created", String)) {
                                    auto created = atoi(noti_o["created"].GetString());
                                    if (created > time_popup) {
                                        time_popup = created;
                                        id_popup = atoi(noti_o["id"].GetString());
                                    }
                                }
                            }
                        }
                        auto notification = mj::model::NotificationInfo();
                        notification.notification_id = notification.time_created = 0;
                        //notification.message = notification.url = notification.title = "";
                        
                        if (JSON_SAFE_CHECK(noti_o, "id", String)) {
                            notification.notification_id = ATOI(noti_o["id"].GetString());
                        }
                        
                        if (JSON_SAFE_CHECK(noti_o, "id", Int)) {
                            notification.notification_id = noti_o["id"].GetInt();
                        }
                        
                        if (JSON_SAFE_CHECK(noti_o, "title", Object))
                        {
                            std::vector<std::string> languages = {"en","vi","th"};
                            
                            for(auto lang : languages)
                            {
                                if(JSON_SAFE_CHECK(noti_o["title"], lang.c_str(), String))
                                {
                                    notification.m_titles.insert({lang.c_str(), noti_o["title"][lang.c_str()].GetString() });
                                }
                            }

                            //notification.title = noti_o["title"].GetString();
                        }
                        
                        if (JSON_SAFE_CHECK(noti_o, "message", Object))
                        {
                            std::vector<std::string> languages = {"en","vi","th"};
                            
                            for(auto lang : languages)
                            {
                                if(JSON_SAFE_CHECK(noti_o["message"], lang.c_str(), String))
                                {
                                    notification.m_messages.insert({lang.c_str(), noti_o["message"][lang.c_str()].GetString() });
                                }
                            }
                            
                           //notification.message = noti_o["message"].GetString();
                        }
                        
                        if (JSON_SAFE_CHECK(noti_o, "url", String)) {
                           notification.url = noti_o["url"].GetString();
                        }
                        
                        if (JSON_SAFE_CHECK(noti_o, "created", String)) {
                            notification.time_created = ATOI(noti_o["created"].GetString());
                        }

						if (JSON_SAFE_CHECK(noti_o, "created", Int)) {
							notification.time_created = noti_o["created"].GetInt();
						}

                        notifications.push_back(notification);
                    }
                }
                
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([notifications]
                {
                    StoryDataModel::getInstance()->addMessageFromServer(notifications);
                    
                    auto unread_message = 0;
                    
                    for (auto notification : notifications)
                    {
                        if (XHSLib->checkMessageUnRead(notification.notification_id)) {
                            ++unread_message;
                        }
                    }

                    if (unread_message > 0)
                    {
                        EventCustom e("mjstory.shownoti");
                        int * edata = new int;
                        *edata = unread_message;
                        e.setUserData(edata);
                        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
                        delete edata;
                    }
                });
            }
            
            if (id_popup != 0) {
                if (!MJDEFAULT->getBoolForKey(StringUtils::format("%s%d", key_popup_showed, id_popup), false)) {
                    MJDEFAULT->setIntegerForKey(key_popup_id_to_show, id_popup);
                } else {
                    MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
                }
            }
            
            //license_listdevice_active
//            this->updateDeviceInfoLicense(map_response);
            

            Director::getInstance()->getScheduler()->schedule([](float) {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_process_refresh_api_done);
                
            }, Application::getInstance(), 0, 0, 0.1, false, "delay_refresh_api_done");
            
			
            auto that = Director::getInstance()->getRunningScene();
            
			that->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create("preppare_data_done", [this](EventCustom * e)
            {
				Director::getInstance()->getEventDispatcher()->removeCustomEventListeners("preppare_data_done");
			
                if (_callback)
                {
					_callback(true, "");
				}
				this->autorelease();
				//                this->autorelease();
			}), that);

            if (JSON_SAFE_CHECK(map_response, "userInfo", Object))
            {
                if (!JSON_SAFE_CHECK(map_response["userInfo"], "password", Bool) || (JSON_SAFE_CHECK(map_response["userInfo"], "password", Bool) && !map_response["userInfo"]["password"].GetBool()))
                {
                    MJDEFAULT->setBoolForKey("need_to_fill_password", true);
                }
                else
                {
                    MJDEFAULT->setBoolForKey("need_to_fill_password", false);
                }
                
                if (JSON_SAFE_CHECK(map_response["userInfo"], "show_rating", Bool))
                {
                    RatingManager::GetInstance()->setRatingConditionFromServer(map_response["userInfo"]["show_rating"].GetBool());
                }
                
                auto id = StringUtils::toString(map_response["userInfo"]["users_id"].GetInt());
                MJDEFAULT->setIntegerForKey(key_user_id, map_response["userInfo"]["users_id"].GetInt());
                auto name = (map_response["userInfo"]["name"].IsString()) ? map_response["userInfo"]["name"].GetString() : "";
                auto mail = (map_response["userInfo"]["email"].IsString()) ? map_response["userInfo"]["email"].GetString() : "";
                auto phone = (map_response["userInfo"]["phone"].IsString()) ? map_response["userInfo"]["phone"].GetString() : "";
                auto address = (map_response["userInfo"]["address"].IsString()) ? map_response["userInfo"]["address"].GetString() : "";
                auto is_verify = (map_response["userInfo"].HasMember("is_verify") && map_response["userInfo"]["is_verify"].IsBool()) ? map_response["userInfo"]["is_verify"].GetBool() : false;

				auto userName = JSON_SAFE_CHECK(map_response["userInfo"],"username_signin", String) ? map_response["userInfo"]["username_signin"].GetString():"";

                if (is_verify)
                {
                    MJDEFAULT->deleteValueForKey("is_skip_login");
                }
                
                ap::Database::getInstance().saveUserInfo(id, name, mail, phone, address, userName);
                
                auto max_profile = (map_response["userInfo"]["max_profile"].IsInt()) ? map_response["userInfo"]["max_profile"].GetInt() : 3;
                MJDEFAULT->setIntegerForKey(key_max_account_profile, max_profile);
                
                std::string info = (map_response["userInfo"].HasMember("config_item") && map_response["userInfo"]["config_item"].IsString()) ? map_response["userInfo"]["config_item"].GetString() : "";
                
                if (_skip_update_info)
                {
                    if (_callback)
                    {
                        _callback(true, "");
                    }
                }
                else
                {
                    mm::RealtimeDatabase::getInstance().isUserExist(map_response["userInfo"]["users_id"].GetInt(), [=](bool is_user_exist,std::string msg)
                    {
                        if (is_user_exist)
                        {
                            if (_callback)
                            {
                                _callback(true, "");
                                delete this;
                            }
                        }
                        else
                        {
                            if (info.length() < 4 || _skip_update_info)
                            {
                                if (_callback) {
                                    _callback(true, "");
                                }
                                this->autorelease();
                            }
                            else
                            {
                                _user_info_filename = mj::helper::getFilenameFromURL(info);
                                
                                auto fname = mj::helper::getFilenameFromURL(info);
                                auto fdirectory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
                                auto fpath = fdirectory + fname;
                                
                                if(FileUtils::getInstance()->isFileExist(fpath))
                                {
                                    mj::helper::unzipAsync(fpath, fdirectory, [=](bool success)
                                    {
                                        if (success)
                                        {
                                            cocos2d::FileUtils::getInstance()->removeFile(fpath);
                                            this->onDownloadAndUnzipDone();
                                        }
                                        else
                                        {
                                            CCLOG("unzip errror");
                                            
                                            if (_callback)
                                            {
                                                _callback(false, "");
                                            };
                                            
                                            this->autorelease();
                                        }
                                    });
                                }
                                else
                                {
                                    
                                    auto that = Director::getInstance()->getRunningScene();
                                    
                                    ms::utils::DownloadUnit unit;
                                    unit.url = info;
                                    unit.download_id = mj::helper::getFilenameFromURL(info);
                                    unit.storage_path = cocos2d::FileUtils::getInstance()->getWritablePath() + "mjstory/" + unit.download_id;
                                    unit.success_event = StringUtils::format("download_success_%s", fname.c_str());
                                    unit.failed_event = StringUtils::format("download_error_%s", fname.c_str());
                                    unit.progress_event = "";//StringUtils::format("download_progress_%s", fpath.c_str());
                                    
                                    CCLOG("LoadUpdate|unit.download_id :%s", unit.download_id.c_str());
                                    
                                    CCLOG("LoadUpdate|unit.storage_path :%s", unit.storage_path.c_str());
                                    
                                    that->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.success_event, [this, unit](EventCustom * e) {
                                        CCLOG("CALL %s", unit.success_event.c_str());
                                        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
                                        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
                                        auto fname = _user_info_filename;
                                        auto fdirectory = FileUtils::getInstance()->getWritablePath() + "mjstory/";
                                        auto fpath = fdirectory + _user_info_filename;
                                        mj::helper::unzipAsync(fpath, fdirectory, [=](bool success) {
                                            if (success) {
                                                cocos2d::FileUtils::getInstance()->removeFile(fpath);
                                                this->onDownloadAndUnzipDone();
                                            }
                                            else {
                                                CCLOG("unzip errror");
                                                if (_callback) {
                                                    _callback(false, "");
                                                };
                                                this->autorelease();
                                            }
                                        });
                                        //                        CCLOG("Download success: %s", unit.storage_path.c_str());
                                    }), that);
                                    CCLOG("add listener %s", unit.success_event.c_str());
                                    
                                    that->getEventDispatcher()->addEventListenerWithSceneGraphPriority(EventListenerCustom::create(unit.failed_event, [this, unit](EventCustom * e) {
                                        CCLOG("CALL %s", unit.failed_event.c_str());
                                        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.success_event);
                                        Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(unit.failed_event);
                                        
                                        StoreError::call(unit.url, unit.failed_event, "download_config_item", {});
                                        if (_callback) {
                                            _callback(false, "");
                                        };
                                        this->autorelease();
                                    }), that);
                                    CCLOG("add listener %s", unit.failed_event.c_str());
                                    
                                    ms::utils::Downloader::getInstance().enqueue(unit);
                                }
                            }
                        }
                    });
                }
            }
            
        }

		void LoadUpdate::onDownloadAndUnzipDone()
    {
			_user_info_filename = mj::helper::replace_string(_user_info_filename, ".zip", ".json");
			std::string str = cocos2d::FileUtils::getInstance()->getStringFromFile(_user_info_filename);
        
            CCLOG("LoadUpdate|jsonProfile %s",str.c_str());
        
			rapidjson::Document document;
			document.Parse<rapidjson::kParseDefaultFlags>(str.c_str());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
			rapidjson::StringBuffer rj_buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
			document.Accept(writer);
			std::cout << rj_buffer.GetString();
#endif
			if (document.HasParseError())
			{
				CCLOG("Parse user info failed");
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("preppare_data_done");
				return;
			}

			if (!document.IsObject())
			{
				CCLOG("User info not a object");
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("preppare_data_done");
				return;
			}
			auto data = document.GetObject();
			auto version = 0;
			if (data.HasMember("version_control") && data["version_control"].IsInt()) {
				version = data["version_control"].GetInt();
			}
			if (version >= MJDEFAULT->getIntegerForKey(profile_version_control, 0)) {
				if (data.HasMember("user_default") && data["user_default"].IsString()) {

					std::vector<std::string> key_string = {
							"mj_license_key"
					};

					std::vector<std::string> key_bool = {
							"mj_onboarding_part3", "mj_onboarding_part2", "key_is_licence","key_first_time_created_profile"
					};

					std::vector<std::string> key_integer = {
							"key_count_guide_story", "key_current_profile_id", "key_lang_display"
					};

					auto info = JsonDump::getInstance()->dumpValueMapFromString(data["user_default"].GetString());

					for (auto i : info) {
						for (auto s : key_string) {
							if (i.first == s) {
								MJDEFAULT->setStringForKey(s, i.second.asString());
								break;
							}
						}

						for (auto s : key_integer) {
							if (i.first == s) {
								MJDEFAULT->setIntegerForKey(s, i.second.asInt());
								break;
							}
						}

						for (auto s : key_bool) {
							if (i.first == s) {
					 	MJDEFAULT->setBoolForKey(s, i.second.asBool());
								break;
							}
						}
					}


					//
					//                    MJ_PLATFORM_CONFIG.setUserDefaults(data["user_default"].GetString());
				}

				auto current_profile_id = MJDEFAULT->getIntegerForKey(key_current_profile_id);

				if (data.HasMember("profile_list") && data["profile_list"].IsArray())
                {
					auto list = data["profile_list"].GetArray();
                    
					for (int i = 0; i < list.Size(); i++)
                    {
                        auto p = list[i].GetObject();
						
                        int profile_id = 0;
						
                        if (p.HasMember("profile_id") && p["profile_id"].IsInt())
                        {
							profile_id = p["profile_id"].GetInt();
						}
						//                        if(p.HasMember("avatar_path") && p["avatar_path"].IsString()){
						//                            auto profile_avatar = p["avatar_path"].GetString();
						//                            auto p = ap::Database::getInstance().getProfileById(profile_id);
						//                            p.avatar = profile_avatar;
						//                            ap::Database::getInstance().updateProfile(p);
						//                        }
						PROFILE_MANAGER.getInstance().switchProfile(profile_id);
						if (p.HasMember("profile_detail") && p["profile_detail"].IsObject()) {
							rapidjson::Value::MemberIterator M;
							const char *key, *value;
							for (M = p["profile_detail"].MemberBegin(); M != p["profile_detail"].MemberEnd(); M++) {
								key = M->name.GetString();
								value = M->value.GetString();
								if (key != NULL && value != NULL)
								{
									PROFILE_MANAGER.setStringForKey(key, value);
                                    CCLOG("LoadUpdate::onDownloadAndUnzipDone|profile_detail key:%s = value:%s", key, value);

								}
							}
						}

						//stories read
						if (p.HasMember("stories_read") && p["stories_read"].IsObject()) {
							rapidjson::Value::MemberIterator M;
							const char *key, *value;
							cocos2d::ValueVector vector;
							for (M = p["stories_read"].MemberBegin(); M != p["stories_read"].MemberEnd(); M++) {
								key = M->name.GetString();
								value = M->value.GetString();
								if (key != NULL && value != NULL)
								{
									cocos2d::ValueMap map;
									map["id_story_lang"] = stoi(key);
									map["read"] = 1;
									map["time_read"] = stoi(value);
									vector.push_back(cocos2d::Value(map));
                                    CCLOG("LoadUpdate::onDownloadAndUnzipDone|stories_read key:%s = value:%s", key, value);

								}
							}

							PROFILE_MANAGER.saveReadStories(vector);
						}
						if (p.HasMember("audios_read") && p["audios_read"].IsObject()) {
							rapidjson::Value::MemberIterator M;
							const char *key, *value;
							cocos2d::ValueVector vector;
							for (M = p["audios_read"].MemberBegin(); M != p["audios_read"].MemberEnd(); M++) {
								key = M->name.GetString();
								value = M->value.GetString();
								if (key != NULL && value != NULL)
								{
									cocos2d::ValueMap map;
									map["audio_id"] = stoi(key);
									map["read"] = 1;
									map["time_read"] = stoi(value);
									vector.push_back(cocos2d::Value(map));
									CCLOG("LoadUpdate::onDownloadAndUnzipDone|audios_read key:%s = value:%s", key, value);

								}
							}

							PROFILE_MANAGER.saveReadAudioBooks(vector);
						}
						if (p.HasMember("lesson_learned") && p["lesson_learned"].IsObject()) {
							rapidjson::Value::MemberIterator M;
							const char *key, *value;
							cocos2d::ValueVector vector;
							for (M = p["lesson_learned"].MemberBegin(); M != p["lesson_learned"].MemberEnd(); M++) {
								key = M->name.GetString();
								value = M->value.GetString();
								if (key != NULL && value != NULL)
								{
									cocos2d::ValueMap map;
									map["_id"] = stoi(key);
									map["category_id"] = stoi(value);
									map["learned"] = 1;
									vector.push_back(cocos2d::Value(map));
									CCLOG("LoadUpdate::onDownloadAndUnzipDone|lesson_learned key:%s = value:%s", key, value);
								}
							}
							PROFILE_MANAGER.saveLessonLearned(vector);
						}
					}

					PROFILE_MANAGER.getInstance().switchProfile(current_profile_id);
				}
			}

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			if (!CONFIG_MANAGER->isVietnameseStories()) {
				//auto str_version = MJDEFAULT->getStringForKey(kstoryversion);
				if (MJDEFAULT->getBoolForKey("key_remove_phonics_tracking", false))
				{
					PROFILE_MANAGER.removeTrackingPhonics();
				}
			}
//#endif
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("preppare_data_done");
        }
    }
}
