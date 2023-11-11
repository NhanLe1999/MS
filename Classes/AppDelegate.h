#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"
//#include "SimpleAudioEngine.h"
#include "network/HttpClient.h"
#include "MJRequest.h"
/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application, public mj::RequestDelegate
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
    void applicationHandleOpenURL(const std::string& url);
    
    /**
     @brief  Register custom CSB readers
     */
    void registerCSBReaders();
    
    void setupSearchPath();
    
    void copytodocument();
    
//    void pushRateStoryToServer();
//    void onFinishPushRateStoryToSever(cocos2d::ValueMap map_response);
    cocos2d::network::HttpRequest *request = nullptr;
    bool checkVersionLess(std::vector<std::string> ver1, std::vector<std::string> ver2);
    void upgradeVersionStory();
    void displayCheckRateApp();
    
    void pushTokenToServer();
//    void callAPILoadUpdate();
    void loadNotification();
    void applicationWillResignActive();
    void applicationDidBecomeActive();
    void applicationWillTerminate();
    void setDataAppinbox(std::vector<std::string> value);
    void addMessageAppinbox();
    std::vector<std::string> getDataAppinbox();
    std::vector<std::string> _valueAppinbox = {};
    std::vector<std::pair<std::string, cocos2d::Value>> valueNotificationAppinbox = {};
    std::vector<std::pair<std::string, cocos2d::Value>> getValueNotificationAppinbox();

    void setMessageOTP(std::string message);
    std::string getMessage();

private:
    std::string _message;
    mj::Request * _request, *_request_appinfo;
    void requestGetDeviceID();
    virtual void onRequestSuccess(int tag, rapidjson::Value data) override;
    virtual void onRequestFailed(int tag, int error_code, std::string error_string, rapidjson::Value data) override;
    
//    void callbackLoadUpdate(rapidjson::Value data);

//    void callAPIAppInfo();
//    void callbackAppInfo(rapidjson::Value data);

    void setConfigApp();
    
    void fixV220();
    
};

#endif // _APP_DELEGATE_H_

