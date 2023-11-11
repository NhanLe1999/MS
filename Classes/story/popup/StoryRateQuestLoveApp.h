//
//  StoryRateQuestLoveApp.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryRateQuestLoveApp__
#define __StoryRateQuestLoveApp__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class StoryRateQuestLoveAppDelegate {
public:
    virtual void onRateQuestLoveAppOk() {};
    virtual void onRateQuestLoveAppCancel() {};
};

class StoryRateQuestLoveApp : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryRateQuestLoveApp);
    static StoryRateQuestLoveApp * createRateQuestLoveApp();
    void onShowHidePopUp(bool isShow);
    void setDelegate(StoryRateQuestLoveAppDelegate *_delegate);
private:
    StoryRateQuestLoveAppDelegate *delegate = nullptr;
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onOk(cocos2d::Ref *sender);
    void onCancel(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);
    void showNetwordDown();
    
    int _type = 1;
    
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryRateQuestLoveAppReader, StoryRateQuestLoveApp);

#endif /* defined(__ALTP__StoryRateQuestLoveApp__) */
