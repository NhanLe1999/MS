//
//  APAuth.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/31/19.
//

#ifndef APAuth_hpp
#define APAuth_hpp

#include <string>

class StoryAlertDelegate;

namespace ap {
    class Auth {
    public:
        static void init();
        static void authenticateWithGoogle(std::string i_newAccessToken);
        static void authenticateWithFacebook(std::string i_newAccessToken);
        static void authenticateWithPhoneNumber(const std::string & phone_number = "");
        static void signOut(StoryAlertDelegate *delegate = nullptr);
        static void clearUserData();
        
        static void authenticateWithGoogleToken(const std::string & token, const std::string & email, const  std::string &i_newAccessToken);
        static void authenticateWithFacebookToken(const std::string & token, const std::string & email, const  std::string &i_newAccessToken);
		static void authenticateWithAKToken(const std::string& token, const std::string& phone);
        static void showErrorAlert(std::string msg);
        static void showNetworkDownAlert(std::string msg, StoryAlertDelegate *delegate);
        static std::string _mail;
        static std::string _token_facebook;
        
    };
}

#endif /* APAuth_hpp */
