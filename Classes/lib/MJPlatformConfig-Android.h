//
//  MJPlatformConfig-Android.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#ifndef MJPlatformConfig_Android_hpp
#define MJPlatformConfig_Android_hpp

#include <stdio.h>
#include "MJPlatformConfig.h"
#include <string>

#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

namespace mj {
    class PlatformConfigAndroid : public PlatformConfig {
    public:
        std::string getAccessToken() override;
        std::string getPackageName() override;
        int getDeviceYearClass() override;
        
        void shareViral(std::string url) override;
        void shareScreenShot(std::string file_url) override;
        void saveScreenShot(std::string file_url, std::string file_name) override;
    protected:
        using PlatformConfig::PlatformConfig;
        std::string getPlatformDeviceID() override;
        std::string getOS() override;
        std::string getAppVersion() override;
        bool phoneSupport(std::string phone) override;
        bool mailSupport(std::string mail) override;
        std::string getDeviceInfo() override;
        std::string getDeviceInfoShort() override;
        void clearUserDefaults() override;
        
        std::string getUserDefaults() override;
        void setUserDefaults(std::string info) override;
        
		void showVMonkeyPage() override;
        void showLearnToReadStorePage() override;
        void showStoriesStorePage() override;
        void showMathStorePage() override;
        
        std::string getAppName() override;
        bool canOpenLearnToRead() override;
        void openLearnToRead() override;
    private:
        std::string _device_info = "";
    };
}

#endif
#endif /* MJPlatformConfig_Android_hpp */
