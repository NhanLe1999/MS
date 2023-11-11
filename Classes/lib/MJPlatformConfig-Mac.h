//
//  MJPlatformConfig-Mac.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/10/17.
//
//

#ifndef MJPlatformConfig_Mac_hpp
#define MJPlatformConfig_Mac_hpp

#include <stdio.h>
#include "MJPlatformConfig.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

namespace mj {
    class PlatformConfigMac : public PlatformConfig {
    public:
        void setFullscreen() override;
//        std::string getReceipt() override;
        std::string getDeviceInfo() override;
        std::string getDeviceInfoShort() override;
        void clearUserDefaults() override;
        std::string getUserDefaults() override;
        void setUserDefaults(std::string info) override;
        
//        void rotateKeyboard(bool isPortrait) override;
//        void forceDismissKeyboard() override;
        void shareViral(std::string url) override;
        void saveScreenShot(std::string url, std::string file_name) override;
        
    protected:
		void showVMonkeyPage() override;
        void showLearnToReadStorePage() override;
        void showStoriesStorePage() override;
        void showMathStorePage() override;
        
        std::string getPackageName() override;
        using PlatformConfig::PlatformConfig;
        std::string getPlatformDeviceID() override;
        
        std::string getOS() override { return "mac"; };
        std::string getAppVersion() override;
        
        bool phoneSupport() override;
        bool mailSupport() override;
        std::string getAppName() override;
        bool canOpenLearnToRead() override;
        void openLearnToRead() override;
        //std::string getDeviceName();
    };
}

#endif

#endif /* MJPlatformConfig_Mac_hpp */
