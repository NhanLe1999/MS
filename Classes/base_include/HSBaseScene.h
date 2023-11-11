//
//  LessonBase.h
//  Demo
//
//  Created by Ha Anh Man on 3/9/16.
//
//

#ifndef __HSBaseScene__
#define __HSBaseScene__
#include "cocos2d.h"
#include "CocoStudioMacro.h"
#include "spine/Json.h"
#include "network/HttpClient.h"

#include "StoryAlert.h"
#include "JsonDump.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace std;
class HSBaseScene : public cocos2d::Scene, public StoryAlertDelegate
{
public:
    CREATE_FUNC(HSBaseScene);
    void showAlertWithContent(std::string content,std::string btOk, std::string btCancel, int tag = 0, bool isOneButton = false, int fontSize = 28);
    void showAlertNetworkDown(int tag, bool isPortrait, bool isOnbt = false);
    void showAlertHaiLongApp(std::string content,std::string btOk, std::string btCancel, std::string btguiderate, int tag = 0);
    void showUpdateAppPopup(int tag, bool is_required = false);
    void onExit();
    void onAlertOK(cocos2d::Ref * sender);
    void onAlertCancel(cocos2d::Ref * sender);
    
    void requestUrl(std::string url, int tagRequest);
    virtual void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest);
    
    void requestPostDataWithUrl(std::string url, std::string data, int tagRequest);
    void showIndicator(bool isShow);
    int matchRegex(const char *string);
    void trim(std::string& str, string character = " ");
    void urlEncode(string &s);
    void onExitTransitionDidStart();
    void releaseRequest();
    cocos2d::network::HttpRequest * request = nullptr;
    string classes_name;
    bool isLoadOffline = false;
    void showPleaseWait(bool isShow, bool isPortrait);
};
#endif 
/* defined(__HSBaseScene__) */
