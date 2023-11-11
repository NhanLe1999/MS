//
//  MJAPIZip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_AppInfo_Zip.h"

#include "MJDefinitions.h"
#include "MJDefault.h"
#include "../external/json/writer.h"
#include "MJHelper.h"
#include "MJAPIGetLinkDownload.h"
#include "StoryConfigManager.h"
#include "MJDefinitions.h"
#include "MSServerManager.h"
#include "MSGradeManager.h"
#include "MJPay.h"

#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"
#include <fstream>

#include "manager/RatingManager.h"
#include "manager/GettingUserInfoManager.h"

#include "MSLessonManager.h"
#include "LTDate.h"
#include "manager/CleverTapManager.h"
#include "MJAPI_DecodeObjectJsonInfo.h"

#define DAYS_BETWEEN_CALL 2

namespace mj { namespace api {
    bool AppInfoZip::shouldBeCalled() {
        return true; // FIXME: tạm thời disable tính năng này
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
        return true;
#endif
        if (CONFIG_MANAGER->isProductEnvironment() || CONFIG_MANAGER->isDevEnvironment()) {
            return true;
        }
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        return true;
#endif
        auto last_time_called = MJDEFAULT->getIntegerForKey(key_api_appinfo_called, 0);
        auto current_time = int(utils::getTimeInMilliseconds() / 1000);
        auto seconds_between_call = DAYS_BETWEEN_CALL * 24 * 3600;
        if (current_time - last_time_called >= seconds_between_call) {
            return true;
        } else {
            auto date = LTDate(current_time);
            auto last_date = LTDate(last_time_called);
            if (date.weekday == 0 /* sunday */ || date.weekday == 5 /* friday */ || date.weekday == 6 /* sunday */) {
                if (last_date.day == date.day && last_date.month == date.month && last_date.year == date.year) {
                    //called in same day
                    return false;
                }
            }
        }
        return true;
    }
    void AppInfoZip::call(std::function<void(bool, int, int, int)> callback) {
        if (!shouldBeCalled()) {
            if (callback) {
                auto v_client_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0);
                auto v_client_story = MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0);
                auto v_client_material = MJDEFAULT->getIntegerForKey(key_api_version_material_local, 0);
                callback(true, v_client_audio, v_client_story, v_client_material);
            }
            return;
        }
        auto refresh_call = new AppInfoZip();
        refresh_call->_callback = callback;
        refresh_call->send();
    }

    AppInfoZip::AppInfoZip() {
        _request = new mj::Request(mj::Request::ActionType::APP_INFO_V2, {
            { "version_game_number",StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_all_games_local, 0)) },
            { "version_lesson_number", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_all_lessons_version_local, 0)) },
            { "api_ver_audio_book", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0))},
            { "api_ver_story", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0))},
            { "api_ver_worksheet", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_material_local, 0))}
        });
        _request->setDelegate(this);
    }
    
    void AppInfoZip::send() {
        if (_request) {
            _request->send();
        }
    }
    
    int AppInfoZip::onRequestRaw(int tag, std::string data) {
        CCLOG("AppInfoZip::onRequestRaw ");

        auto unzip_json = mj::helper::unzip_json(data);
        if (!unzip_json) {
            _callback(false, 0, 0, 0);
            return -1;
        }
        rapidjson::Document doc;
        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char *)unzip_json);
        
        if (doc.HasParseError()) {
            if (_callback) {
                _callback(false, 0, 0, 0);
            }
            return -1;
        }
        
        if (!doc.IsObject()) {
            _callback(false, 0, 0, 0);
            return -1;
        }
        
        auto database_version = JSON_SAFE_CHECK(doc, "database_version", Int) ? doc["database_version"].GetInt() : 0;
        if (database_version > MJDEFAULT->getIntegerForKey("key_current_database_version", 0)) {
            MJDEFAULT->setIntegerForKey("key_current_database_version", database_version);
            mj::helper::resetDatabase();
            call(_callback);
            return 1;
        }
        
        int v_audio = -1;
        int v_story = -1;
        int v_material = -1;
        auto map_response = doc.GetObject();
        if (JSON_SAFE_CHECK(map_response, "list_api_use", Object)) {
            auto o = map_response["list_api_use"].GetObject();
            if (JSON_SAFE_CHECK(o, "api_ver_audio_book", Int)) {
                v_audio = o["api_ver_audio_book"].GetInt();
                MJDEFAULT->setIntegerForKey(key_api_version_audio_v2_fromServer, v_audio);
            }
            if (JSON_SAFE_CHECK(o, "api_ver_story", Int)) {
                v_story = o["api_ver_story"].GetInt();
                MJDEFAULT->setIntegerForKey(key_api_version_story_v2_fromJsonServer, v_audio);
            }
            if (JSON_SAFE_CHECK(o, "api_ver_worksheet", Int)) {
                v_material = o["api_ver_worksheet"].GetInt();
                MJDEFAULT->setIntegerForKey(key_api_version_material_fromServer, v_audio);

            }
        }
        
        if (JSON_SAFE_CHECK(map_response, "common_resource", Object)) {
            auto o = map_response["common_resource"].GetObject();
            auto version_common = o["version_common"].GetInt();	
            auto path_common = o["path_common"].GetString();
//         auto path_common = "http://datav2.daybehoc.com/platform/uploads/common-resource/COMMON_RESOURCE_51.zip";
            MJDEFAULT->setIntegerForKey(key_common_res_version_request, version_common);
            MJDEFAULT->setStringForKey(key_common_res_path, path_common);
            
        }
		
		if (JSON_SAFE_CHECK(map_response, "is_submit", Bool)) {
            auto is_submit = map_response["is_submit"].GetBool();
			MJDEFAULT->setBoolForKey(key_is_submit, is_submit);
		}

		if (JSON_SAFE_CHECK(map_response, "show_feature_license", Bool))
		{
			MJDEFAULT->setBoolForKey("is_license_feature_enable", map_response["show_feature_license"].GetBool());
		}

		if (JSON_SAFE_CHECK(map_response, "is_skip", Bool)) {
			MJDEFAULT->setBoolForKey(key_skip, map_response["is_skip"].GetBool());
		}
		else {
			MJDEFAULT->setBoolForKey(key_skip, false);
		}
        if (JSON_SAFE_CHECK(map_response, "feature_phonics", Bool)) {
            bool is_feature_phonics = map_response["feature_phonics"].GetBool();
            MJDEFAULT->setBoolForKey(key_feature_phonics, is_feature_phonics);
        }
        if (JSON_SAFE_CHECK(map_response, "feature_worksheet", Bool)) {
            bool is_feature_worksheet = map_response["feature_worksheet"].GetBool();
            MJDEFAULT->setBoolForKey(key_feature_worksheet, true);
        }
        if (JSON_SAFE_CHECK(map_response, "is_vn", Bool)) {
            auto isServerVN = map_response["is_vn"].GetBool();
            MJDEFAULT->setBoolForKey(key_serverVN, isServerVN);
        }
        
        if (JSON_SAFE_CHECK(map_response, "is_licence", Bool)) {
            MJDEFAULT->setBoolForKey(key_is_licence, map_response["is_licence"].GetBool());
        }
        
        if (JSON_SAFE_CHECK(map_response, "app_ver", Int)) {
            MJDEFAULT->setStringForKey(key_version_api_app, StringUtils::toString(map_response["app_ver"].GetInt()));
        }
        
        if (JSON_SAFE_CHECK(map_response, "country_code", String)) {
            MJDEFAULT->setStringForKey(key_country_code, map_response["country_code"].GetString());
        }
        
        if (CONFIG_MANAGER->isDevEnvironment() || CONFIG_MANAGER->isProductEnvironment()) {
            
        } else {
            if (JSON_SAFE_CHECK(map_response, "use_url", String)) {
                std::vector<std::string> hosts;
                hosts.push_back(map_response["use_url"].GetString());
                MJDEFAULT->setStringForKey("use_url", hosts[0]);
                mj::Request::setHost(hosts);
            }
        }
        
        if (JSON_SAFE_CHECK(map_response, "pay_use_url", String)) {
            mj::Request::_onepay_url = map_response["pay_use_url"].GetString();
            //MJDEFAULT->setStringForKey("pay_use_url", mj::Request::_onepay_url);
        }
        
        if (JSON_SAFE_CHECK(map_response, "coupon_code", String)) {
            MJDEFAULT->setStringForKey(keyCoupon, map_response["coupon_code"].GetString());
        }
        
        if (JSON_SAFE_CHECK(map_response, "group_server", Array)) {
            auto server_ids = map_response["group_server"].GetArray();
            std::vector<int> ids_arr;
            for (auto i = 0; i < server_ids.Size(); i++) {
                ids_arr.push_back(server_ids[i].GetInt());
            }
            //            mj::GetLinkDownloadAPI::_server_ids = ids_arr;
            ms::ServerManager::getInstance().saveServerIds(ids_arr);
        }
        
        if (map_response.HasMember("version_update") && map_response["version_update"].IsString()) {
            auto ver_update_app = map_response["version_update"].GetString();
            MJDEFAULT->setStringForKey(key_ver_update_app, ver_update_app);
        }
        
        if (map_response.HasMember("version_required") && map_response["version_required"].IsString()) {
            auto version_required = map_response["version_required"].GetString();
            MJDEFAULT->setStringForKey(key_version_required, version_required);
        }
        if (JSON_SAFE_CHECK(map_response, "game_list", Object)) {
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeGameListData(tag, map_response["game_list"].GetObject());
        }
        
        if (JSON_SAFE_CHECK(map_response, "product_list", Array)) {
            rapidjson::Value list = map_response["product_list"].GetArray();
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            list.Accept(writer);
            auto json_string = std::string(buffer.GetString());
            MJDEFAULT->setStringForKey(keyPriceInfo, json_string);
            mj::Pay::getInstance().refresh(json_string);
        }
        else {
            mj::Pay::getInstance().refresh();
        }

		if (JSON_SAFE_CHECK(map_response, "redirect_uri", String))
		{
			MJDEFAULT->setStringForKey("redirect_uri", map_response["redirect_uri"].GetString());
		}
        
        if (JSON_SAFE_CHECK(map_response, "categories_list", Array)) {
            auto list = map_response["categories_list"].GetArray();
            std::vector<mj::model::GradeInfo> grades;
            for (auto i = 0; i < list.Size(); ++i) {
                if (list[i].IsObject()) {
                    auto block = list[i].GetObject();
                    
                    mj::model::GradeInfo info;
                    info.grade_id = JSON_SAFE_CHECK(block, "id", Int) ? block["id"].GetInt() : info.grade_id;
                    info.name = JSON_SAFE_CHECK(block, "name", String) ? block["name"].GetString() : info.name;
                    info.lang_id = JSON_SAFE_CHECK(block, "language_id", Int) ? block["language_id"].GetInt() : info.lang_id;
                    
                    grades.push_back(info);
                }
            }
            StoryDataModel::getInstance()->insertNewGrades(grades);
            ms::GradeManager::getInstance().updateGrades();
        }

        if (JSON_SAFE_CHECK(map_response, "lesson_list", Object))
        {
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLessonListData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Default, map_response["lesson_list"].GetObject());
        }
		if (JSON_SAFE_CHECK(map_response, "location", Object))
		{
            auto object = map_response["location"].GetObject();
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLocationData(tag, object);
		}

        if (JSON_SAFE_CHECK(map_response, "countryCodeHere", String))//get current code of the country => for show flag and phone code.
		{
            MJDEFAULT->setStringForKey("countryCodeHere", map_response["countryCodeHere"].GetString());
        }

        if (JSON_SAFE_CHECK(map_response, "push_error_firebase", Bool)) {
            MJDEFAULT->setBoolForKey(key_push_event_crash_to_fibase, map_response["push_error_firebase"].GetBool());
        }

		RatingManager::GetInstance()->parseProblems((char *)unzip_json);
		
		GettingUserInfoManager::GetInstance()->parsePopupTitlesFromSever((char *)unzip_json);
		
        if (_callback) {
            _callback(true, v_audio, v_story, v_material);
        }
        
        return 1;
    }
    
    void AppInfoZip::onRequestSuccess(int tag, rapidjson::Value data) {
        delete this;
    }
    
    void AppInfoZip::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        MJDEFAULT->setStringForKey(key_reason_error, message);
        if (_callback) {
            _callback(false, -1, -1, -1);
        }
        delete this;
    }
}}
