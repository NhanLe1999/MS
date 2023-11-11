//
//  APPhoneAuth.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APPhoneAuth.h"
#include "cocos2d.h"
#include "APPhoneAuthiOS.h"
#include "APPhoneAuth-Android.h"
#include "APPhoneAuth-desktop.h"
#include "MJDefault.h"
//#include "APFacebookOAuth-iOS.h"

namespace ap {
    PhoneAuth * PhoneAuth::createPhoneAuth(std::function<void(const std::string &, const std::string &)> callback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        return new PhoneAuthDesktop(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return new PhoneAuthiOS(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return new PhoneAuthAndroid(callback);
#endif
        return nullptr;
    }
    
    PhoneAuth::PhoneAuth(std::function<void(const std::string &phone_number, const std::string &)> callback) : _callback(callback) {
    }
    
    void PhoneAuth::onDidAuthenticated(const std::string & phone_number, const std::string & id_token) {
        if (_callback) {
            _callback(phone_number, id_token);
            return;
        }
    }
}
