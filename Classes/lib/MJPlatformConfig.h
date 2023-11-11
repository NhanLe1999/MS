//
//  MJPlatformConfig.hpp
//  M
//
//  Created by To Ngoc Linh on 5/22/17.
//
//

#ifndef MJPlatformConfig_hpp
#define MJPlatformConfig_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include "MJDefinitions.h"
#define MJ_PLATFORM_CONFIG mj::PlatformConfig::getInstance()

namespace mj {
    class PlatformConfig {
    public:
        virtual void setFullscreen() {};
        static PlatformConfig & getInstance();
        std::string getDeviceId();
        virtual std::string getAppVersion() {return "";};
        virtual std::string getBuildNumber() {return "";};
        virtual std::string getOS() {return "";};
        virtual std::string getDeviceInfo() { return ""; };
        virtual std::string getDeviceInfoShort() { return ""; }
        virtual std::string getPackageName() { return ""; }
        virtual int getDeviceYearClass() { return 2018; }
        
        virtual double getNotchHeight() { return 0; }
        
        void setDeviceID(std::string device_id);

        virtual std::string getReceipt() { return ""; };
        virtual std::string getAccessToken() { return ""; }
        
        virtual bool phoneSupport(std::string) {return false; };
        virtual bool mailSupport(std::string) {return false; };

        virtual void rotateKeyboard(bool isPortrait) {};
        virtual void forceDismissKeyboard() {};

        virtual void clearUserDefaults() {};
        virtual std::string getUserDefaults() {return ""; };
        virtual void setUserDefaults(std::string info) { };
        
        bool isTablet();
        
        int getDefaultLangCode();
        virtual int getDefaultLanguageCode();
        
		virtual void showVMonkeyPage() {};
        virtual void showLearnToReadStorePage() {};
        virtual void showStoriesStorePage() {};
        virtual void showMathStorePage() {};
        
        virtual double getItemScaleFactor();
        virtual std::string getAppName() { return ""; };

        virtual std::string getPlatformDeviceID() {return ""; };
        virtual bool canOpenLearnToRead() { return false; };
        virtual void openLearnToRead() {};
        
        virtual void shareViral(std::string url){ };
        virtual void shareScreenShot(std::string file_url){ };
        virtual void saveScreenShot(std::string file_url, std::string file_name){ };
    protected:
        explicit PlatformConfig() {};

    private:
        PlatformConfig(PlatformConfig const &);
        void operator = (PlatformConfig const &);
        
        static double _scale_factor;
    };
}

#endif /* MJPlatformConfig_hpp */
