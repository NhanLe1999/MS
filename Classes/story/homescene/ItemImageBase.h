//
//  LessonBase.h
//  Demo
//
//  Created by Ha Anh Man on 3/9/16.
//
//

#ifndef __ItemImageBase__
#define __ItemImageBase__
#include "cocos2d.h"
#include "CocoStudioMacro.h"
#include "network/HttpClient.h"
//#include "Alert.h"
#include "JsonDump.h"
#include "ui/CocosGUI.h"
#include "WebService.h"

using namespace cocos2d;
using namespace std;
class ItemImageBase : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(ItemImageBase);
    void onExitTransitionDidStart();
    void onExit();
    void loadImageFromURL(string url, string img_id);
    void onImageDownloaded(cocos2d::network::HttpClient * pClient, cocos2d::network::HttpResponse * pResponse);
    virtual void onFinishLoadImage();
    std::string image_id = string("");
    std::string getFileNameFromUrl(string url);
    
    void loadImageFromURLNotShow(string url);
    void onloadImageFromURLNotShowDone(cocos2d::network::HttpClient * pClient, cocos2d::network::HttpResponse * pResponse);
    std::string image_notshow = string("");
    cocos2d::network::HttpRequest * request = nullptr, *request_loadIMG = nullptr;
};
#endif
/* defined(__ItemImageBase__) */
