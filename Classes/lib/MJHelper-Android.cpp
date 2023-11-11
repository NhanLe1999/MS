//
//  MJHelper-Android.cpp
//  Azu7
//
//  Created by To Ngoc Linh on 6/2/17.
//
//

#include "MJHelper-Android.h"

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "jni/JniHelper.h"
#include <jni.h>

#define HELPER_CLASS "org.cocos2dx.cpp.MJHelper"

namespace mj {
    namespace helper {
        /*void setOneSignalEmail(std::string email) {
            cocos2d::JniHelper::callStaticVoidMethod(HELPER_CLASS, "setOneSignalEmail");
        }*/

        void setOrientationPortrait(bool isPortrait) {

        }

        void shareStory() {
            cocos2d::JniHelper::callStaticVoidMethod(HELPER_CLASS, "share", "http://monkeyjunior.vn");
        }
    
        void openDeviceAppSettings_android() {
            cocos2d::JniHelper::callStaticVoidMethod(HELPER_CLASS, "openDeviceAppSettings");
        }
    }
}

#endif
