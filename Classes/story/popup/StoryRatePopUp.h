//
//  StoryRatePopUp.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryRatePopUp__
#define __StoryRatePopUp__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryRatePopUp : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryRatePopUp);
    static StoryRatePopUp * createPage(int type = 1);
    void onShowHidePopUp(bool isShow);
    
    enum RATE_TYPE {
        RATE_FREE = 1,
        RATE_BONUS = 2,
        RATE_ACTIVE = 3,
        RATE_THANKS = 4,
    };
    
private:
    void didLoadFromCSB(int type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onRate(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);
    
    Text *content, *title, *num_storieslb, *lbbtrate, *alertlb;
    int _type = 1;
    string classes_name = "ratepopup";
    void showNetwordDown();
    std::string getNameCSBAni();
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryRatePopUpReader, StoryRatePopUp);

#endif /* defined(__ALTP__StoryRatePopUp__) */
