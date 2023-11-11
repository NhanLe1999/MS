//
//  LessonBase.h
//  Demo
//
//  Created by Ha Anh Man on 3/9/16.
//
//

#ifndef __HSBaseLayout__
#define __HSBaseLayout__
#include "cocos2d.h"
#include "CocoStudioMacro.h"
#include "StoryAlert.h"
#include "JsonDump.h"
#include "ui/CocosGUI.h"
#include "WebService.h"
#include "network/HttpClient.h"
using namespace cocos2d;
using namespace std;
class HSBaseLayout : public cocos2d::ui::Layout, public StoryAlertDelegate
{
public:
    CREATE_FUNC(HSBaseLayout);
    void showAlertWithContent(std::string content,std::string btOk, std::string btCancel, int tag = 0, bool isOneButton = false, int fontSize = 28);
    StoryAlert * showAlertNetworkDown(int tag, bool isPortrait, bool isOnbt = false);
    void showAlertPlayGame(std::string content,std::string btOk, std::string btCancel, int tag);
    
    void onAlertOK(cocos2d::Ref * sender);
    void onAlertCancel(cocos2d::Ref * sender);
    
    void requestUrl(std::string url, int tagRequest);
    void requestPostDataWithUrl(std::string url, std::string data, int tagRequest);
    virtual void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest);
    virtual void onErrorRequestUrl(int tagRequest){}
    
    void showIndicator(bool isShow);
    void showPleaseWait(bool isShow, bool isPortrait = false);
    int matchRegex(const char *string);
    void trim(std::string& str, string character = " ");
    void urlEncode(string &s);
    void onExitTransitionDidStart();
    void releaseRequest();
    cocos2d::network::HttpRequest * request = nullptr;
    string classes_name;
    bool isLoadOffline = false;
    
};
#endif 
/* defined(__HSBaseLayout__) */
