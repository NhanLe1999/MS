//
//  APLoadUpdateAPI.hpp
//  stories_debug
//
//  Created by To Ngoc Linh on 7/30/19.
//

#ifndef APLoadUpdateAPI_hpp
#define APLoadUpdateAPI_hpp

#include "APBaseAPI.h"

#include "MSDownloader.h"

namespace ap {
    namespace api {
        class LoadUpdate : public RequestDelegate, public cocos2d::Ref {
        public:
            static void call(bool skip_update_info, const std::function<void(bool, std::string)> & callback = nullptr);
        private:
            LoadUpdate(bool skip_update_info, const std::function<void(bool, std::string)> & callback);
            
            void send() const;
            void onRequestSuccess(int tag, int code, const std::string & message, const rapidjson::Value & data) override;
            void onRequestError(int tag, int error_code, const std::string & message, const rapidjson::Value & data) override;
            void updateInfo(int tag, const rapidjson::Value &data);
            
            void onDownloadAndUnzipDone();
            
            bool _skip_update_info;
            std::string _user_info_filename = "";
            Base * _request = nullptr;
            std::function<void(bool, std::string)> _callback = nullptr;
        };
    }
}

#endif /* APLoadUpdateAPI_hpp */
