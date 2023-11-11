//
//  StoryData.h
//  MonkeyJunior
//
//  Created by developer on 2/29/16.
//
//

#ifndef StoryData_h
#define StoryData_h
#include "cocos2d.h"
using namespace std;
USING_NS_CC;

class StoryData
{
public:
    static StoryData* getInstance();
    bool isWidth = false;
    float contentScaleXY = 1;
    //cocos2d::Size screenSize;
    ValueMap configData ;//= {};
    //lay thong tin cua thang hdr
    bool isHdr = false;
    int isVN = 0, edu_alert_tag = 0, deviceType = 0;
    bool alert_show = false, client_downloading = false;
    
private:
    StoryData(){};
    static StoryData* _instance;
};

#endif /* StoryData_h */
