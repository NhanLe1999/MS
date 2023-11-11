//
//  APGoogleOAuth.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/7/19.
//

#ifndef APGoogleOAuth_hpp
#define APGoogleOAuth_hpp

#include <string>
#include <functional>

namespace ap {
    class GoogleOAuth {
    public:
        static GoogleOAuth * createGoogleOAuth(std::function<void(const std::string &, const std::string &)> callback);
        GoogleOAuth(std::function<void(const std::string &, const std::string &)> callback = nullptr);
        virtual void signIn() = 0;
        void onDidAuthenticated(const std::string & id_token, const std::string & email = "");
        
    private:
        std::function<void(const std::string &, const std::string &)> _callback;
    };
}

#endif /* APGoogleOAuth_hpp */
