//
//  APPhoneAuth-desktop.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "cocos2d.h"
#include "APPhoneAuth-desktop.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "network/HttpClient.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

auto AK_CLIENT_ID = CONFIG_MANAGER->isAppVietNamStories? "1157923674360984" : "520137134992261";
auto AK_APP_SECRET = CONFIG_MANAGER->isAppVietNamStories? "749f94da0666408cb0fd8ba816f9f8b3" : "a71da0c38ff56c96bb83e3ecefd48128";
constexpr auto AK_AUTH_ENDPOINT = "https://www.accountkit.com/v1.0/basic/dialog/sms_login/";
constexpr auto AK_ACCESS_TOKEN_ENDPOINT = "https://graph.accountkit.com/v1.3/access_token?";
constexpr auto AK_ID_ACCOUNT_ENDPOINT = "https://graph.accountkit.com/v1.3/me/?";
auto AK_AT = CONFIG_MANAGER->isAppVietNamStories? "AA|1157923674360984|749f94da0666408cb0fd8ba816f9f8b3" : "AA|520137134992261|a71da0c38ff56c96bb83e3ecefd48128";

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

namespace ap {
    
    void PhoneAuthDesktop::setToken(const std::string & token) {
        _token = token;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    void PhoneAuthDesktop::signIn(const std::string & phone_number) {
        //build oauth uri
		_phone_number = phone_number;
        auto auth_uri = buildAuthURI();
        //open uri
        cocos2d::Application::getInstance()->openURL(auth_uri);
        
        _listener = new http_listener(U("http://localhost:8889"));
        _listener->support([this](http::http_request request) {

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			mj::helper::bringWindowsToFront();
#endif
            CCLOG("listen to %s", request.request_uri().to_string().c_str());
            auto uri = request.request_uri().to_string();
            uri = uri::decode(uri);
            auto components = uri::split_query(uri);
            for (auto p : components) {
                if (p.first.find(U("code")) != std::string::npos) {
                    CCLOG("code: %s", p.second.c_str());
                    auto authorization_code = p.second;
                    //                    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, authorization_code] {
                    //                        cocos2d::Director::getInstance()->getScheduler()->schedule([this, authorization_code](float dt) {
                    this->exchangeToken(conversions::to_utf8string(authorization_code));
                    //                        }, cocos2d::Application::getInstance(), 0, 0, 2, false, "delay_google_oauth_exchange_token");
                    //                    });
                }
            }
			request.reply(web::http::status_codes::OK, json::value::string(U("Login success. Please close website!"))).then([this](pplx::task<void> result) {
				try
				{
					result.wait();
					CCLOG("response oke");
					_listener->close();
				}
				catch (std::exception& e)
				{
					CCLOG("response fail");
					_listener->close();
				}
				});
        });
        _listener->open().wait();
    }
#endif
    
    std::string PhoneAuthDesktop::buildAuthURI() {
        std::string auth_uri = "";
//        auth_uri = addParam(auth_uri, "response_type", "code");
//        auth_uri = addParam(auth_uri, "scope", "openid profile");
		auth_uri = addParam(auth_uri, "app_id", AK_CLIENT_ID);
        auth_uri = addParam(auth_uri, "redirect", "http://localhost:8889/");
//        auth_uri = addParam(auth_uri, "access_type", "offline");
        auth_uri = addParam(auth_uri, "state", "asdf");
		if (_phone_number != "") {
			auth_uri = addParam(auth_uri, "phone_number", _phone_number);
		}
        auth_uri = "?" + auth_uri;
        auth_uri = AK_AUTH_ENDPOINT + auth_uri;
        
        return auth_uri;
    }
    
    std::string PhoneAuthDesktop::addParam(const std::string & base, const std::string & param, const std::string &value, bool encode) {
        if (encode)
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + mj::helper::urlencode(value);
        else
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + value;
    }
    
    void PhoneAuthDesktop::exchangeToken(const std::string & authorization_code) {
        std::string token_uri = AK_ACCESS_TOKEN_ENDPOINT;
        std::string param;
		param = addParam(param, "grant_type", "authorization_code", false);
        param = addParam(param, "code", authorization_code, false);
		
        param = addParam(param, "access_token", AK_AT, false);
        param = addParam(param, "client_secret", AK_APP_SECRET, false);
        
        //param = addParam(param, "redirect_uri", "http://localhost:8889", false);

        CCLOG("GET param: %s", param.c_str());

        auto request = new cocos2d::network::HttpRequest();
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		std::string get_url = token_uri + param;
        request->setUrl(get_url);
        //request->setRequestData(param.data(), param.size());
        std::vector<std::string> headers;
        headers.push_back("Content-Type: application/x-www-form-urlencoded");
        auto temp_header = request->getHeaders();
        request->setHeaders(headers);
        request->setResponseCallback([=](cocos2d::network::HttpClient *client, cocos2d::network::HttpResponse *response) {
            if (!response->isSucceed()) {
                return;
            }
            auto buffer = *response->getResponseData();
            auto buffer_string = std::string(buffer.begin(), buffer.end());

            if (buffer_string.empty()) {
                return;
            }

            rapidjson::Document doc;
            doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

            if (doc.HasParseError()) {
                rapidjson::Value a(rapidjson::kObjectType);
                return;
            }

            if (doc.HasMember("access_token") && doc["access_token"].IsString()) {
                _access_token = doc["access_token"].GetString();
				setToken(_access_token);
				getIdPhone(_access_token);
            }
            /*#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            rapidjson::StringBuffer rj_buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
            doc.Accept(writer);
            CCLOG(rj_buffer.GetString());
			
            #endif*/
        });
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
	void PhoneAuthDesktop::getIdPhone(std::string access_token) {
		std::string token_uri = AK_ID_ACCOUNT_ENDPOINT;
		std::string param;
		param = addParam(param, "access_token", access_token, false);
		std::string get_url = token_uri + param;
		auto request = new cocos2d::network::HttpRequest();
		request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		request->setUrl(get_url);
		//request->setRequestData(param.data(), param.size());
		std::vector<std::string> headers;
		headers.push_back("Content-Type: application/x-www-form-urlencoded");
		auto temp_header = request->getHeaders();
		request->setHeaders(headers);
		request->setResponseCallback([=](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
			if (!response->isSucceed()) {
				return;
			}  
			auto buffer = *response->getResponseData();
			auto buffer_string = std::string(buffer.begin(), buffer.end());

			if (buffer_string.empty()) {
				return;
			}

			rapidjson::Document doc;
			doc.Parse<rapidjson::ParseFlag::kParseDefaultFlags>(buffer_string.c_str());

			if (doc.HasParseError()) {
				rapidjson::Value a(rapidjson::kObjectType);
				return;
			}

			if (doc.HasMember("phone") && doc["phone"].IsObject()) {
				auto data_phone = doc["phone"].GetObject();
				if (data_phone.HasMember("number") && data_phone["number"].IsString()) {
					_phone_number = data_phone["number"].GetString();
					onDidAuthenticated(_phone_number, _access_token);
				}
			}

			
			});
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
	}

}

#endif
