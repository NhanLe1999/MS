//
//  APGoogleOAuth-iOS.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/19/19.
//

#ifndef APGoogleOAuth_iOS_hpp
#define APGoogleOAuth_iOS_hpp

#include "APGoogleOAuth.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

namespace ap {
    class GoogleOAuthiOS : public GoogleOAuth {
    public:
        void signIn() override;
        using GoogleOAuth::GoogleOAuth;

    private:
    };
}
#endif
#endif /* APGoogleOAuth_iOS_hpp */
