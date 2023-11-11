//
//  APPhoneAuth-desktop.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APPhoneAuth_desktop_hpp
#define APPhoneAuth_desktop_hpp

#include "APPhoneAuth.h"

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
    class PhoneAuthDesktop : public PhoneAuth {
    public:
        void signIn(const std::string & phone_number = "") override;
        
    private:
        using PhoneAuth::PhoneAuth;
        std::string buildAuthURI();
        void exchangeToken(const std::string & authorization_code);
        
        std::string addParam(const std::string & base, const std::string & key, const std::string & value, bool encode = true);
        
        void setToken(const std::string & token);
        
        std::string _token;
		std::string _access_token;
		std::string _phone_number;
        
		void getIdPhone(std::string access_token);
        web::http::experimental::listener::http_listener * _listener;
    };
}
#endif
#endif /* APPhoneAuth_desktop_hpp */
