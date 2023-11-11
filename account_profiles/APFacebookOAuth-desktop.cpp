//
//  APFacebookOAuth-desktop.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

#include "firebase/app.h"
#include "firebase/auth.h"

#include "APFacebookOAuth-desktop.h"
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJHelper.h"
#include "StoryConfigManager.h"
#include "MJDefault.h"

 auto FB_CLIENT_ID = CONFIG_MANAGER->isAppVietNamStories? "1157923674360984" : "520137134992261";
 auto FB_CLIENT_SECRET =  CONFIG_MANAGER->isAppVietNamStories? "c48445dd0d587e53d9830732043af3b5" : "b463ba42e9312d07d4c58578f1cf7132";
 auto FB_AUTH_ENDPOINT = "https://www.facebook.com/v4.0/dialog/oauth";
 auto FB_EMAIL_ENDPOINT = "https://graph.facebook.com/v4.0/me?fields=email";

#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;


namespace ap {
    
    void FacebookOAuthDesktop::setToken(const std::string & token) {
        _token = token;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    void FacebookOAuthDesktop::signIn() {
		
		

		//open uri
		auto auth_uri = buildAuthURI();
		cocos2d::Application::getInstance()->openURL(auth_uri);
		
        _listener = new http_listener(U("http://localhost:8889/"));
		_listener->support([this](http::http_request request) {
			//            _listener->close().wait();
			
			CCLOG("listen to %s", request.request_uri().to_string().c_str());
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
			mj::helper::bringWindowsToFront();
#endif
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([] {
				cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("event_completed_login_browser");
				});
			auto uri = request.request_uri().to_string();

			//request.get
			uri = uri::decode(uri);

			auto components = uri::split_query(uri);
			for (auto p : components) {
				if (p.first.find(U("access_token")) != std::string::npos) {
					CCLOG("code: %s", p.second.c_str());
					auto access_token = p.second;
					this->setToken(conversions::to_utf8string(access_token));
					this->getEmail(conversions::to_utf8string(access_token));
				}
			}
			///cocos2d::Application::getInstance()->openURL("https://google.com");
			//respond(request, status_codes::OK, json::value::string(U("Request received for: ")));
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
    
    std::string FacebookOAuthDesktop::buildAuthURI() {
        std::string auth_uri = "";
		auto redirect_api = MJDEFAULT->getStringForKey("redirect_uri", "https://api.monkeyuni.net/api/redirect-uri?uri=");
		if (redirect_api == "") redirect_api = "https://api.monkeyuni.net/api/redirect-uri?uri=";
		auth_uri = addParam(auth_uri, "redirect_uri", redirect_api + "http://localhost:8889/");
		auth_uri = addParam(auth_uri, "client_id", FB_CLIENT_ID);
        auth_uri = addParam(auth_uri, "state", "abc1234");
		auth_uri = addParam(auth_uri, "response_type", "token");
		auth_uri = addParam(auth_uri, "scope", "email");
        
        auth_uri = "?" + auth_uri;
        auth_uri = FB_AUTH_ENDPOINT + auth_uri;
        
        return auth_uri;
    }
    
    std::string FacebookOAuthDesktop::addParam(const std::string & base, const std::string & param, const std::string &value, bool encode) {
        if (encode)
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + mj::helper::urlencode(value);
        else
            return base + std::string((base.empty() ? "" : "&")) + param + "=" + value;
    }
    
    void FacebookOAuthDesktop::getEmail(const std::string & access_token) {
		_token = access_token;
        std::string token_uri = FB_EMAIL_ENDPOINT;

        token_uri = addParam(token_uri, "access_token", access_token, false);
        


        auto request = new cocos2d::network::HttpRequest();
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setUrl(token_uri);
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

            if (doc.HasMember("email") && doc["email"].IsString()) {
                auto email = doc["email"].GetString();
                this->onDidAuthenticated(access_token,email);
			}
			else {
				this->onDidAuthenticated(access_token, "");
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
