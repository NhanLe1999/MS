//
//  APBaseAPI.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APBaseAPI.h"
#include "cocos2d.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include <iostream>
#include "../APDatabase.h"
#include "MJDefault.h"

#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

constexpr auto ENDPOINT_DELIMITER = "?";

constexpr auto P_APP_ID = "app_id";
constexpr auto P_DEVICE_ID = "device_id";
constexpr auto P_OS = "os";
constexpr auto P_DEVICE_INFO = "info";
constexpr auto P_API_VERSION = "app_ver";
constexpr auto P_DEVICE_TYPE = "device_type";
constexpr auto P_LANGUAGE_DISPLAY = "lang";
constexpr auto P_SUBVER = "subversion";

constexpr auto KMJ_REQUEST_ERROR_NO_RESPONSE = 1;
constexpr auto KMJ_REQUEST_ERROR_INVALID_JSON = 2;
constexpr auto KMJ_REQUEST_ERROR_SUCCESS_BUT_DATA_FAILED = 3;
constexpr auto KMJ_REQUEST_ERROR_EXPECTED_DATA = 4;
constexpr auto KMJ_REQUEST_ERROR_TIMEOUT = 6;



#define JSON_SAFE_CHECK(a, b, c) a.HasMember(b) && !a[b].IsNull() && a[b].Is##c()
#define JSON_EXTRACT(obj, key, type, default) JSON_SAFE_CHECK(obj, key, type) ? obj[key].Get##type() : default

namespace ap {
    namespace api {
        
        APIConfig Base::_global_config;
        
        std::once_flag switch_environment_flag;
        
        Base::Base(const PlatformType platform, const std::string & endpoint, const parameters & param /* = {} */, const RequestType type /* = GET */,
        const float time_out /* = DEFAULT_API_TIMEOUT */) {
            std::call_once(switch_environment_flag, [] {
            });
            setGlobalConfiguration(_global_config);
            _delegate = nullptr;
            _platform = platform;
            _endpoint = endpoint;
            _param = param;
            if (type == RequestType::GET) {
                _request_type = network::HttpRequest::Type::GET;
            } else {
                _request_type = network::HttpRequest::Type::POST;
            }
            _timeout = time_out;
            _timeout_key = "";
            
            _url = buildURL();
        }
        
        Base::Base(const int env, const PlatformType platform, const std::string & endpoint, const parameters & param /* = {} */,
        const RequestType type /* = GET */,
        const float time_out /* = DEFAULT_API_TIMEOUT */) {
            _delegate = nullptr;
            _platform = platform;
            _endpoint = endpoint;
            _param = param;
            if (type == RequestType::GET) {
                _request_type = network::HttpRequest::Type::GET;
            } else {
                _request_type = network::HttpRequest::Type::POST;
            }
            _timeout = time_out;
            _timeout_key = "";
            
            _url = buildURL();
        }
        
        Base::~Base() {
			_request->setResponseCallback(nullptr);
            _request->release();
            _request = nullptr;
            
            Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
        }
        
        std::string Base::buildURL() {
            const std::vector<std::string> OLD_DOMAIN = {
                "https://www.monkeyuni.net/service/v1/",
                "https://dev.monkeyuni.net/service/v1/",
                "https://daybehoc.com/service/v1/"
            };
            
            const std::vector<std::string> NEW_DOMAIN = {
                "https://www.api.monkeyuni.net/api/v1/mobile/",
                "https://api.dev.monkeyuni.net/api/v1/mobile/",
                "https://api.daybehoc.com/api/v1/mobile/"
            };

			const std::vector<std::string> NEW_DOMAIN_V2 = {
				"https://www.api.monkeyuni.net/api/v2/mobile/",
				"https://api.dev.monkeyuni.net/api/v2/mobile/",
				"https://api.daybehoc.com/api/v2/mobile/"
			};

			const std::vector<std::string> NEW_DOMAIN_V3 = {
				"https://www.api.monkeyuni.net/api/v3/mobile/",
				"https://api.dev.monkeyuni.net/api/v3/mobile/",
				"https://api.daybehoc.com/api/v3/mobile/"
			};
            
			const std::vector<std::string> NEW_DOMAIN_V4 = {
				"https://www.api.monkeyuni.net/api/v4/mobile/",
				"https://api.dev.monkeyuni.net/api/v4/mobile/",
				"https://api.daybehoc.com/api/v4/mobile/"
			};

            std::string url;
			
			switch (_platform)
			{
			case ap::api::OLD:
				url = OLD_DOMAIN[_global_config.env];
				break;
			case ap::api::NEW:
				url = NEW_DOMAIN[_global_config.env];
				break;
			case ap::api::NEWV2:
				url = NEW_DOMAIN_V2[_global_config.env]; 
				break;
			case ap::api::NEWV3:
				url = NEW_DOMAIN_V3[_global_config.env]; 
				break;
			case ap::api::NEWV4:
				url = NEW_DOMAIN_V4[_global_config.env];
				break;
			default:
				break;
			}

            url += _endpoint;
            
            if (_request_type == network::HttpRequest::Type::GET) {
                url += ENDPOINT_DELIMITER;
                url += buildBaseParams();
                
                for (const auto & pair : _param) {
                    addParam(url, pair.first, pair.second);
                }
            } else {
                _post_param_str = buildBaseParams();
                for (const auto & pair : _param) {
                    addParam(_post_param_str, pair.first, pair.second);
                }
            }
            
            return url;
        }
        
        std::string Base::getURL(){
            return _request->getUrl();
        }
        
        std::string Base::getParam(){
            rapidjson::Document doc;
            // define document as object
            doc.SetObject();
            rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
         
            if(_param.size() > 0){
                for(auto p: _param ){
                    rapidjson::Value key (rapidjson::kStringType);
                    rapidjson::Value value;
                    key.SetString(p.first.c_str(), allocator);
                    value.SetString(p.second.c_str(), allocator );
                    doc.AddMember(key, value, allocator);
                }
            }
            
            // convert json to string
            rapidjson::StringBuffer strbuf;
            rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
            doc.Accept(writer);
            return strbuf.GetString();
        }
        
        void Base::addParam(std::string & base, const std::string & key, const std::string & value) {
            base = base + std::string((base.empty() ? "" : "&")) + key + "=" + urlencode(value); //url encode
        }
        
        std::string Base::buildBaseParams() {
            std::string param;
            addParam(param, P_APP_ID, std::to_string(_global_config.app_id));
            addParam(param, P_DEVICE_ID, _global_config.device_id);
            addParam(param, P_OS, _global_config.operation_system);
            addParam(param, P_DEVICE_INFO, _global_config.device_info);
            addParam(param, P_SUBVER, _global_config.app_version);
//            addParam(param, P_API_VERSION, V_API_VER);
            addParam(param, P_DEVICE_TYPE, std::to_string(_global_config.device_type));
//            addParam(param, P_LANGUAGE_DISPLAY, std::to_string(_global_config.language));
            auto lang_code ="";
            
            if (_global_config.language == api::Language::TH_TH)
            {
                lang_code = "th-TH";
            }
            else if (_global_config.language == api::Language::VI_VN)
            {
                lang_code = "vi-VN";
            }
			else if (_global_config.language == api::Language::ID_ID)
			{
				lang_code = "id-ID";
			}
            else
            {
                lang_code = "en-US";
            }

            addParam(param, P_LANGUAGE_DISPLAY, lang_code);
            addParam(param, "is_test", (MJDEFAULT->getBoolForKey("debugkey_is_test_license"))? "1" : "0");
            
//            auto info = mm::PlatformConfig::getInstance().getPlatformDeviceID();
//            char * buffer;
//            base64Encode(reinterpret_cast<const unsigned char *>(info.c_str()), int(info.length()), & buffer);
//            addParam(param, "key_has", std::string(buffer));
//            free(buffer);
            
            return param;
        }
        
        void Base::send(std::string i_accessToken) 
		{
            _request = new cocos2d::network::HttpRequest();
            _request->setRequestType(_request_type);

            if (_tag > 0)
			{
                _request->setTag(cocos2d::StringUtils::toString(_tag));
            }
            
			std:string curl = StringUtils::format("curl --location --request POST '%s' \\", _url.c_str());

			const std::string k_accessToken = i_accessToken.empty() ? ap::Database::getInstance().getAccessToken().c_str() : i_accessToken.c_str();

			curl += "--header 'Content-Type: text/plain' \\";

			auto timeSinceEpoch = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            auto timeSinceEpochStr = std::to_string(timeSinceEpoch);
            char * buffer = new char[1000];
            base64Encode(reinterpret_cast<const unsigned char *>(timeSinceEpochStr.c_str()), (int)timeSinceEpochStr.length(), &buffer);
            std::vector<std::string> headers;
            headers.push_back(StringUtils::format("token: %s", i_accessToken.empty() ? ap::Database::getInstance().getAccessToken().c_str() : i_accessToken.c_str()));
            headers.push_back(StringUtils::format("x-api-key: %s", buffer));
			
            _request->setHeaders(headers);
            delete[] buffer;
            

            CCLOG("Sending request for url: %s", _url.c_str());
            for(auto h: headers){
                CCLOG("%s", h.c_str());

				curl += StringUtils::format("--header '%s' \\", h.c_str());
            }

            _request->setUrl(_url);
            
            if (_request_type == cocos2d::network::HttpRequest::Type::POST)
			{
                _request->setRequestData(_post_param_str.data(), _post_param_str.size());

#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
				std::string str = "";
			
				CCLOG("param:start------------------------------------- \n");

				for (int i = 0; i < _post_param_str.size(); i++)
				{
					if (_post_param_str.at(i) != '&' && i != _post_param_str.size()-1)
					{
						str.push_back(_post_param_str.at(i));
					}
					else
					{
						CCLOG("%s", str.c_str());

						if (i == _post_param_str.size() - 1)
						{
							str.push_back(_post_param_str.at(i));
						}

						if (!str.empty() && str.at(str.size() - 1) != '=')
						{
							curl += StringUtils::format("--form '%s' \\", str.c_str());
						}

						str.clear();
					}
				}

				if (!curl.empty() && curl.at(curl.size() - 1) != '\\')
				{
					curl.pop_back();
				}

				CCLOG("api send curl %s", curl.c_str());
#endif
                CCLOG("POST param: %s", _post_param_str.c_str());
            }
            
            _request->setResponseCallback(CC_CALLBACK_2(Base::responseCallback, this));
            cocos2d::network::HttpClient::getInstance()->send(_request);
            
            //timer
            Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
            _timeout_key = StringUtils::format("connection_timeout_%d", random(0, 999999));
            Director::getInstance()->getScheduler()->schedule([=](float) {
                this->onRequestTimeout();
            }, this, 0, 0, _timeout, false, _timeout_key);
        }
        
        void Base::responseCallback(cocos2d::network::HttpClient * /*client*/, cocos2d::network::HttpResponse * response) {
            Director::getInstance()->getScheduler()->unschedule(_timeout_key, this);
            auto buffer = * response->getResponseData();
            auto buffer_string = std::string(buffer.begin(), buffer.end());
            
            CCLOG("response %s", buffer_string.c_str());
            
            if (!response->isSucceed()) {
                onRequestError(KMJ_REQUEST_ERROR_NO_RESPONSE, "", rapidjson::Value(""));
                return;
            }
            
            AsyncTaskPool::getInstance()->enqueue(AsyncTaskPool::TaskType::TASK_NETWORK, [](void *) {
                
            }, nullptr, [this, buffer_string] {
                if (buffer_string.empty()) {
                    onRequestError(KMJ_REQUEST_ERROR_INVALID_JSON, "", rapidjson::Value(""));
                    return;
                }
                
                rapidjson::Document doc;
                doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());
                
                if (doc.HasParseError()) {
                    onRequestError(KMJ_REQUEST_ERROR_INVALID_JSON, "", rapidjson::Value(""));
                    return;
                }
                
                if (_log_raw) {
                    rapidjson::StringBuffer rj_buffer;
                    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
                    doc.Accept(writer);
                    std::cout << rj_buffer.GetString();
                }
                
                
                if (!doc.IsObject() && !doc.IsArray()) {
                    onRequestError(KMJ_REQUEST_ERROR_INVALID_JSON, "", rapidjson::Value(""));
                    return;
                }
                
                if (!doc.HasMember("status")) {
                    onRequestError(KMJ_REQUEST_ERROR_INVALID_JSON, "", rapidjson::Value(""));
                    return;
                }
                
                if (doc["status"].IsNumber()) {
                    auto status = doc["status"].GetInt();
                    if (status == 1) {
                        auto code = JSON_EXTRACT(doc, "code", Int, 0);
                        auto message = JSON_EXTRACT(doc, "message", String, "");
                        onRequestSuccess(code, message, doc.GetObject());
                    } else {
                        if (doc.HasMember("message") && doc["message"].IsString())
                            onRequestError(KMJ_REQUEST_ERROR_SUCCESS_BUT_DATA_FAILED, doc["message"].GetString(), rapidjson::Value(""));
                        else
                            onRequestError(KMJ_REQUEST_ERROR_SUCCESS_BUT_DATA_FAILED, "", rapidjson::Value(0));
                    }
                    return;
                }
                
                auto status = std::string(doc["status"].GetString());
                if (status != "success") {
//<<<<<<< HEAD
//					auto code = JSON_EXTRACT(doc, "code", Int, 0);
//					if (code == 302 || code == 303 || code == 305) {
//						if (doc.HasMember("message") && doc["message"].IsString()) {
//							if (doc["data"].IsObject()) {
//								auto message = JSON_EXTRACT(doc, "message", String, "");
//								auto dict = doc["data"].GetObject();
//								onRequestError(code, message, dict);
//							}
//							else {
//								auto message = JSON_EXTRACT(doc, "message", String, "");
//								onRequestError(code, message, doc["data"]);
//							}
//						}
//						else {
//							onRequestError(code, "", doc["data"]);
//						}
//						return;
//					}
//					else {
//						auto message = std::string(doc["message"].GetString());
//						auto object = doc.GetObject();
//						onRequestError(KMJ_REQUEST_ERROR_SUCCESS_BUT_DATA_FAILED, message, doc.GetObject());
//						return;
//					}
//=======
                    auto code = JSON_EXTRACT(doc, "code", Int, 0);
                    if (code == 302 || code == 303 || code == 305) {
                        if (doc.HasMember("message") && doc["message"].IsString()) {
                            if (doc["data"].IsObject()) {
                                auto message = JSON_EXTRACT(doc, "message", String, "");
                                auto dict = doc["data"].GetObject();
                                onRequestError(code, message, dict);
                            }
                            else {
                                auto message = JSON_EXTRACT(doc, "message", String, "");
                                onRequestError(code, message, doc["data"]);
                            }
                        }
                        else {
                            onRequestError(code, "", doc["data"]);
                        }
                        return;
                    }
                    else {
                        auto message = std::string(doc["message"].GetString());
                        auto object = doc.GetObject();
                        onRequestError(code, message, doc.GetObject());
                        return;
                    }
//>>>>>>> ms_nam
                }
                
                if (doc.HasMember("data")) {
                    if (doc["data"].IsObject()) {
                        auto code = JSON_EXTRACT(doc, "code", Int, 0);
                        auto message = JSON_EXTRACT(doc, "message", String, "");
                        auto dict = doc["data"].GetObject();
                        onRequestSuccess(code, message, dict);
                    } else if (doc["data"].IsArray()) {
                        auto code = JSON_EXTRACT(doc, "code", Int, 0);
                        auto message = JSON_EXTRACT(doc, "message", String, "");
                        auto dict = doc["data"].GetArray();
                        onRequestSuccess(code, message, dict);
                    } else {
                        auto code = JSON_EXTRACT(doc, "code", Int, 0);
                        auto message = JSON_EXTRACT(doc, "message", String, "");
                        onRequestSuccess(code, message, doc["data"]);
                    }
                } else {
                    onRequestError(KMJ_REQUEST_ERROR_EXPECTED_DATA, "", doc.GetObject());
                }
            });
        }
        
        void Base::onRequestTimeout() {
            onRequestError(KMJ_REQUEST_ERROR_TIMEOUT, "Request Timeout.", rapidjson::Value(""));
        }
        
        void Base::onRequestSuccess(const int code, const std::string & message, const rapidjson::Value & data) {
            if (_delegate) {
                _delegate->onRequestSuccess(_tag, code, message, data);
            }
            this->release();
        }
        
        void Base::onRequestError(const int error_code, const std::string & error_string, const rapidjson::Value & data) {
            
            if (_delegate) {
                _delegate->onRequestError(_tag, error_code, error_string, data);
            }
            this->release();
        }
        
        std::string Base::urlencode(const std::string & c) {
            auto char2Hex = [](const unsigned char dec) {
                auto dig_1 = char((dec & 0xF0u) >> 4u);
                auto dig_2 = char((dec & 0x0Fu));
                if (0 <= dig_1 && dig_1 <= 9) dig_1 += 48; //0,48 in ascii
                if (10 <= dig_1 && dig_1 <= 15) dig_1 += 65 - 10; //A,65 in ascii
                if (0 <= dig_2 && dig_2 <= 9) dig_2 += 48;
                if (10 <= dig_2 && dig_2 <= 15) dig_2 += 65 - 10;
                
                std::string r;
                r.append(& dig_1, 1);
                r.append(& dig_2, 1);
                return r;
            };
            
            std::string escaped;
            auto max = int(c.length());
            for (auto i = 0; i < max; i++) {
                if (('0' <= c[i] && c[i] <= '9') || //0-9
                    ('A' <= c[i] && c[i] <= 'Z') || //ABC...XYZ
                    ('a' <= c[i] && c[i] <= 'z') || //abc...xyz
                    (c[i] == '~' || c[i] == '-' || c[i] == '_' || c[i] == '.')) {
                    escaped.append(& c[i], 1);
                } else {
                    escaped.append("%");
                    escaped.append(char2Hex(c[i])); //converts char 255 to string "FF"
                }
            }
            return escaped;
        }
    }
}
