#include "AppsFlyerManager.h"

#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32

USING_NS_CC;

#define FORMAT_UPLOAD_APPSFLYER_API "{\"appsflyer_id\":\"%s\","\
                                  "\"eventTime\":\"%s\","\
                                    "\"eventName\":\"%s\","\
                                    "\"eventValue\":%s}"

#define FORMAT_EVENT_VALUE_APPSFLYER_API "\"%s\":\"%s\""
AppsFlyerManager::AppsFlyerManager()
{
    init();
}

void AppsFlyerManager::init()
{
    this->_isCustomerUIDInitialized = false;
    CCLOG("Init appsflyer manager");
}

bool AppsFlyerManager::isCUIDInitialized(){
    return this->_isCustomerUIDInitialized;
}

void AppsFlyerManager::logEventAppsFlyer(const std::string& eventName, cocos2d::ValueMap values)
{
#if !DISABLE_APPSFLYER

    
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if (!_isCustomerUIDInitialized) {
        storePendingEventAppsFlyer(eventName, values);
    }
    else {
        AppsFlyerX::logEvent(eventName, values);
    }
#endif
#endif
}

void AppsFlyerManager::logEventAppsFlyer(const std::string& eventName) {
    logEventAppsFlyer(eventName, { std::make_pair("default_value",cocos2d::Value(true)) });
}

void AppsFlyerManager::storePendingEventAppsFlyer(std::string eventName, cocos2d::ValueMap values)
{
    _eventAppsFlyerStacks.push_back({ eventName,values });
}

void AppsFlyerManager::storePendingEventAppsFlyer(std::string eventName)
{
    _eventAppsFlyerStacks.push_back({ eventName,{std::make_pair("default_value",cocos2d::Value(true))} });

}

void AppsFlyerManager::logPendingEventsAppsFlyer()
{
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

    Director::getInstance()->getScheduler()->schedule([=](float dt) {

        if (!_eventAppsFlyerStacks.empty()) {

            auto value = *_eventAppsFlyerStacks.begin();

            _eventAppsFlyerStacks.erase(_eventAppsFlyerStacks.begin());

            logEventAppsFlyer(value.first, value.second);

            return;
        }
        else
        {
            _eventAppsFlyerStacks.clear();
            Director::getInstance()->getScheduler()->unschedule("log_pending_events_appsflyer", cocos2d::Application::getInstance());
        }

        }, cocos2d::Application::getInstance(), 1.0f, false, "log_pending_events_appsflyer");
#endif
}

void AppsFlyerManager::sendAPI(const std::string& eventName, cocos2d::ValueMap values)
{
    std::string appsflyerJsonString = StringUtils::format(
        FORMAT_UPLOAD_APPSFLYER_API,
        getAppsFlyerUID().c_str(),
        std::to_string(mj::helper::getTime()).c_str(),
        eventName.c_str(),
        convertValueToString(values).c_str()
    );

    auto request = new cocos2d::network::HttpRequest();
    request->setRequestType(cocos2d::network::HttpRequest::Type::POST);
    request->setUrl("https://api2.appsflyer.com/inappevent/id1236664909");
    request->setRequestData(appsflyerJsonString.c_str(), appsflyerJsonString.size());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    request->setHeaders({
        "authentication:HEX6DVgSUPuyhSY6Z3PDRh",//dev key
        "Content-Type: application/json"
        });
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    request->setHeaders({
        "authentication:QzJHt38rkzMwZVj3mhHy4o",//dev key
        "Content-Type: application/json"
        });
#endif
    
    request->setResponseCallback([](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response) {
        auto buffer = *response->getResponseData();
        auto buffer_string = std::string(buffer.begin(), buffer.end());
        if (!response->isSucceed()) {
        
        }
        else {

        }
        });
    cocos2d::network::HttpClient::getInstance()->send(request);
    request->release();


    return;

}

void AppsFlyerManager::responseCallback(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
{

}

std::vector<std::pair<std::string, cocos2d::ValueMap>> AppsFlyerManager::getEventAppsFlyerStacks(){
    return  _eventAppsFlyerStacks;
}

std::string AppsFlyerManager::getAppsFlyerUID()
{
	std::string appsFlyerUID;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)||(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    appsFlyerUID = AppsFlyerX::getAppsFlyerUID();
#endif
    if (appsFlyerUID.empty()) {
        return "1648548249989-6974120";
    }
    return appsFlyerUID;
}

std::string AppsFlyerManager::convertValueToString(cocos2d::ValueMap values)
{
    std::string s = "";
    int i = 1;
    for (auto v: values) {
        if (i == 1) { 
            s += "{"; 
        }

        if (i > 1 && i <= values.size()) {
            s += ",";
        }
        s += StringUtils::format(FORMAT_EVENT_VALUE_APPSFLYER_API,
            v.first.c_str(),
            v.second.asString().c_str());

        if (i == values.size()) {
            s += "}";
        }
        i++;
    }
    return s;
}


void AppsFlyerManager::setAppsFlyerCUID(const std::string& uId){
    if(uId.empty()) return;
    CCLOG("Appsflyer customer ID: %s", uId.c_str());
#if !DISABLE_APPSFLYER
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)    
    AppsFlyerX::setCustomerUserID(uId);
    _isCustomerUIDInitialized = true;
#endif
#endif
}

void AppsFlyerManager::startAppsFlyer(){

#if !DISABLE_APPSFLYER
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID && !CC_PLATFORM_HUAWEI)
    auto ctID = CleverTapManager::GetInstance()->getCTIdentity();
    this->setAppsFlyerCUID(ctID);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	AppsFlyerX::setAppsFlyerDevKey("HEX6DVgSUPuyhSY6Z3PDRh");
    AppsFlyerX::setAppleAppID("1236664909");
    AppsFlyerX::setUseUninstallSandbox(true);
    AppsFlyerX::disableCollectASA(true);
    AppsFlyerX::setIsDebug(true);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AppsFlyerX::setAppsFlyerDevKey("QzJHt38rkzMwZVj3mhHy4o");
#endif
    AppsFlyerX::registerUninstall("<TOKEN>");
    AppsFlyerX::start();
#endif

#endif
}

#endif
#endif
