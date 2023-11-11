//
//  MJAPIRefresh.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 8/4/17.
//

#include "MJAPIRefresh.h"
#include "MJRequest.h"
#include "MJDefault.h"
#include "MJPay.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "StoryFreeManager.h"
#include "MJStoryInfoModel.h"
#include "StoryDataModel.h"
#include "HSLib.h"
#include "../external/json/writer.h"

namespace mj {
    void RefreshAPI::call(std::function<void(bool)> callback) {
        auto refresh_call = new RefreshAPI();
        refresh_call->_callback = callback;
        refresh_call->send();
    }

    RefreshAPI::RefreshAPI() {
        _request = new mj::Request(mj::Request::ActionType::LOAD_UPDATE);
        _request->setDelegate(this);
    }

    void RefreshAPI::send() {
        if (_request) {
            _request->send();
        }
    }

    void RefreshAPI::onRequestSuccess(int tag, rapidjson::Value data) {
        auto map_response = data.GetObject();

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

        bool isActive = map_response["is_active"].GetBool();
        if (!isActive) {
            if (map_response.HasMember("free_time_use")) {
                MJDEFAULT->setIntegerForKey(key_freetime_expire, map_response["free_time_use"].GetInt());
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
        }

        std::string license = "";
        if (map_response.HasMember("licence") && map_response["licence"].IsString()) {
            license = map_response["licence"].GetString();
        }
        auto time_expire = 0;
        if (map_response.HasMember("time_expire") && map_response["time_expire"].IsInt()) {
            time_expire = map_response["time_expire"].GetInt();
        }

        if (time_expire == 0 && isActive) {
            time_expire = -1;
        }
        mj::Pay::getInstance().forceOverrideLicense(license, time_expire);

        if (map_response.HasMember("is_licence")) {
            MJDEFAULT->setBoolForKey(key_is_licence, map_response["is_licence"].GetBool());
        }

        if (map_response.HasMember("country_code")) {
            MJDEFAULT->setStringForKey(key_country_code, map_response["country_code"].GetString());
        }

        if (map_response.HasMember("version_update") && map_response["version_update"].IsString()) {
            auto ver_update_app = map_response["version_update"].GetString();
            MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
        }
        
        if (map_response.HasMember("version_required") && map_response["version_required"].IsString()) {
            auto version_required = map_response["version_required"].GetString();
            MJDEFAULT->setStringForKey(key_version_required, version_required);
        }
        
        if (map_response.HasMember("update") && map_response["update"].IsObject()) {
            rapidjson::Value obj_update = map_response["update"].GetObject();

            auto status_update = std::string(obj_update["status"].GetString());
            if (status_update != "") {
                if (status_update == "require") {
                    MJDEFAULT->setIntegerForKey(key_time_update_skip, 0);
                }

                MJDEFAULT->setBoolForKey(key_update_show, true);

                rapidjson::StringBuffer rj_buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(rj_buffer);
                obj_update.Accept(writer);
                auto json_string = std::string(rj_buffer.GetString());

                MJDEFAULT->setStringForKey(key_update_content, json_string);
            }
            
            if (obj_update.HasMember("version_update") && obj_update["version_update"].IsString()) {
                auto ver_update_app = obj_update["version_update"].GetString();
                MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
            } else
                MJDEFAULT->setStringForKey(key_ver_update_app, "0");
            
            if (obj_update.HasMember("version_required") && obj_update["version_required"].IsString()) {
                auto version_required = obj_update["version_required"].GetString();
                MJDEFAULT->setStringForKey(key_version_required, version_required);
            }   else
                MJDEFAULT->setStringForKey(key_version_required, "0");
        }   else {
            MJDEFAULT->setStringForKey(key_ver_update_app, "0");
            MJDEFAULT->setStringForKey(key_version_required, "0");
            MJDEFAULT->setStringForKey(key_update_content, "");
        }
        
        auto time_popup = 0;
        auto id_popup = 0;
        
        if (JSON_SAFE_CHECK(map_response, "show_pay", Bool)) {
            MJDEFAULT->setBoolForKey("key.default.show.payment", map_response["show_pay"].GetBool());
        }
        
        if (JSON_SAFE_CHECK(map_response, "domain_download", Object)) {
            auto block = map_response["domain_download"].GetObject();
            if (JSON_SAFE_CHECK(block, "story", String)) {
                MJDEFAULT->setStringForKey(key_alternative_domain_story, block["story"].GetString());
            }
            
            if (JSON_SAFE_CHECK(block, "audio_book", String)) {
                MJDEFAULT->setStringForKey(key_alternative_domain_audio, block["audio_book"].GetString());
            }
            
            if (JSON_SAFE_CHECK(block, "lesson", String)) {
                MJDEFAULT->setStringForKey(key_alternative_domain_lesson, block["lesson"].GetString());
            }
        }

//        if (JSON_SAFE_CHECK(map_response, "notify", Array)) {
//            std::vector<mj::model::NotificationInfo> notifications;
//            auto noti_info = map_response["notify"].GetArray();
//            for (auto i = 0; i < noti_info.Size(); i++) {
//                if (noti_info[i].IsObject()) {
//                    auto noti_o = noti_info[i].GetObject();
//                    if (JSON_SAFE_CHECK(noti_o, "popup", Int)) {
//                        if (noti_o["popup"].GetInt() == 1) {
//                            if (JSON_SAFE_CHECK(noti_o, "created", String)) {
//                                auto created = atoi(noti_o["created"].GetString());
//                                if (created > time_popup) {
//                                    time_popup = created;
//                                    id_popup = atoi(noti_o["id"].GetString());
//                                }
//                            }
//                        }
//                    }
////                    auto notification = mj::model::NotificationInfo(noti_info[i].GetObject());
////                    notifications.push_back(notification);
//                }
//            }
//
//            Director::getInstance()->getScheduler()->performFunctionInCocosThread([notifications] {
//                StoryDataModel::getInstance()->addMessageFromServer(notifications);
//                auto unread_message = 0;
//                for (auto notification : notifications) {
//                    if (XHSLib->checkMessageUnRead(notification.notification_id)) {
//                        ++unread_message;
//                    }
//                }
//
//                if (unread_message > 0) {
//                    EventCustom e("mjstory.shownoti");
//                    int * edata = new int;
//                    *edata = unread_message;
//                    e.setUserData(edata);
//                    Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
//                    delete edata;
//                }
//            });
//        }
//
//        if (id_popup != 0) {
//            if (!MJDEFAULT->getBoolForKey(StringUtils::format("%s%d", key_popup_showed, id_popup), false)) {
//                MJDEFAULT->setIntegerForKey(key_popup_id_to_show, id_popup);
//            } else {
//                MJDEFAULT->setIntegerForKey(key_popup_id_to_show, 0);
//            }
//        }
        
        //license_listdevice_active
//        this->updateDeviceInfoLicense(map_response);
        
        if (_callback) {
            _callback(true);
        }

        Director::getInstance()->getScheduler()->schedule([](float) {
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_process_refresh_api_done);
        }, Application::getInstance(), 0, 0, 0.1, false, "delay_refresh_api_done");

        delete this;
    }

    void RefreshAPI::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        mj::Pay::getInstance().updateActiveStatus(false);
        if (_callback) {
            _callback(false);
        }
        delete this;
    }
    
    void RefreshAPI::updateDeviceInfoLicense(rapidjson::Value response_data){
        if (response_data.HasMember("products") && response_data["products"].IsArray()) {
            std::vector<license::Package> packages;
            std::vector<std::tuple<std::string, int, int>> relationship;
            
            auto list_licenses = response_data["products"].GetArray();
            for (auto it = list_licenses.Begin(); it != list_licenses.End(); ++it) {
                if (!it->IsObject()) {
                    continue;
                }
                license::Package package;
                auto license_data = it->GetObject();
                
                if (JSON_SAFE_CHECK(license_data, "licence", String)) {
                    package.license = license_data["licence"].GetString();
                }
                
                if (JSON_SAFE_CHECK(license_data, "product_id", String)) {
                    package.product_id = license_data["product_id"].GetString();
                }
                
                if (JSON_SAFE_CHECK(license_data, "email_address", String)) {
                    package.mail = license_data["email_address"].GetString();
                }
                
                if (JSON_SAFE_CHECK(license_data, "phone", String)) {
                    package.phone = license_data["phone"].GetString();
                }
                
                if (JSON_SAFE_CHECK(license_data, "product_name", String)) {
                    package.name = license_data["product_name"].GetString();
                }
                
                if (JSON_SAFE_CHECK(license_data, "licence_time_active", Int)) {
                    package.time_enter = license_data["licence_time_active"].GetInt();
                }
                
                if (JSON_SAFE_CHECK(license_data, "device_time_active", Int)) {
                    package.time_effective = license_data["device_time_active"].GetInt();
                }
                
                if (JSON_SAFE_CHECK(license_data, "time_expire", Int)) {
                    package.time_expiry = license_data["time_expire"].GetInt();
                }
                
                if (JSON_SAFE_CHECK(license_data, "time_expire", String)) {
                    package.time_expiry = std::atoi(license_data["time_expire"].GetString());
                }
                
                if (JSON_SAFE_CHECK(license_data, "total_active", String)) {
                    auto ss = license_data["total_active"].GetString();
                    auto split = mj::helper::split(ss, '/');
                    if (split.size() == 2) {
                        package.total_active = std::atoi(split[1].c_str());
                        package.current_active = std::atoi(split[0].c_str());
                    } else {
                        package.total_active = 0;
                        package.current_active = 0;
                    }
                }
                
                if (package.license.empty()) {
                    continue;
                }
                packages.push_back(package);
                
                if (JSON_SAFE_CHECK(license_data, "list_device", Object)) {
                    std::vector<license::LicenseDeviceInfo> devices;
                    auto list_devices = license_data["list_device"].GetObject();
                    if (JSON_SAFE_CHECK(list_devices, "list_active", Array)) {
                        auto list_devices_active = list_devices["list_active"].GetArray();
                        for (auto idevice = list_devices_active.Begin(); idevice != list_devices_active.End(); ++idevice) {
                            if (!idevice->IsObject()) continue;
                            
                            auto device = idevice->GetObject();
                            license::LicenseDeviceInfo device_info;
                            if (JSON_SAFE_CHECK(device, "device_id", Int)) {
                                device_info.device_id = device["device_id"].GetInt();
                            }
                            
                            if (JSON_SAFE_CHECK(device, "device_id", String)) {
                                device_info.device_id = std::atoi(device["device_id"].GetString());
                            }
                            
                            if (JSON_SAFE_CHECK(device, "active_date", Int)) {
                                device_info.active_date = device["active_date"].GetInt();
                            }
                            
                            if (JSON_SAFE_CHECK(device, "active_date", String)) {
                                device_info.active_date = std::atoi(device["active_date"].GetString());
                            }
                            
                            if (JSON_SAFE_CHECK(device, "device_info", String)) {
                                device_info.device_name = device["device_info"].GetString();
                            }
                            
                            device_info.device_status = 1;
                            
                            devices.push_back(device_info);
                            
                            relationship.push_back(std::make_tuple(package.license, device_info.device_id, 1));
                        }
                    }
                    
                    if (JSON_SAFE_CHECK(list_devices, "list_remove", Array)) {
                        auto list_devices_active = list_devices["list_remove"].GetArray();
                        for (auto idevice = list_devices_active.Begin(); idevice != list_devices_active.End(); ++idevice) {
                            if (!idevice->IsObject()) continue;
                            
                            auto device = idevice->GetObject();
                            license::LicenseDeviceInfo device_info;
                            if (JSON_SAFE_CHECK(device, "device_id", Int)) {
                                device_info.device_id = device["device_id"].GetInt();
                            }
                            
                            if (JSON_SAFE_CHECK(device, "device_id", String)) {
                                device_info.device_id = std::atoi(device["device_id"].GetString());
                            }
                            
                            if (JSON_SAFE_CHECK(device, "active_date", Int)) {
                                device_info.active_date = device["active_date"].GetInt();
                            }
                            
                            if (JSON_SAFE_CHECK(device, "time_remove", Int)) {
                                device_info.deactive_date = device["time_remove"].GetInt();
                            }
                            
                            if (JSON_SAFE_CHECK(device, "active_date", String)) {
                                device_info.active_date = std::atoi(device["active_date"].GetString());
                            }
                            
                            if (JSON_SAFE_CHECK(device, "time_remove", String)) {
                                device_info.deactive_date = std::atoi(device["time_remove"].GetString());
                            }
                            
                            if (JSON_SAFE_CHECK(device, "device_info", String)) {
                                device_info.device_name = device["device_info"].GetString();
                            }
                            
                            device_info.device_status = 0;
                            
                            devices.push_back(device_info);
                            
                            relationship.push_back(std::make_tuple(package.license, device_info.device_id, 0));
                        }
                    }
                    
                    Director::getInstance()->getScheduler()->schedule([devices](float) {
                        StoryDataModel::getInstance()->updateLicenseDeviceInfo(devices);
                    }, Application::getInstance(), 0, 0, 0.1, false, "delay_add_devices");
                }
            }
            Director::getInstance()->getScheduler()->schedule([packages, relationship](float) {
                StoryDataModel::getInstance()->addLicensePackages(packages);
                StoryDataModel::getInstance()->updateRelationshipLicenseDevice(relationship);
            }, Application::getInstance(), 0, 0, 0.1, false, "delay_add_license_package");

        }
        
        
        auto data = response_data.GetObject();
        std::string phone_info, mail_info, total_active;
        if (data.HasMember("phone") && data["phone"].IsString()) {
            phone_info = data["phone"].GetString();
        }
        
        if (data.HasMember("email") && data["email"].IsString()) {
            mail_info = data["email"].GetString();
        }
        if (data.HasMember("total_active") && data["total_active"].IsString()) {
            total_active = data["total_active"].GetString();
        }
        
        LICENSE_INFO_MODEL->setInfoVerify(phone_info, mail_info);
        std::vector<license::LicenseDeviceInfo> list_device_info;
        if (data.HasMember("device_in_licence") && data["device_in_licence"].IsObject()) {
            auto device_in_licence = data["device_in_licence"].GetObject();
            list_device_info.clear();
            if (device_in_licence.HasMember("list_active") && device_in_licence["list_active"].IsArray()) {
                auto list_active = device_in_licence["list_active"].GetArray();
                for (int i = 0; i < (int) list_active.Size(); i++) {
                    auto device_data = list_active[i].GetObject();
                    license::LicenseDeviceInfo device_info;
                    if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                        device_info.device_id = atoi(device_data["device_id"].GetString());
                    }
                    if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                        device_info.active_date = atoi(device_data["active_date"].GetString());
                    }
                    if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                        device_info.device_name = device_data["device_info"].GetString();
                    }
                    device_info.device_status = 1;
                    list_device_info.push_back(device_info);
                }
            }
            
            if (device_in_licence.HasMember("list_remove") && device_in_licence["list_remove"].IsArray()) {
                auto list_remove = device_in_licence["list_remove"].GetArray();
                for (int i = 0; i < (int) list_remove.Size(); i++) {
                    auto device_data = list_remove[i].GetObject();
                    license::LicenseDeviceInfo device_info;
                    if (device_data.HasMember("device_id") && device_data["device_id"].IsString()) {
                        device_info.device_id = atoi(device_data["device_id"].GetString());
                    }
                    
                    if (device_data.HasMember("active_date") && device_data["active_date"].IsString()) {
                        device_info.active_date = atoi(device_data["active_date"].GetString());
                    }
                    
                    if (device_data.HasMember("device_info") && device_data["device_info"].IsString()) {
                        device_info.device_name = device_data["device_info"].GetString();
                    }
                    
                    if (device_data.HasMember("time_remove") && device_data["time_remove"].IsString()) {
                        device_info.deactive_date = atoi(device_data["time_remove"].GetString());
                    }
                    device_info.device_status = 0;
                    list_device_info.push_back(device_info);
                }
            }
        }
        
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([list_device_info] {
            LICENSE_INFO_MODEL->updateLicenseDeviceInfo(list_device_info);
        });
        MJDEFAULT->setStringForKey("phone_info", phone_info);
        MJDEFAULT->setStringForKey("mail_info", mail_info);
        MJDEFAULT->setStringForKey("total_active", total_active);

    }
}


