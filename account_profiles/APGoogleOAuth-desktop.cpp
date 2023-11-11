//
//  APGoogleOAuth-desktop.cpp
//  stories_release
//
//  Created by To Ngoc Linh on 6/14/19.
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#include "APGoogleOAuth-desktop.h"
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"

auto CLIENT_ID = CONFIG_MANAGER->isAppVietNamStories? "903853614382-t2ej1rhshogjtf7ks1rcav5b54ovvd6k.apps.googleusercontent.com" : "518533842801-lu80s0fntoma1kns3q82s880eohe7phl.apps.googleusercontent.com";
auto CLIENT_SECRET = CONFIG_MANAGER->isAppVietNamStories? "8K_NTICKdEqYVFotWYSES4vK" : "RbpuF1Z60aRI4l-yL1PmylkL";
constexpr auto AUTH_ENDPOINT = "https://accounts.google.com/o/oauth2/v2/auth";
constexpr auto TOKEN_ENDPOINT = "https://www.googleapis.com/oauth2/v4/token/";

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

namespace ap {

    void GoogleOAuthDesktop::setToken(const std::string & token) {
        _token = token;
    }

    int port = 8888;

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    void GoogleOAuthDesktop::signIn() {
        
        port = cocos2d::random(8881,8899);
        //build oauth uri
        auto auth_uri = buildAuthURI();
        
        //open uri
        cocos2d::Application::getInstance()->openURL(auth_uri);
		//cocos2d::Application::getInstance()->
        
        CCLOG("GoogleOAuthDesktop|auth_uri= %s",auth_uri.c_str());
		
		auto url = utility::conversions::to_string_t(StringUtils::format("http://localhost:%d", port));

        _listener = new http_listener(url);

        _listener->support([this](http::http_request request) {
//            _listener->close().wait();
    #if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			mj::helper::bringWindowsToFront();
    #endif
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_completed_login_browser");
				});
            CCLOG("listen to %s", request.request_uri().to_string().c_str());
            auto uri = request.request_uri().to_string();
            uri = uri::decode(uri);
            auto components = uri::split_query(uri);
            for (const auto & p : components) {
				CCLOG("auth - key: %s, value %s", conversions::to_utf8string(p.first).c_str(), conversions::to_utf8string(p.second).c_str());
                if (p.first.find(U("code")) != std::string::npos) {
                    CCLOG("code: %s", p.second.c_str());
                    auto authorization_code = p.second;
                    this->exchangeToken(conversions::to_utf8string(authorization_code));                            
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
				});;
        });
        
        _listener->open().wait();
    }
    #endif

    std::string GoogleOAuthDesktop::buildAuthURI() {
        std::string auth_uri = "";
        auth_uri = addParam(auth_uri, "response_type", "code");
        auth_uri = addParam(auth_uri, "scope", "profile email");
        auth_uri = addParam(auth_uri, "redirect_uri", StringUtils::format("http://localhost:%d",port));//cocos2d::StringUtils::format("http://localhost:%d",cocos2d::random(8889,8890))
        auth_uri = addParam(auth_uri, "client_id", CLIENT_ID);
        auth_uri = addParam(auth_uri, "access_type", "offline");
        auth_uri = addParam(auth_uri, "include_granted_scope", "true");
        
        auth_uri = "?" + auth_uri;
        auth_uri = AUTH_ENDPOINT + auth_uri;
        
        return auth_uri;
    }

    std::string GoogleOAuthDesktop::addParam(const std::string & base, const std::string & param, const std::string &value, bool encode) {
        if (encode)
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + mj::helper::urlencode(value);
        else
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + value;
    }

    void GoogleOAuthDesktop::exchangeToken(const std::string & authorization_code) {
        std::string token_uri = TOKEN_ENDPOINT;
        std::string param;
        param = addParam(param, "code", authorization_code, false);
        param = addParam(param, "client_id", CLIENT_ID, false);
        param = addParam(param, "client_secret", CLIENT_SECRET, false);
        param = addParam(param, "grant_type", "authorization_code", false);
        param = addParam(param, "redirect_uri", StringUtils::format("http://localhost:%d",port), false);
        
        CCLOG("POST param: %s", param.c_str());
        
        auto request = new cocos2d::network::HttpRequest();
        request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
        request->setUrl(token_uri);
        request->setRequestData(param.data(), param.size());
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
				auto access_token = doc["access_token"].GetString();
				CCLOG("access_token: %s", access_token);
				this->setToken(access_token);
				this->getUserInfo(access_token);
			}
            
            /*if (doc.HasMember("id_token") && doc["id_token"].IsString()) {
                _token = doc["id_token"].GetString();
            }*/

//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
            rapidjson::StringBuffer rj_buffer;
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
            doc.Accept(writer);
            std::cout << rj_buffer.GetString();
//#endif
        });
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }

    void GoogleOAuthDesktop::getUserInfo(const std::string & access_token) {
		std::string api = "https://www.googleapis.com/oauth2/v2/userinfo";

		auto request = new cocos2d::network::HttpRequest();
		request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
		request->setUrl(api);
		std::vector<std::string> headers;
		headers.push_back(cocos2d::StringUtils::format("Authorization: Bearer %s", access_token.c_str()));
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

			if (doc.HasMember("email") && doc["email"].IsString()) {
				auto email = doc["email"].GetString();
				this->onDidAuthenticated(_token, email);
			}

			//#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
			rapidjson::StringBuffer rj_buffer;
			rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(rj_buffer);
			doc.Accept(writer);
			std::cout << rj_buffer.GetString();
			//#endif
			});
		cocos2d::network::HttpClient::getInstance()->send(request);
		request->release();
    }
}

#endif
