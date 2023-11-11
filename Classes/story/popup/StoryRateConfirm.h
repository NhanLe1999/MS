//
//  StoryRateConfirm.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryRateConfirm__
#define __StoryRateConfirm__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class StoryRateConfirmDelegate {
public:
    virtual void onRateConfirmOk() {};
};

class StoryRateConfirm : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryRateConfirm);
    static StoryRateConfirm * createRateConfirm();
    void onShowHidePopUp(bool isShow);
    void setDelegate(StoryRateConfirmDelegate *_delegate);
private:
    StoryRateConfirmDelegate *delegate = nullptr;
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onOk(cocos2d::Ref *sender);
    void onCancel(cocos2d::Ref *sender);
    void onGuide(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);
    void showNetwordDown();
    
    
    int _type = 1;
    
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryRateConfirmReader, StoryRateConfirm);

#endif /* defined(__ALTP__StoryRateConfirm__) */
