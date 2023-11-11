//
//  APFacebookOAuth-desktop.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APFacebookOAuth_desktop_hpp
#define APFacebookOAuth_desktop_hpp

#include "APFacebookOAuth.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
namespace web {
    namespace http {
        namespace experimental {
            namespace listener {
                class http_listener;
            }
        }
    }
}

namespace ap {
    class FacebookOAuthDesktop : public FacebookOAuth {
    public:
        void signIn() override;
        
    private:
        using FacebookOAuth::FacebookOAuth;
        std::string buildAuthURI();
        //void exchangeToken(const std::string & authorization_code);
		void getEmail(const std::string& access_token);
        std::string addParam(const std::string & base, const std::string & key, const std::string & value, bool encode = true);
        
        void setToken(const std::string & token);
        
        std::string _token;
        
        web::http::experimental::listener::http_listener * _listener;
    };
}
#endif

#endif /* APFacebookOAuth_desktop_hpp */
