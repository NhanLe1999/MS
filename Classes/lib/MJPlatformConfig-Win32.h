//
//  MJPlatformConfig-Win32.hpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#ifndef MJPlatformConfig_Win32_hpp
#define MJPlatformConfig_Win32_hpp

#include <stdio.h>
#include "MJPlatformConfig.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

namespace mj {
    class PlatformConfigWin32 : public PlatformConfig {
    public:
        std::string getReceipt() override;
        
    public:
        PlatformConfigWin32(){};
        std::string getPlatformDeviceID() override;
        
        std::string getOS() override;
        std::string getAppVersion() override;
        bool phoneSupport(std::string ) override;
        bool mailSupport(std::string) override;

		std::string getUserDefaults() override;
		void setUserDefaults(std::string info) override;

		std::string getDeviceInfo() override;
		std::string getDeviceInfoShort() override;
        
		void showVMonkeyPage() override;
        void showLearnToReadStorePage() override;
        void showStoriesStorePage() override;
        void showMathStorePage() override;
        
        std::string getAppName() override;
        bool canOpenLearnToRead() override;
        void openLearnToRead() override;
        
        void shareViral(std::string url) override;
        void saveScreenShot(std::string url, std::string file_name) override;
    };
}

#endif

#endif /* MJPlatformConfig_Win32_hpp */
