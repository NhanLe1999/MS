//
//  APFacebookOAuth.cpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#include "APFacebookOAuth.h"
#include "cocos2d.h"
#include "APFacebookOAuth-desktop.h"
#include "APFacebookOAuth-iOS.h"
#include "APFacebookOAuth-Android.h"

namespace ap {
    FacebookOAuth * FacebookOAuth::createFacebookOAuth(FB_AUTH_CALLBACK callback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        return new FacebookOAuthDesktop(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return new FacebookOAuthiOS(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return new FacebookOAuthAndroid(callback);
#endif
        return nullptr;
    }
    
    FacebookOAuth::FacebookOAuth(FB_AUTH_CALLBACK callback) : _callback(callback) {
        
    }
    
    void FacebookOAuth::onDidAuthenticated(const std::string & id_token, const std::string & email) {
        if (_callback) {
            _callback(id_token, email);
            return;
        }
    }
}
