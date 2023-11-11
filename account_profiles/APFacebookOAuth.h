//
//  APFacebookOAuth.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APFacebookOAuth_hpp
#define APFacebookOAuth_hpp

#include <string>
#include <functional>

#define FB_AUTH_CALLBACK std::function<void(const std::string &,const std::string &)>

namespace ap {
    class FacebookOAuth {
    public:
        static FacebookOAuth * createFacebookOAuth(FB_AUTH_CALLBACK callback);
        FacebookOAuth(FB_AUTH_CALLBACK callback = nullptr);
        virtual void signIn() = 0;

		void onDidAuthenticated(const std::string& id_token, const std::string& email = "");

        
    private:
		FB_AUTH_CALLBACK _callback;
    };
}
#endif /* APFacebookOAuth_hpp */
