//
//  MJRequest.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/25/17.
//
//

#include "MJRequest.h"
#include <string>
#include <type_traits>
#include <iostream>
#include "MJPlatformConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "StoryData.h"
#include "MJDefault.h"
#include "MJPay.h"
#include "StoryConfigManager.h"
#include "MJHelper.h"
#include "MJDebugView.h"
#include "APDatabase.h"
#include "MsNewRequest.h"
#include "APLoginView.h"
#include "APAuth.h"

#include "APStoreErrorAPI.h"

#define p_APP_ID "app_id"
#define p_DEVICE_ID "device_id"
#define p_OS "os"
#define p_DEVICE_INFO "info"
#define p_API_VERSION "app_ver"
#define p_DEVICE_TYPE "device_type"
#define p_LANGUAGE_ID "lang_id"
#define p_LANGUAGE_DISPLAY "lang"
#define p_SUBVER "subversion"
#define p_GLOBAL "test_us"

#define URL_PATH "/service/v1/"

#define v_APP_ID "40"
#define v_APP_ID_VN "51"
#define v_API_VER "1"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#define v_OS "ios"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define v_OS "android"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define v_OS "win32"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define v_OS "mac"
#endif

#define URL_DELIMITER "?"
#define ONEPAY_URL std::string("http://test.daybehoc.com/payment/onepay/index?")

#define DOMAIN_BEHOCCHU "https://www.monkeyuni.net"
#define DOMAIN_DAYBEHOC "https://daybehoc.com"
#define DOMAIN_TESTDAYBEHOC "https://dev.monkeyuni.net"

#define NEW_DOMAIN_DEV "https://api.dev.monkeyuni.net"
#define NEW_DOMAIN_LIVE "https://api.monkeyuni.net"
#define NEW_DOMAIN_PRODUCT "https://api.daybehoc.com"

std::string mj::Request::_onepay_url = "";
std::string urlencode(const std::string &c);
namespace mj {
    const std::map<int, std::string> Request::_action_map = {
            {mj::Request::ActionType::REGISTER_DEVICE, "location/register"},
            {mj::Request::ActionType::REGISTER_PUSH_TOKEN, "story/register"},
            {mj::Request::ActionType::ONEPAY, "payment/onepay/index"},
            {mj::Request::ActionType::REGISTER_COD, "pay/buyregister"},
            {mj::Request::ActionType::ACTIVE_LICENSE, "licence/active"},
            {mj::Request::ActionType::LOAD_UPDATE, "story/loadupdate"},
            {mj::Request::ActionType::INAPP_INFO, "pay/listpay"},
            {mj::Request::ActionType::NOTIFICATION, "story/listmessage"},
            {mj::Request::ActionType::HOME, "story/home"},
            {mj::Request::ActionType::APPINFO, "utils/appinfo"},
            {mj::Request::ActionType::VERIFY_RECEIPT, "pay/verifystore"},
            {mj::Request::ActionType::GET_STORY, "story/getstory"},
            {mj::Request::ActionType::VALIDATE_COUPON, "utils/validatecoupon"},
            {mj::Request::ActionType::RATE_STORY, "story/ratestory"},
            {mj::Request::ActionType::CATEGORY, "story/category"},
            {mj::Request::ActionType::ALL_STORIES, "story/level"},
            {mj::Request::ActionType::HOME_V2, "story/home2"},
            {mj::Request::ActionType::LEVEL_INFO, "story/getreadinglevel"},
            {mj::Request::ActionType::RATE_APP, "story/rateapp"},
//            {mj::Request::ActionType::SENT_DOWNLOAD, "story/senddata"},
            {mj::Request::ActionType::SENT_DOWNLOAD, "http://<new_platform>/api/v1/mobile/story/save-download"},
            {mj::Request::ActionType::SENT_FEEDBACK, "utils/feedback"},
            {mj::Request::ActionType::GET_AUDIOBOOK, "http://<new_platform>/api/v1/mobile/audiobook/info"},
            {mj::Request::ActionType::ALL_AUDIOBOOKS, "utils/audiobook"},
            {mj::Request::ActionType::ALL_LESSONS, "http://<new_platform>/api/v1/mobile/lesson/list"},
            {mj::Request::ActionType::LESSON_DETAILS, "http://<new_platform>//api/v2/mobile/activities/list"},
            {mj::Request::ActionType::ALL_GAMES, "http://<new_platform>/api/v1/mobile/game/list"},
            {mj::Request::ActionType::ALL_NEW_CATEGORIES, "http://<new_platform>/api/v1/mobile/categories/list"},
            {mj::Request::ActionType::TRACK_LESSON, "http://<new_platform>/api/v1/mobile/activities/store-tracking"},
            {mj::Request::ActionType::SAMPLE_STORIES, "http://<new_platform>/api/v1/mobile/categories/sample-story"},
            {mj::Request::ActionType::AUDIOBOOK_SERIES, "utils/audiobookdetail"},
            {mj::Request::ActionType::APP_INFO_V2, "http://<new_platform>/api/v2/mobile/utils/appinfov2"},
            {mj::Request::ActionType::APP_INFO_ZIP, "http://<new_platform>/api/v1/mobile/utils/appinfov2"},
            {mj::Request::ActionType::AUDIOBOOK, "http://<new_platform>//api/v1/audiobook/list"},
            {mj::Request::ActionType::AUDIOBOOK_ZIP, "http://<new_platform>/api/v1/mobile/audiobook/listv2"},
            {mj::Request::ActionType::AUDIOBOOK_DETAIL, "http://<new_platform>/api/v2/mobile/audiobook/detail"},
            {mj::Request::ActionType::STORY, "http://<new_platform>//api/v1/story/list"},
            {mj::Request::ActionType::STORY_ZIP, "http://<new_platform>/api/v1/mobile/story/listv2"}, 
            {mj::Request::ActionType::BOOK_LOG, "http://<new_platform>/api/v1/mobile/utils/story-book-log"},
            {mj::Request::ActionType::DOWNLOAD_LOG, "http://<new_platform>/api/request/pus_logs_error"},
            {mj::Request::ActionType::REMOVE_LICENSE, "http://<new_platform>/api/v1/mobile/license/remove-device"},
            {mj::Request::ActionType::FORGOT_LICENSE, "http://<new_platform>/api/v1/mobile/license/forgot"},
            {mj::Request::ActionType::CHANGE_LICENSE, "http://<new_platform>/api/v1/mobile/license/get-old-licence"},
            {mj::Request::ActionType::SAVE_MAIL, "http://<new_platform>/api/v1/mobile/device/registerinfo"},
            {mj::Request::ActionType::VERIFY_INFO_LICENSE, "http://<new_platform>/api/v1/mobile/license/verify-info-licence"},
            {mj::Request::ActionType::VERIFY_INFO_VIRAL, "http://<new_platform>/api/v1/mobile/viral/verify"},
            {mj::Request::ActionType::SHARE_VIRAL, "http://<new_platform>/viral.html"},
            {mj::Request::ActionType::SYNC_AUDIO, "http://<new_platform>/api/v1/mobile/audiobook/info"},
            {mj::Request::ActionType::STORY_DETAIL, "http://<new_platform>/api/v2/mobile/story/lesson-interactive"},
            {mj::Request::ActionType::MATERIAL_ZIP, "http://<new_platform>/api/v1/worksheet/list"},
            {mj::Request::ActionType::ACTIVE_LICENSE_V2, "http://<new_platform>/api/v1/mobile/account/licence-active"},
            {mj::Request::ActionType::REGISTER_INSTALL, "http://<new_platform>/api/v1/mobile/device/register-device-install"},
            {mj::Request::ActionType::APP_INFO_PHONIC, "http://<new_platform>/api/v1/mobile/utils/appinfov2"},
            {mj::Request::ActionType::PATH_INSTALL_ZIP, ""},
            {mj::Request::ActionType::LOAD_UPDATE_INFO, "http://<new_platform>/api/v2/account/load-update" },
            {mj::Request::ActionType::CATEGORIES_ZIP,"http://<new_platform>/api/v1/categories/list"},
            {mj::Request::ActionType::GAMELIST_ZIP, "http://<new_platform>/api/v1/game/list-game"},
            {mj::Request::ActionType::LESSON_LIST_ZIP, "http://<new_platform>/api/v1/lesson/list"},
            {mj::Request::ActionType::LESSON_INTERATIVE_ZIP, ""},
            {mj::Request::ActionType::ACTIVITIES_LIST_ZIP, ""},
    };

    std::vector<std::string> Request::_servers = {
    };

    std::once_flag mj_request_set_default_hosts;

    Request::Request(ActionType action, Data data, RequestType type, float timeout) {
        std::call_once(mj_request_set_default_hosts, [] {
            ms::newApi::Request::getInstance().setNewRequest(MJDEFAULT->getBoolForKey("new_lib_api", true));
            auto saved_host = MJDEFAULT->getStringForKey("use_url", "");
            if (!saved_host.empty()) {
                std::vector<std::string> hosts = {saved_host};
                Request::setHost(hosts);
            } else {
                auto host = std::string(DOMAIN_BEHOCCHU);
                if (CONFIG_MANAGER->isProductEnvironment()) {
                    host = DOMAIN_DAYBEHOC;
                } else if (CONFIG_MANAGER->isDevEnvironment()) {
                    host = DOMAIN_TESTDAYBEHOC;
                }
                Request::setHost({host});
            }
        });
        _delegate = nullptr;
        _server_id = 0;
        _action = action;
        _data = data;
        _request = nullptr;
        _tag = 0;
        _timeout = timeout;
        _timeout_key = "";

        switch (type) {
            case RequestType::GET:
                _request_type = cocos2d::network::HttpRequest::Type::GET;
                _typeRequest = "GET";
                break;

            case RequestType::POST:
                _request_type = cocos2d::network::HttpRequest::Type::POST;
                _typeRequest = "POST";
                break;

            default:
                break;
        }
        _url = buildURL();
    }
 

    Request::~Request() {
        CCLOG("Deleting request");
        _request->setResponseCallback(nullptr);
        _request->release();
        _request = nullptr;

        Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
    }

    void Request::setTag(int tag) {
        _tag = tag;
    }

    std::string Request::buildURL() {
        auto url = _action_map.at(_action);
        if (_request_type == cocos2d::network::HttpRequest::Type::GET) 
		{
            url += URL_DELIMITER;
            url += buildBaseParams();
            
            if (_action != ALL_STORIES && _action != HOME_V2) {
                addParam(url, p_LANGUAGE_ID, StringUtils::toString(MJDEFAULT->getIntegerForKey(key_lang_display, 1)));
            }
            
            for (auto pair : _data) {
                addParam(url, pair.first, pair.second);
            }

			_param = url;

			CCLOG("MJRequest::buildURL param= %s", url.c_str());

        }
		else 
		{
            //_data
            _data_post_str = buildBaseParams();
            for (auto pair : _data) 
			{
                addParam(_data_post_str, pair.first, pair.second);
            }
            CCLOG("MJRequest::buildURL _data_post_str = %s", _data_post_str.c_str());
        }

        return url;
    }

    void Request::setHeaders(std::vector<std::string> headers){
        auto timeSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        auto timeSinceEpochStr = std::to_string(timeSinceEpoch);
        char * buffer = new char[1000];
        base64Encode(reinterpret_cast<const unsigned char *>(timeSinceEpochStr.c_str()), (int)timeSinceEpochStr.length(), &buffer);
        std::string headers1 = StringUtils::format("token: %s", ap::Database::getInstance().getAccessToken().c_str());
        std::string headers2 = StringUtils::format("x-api-key: %s", buffer);
        _headers.push_back(headers1);
        _headers.push_back(headers2);
        if(ms::newApi::Request::getInstance().isNewRequest())
        {
            std::vector<std::string> listHeaders = {};
            listHeaders.push_back(headers1);
            listHeaders.push_back(headers2);
            ms::newApi::Request::getInstance().setListHeaders(listHeaders);
        }
#if TURN_ON_NEW_REQUEST
#endif
    }
    
    void Request::send() {

        _request = new cocos2d::network::HttpRequest();
        
        setHeaders({ });
        
        _request->setHeaders(_headers);
        for(auto h: _headers){
            CCLOG("%s:", h.c_str());
        }

        if(ms::newApi::Request::getInstance().isNewRequest())
        {
            ms::newApi::Request::getInstance().setTypeRequest(_typeRequest);
        }else{
            _request->setRequestType(_request_type);
                if (_tag > 0) {
                _request->setTag(cocos2d::StringUtils::toString(_tag));
            }
        }
        
#if TURN_ON_NEW_REQUEST
#else
#endif

        std::string url ;
        if (_url.find("http") == std::string::npos) {

            url = _servers[_server_id] + _url;
        }
        else
        {
            url =  _url;

        }
        if (_url.find("http://<new_platform>") != std::string::npos) {
            if (CONFIG_MANAGER->isProductEnvironment()) {
                url = mj::helper::replace_string(_url, "http://<new_platform>", NEW_DOMAIN_PRODUCT);
            } else if (CONFIG_MANAGER->isDevEnvironment()) {
                url = mj::helper::replace_string(_url, "http://<new_platform>", NEW_DOMAIN_DEV);
            } else {
                url = mj::helper::replace_string(_url, "http://<new_platform>", NEW_DOMAIN_LIVE);
            }
        }

        CCLOG("MJRequest|Sending request for url: %s", url.c_str());
        MJDEFAULT->setStringForKey(key_link_url_download, url);
        setCallbackSucess();
        setCallbackError();
        if(ms::newApi::Request::getInstance().isNewRequest())
        {
            ms::newApi::Request::getInstance().setUrl(url);
            ms::newApi::Request::getInstance().setCallBackRequest(_callbackSucess);
            ms::newApi::Request::getInstance().setCallBackRequestError();
            ms::newApi::Request::getInstance().send();
        }else{
            _request->setUrl(url);

            if (_request_type == cocos2d::network::HttpRequest::Type::POST) {
                _request->setRequestData(_data_post_str.data(), _data_post_str.size());
                CCLOG("MJRequest|POST param: %s", _data_post_str.c_str());
            }
            
            _request->setResponseCallback([=](cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response) {
                Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
                if (!response->isSucceed()) {
                    MJDEFAULT->setStringForKey(key_type_error, "Download");
                    rapidjson::Value a(rapidjson::kObjectType);
                    onRequestError(kmjRequestErrorNoResponse, "", a.GetObject());
                    return;
                }
                auto buffer = *response->getResponseData();
                auto buffer_string = std::string(buffer.begin(), buffer.end());

                if(_callbackSucess)
                {
                    _callbackSucess(buffer_string, true);
                }

                });
            cocos2d::network::HttpClient::getInstance()->send(_request);
            cocos2d::network::HttpClient::getInstance()->setTimeoutForRead(_timeout);

            Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
            _timeout_key = StringUtils::format("connection_timeout_%d", random(0, 999999));
            Director::getInstance()->getScheduler()->schedule([=](float) {
                this->onRequestTimeout();
            }, this, 0, 0, _timeout, false, _timeout_key);
        }
#if TURN_ON_NEW_REQUEST
#else
#endif
    }

    void Request::onRequestTimeout() {
        CCLOG("MJRequest|Request Timeout. %s %f", _action_map.at(_action).c_str(), _timeout);
        rapidjson::Value a(rapidjson::kObjectType);
        onRequestError(kmjRequestErrorTimeout, "", a.GetObject());
    }

    void Request::addParam(std::string &base, std::string key, std::string value) {
        base = base + std::string((base.empty() ? "" : "&")) + key + "=" + urlencode(value);
    }

    std::string Request::buildBaseParams() {
        std::string param = "";
		auto app_id = v_APP_ID;
		if (CONFIG_MANAGER->isVietnameseStories() || CONFIG_MANAGER->isTestActVmonkey()) {
			app_id = v_APP_ID_VN;
		}
#if TURN_ON_NEW_DOMAIN
        addParam(param, "new_story", "true");
#endif
        
		addParam(param, p_APP_ID, app_id);
        //addParam(param, p_APP_ID, ((CONFIG_MANAGER->isVietnameseStories()?v_APP_ID_VN:v_APP_ID) || (CONFIG_MANAGER->isTestActVmonkey()? v_APP_ID_VN:v_APP_ID)) );
        addParam(param, p_DEVICE_ID, MJ_PLATFORM_CONFIG.getDeviceId());
        addParam(param, p_OS, MJ_PLATFORM_CONFIG.getOS());
        addParam(param, p_DEVICE_INFO, MJ_PLATFORM_CONFIG.getDeviceInfoShort());
        addParam(param, p_SUBVER, MJ_PLATFORM_CONFIG.getAppVersion());
        addParam(param, p_API_VERSION, MJDEFAULT->getStringForKey(key_version_api_app, v_API_VER));
        auto info = MJ_PLATFORM_CONFIG.getPlatformDeviceID();
        char * buffer = new char[1000];
        base64Encode(reinterpret_cast<const unsigned char *>(info.c_str()), (int)info.length(), &buffer);
        addParam(param, "key_has", std::string(buffer));
        delete[] buffer;
        
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
        if (!CONFIG_MANAGER->isCountryVN()) {
            addParam(param, p_GLOBAL, "1");
        }
#endif
        
        // TODO: cung cấp thêm thông tin cho 2 trường này
        addParam(param, p_DEVICE_TYPE, StoryData::getInstance()->isHdr ? "4" : "2");
        auto lang_display_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
        auto lang_code = std::string("");
        if (lang_display_id == LANGUAGE_VN_CODE) {
            lang_code = "vi-VN";

        } else if (lang_display_id == LANGUAGE_THAI_CODE) {
            lang_code = "th-TH";
        } else if (lang_display_id == LANGUAGE_ID_CODE) {
            lang_code = "id-ID";
        } else {
            lang_code = "en-US";
        }

        addParam(param, p_LANGUAGE_DISPLAY, lang_code);
        
        auto test = std::string("");
        if(MJDEFAULT->getBoolForKey("debugkey_is_test_license", false)){
            test = "1";
        }else{
            test = "0";
        }
        addParam(param, "is_test", test);
        return param;
    }

    void Request::onRequestSuccess(rapidjson::Value data) {
        MJDEFAULT->deleteValueForKey(key_count_retry);
        if (_delegate) {
            if (data.IsObject())
                _delegate->onRequestSuccess(_tag, data.GetObject());
            else if (data.IsArray())
                _delegate->onRequestSuccess(_tag, data.GetArray());
            else
                _delegate->onRequestSuccess(_tag, rapidjson::Value(""));
        }

        this->release();
    }

    void Request::onRequestError(int error_code, std::string error_string, rapidjson::Value data) {
      if(_callbackError)
      {
          _callbackError(error_code, error_string, data);
      }
    }

    void Request::setHost(std::vector<std::string> hosts) {
        _servers.clear();
        for (auto host : hosts) {
            _servers.push_back(host + URL_PATH);
        }
    }

    std::string Request::getOnePayURL(std::string product_id) {
        auto base_param = mj::Request::buildBaseParams();
        mj::Request::addParam(base_param, "product_id", product_id);
        mj::Request::addParam(base_param, "coupon", mj::Pay::getInstance().getCoupon());
        mj::Request::addParam(base_param, "redirect_uri", "esonepay://pay");
        
        auto url = _onepay_url + "/" + _action_map.at(mj::Request::ActionType::ONEPAY) + URL_DELIMITER;
        url += base_param;

        return url;
    }
    
    std::string Request::getViralSharingURL(string name){
        std::string param = "";
        addParam(param, p_DEVICE_TYPE, StoryData::getInstance()->isHdr ? "4" : "2");
        addParam(param, p_APP_ID, CONFIG_MANAGER->isVietnameseStories()?v_APP_ID_VN:v_APP_ID);
        addParam(param, p_DEVICE_ID, MJ_PLATFORM_CONFIG.getDeviceId());
        addParam(param, "name", name);
        auto url = _action_map.at( mj::Request::ActionType::SHARE_VIRAL) + URL_DELIMITER;
        url += param;
        
        if (url.find("http://<new_platform>") != std::string::npos) {
            if (CONFIG_MANAGER->isProductEnvironment()) {
                url = mj::helper::replace_string(url, "http://<new_platform>", "http://test.monkeystories.vn");
            } else if (CONFIG_MANAGER->isDevEnvironment()) {
                url = mj::helper::replace_string(url, "http://<new_platform>", "http://dev.monkeystories.vn");
            } else {
                if(CONFIG_MANAGER->isCountryVN()){
                    url = mj::helper::replace_string(url, "http://<new_platform>", "http://monkeystories.vn");
                }else{
                    url = mj::helper::replace_string(url, "http://<new_platform>", "http://monkeystories.net");

                }
            }
        }
        return url;
    }
    
    std::string Request::getURL(){
        if (_url != "") {
            return _url;
        }
        return "";
    }

    void Request::setCallbackNewReadRequest(std::string responData, bool isSucess)
    {
        Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
                   if (isSucess) {
                       MJDEFAULT->setStringForKey(key_type_error, "Download");
                       rapidjson::Value a(rapidjson::kObjectType);
                       onRequestError(kmjRequestErrorNoResponse, "", a.GetObject());
                       return;
                   }
        
                    auto buffer_string = responData;


                       if (buffer_string.empty()) {
                           rapidjson::Value a(rapidjson::kObjectType);
                           MJDEFAULT->setStringForKey(key_type_error, "api");
                           onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                           return;
                       }
                       
                       if (_delegate) {
                           auto r = _delegate->onRequestRaw(_tag, buffer_string);
                           if (r == 1) {
                               this->onRequestSuccess(rapidjson::Value(""));
                               return;
                           }
                           
                           if (r == -1) {
                               MJDEFAULT->setStringForKey(key_type_error, "api");
                               rapidjson::Value a(rapidjson::kObjectType);
                               this->onRequestError(kmjRequestErrorExpectedData, "", a.GetObject());
                               return;
                           }
                       }
                       
                       rapidjson::Document doc;
                       doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
                       
                       if (doc.HasParseError()) {
                           MJDEFAULT->setStringForKey(key_type_error, "api");
                           rapidjson::Value a(rapidjson::kObjectType);
                           onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                           return;
                       }
                       
       #if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
                       rapidjson::StringBuffer rj_buffer;
                       rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
                       doc.Accept(writer);
                       std::cout << rj_buffer.GetString();
       #endif
                       if (!doc.IsObject() && !doc.IsArray()) {
                           MJDEFAULT->setStringForKey(key_type_error, "api");
                           rapidjson::Value a(rapidjson::kObjectType);
                           onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                           return;
                       }
                       
                       if (!doc.HasMember("status")) {
                           MJDEFAULT->setStringForKey(key_type_error, "api");
                           rapidjson::Value a(rapidjson::kObjectType);
                           onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                           return;
                       }
                       
                       if (doc["status"].IsNumber()) {
                           auto status = doc["status"].GetInt();
                           if (status == 1) {
                               onRequestSuccess(doc.GetObject());
                           } else {
                               if (doc.HasMember("message") && doc["message"].IsString()){
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   rapidjson::Value a(rapidjson::kObjectType);
                                   onRequestError(kmjRequestErrorSuccessButDataFailed, doc["message"].GetString(), a.GetObject());
                               }
                               else
                               {
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   rapidjson::Value a(rapidjson::kObjectType);
                                   onRequestError(kmjRequestErrorSuccessButDataFailed, "", a.GetObject());
                               }
                               
                           }
                           return;
                       }
                       
                       auto status = std::string(doc["status"].GetString());
                       if (status != "success") {
                           std::string message;
                           if (JSON_SAFE_CHECK(doc, "message", String)) {
                               message = std::string(doc["message"].GetString());
                           } else if (JSON_SAFE_CHECK(doc, "message", Int)) {
                               message = StringUtils::toString(doc["message"].GetInt());
                           }
                           auto object = doc.GetObject();
                           auto code = JSON_SAFE_CHECK(doc, "code", Int) ? doc["code"].GetInt() : -1;
                           if (_action == ACTIVE_LICENSE_V2 && status == "warning") {
                               code = CODE_ACTIVE_LICENSE_WARNING;
                           } else if (_action == ACTIVE_LICENSE_V2 && status == "deny") {
                               code = CODE_ACTIVE_LICENSE_DENY;
                           }
                           
                           if (code != -1) {
                               if (JSON_SAFE_CHECK(doc, "data", Object)) {
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   onRequestError(code, message, doc["data"].GetObject());
                               } else {
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   onRequestError(code, message, rapidjson::Value(0));
                               }
                           } else {
                               if (JSON_SAFE_CHECK(doc, "data", Object)) {
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   onRequestError(kmjRequestErrorSuccessButDataFailed, message, doc["data"].GetObject());
                               }
                               else {
                                   MJDEFAULT->setStringForKey(key_type_error, "api");
                                   onRequestError(kmjRequestErrorSuccessButDataFailed, message, rapidjson::Value(0));
                               }
                           }
                           return;
                       }
                       
                       if (_action == mj::Request::ActionType::ALL_LESSONS || _action == mj::Request::ActionType::FORGOT_LICENSE || _action == mj::Request::ActionType::CHANGE_LICENSE || _action == mj::Request::ActionType::ACTIVE_LICENSE_V2) {
                           auto data = doc.GetObject();
                           this->onRequestSuccess(data);
                           return;
                       }
                       
                       if (doc.HasMember("data")) {
                           if (doc["data"].IsObject()) {
                               auto dict = doc["data"].GetObject();
                               onRequestSuccess(dict);
                           } else if (doc["data"].IsArray()) {
                               auto dict = doc["data"].GetArray();
                               if(dict.Size() != 0){
                                   onRequestSuccess(dict);
                               }
                           } else {
                               onRequestSuccess(rapidjson::Value(""));
                           }
                       } else {
                           MJDEFAULT->setStringForKey(key_type_error, "api");
                           onRequestError(kmjRequestErrorExpectedData, "", doc.GetObject());
                       }
    }

    void Request::setCallbackSucess()
    {
        _callbackSucess = [=] (std::string respone, bool isSucess) {

            if(!isSucess)
            {
                MJDEFAULT->setStringForKey(key_type_error, "Download");
                rapidjson::Value a(rapidjson::kObjectType);
                onRequestError(kmjRequestErrorNoResponse, "", a.GetObject());
                return;
            }

            if (respone.empty()) {
                rapidjson::Value a(rapidjson::kObjectType);
                MJDEFAULT->setStringForKey(key_type_error, "api");
                onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                return;
            }

            if (_delegate) {
                auto r = _delegate->onRequestRaw(_tag, respone);
                if (r == 1) {
                    this->onRequestSuccess(rapidjson::Value(""));
                    return;
                }

                if (r == -1) {
                    MJDEFAULT->setStringForKey(key_type_error, "api");
                    rapidjson::Value a(rapidjson::kObjectType);
                    this->onRequestError(kmjRequestErrorExpectedData, "", a.GetObject());
                    return;
                }
            }

            rapidjson::Document doc;
            doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(respone.c_str());

            if (doc.HasParseError()) {
                MJDEFAULT->setStringForKey(key_type_error, "api");
                rapidjson::Value a(rapidjson::kObjectType);
                onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                return;
            }

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            rapidjson::StringBuffer rj_buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
            doc.Accept(writer);
            std::cout << rj_buffer.GetString();
#endif
            if (!doc.IsObject() && !doc.IsArray()) {
                MJDEFAULT->setStringForKey(key_type_error, "api");
                rapidjson::Value a(rapidjson::kObjectType);
                onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                return;
            }

            if (!doc.HasMember("status")) {
                MJDEFAULT->setStringForKey(key_type_error, "api");
                rapidjson::Value a(rapidjson::kObjectType);
                onRequestError(kmjRequestErrorInvalidJSON, "", a.GetObject());
                return;
            }

            if (doc["status"].IsNumber()) {
                auto status = doc["status"].GetInt();
                if (status == 1) {
                    onRequestSuccess(doc.GetObject());
                } else {
                    if (doc.HasMember("message") && doc["message"].IsString()){
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        rapidjson::Value a(rapidjson::kObjectType);
                        onRequestError(kmjRequestErrorSuccessButDataFailed, doc["message"].GetString(), a.GetObject());
                    }
                    else
                    {
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        rapidjson::Value a(rapidjson::kObjectType);
                        onRequestError(kmjRequestErrorSuccessButDataFailed, "", a.GetObject());
                    }

                }
                return;
            }

            auto status = std::string(doc["status"].GetString());
            if (status != "success") {
                std::string message;
                if (JSON_SAFE_CHECK(doc, "message", String)) {
                    message = std::string(doc["message"].GetString());
                } else if (JSON_SAFE_CHECK(doc, "message", Int)) {
                    message = StringUtils::toString(doc["message"].GetInt());
                }
                auto object = doc.GetObject();
                auto code = JSON_SAFE_CHECK(doc, "code", Int) ? doc["code"].GetInt() : -1;
                if (_action == ACTIVE_LICENSE_V2 && status == "warning") {
                    code = CODE_ACTIVE_LICENSE_WARNING;
                } else if (_action == ACTIVE_LICENSE_V2 && status == "deny") {
                    code = CODE_ACTIVE_LICENSE_DENY;
                }

                if (code != -1) {
                    if (JSON_SAFE_CHECK(doc, "data", Object)) {
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        onRequestError(code, message, doc["data"].GetObject());
                    } else {
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        onRequestError(code, message, rapidjson::Value(0));
                    }
                } else {
                    if (JSON_SAFE_CHECK(doc, "data", Object)) {
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        onRequestError(kmjRequestErrorSuccessButDataFailed, message, doc["data"].GetObject());
                    }
                    else {
                        MJDEFAULT->setStringForKey(key_type_error, "api");
                        onRequestError(kmjRequestErrorSuccessButDataFailed, message, rapidjson::Value(0));
                    }
                }
                return;
            }

            if (_action == mj::Request::ActionType::ALL_LESSONS || _action == mj::Request::ActionType::FORGOT_LICENSE || _action == mj::Request::ActionType::CHANGE_LICENSE || _action == mj::Request::ActionType::ACTIVE_LICENSE_V2) {
                auto data = doc.GetObject();
                this->onRequestSuccess(data);
                return;
            }

            if (doc.HasMember("data")) {
                if (doc["data"].IsObject()) {
                    auto dict = doc["data"].GetObject();
                    onRequestSuccess(dict);
                } else if (doc["data"].IsArray()) {
                    auto dict = doc["data"].GetArray();
                    if(dict.Size() != 0){
                        onRequestSuccess(dict);
                    }
                } else {
                    onRequestSuccess(rapidjson::Value(""));
                }
            } else {
                MJDEFAULT->setStringForKey(key_type_error, "api");
                onRequestError(kmjRequestErrorExpectedData, "", doc.GetObject());
            }
        };
    }

    void Request::setCallbackError() {
        _callbackError = [=] (int error_code, std::string error_string, rapidjson::Value &data) {

            if (error_code == 401)
            {
                MJDEFAULT->setBoolForKey("key_hide_calculator", true);
                Director::getInstance()->getScheduler()->performFunctionInCocosThread([this] {
                    CCLOG("Start transition scene");
                    ap::Auth::clearUserData();
                    auto login = APLoginView::createView(APLoginView::SCREEN_TYPE::LOGIN);
                    Director::getInstance()->getRunningScene()->addChild(login, 9999);
                    });
                return;
            }

            auto countRetry = MJDEFAULT->getIntegerForKey(key_count_retry);
            mj::helper::pushEventCrashNetWork(_url, error_string, "api", error_code, countRetry);
            MJDEFAULT->setIntegerForKey(key_count_retry, countRetry++);

            ap::api::StoreError::call(_request->getUrl(), error_string, _action_map.at(_action) , _request_type == cocos2d::network::HttpRequest::Type::GET? "{" + _data_post_str + "}": _param);

            CCLOG("MJRequest::onRequestError url:%s , error_string:%s action:%s body_request:%s", _url.c_str(), error_string.c_str(), _action_map.at(_action).c_str(), std::string("{" + _data_post_str + "}").c_str());

            switch (error_code) {
                case kmjRequestErrorSuccessButDataFailed:
                case kmjRequestErrorJSONCompromised:
                    if (_delegate) {
//                    _delegate->onRequestFailed(_tag, error_code, error_string);
                        if (data.IsObject()) {
                            _delegate->onRequestFailed(_tag, error_code, error_string, data.GetObject());
                        }else{
                            rapidjson::Value a(rapidjson::kObjectType);
                            _delegate->onRequestFailed(_tag, error_code, error_string, a.GetObject());
                        }
                    }
                    this->release();
                    break;

                default:
                    _server_id++;
                    if (_server_id >= _servers.size()) {
                        if (_delegate) {
                            if (_action == ACTIVE_LICENSE) {
                                if (error_code == 430){
                                    std::string mail, phone;
                                    mail = JSON_SAFE_CHECK(data, "email", String) ? data["email"].GetString() : "";
                                    phone = JSON_SAFE_CHECK(data, "phone", String) ? data["phone"].GetString() : "";
                                    error_string = mail + "|" + phone;
                                }
                            }
                            if (data.IsObject()) {
                                _delegate->onRequestFailed(_tag, error_code, error_string, data.GetObject());
                            }else{
                                rapidjson::Value a(rapidjson::kObjectType);
                                _delegate->onRequestFailed(_tag, error_code, error_string, a.GetObject());
                            }
//                        _delegate->onRequestFailed(_tag, error_code, error_string);
                        }
                        this->release();
                        return;
                    }
                    _request->setResponseCallback(nullptr);
                    _request->release();
                    send();
                    break;
            }
        };
    }
}


//encode and decode url

std::string char2hex(char dec) {
    char dig1 = (dec & 0xF0) >> 4;
    char dig2 = (dec & 0x0F);
    if (0 <= dig1 && dig1 <= 9) dig1 += 48;    //0,48 in ascii
    if (10 <= dig1 && dig1 <= 15) dig1 += 65 - 10; //A,65 in ascii
    if (0 <= dig2 && dig2 <= 9) dig2 += 48;
    if (10 <= dig2 && dig2 <= 15) dig2 += 65 - 10;

    std::string r;
    r.append(&dig1, 1);
    r.append(&dig2, 1);
    return r;
}

std::string urlencode(const std::string &c) {
    std::string escaped;
    int max = (int) c.length();
    for (auto i = 0; i < max; i++) {
        if (('0' <= c[i] && c[i] <= '9') ||//0-9
                ('A' <= c[i] && c[i] <= 'Z') ||//ABC...XYZ
                ('a' <= c[i] && c[i] <= 'z') || //abc...xyz
                (c[i] == '~' || c[i] == '-' || c[i] == '_' || c[i] == '.')) {
            escaped.append(&c[i], 1);
        } else {
            escaped.append("%");
            escaped.append(char2hex(c[i]));//converts char 255 to string "FF"
        }
    }
    return escaped;
}

