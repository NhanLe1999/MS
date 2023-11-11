//
//  base_include.h
//  MonkeyJunior
//
//  Created by Ha Anh Man on 4/13/16.
//
//

#ifndef MonkeyJunior_base_include_h
#define MonkeyJunior_base_include_h

#include "cocos2d.h"
#include "GlobalConstants.h"
#include "StoryData.h"
#include "GlobalFunc.h"
#include "CocoStudioMacro.h"
#include <SimpleAudioEngine.h>
#include "MJDefault.h"
using namespace cocos2d;
using namespace std;
using namespace ui;

class EduHttpRequestDelegate {
public:
    virtual void connectOtherServer(){};
    virtual void eduShowAlert(int tag,string message, string okbutton = "OK", string cancelbutton = "", bool callback = false, bool showExit = true, bool xanhdo = true){};
    virtual void disableAllButtonSub(bool disable = true){};
    virtual void requestCallback(ValueMap response){};
};

#endif
//
