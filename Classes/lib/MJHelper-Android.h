//
//  MJHelper-Android.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#ifndef MJHelper_Android_hpp
#define MJHelper_Android_hpp

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

namespace mj {
    namespace helper {
        void setOrientationPortrait(bool isPortrait);
        void shareStory();
        void openDeviceAppSettings_android();
    }
}

#endif
#endif /* MJHelper_Android_hpp */
