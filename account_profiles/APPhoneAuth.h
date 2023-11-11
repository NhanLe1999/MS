//
//  APPhoneAuth.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APPhoneAuth_hpp
#define APPhoneAuth_hpp

#include <string>
#include <functional>

namespace ap {
    class PhoneAuth {
    public:
        static PhoneAuth * createPhoneAuth(std::function<void(const std::string &, const std::string &)> callback);
        PhoneAuth(std::function<void(const std::string &, const std::string &)> callback = nullptr);
        virtual void signIn(const std::string & phone_number = "") = 0;
        void onDidAuthenticated(const std::string & phone_number, const std::string & id_token);
        
    private:
        std::function<void(const std::string &, const std::string &)> _callback;
    };
}
#endif /* APPhoneAuth_hpp */
