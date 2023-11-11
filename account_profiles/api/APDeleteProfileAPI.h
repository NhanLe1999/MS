//
//  APDeleteProfileAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#ifndef APDeleteProfileAPI_hpp
#define APDeleteProfileAPI_hpp

#include "APBaseAPI.h"

namespace ap {
    namespace api {
        class DeleteProfile : public RequestDelegate {
        public:
            static void call(const int & profile_id, const std::function<void(bool, std::string)> & callback);
        private:
            void send() const;

            DeleteProfile(const int & profile_id, const std::function<void(bool, std::string)> & callback);
        
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            
			int _profile_id;
            std::function<void(bool, std::string)> _callback;
            Base * _request = nullptr;
        };
    }
}

#endif /* APDeleteProfileAPI_hpp */
