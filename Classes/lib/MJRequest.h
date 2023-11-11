//
//  MJRequest.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/25/17.
//
//

#ifndef MJRequest_hpp
#define MJRequest_hpp

#include <stdio.h>
#include "network/HttpClient.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "cocos2d.h"

#define kmjRequestErrorNoResponse 1
#define kmjRequestErrorInvalidJSON 2
#define kmjRequestErrorSuccessButDataFailed 3
#define kmjRequestErrorExpectedData 4
#define kmjRequestErrorJSONCompromised 5
#define kmjRequestErrorTimeout 6

#define DEFAULT_REQUEST_TIMEOUT 50.0f

#define CODE_ACTIVE_LICENSE_WARNING 0x100090
#define CODE_ACTIVE_LICENSE_DENY 0x100091

namespace mj {
    class RequestDelegate {
    public:
        virtual int onRequestRaw(int tag, std::string data) { return 0; };
        virtual void onRequestSuccess(int tag, rapidjson::Value data) = 0;
//        virtual void onRequestFailed(int tag, int error_code, std::string error_string) = 0;
        virtual void onRequestFailed(int tag, int error_code, std::string message, rapidjson::Value data) = 0;
    };

    class Request : public cocos2d::Ref {
    public:
        enum ActionType {
            REGISTER_DEVICE,
            REGISTER_PUSH_TOKEN,
            REGISTER_INSTALL,
            SYNC,
            ACTIVE_LICENSE,
            UPDATE_LICENSE,
            ONEPAY,
            REGISTER_COD,
            LOAD_UPDATE,
            INAPP_INFO,
            NOTIFICATION,
            HOME,
            APPINFO,
            VERIFY_RECEIPT,
            GET_STORY,
            VALIDATE_COUPON,
            RATE_STORY,
            CATEGORY,
            ALL_STORIES,
            HOME_V2,
            LEVEL_INFO,
            RATE_APP,
            SENT_DOWNLOAD,
            SENT_FEEDBACK,
            GET_AUDIOBOOK,
            ALL_AUDIOBOOKS,
            ALL_LESSONS,
            LESSON_DETAILS,
            ALL_GAMES,
            ALL_NEW_CATEGORIES,
            TRACK_LESSON,
            AUDIOBOOK_SERIES,
            SAMPLE_STORIES,
            APP_INFO_V2,
            APP_INFO_ZIP,
			AUDIOBOOK,
            AUDIOBOOK_ZIP,
            AUDIOBOOK_DETAIL,
            STORY,
            STORY_ZIP,
            BOOK_LOG,
            DOWNLOAD_LOG,
            REMOVE_LICENSE,
            FORGOT_LICENSE,
            CHANGE_LICENSE,
            SAVE_MAIL,
            VERIFY_INFO_LICENSE,
            VERIFY_INFO_VIRAL,
            SHARE_VIRAL,
            SYNC_AUDIO,
            STORY_DETAIL,
            ACTIVE_LICENSE_V2,
            APP_INFO_PHONIC,
            MATERIAL_ZIP,
            PATH_INSTALL_ZIP,
            LOAD_UPDATE_INFO,
            CATEGORIES_ZIP,
            GAMELIST_ZIP,
            LESSON_LIST_ZIP, 
            LESSON_INTERATIVE_ZIP,
            ACTIVITIES_LIST_ZIP,

        };

        enum RequestType {
            GET,
            POST
        };

        typedef std::map<std::string, std::string> Data;

        Request(ActionType action, Data data = {}, RequestType type = GET, float timeout = DEFAULT_REQUEST_TIMEOUT);
        ~Request();

        CC_SYNTHESIZE(RequestDelegate *, _delegate, Delegate);

        static void setHost(std::vector<std::string> hosts);

        void setHeaders(std::vector<std::string> headers);
        void send();
        void setTag(int tag);

        static std::string buildBaseParams();
        static void addParam(std::string & param, std::string key, std::string value);

        static std::string getOnePayURL(std::string product_id);

        static std::string _onepay_url;
        
        static std::string getViralSharingURL(std::string name);
        
        std::string getURL();
        
        void setCallbackNewReadRequest(std::string responData, bool isSucess);
        
        void setUrl(std::string url) {
            _url = url;
        };

        void setCallbackSucess();
        void setCallbackError();

    private:
        int _tag;
        float _timeout;
        std::string _url;
        ActionType _action;
        cocos2d::network::HttpRequest * _request = nullptr;
        cocos2d::network::HttpRequest::Type _request_type;

        std::string _typeRequest = "";

        Data _data;
        int _server_id;
        static const std::map<int, std::string> _action_map;
        static std::vector<std::string> _servers;
        std::string _data_post_str;
        std::string buildURL();

        void onRequestTimeout();

        void onRequestError(int error_code, std::string error_string, rapidjson::Value data);
        void onRequestSuccess(rapidjson::Value data);

        std::string _timeout_key;
        std::vector<std::string> _headers;
        static bool is_phonic;
        std::function<void(std::string,bool)> _callbackSucess = nullptr;
        std::function<void(int, std::string, rapidjson::Value&)> _callbackError = nullptr;

		std::string _param;
    };
}

#endif /* MJRequest_hpp */
