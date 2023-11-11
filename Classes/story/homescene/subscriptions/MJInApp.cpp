//
//  MJInApp.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 7/18/17.
//

#include "MJInApp.h"
#include "platform/CCPlatformConfig.h"
#include "MJRequest.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
#include "MJInApp-Apple.h"
#define IAP_IMP InAppApple
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "MJInApp-Android.h"
#define IAP_IMP InAppAndroid
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "MJInApp-Win32.h"
#define IAP_IMP InAppWin32
#else 
#define IAP_IMP InApp
#endif

namespace mj {
    InApp & InApp::getInstance() {
        static IAP_IMP sharedIAP;
        return sharedIAP;
    }

    void InApp::payWithOnePay(std::string product_id, std::string coupon) {
        CCLOG("onepay_url = %s", mj::Request::getOnePayURL(product_id).c_str());
        cocos2d::Application::getInstance()->openURL(mj::Request::getOnePayURL(product_id));
    }
}
