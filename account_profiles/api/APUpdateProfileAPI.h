//
//  APUpdateProfileAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APUpdateProfileAPI_hpp
#define APUpdateProfileAPI_hpp

#include "APBaseAPI.h"
#include "APProfile.h"

namespace ap {
    namespace api {
        class UpdateProfile final : public RequestDelegate {
        public:
            static void call(const Profile & profile, const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
        private:
            void send() const;
            
            UpdateProfile(const Profile & p,
                  const std::function<void(bool is_success, std::string msg)> & callback = nullptr);
            
            Profile _profile;
            std::function<void(bool is_success, std::string msg)> _callback;
            
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
			std::string buildConfigItem();
            std::map<std::string, std::string> JsonStringToDict (std::string const& s);
            Base * _request = nullptr;
        };
    }
}
#endif /* APUpdateProfileAPI_hpp */
