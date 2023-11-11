//
//  APGoogleOAuth.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/7/19.
//

#include "APGoogleOAuth.h"
#include "cocos2d.h"
#include "APGoogleOAuth-desktop.h"
#include "APGoogleOAuth-iOS.h"
#include "APGoogleOAuth-Android.h"

namespace ap {
    GoogleOAuth * GoogleOAuth::createGoogleOAuth(std::function<void(const std::string &, const std::string &)> callback) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        return new GoogleOAuthDesktop(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        return new GoogleOAuthiOS(callback);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        return new
        GoogleOAuthAndroid(callback);
#endif
        return nullptr;
    }
    
    GoogleOAuth::GoogleOAuth(std::function<void(const std::string &, const std::string &)> callback) : _callback(callback) {
        
    }
    
    void GoogleOAuth::onDidAuthenticated(const std::string & id_token, const std::string & email) {
        if (_callback) {
            _callback(id_token, email);
            return;
        }
    }
}
