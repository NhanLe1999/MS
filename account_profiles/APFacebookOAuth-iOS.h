//
//  APFacebookOAuth-iOS.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APFacebookOAuth_iOS_hpp
#define APFacebookOAuth_iOS_hpp

#include "APFacebookOAuth.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
namespace ap {
    class FacebookOAuthiOS : public FacebookOAuth {
    public:
        void signIn() override;
        using FacebookOAuth::FacebookOAuth;
        
    private:
    };
}
#endif

#endif /* APFacebookOAuth_iOS_hpp */
