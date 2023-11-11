//
//  APProfilesListAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 6/20/19.
//

#ifndef APProfilesListAPI_hpp
#define APProfilesListAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class ProfilesList final : public RequestDelegate {
        public:
            static void call(const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

        private:
            void send() const;

            ProfilesList(const std::function<void(bool is_success, std::string msg)> & callback = nullptr);

            std::function<void(bool is_success, std::string msg)> _callback;

            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;

            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;

            Base * _request = nullptr;
            
            std::vector<std::pair<std::string, std::string>> JsonStringToDict (std::string const& s);
        };
    }
}
#endif /* APProfilesListAPI_hpp */
