//
//  MJPlatformConfig-Apple.hpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#ifndef MJPlatformConfig_Apple_hpp
#define MJPlatformConfig_Apple_hpp

#include <stdio.h>
#include "MJPlatformConfig.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

namespace mj {
    class PlatformConfigApple : public PlatformConfig {
    public:
        std::string getReceipt() override;
        std::string getDeviceInfo() override;
        std::string getDeviceInfoShort() override;
        std::string getPackageName() override;
        
        double getNotchHeight() override;

        void rotateKeyboard(bool isPortrait) override;
        void forceDismissKeyboard() override;
        void clearUserDefaults() override;
        std::string getUserDefaults() override;
        void setUserDefaults(std::string info) override;
        
        int getDeviceYearClass() override;
        
        int getDefaultLanguageCode() override;
        
		void showVMonkeyPage() override;
        void showLearnToReadStorePage() override;
        void showStoriesStorePage() override;
        void showMathStorePage() override;
        
        bool canOpenLearnToRead() override;
        void openLearnToRead() override;
        
        void shareViral(std::string url) override;
        void shareScreenShot(std::string file_url) override;
        void saveScreenShot(std::string file_url, std::string file_name) override;
    protected:
        using PlatformConfig::PlatformConfig;
        std::string getPlatformDeviceID() override;

        std::string getOS() override;
        std::string getAppVersion() override;
        std::string getBuildNumber() override;

        bool phoneSupport() override;
        bool mailSupport() override;

        std::string getDeviceName();
        
        std::string getAppName() override;
    };
}

#endif

#endif /* MJPlatformConfig_Apple_hpp */
