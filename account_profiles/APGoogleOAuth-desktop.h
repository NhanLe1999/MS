//
//  APGoogleOAuth-desktop.hpp
//  stories_release
//
//  Created by To Ngoc Linh on 6/14/19.
//

#ifndef APGoogleOAuth_desktop_hpp
#define APGoogleOAuth_desktop_hpp

#include "APGoogleOAuth.h"

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
    class GoogleOAuthDesktop : public GoogleOAuth {
    public:
        void signIn() override;
        
    private:
        using GoogleOAuth::GoogleOAuth;
        std::string buildAuthURI();
        void exchangeToken(const std::string & authorization_code);
		void getUserInfo(const std::string& access_token);

        std::string addParam(const std::string & base, const std::string & key, const std::string & value, bool encode = true);
        void setToken(const std::string & token);
        
        std::string _token;
        web::http::experimental::listener::http_listener * _listener;
    };
}
#endif

#endif /* APGoogleOAuth_desktop_hpp */
