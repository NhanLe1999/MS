//
//  APGoogleOAuth-Android.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APGoogleOAuth_Android_hpp
#define APGoogleOAuth_Android_hpp

#include "APGoogleOAuth.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
namespace ap {
    class GoogleOAuthAndroid : public GoogleOAuth, public cocos2d::Node {
    public:
        void signIn() override;
        using GoogleOAuth::GoogleOAuth;
        
    private:
    };
}
#endif

#endif /* APGoogleOAuth_Android_hpp */
