//
//  MJAPIZip.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 4/19/18.
//

#include "MJAPI_AppInfo_Phonic_Zip.h"

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


#include "MSLessonManager.h"
#include "LTDate.h"
#include"MJAPI_DecodeObjectJsonInfo.h"
#define DAYS_BETWEEN_CALL 2

namespace mj { namespace api {
    bool AppInfoPhonicZip::shouldBeCalled() {
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
    void AppInfoPhonicZip::call(std::function<void(bool, int, int)> callback) {
        if (!shouldBeCalled()) {
            if (callback) {
                auto v_client_audio = MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0);
                auto v_client_story = MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0);
                callback(true, v_client_audio, v_client_story);
            }
            return;
        }
        auto refresh_call = new AppInfoPhonicZip();
        refresh_call->_callback = callback;
        refresh_call->send();
    }
    
    AppInfoPhonicZip::AppInfoPhonicZip() {
        _request = new mj::Request(mj::Request::ActionType::APP_INFO_PHONIC, {
            { "version_game_number",  "0" },
            { "version_lesson_number", "0" },
            { "api_ver_audio_book", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_audio_local, 0))},
            { "api_ver_story", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_api_version_story_local, 0))}
        });
        _request->setDelegate(this);
    }
    
    void AppInfoPhonicZip::send() {
        if (_request) {
            _request->send();
        }
    }
    
    int AppInfoPhonicZip::onRequestRaw(int tag, std::string data) {
        CCLOG("onRequestRaw::onRequestRaw ");

        auto unzip_json = mj::helper::unzip_json(data);
        if (!unzip_json) {
            return -1;
        }
        rapidjson::Document doc;
        doc.Parse<rapidjson::ParseFlag::kParseStopWhenDoneFlag>((char *)unzip_json);
        
        if (doc.HasParseError()) {
            return -1;
        }
        
        if (!doc.IsObject()) {
            return -1;
        } 
        MJDEFAULT->setIntegerForKey(key_api_appinfo_called, int (utils::getTimeInMilliseconds() / 1000));
        
        int v_audio = -1;
        int v_story = -1;
        auto map_response = doc.GetObject();
        mj::api::MJAPIDecodeObjectJsonInfo::onDecodeAppInfoData(&doc);
        
        if (JSON_SAFE_CHECK(map_response, "categories_list", Array)) {
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeCategoriesInfoData(1, map_response["categories_list"].GetObject());
        }
        
        if (JSON_SAFE_CHECK(map_response, "lesson_list", Object)) {
            mj::api::MJAPIDecodeObjectJsonInfo::onDecodeLessonListData(mj::api::MJAPIDecodeObjectJsonInfo::IsStateUpdateFistInstall::Default,map_response["lesson_list"].GetObject());
        }
        
        if (JSON_SAFE_CHECK(map_response, "current_lesson", Array)) {
            auto arr = map_response["current_lesson"].GetArray();
            std::vector<int> learned;
            for (auto i = 0; i < arr.Size(); ++i) {
                if (!arr[i].IsInt()) {
                    continue;
                }
                
                learned.push_back(arr[i].GetInt());
            }
            
            StoryDataModel::getInstance()->setLessonLearned(learned);
        }
        
        
        if (_callback) {
            _callback(true, v_audio, v_story);
        }
        
        return 1;
    }
    
    void AppInfoPhonicZip::onRequestSuccess(int tag, rapidjson::Value data) {
        delete this;
    }
    
    void AppInfoPhonicZip::onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) {
        if (_callback) {
            _callback(false, -1, -1);
        }
        delete this;
    }
}}
