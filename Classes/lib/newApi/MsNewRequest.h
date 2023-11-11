
#ifndef MsNewRequest_hpp
#define MsNewRequest_hpp

#include "MJRequest.h"
#include "network/CCDownloader.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define TURN_ON_NEW_REQUEST true
#else
#define TURN_ON_NEW_REQUEST true
#endif



namespace ms {
    namespace newApi {
        class Request {
		public:
            
            static Request& getInstance() {
                static Request sharedManager;
                return sharedManager;
            };
            
            void setCallBackRequest(std::function<void(std::string , bool)> callback = nullptr);
            void onRequestSucess(std::string response, bool isSuceess);
            void setUrl(std::string url);
            void send();
            void setTypeRequest(std::string type);
            void setParamTypeRequestPost(std::string param);
            void setListHeaders(std::vector<std::string> listHeaders);
            void insertHeader(std::string header);
            std::vector<std::string> getListHeaders();
            bool isNewRequest();
            void setNewRequest(bool newRequest);
            void setNotRequestApiLogOut(bool isLogOut);
            void setCallBackRequestError(std::function<void(int, std::string, rapidjson::Value&)> callbackError = nullptr);
            void onRequestError(int code, std::string errorMessage, std::string respone);
        private:
            Request();
            std::string _url = "";
            std::function<void(std::string, bool)> _callbackSucess;
            std::function<void(int, std::string, rapidjson::Value&)> _callbackError;

            int _typeRequest = -1;

            std::string _parammeters = "";
            std::vector<std::string> _listHeader;
            
            bool _isNewRequest = true;
            bool _isNewRequestLogout = true;
        };
    }
}
#endif
