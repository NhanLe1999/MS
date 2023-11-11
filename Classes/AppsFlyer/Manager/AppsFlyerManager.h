#pragma once
#include "cocos2d.h"
#include "BaseSingleton.h"
#include "MJDefinitions.h"
#include "MJHelper.h"
#include "network/HttpClient.h"
#include "GlobalFunc.h"
#include "CleverTapManager.h"
#if ((CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)||(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)) && !DISABLE_APPSFLYER
#include "AppsFlyerX.h"

//#include "RequestWithFile.h"
//#include "MkHelperUtils.h"
#include"MJHelper.h"
#define APPSFLYER_MANAGER AppsFlyerManager::getInstance()

class AppsFlyerManager : public BaseSingleton<AppsFlyerManager>
{
private:
	std::vector<std::pair<std::string, cocos2d::ValueMap>> _eventAppsFlyerStacks;
    cocos2d::network::HttpRequest* _request = nullptr;
public:

    AppsFlyerManager();

    void init();
    
    void logEventAppsFlyer(const std::string& eventName, cocos2d::ValueMap values);

    void logEventAppsFlyer(const std::string& eventName);

    void storePendingEventAppsFlyer(std::string eventName, cocos2d::ValueMap values);

    void storePendingEventAppsFlyer(std::string eventName);

    void logPendingEventsAppsFlyer();

    void sendAPI(const std::string& eventName, cocos2d::ValueMap values);

    void responseCallback(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

	std::vector<std::pair<std::string, cocos2d::ValueMap>> getEventAppsFlyerStacks();

    std::string getAppsFlyerUID();

    std::string convertValueToString(cocos2d::ValueMap values);

	bool isCUIDInitialized();
    
    void setAppsFlyerCUID(const std::string& uId);

	void startAppsFlyer();

private:
    bool _isCustomerUIDInitialized;

};
#endif
