//
//  MJHelper-Apple.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJHelper_Apple_hpp
#define MJHelper_Apple_hpp

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait);
        void shareStory();
        void sharing(std::string msg,std::string imagePath); // ham nay viet ra danh cho ipad
        void logLanguages();
        void openDeviceAppSettings_ios();
        void changeIconAPP();
        std::string getTimeDate();
        void AddTextFiled();
        void connectUrl();
        void setSpeedTest();
        void setBackGroundColorForTextFiled(bool isColor);
        void pushEventLostConnecttionToClevertab(std::string link, std::string errorType, std::string type, std::string errorcode, std::string countRetry, std::string ipAndspeed);
        std::string getTextFiled();
        void deleteTextFiled();
        float getDiskSpaceIOS();
        void beginDownloadIos(std::string url, std::string pathFile, bool isDownloadThumb = false);
        void stopConnection(bool isDownloadThumb = false);
    }
}

#endif

#endif /* MJHelper_Apple_hpp */
