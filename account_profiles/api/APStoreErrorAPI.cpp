//
//  APStoreErrorAPI.cpp
//  stories_debug
//
//  Created by AnhBNG on 8/26/2019
//

#include "APStoreErrorAPI.h"

#include "json/stringbuffer.h"
#include "json/writer.h"

#include "MJDefault.h"
#include "MSEventLogger.h"

namespace ap {
    namespace api {
        void StoreError::call(std::string url, std::string err_msg, std::string action_name, std::string body_request)
        {
            try
            {
                std::vector<std::pair<std::string, cocos2d::Value>> properties;
                
                if(!url.empty())
                {
                    int i = 1;
                    
                    const int urlMaxLenth = 100;
                    
                    while (url.size() > urlMaxLenth)
                    {
                        std::string subURL = url.substr(0,urlMaxLenth);
                        
                        properties.push_back({StringUtils::format("url_%d", ++i),cocos2d::Value(subURL)});
                        
                        url = url.substr(urlMaxLenth,url.size()-1);
                    }
                    
                    properties.push_back({StringUtils::format("url_%d", ++i),cocos2d::Value(url)});
                }
                
                properties.push_back({"action_name", cocos2d::Value(action_name)});
                properties.push_back({"message", cocos2d::Value(err_msg)});
                properties.push_back({"body_request", cocos2d::Value(body_request)});
                properties.push_back({"profile_id", cocos2d::Value(StringUtils::toString(MJDEFAULT->getIntegerForKey(key_current_profile_id)))});
                properties.push_back({"device_id", cocos2d::Value(MJ_PLATFORM_CONFIG.getDeviceId())});
                properties.push_back({"app_ver", cocos2d::Value(MJ_PLATFORM_CONFIG.getAppVersion())});
                
                ms::EventLogger::getInstance().logEvent("store_error_api",properties);

            }
            catch (const std::runtime_error& e)
            {
                
            }
          
           // auto api_call = new StoreError(url, err_msg, action_name, body_request);
           // api_call->send();
        }
    
        void StoreError::send() const
        {
            if (_request)
            {
                _request->send();
            }
        }
    
        StoreError::StoreError(std::string url, std::string err_msg, std::string action_name, std::string body_request){
            _request = new Base(PlatformType::NEW, "account/store-error-download", {
                {"profile_id", StringUtils::toString(MJDEFAULT->getIntegerForKey(key_current_profile_id))},
                {"app_id", StringUtils::toString((CONFIG_MANAGER->isVietnameseStories() ? v_APP_ID_VN : v_APP_ID))},
                {"url", url },
                {"device_id", MJ_PLATFORM_CONFIG.getDeviceId()},
                {"app_ver", MJ_PLATFORM_CONFIG.getAppVersion()},
                {"error_message", err_msg},
                {"action_name", action_name},
                {"body_request", body_request}
            }, api::RequestType::POST);
            _request->setDelegate(this);
            _request->enableLog();
        }
    
        void StoreError::onRequestSuccess(int tag, int code, const std::string &message, const rapidjson::Value &data) {

            if (_callback) {
                _callback(true, "");
            }
            delete this;
        }
    
        void StoreError::onRequestError(int tag, int error_code, const std::string &message, const rapidjson::Value &data) {
            if (_callback) {
                _callback(false, message);
            }
            delete this;
        }
    }
}
