

#include "MsNewRequest.h"

#include "MJHelper.h"
#include "MJDefault.h"
#include "MJDefinitions.h"
#include "cocos2d.h"


namespace ms {
    namespace newApi {
        void Request::setCallBackRequest(std::function<void(std::string, bool)> callback) {
            _callbackSucess = callback;
        }
    
        void Request::onRequestSucess(std::string response, bool isSucess)
        {
            if(_callbackSucess)
            {
                _callbackSucess(response, isSucess);
            }
        }

        void Request::setCallBackRequestError(std::function<void(int, std::string, rapidjson::Value&)> callbackError)
        {
            _callbackError = callbackError;
        }

        void Request::onRequestError(int code, std::string errorMessage, std::string respone)
        {
            rapidjson::Value a(rapidjson::kObjectType);
            if(_callbackError)
            {
                _callbackError(code, errorMessage, a);
            }
        }
    
        void Request::setUrl(std::string url)
        {
            _url = url;
        }
    
        void Request::setNewRequest(bool newRequest)
        {
            _isNewRequest = newRequest;
        }
    
        void Request::setNotRequestApiLogOut(bool isLogOut)
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            _isNewRequestLogout = isLogOut;
#else
            _isNewRequestLogout = false;
#endif
        }
    
        bool Request::isNewRequest()
        {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
            return _isNewRequest && _isNewRequestLogout;
#else
            return false;
#endif
        }
    
        void Request::send()
        {
            std::string header1 = "", header2 = "";

            if(_listHeader.size() == 2)
            {
                header1 = _listHeader[0];
                header2 = _listHeader[1];
            }

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
        cocos2d::JniHelper::callStaticVoidMethod("org.cocos2dx.cpp.ReadRequest","createReadRequest", _url, _typeRequest, _parammeters, header1, header2);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#endif
        }
    
        void Request::setTypeRequest(std::string type)
        {
            _typeRequest = (type == "GET") ? 0 : 1;
        }

        void Request::setParamTypeRequestPost(std::string parmam)
        {
            _parammeters = parmam;
        }

        void Request::setListHeaders(std::vector<std::string> listHeaders)
        {
            _listHeader = listHeaders;
        }
        void Request::insertHeader(std::string header)
        {
            _listHeader.push_back(header);
        }
        std::vector<std::string> Request::getListHeaders()
        {
            return _listHeader;
        }
        Request::Request()
        {
            
        }
    
        
    }
}
