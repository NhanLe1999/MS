//
//  APBaseAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APBaseAPI_hpp
#define APBaseAPI_hpp

#include <string>
#include <map>
#include <unordered_map>
#include "network/HttpClient.h"
#include "../external/json/document.h"
#include "MJPlatformConfig.h"
#include "MJDefault.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"
#define v_APP_ID 40
#define v_APP_ID_VN 51
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

constexpr auto DEFAULT_API_TIMEOUT = 10;

constexpr auto MM_DEV_ENV = 2;
constexpr auto MM_TEST_ENV = 1;
constexpr auto MM_PRODUCTION_ENV = 0;

namespace ap {
    namespace api {
        enum PlatformType {
            OLD,
            NEW,
			NEWV2,
			NEWV3,
			NEWV4
        };
        
        enum RequestType {
            GET,
            POST
        };
        
        enum Environment {
            LIVE,
            DEV,
            PRODUCT
        };
        
        enum DeviceResolution {
            NORMAL = 2,
            HIGH_DEFINITION = 4
        };
        
        enum Language {
            EN_US = 1,
            TH_TH = 2,
			ID_ID = 3,
            VI_VN = 4
        };

		enum class RESPONSE_CODE_FROM_REGISTER_API
		{
			OPEN_OTP_POPUP = 302,
			OPEN_CREATING_PASSWORD_POPUP = 306,
			OPEN_ENTERING_PASSWORD_POPUP = 307,
			OPEN_GOOGLE_LOGIN_POPUP = 303,
		};
        
        struct APIConfig {
            int app_id;
            Environment env;
            std::string device_id;
            std::string operation_system;
            std::string device_info;
            std::string app_version;
            DeviceResolution device_type;
            Language language;

			APIConfig()
			{
			    app_id = 40;
				// environment
				env = api::Environment::DEV;
				// language
				auto lang_display_id = 1;// MJDEFAULT->getIntegerForKey(key_lang_display, 1);
				auto lang_code = std::string("");
				if (lang_display_id == 1) {
					language = api::Language::EN_US;
				}
				else {
					language = api::Language::VI_VN;
				}
                operation_system = "mac";//MJ_PLATFORM_CONFIG.getOS();//"mac";

				device_id = "9";//MJ_PLATFORM_CONFIG.getDeviceId();
				device_info = "";//MJ_PLATFORM_CONFIG.getDeviceInfo();
				device_type = api::DeviceResolution::HIGH_DEFINITION;
//				device_type = api::DeviceResolution::HIGH_DEFINITION;
			}
        };
        
        class RequestDelegate {
        public:
            virtual ~RequestDelegate() = default;
            virtual bool onReceivedRawData(int tag, const std::string & data) { return false; };
            virtual void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) = 0;
            virtual void onRequestError(int tag, int error_code, const std::string & error_string, const rapidjson::Value & data) = 0;
        };
        
        class Base final : public cocos2d::Ref {
        public:
            typedef std::unordered_map<std::string, std::string> parameters;
            
            Base(PlatformType platform, const std::string & endpoint, const parameters & param = std::unordered_map<std::string, std::string>(),
            RequestType type = GET, float time_out = DEFAULT_API_TIMEOUT);
            
            Base(int env, PlatformType platform, const std::string & endpoint,
            const parameters & param = std::unordered_map<std::string, std::string>(),
            RequestType type = GET, float time_out = DEFAULT_API_TIMEOUT);
            
            ~Base() override;
            
            void setDelegate(RequestDelegate * var) { _delegate = var; };
            
            void send(std::string i_accessToken = "");

            static std::string buildBaseParams();
            
            static void addParam(std::string & base, const std::string & key, const std::string & value);
            
            static std::string getOnePayURL(const std::string & product_id);
            
            static void setOnepayDomain(const std::string & domain);
            
            static std::string getOnePayDomain();
            
            //log error
            std::string getURL();
            std::string getParam();
            
        protected:
            bool _log_raw = false;
            RequestDelegate * _delegate = nullptr;
            
        public:
            void enableLog() { _log_raw = true; }
            static void setGlobalConfiguration (APIConfig config) { 
				_global_config = config;
                _global_config.app_id = (CONFIG_MANAGER->isVietnameseStories() ? v_APP_ID_VN : v_APP_ID);
                // evironment
				if (CONFIG_MANAGER->isProductEnvironment() || CONFIG_MANAGER->isDevEnvironment()) {
                    _global_config.env = api::Environment::DEV;
                }
                else {
                    _global_config.env = api::Environment::LIVE;
                }
//                _global_config.env = api::Environment::DEV;

                // language
                auto lang_display_id = MJDEFAULT->getIntegerForKey(key_lang_display, LANGUAGE_EN_CODE);
                
                auto lang_code = std::string("");
                
                if (lang_display_id == LANGUAGE_THAI_CODE)
                {
                    _global_config.language = api::Language::TH_TH;
                }
                else if (lang_display_id == LANGUAGE_VN_CODE)
                {
                    _global_config.language = api::Language::VI_VN;
                }
				else if (lang_display_id == LANGUAGE_ID_CODE)
				{
					_global_config.language = api::Language::ID_ID;
				}
                else
                {
                    _global_config.language = api::Language::EN_US;
                }
                
               _global_config. operation_system = MJ_PLATFORM_CONFIG.getOS();//"mac";
                _global_config.device_id = MJ_PLATFORM_CONFIG.getDeviceId();
                _global_config.device_info = MJ_PLATFORM_CONFIG.getDeviceInfo();
				_global_config.app_version = MJ_PLATFORM_CONFIG.getAppVersion();
			}
            
            //encode and decode url
            static std::string urlencode(const std::string & c);
        private:
            static APIConfig _global_config;
            
            int _tag = 0;
            float _timeout;
            std::string _url;
            std::string _endpoint;
            PlatformType _platform;
            cocos2d::network::HttpRequest * _request = nullptr;
            cocos2d::network::HttpRequest::Type _request_type;
            parameters _param;
            static const std::map<int, std::string> action_map;
            std::string _post_param_str;
            
            std::string buildURL();
            
            void onRequestTimeout();
            
            void onRequestError(int error_code, const std::string & error_string, const rapidjson::Value & data);
            
            void onRequestSuccess(int code, const std::string & message, const rapidjson::Value & data);
            
            void responseCallback(cocos2d::network::HttpClient * client, cocos2d::network::HttpResponse * response);
            
            std::string _timeout_key;
        };
    }
}

#endif /* APBaseAPI_hpp */
