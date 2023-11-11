//
//  MJPlatformConfig.cpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"
#include "MJPlatformConfig.h"
#include "network/HttpClient.h"
#include "MJDefault.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "MJPlatformConfig-Apple.h"
#define PLATFORM_CONFIG_IMP PlatformConfigApple
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MJPlatformConfig-Android.h"
#define PLATFORM_CONFIG_IMP PlatformConfigAndroid
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#define PLATFORM_CONFIG_IMP PlatformConfigWin32
#include "MJPlatformConfig-Win32.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#define PLATFORM_CONFIG_IMP PlatformConfigMac
#include "MJPlatformConfig-Mac.h"
#else
#define PLATFORM_CONFIG_IMP PlatformConfig
#endif

#include "NativeBridge.h"
#include "StoryConfigManager.h"
#include "StoryLanguageManager.h"

namespace mj {
    PlatformConfig & PlatformConfig::getInstance() {
        static PLATFORM_CONFIG_IMP sharedPlatformConfig;
        return sharedPlatformConfig;
    }

    std::string PlatformConfig::getDeviceId() {
        auto device_id = MJDEFAULT->getStringForKey(key_device_identifier, "");
        if (device_id.empty()) {
            device_id = getPlatformDeviceID();
            device_id += "|null";
        }
        return device_id;
    }

    void PlatformConfig::setDeviceID(std::string device_id) {
        MJDEFAULT->setStringForKey(key_device_identifier, device_id);
    }
    
    bool PlatformConfig::isTablet() {
        return NativeBridge::isTabletDevice();
    }
    
    double PlatformConfig::_scale_factor = 0.f;
    double PlatformConfig::getItemScaleFactor() {
        if (_scale_factor == 0) {
            _scale_factor = isTablet() ? 0.8f : 1.4f;
        }
        
        return _scale_factor;
    }
    
    int PlatformConfig::getDefaultLangCode()
    {
        if (CONFIG_MANAGER->isVietnameseStories()) {
            return 4;
        }
        
        const std::string k_languageCode = Application::getInstance()->getCurrentLanguageCode();
 
        return (k_languageCode == "vi") ? LANGUAGE_VN_CODE :(k_languageCode == "th" ?LANGUAGE_THAI_CODE: LANGUAGE_EN_CODE);

        //return getDefaultLanguageCode();
    }
    
    int PlatformConfig::getDefaultLanguageCode() {
        return 1;
    }
}
