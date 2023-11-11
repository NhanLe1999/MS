//
//  StorySubscribePopup.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StorySubscribePopup__
#define __StorySubscribePopup__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class StorySubscribePopupDelegate {
public:
    virtual void onSubscribeOk();
    virtual void onSubscribeCancel() {};
    virtual void onSubscribeFree();
};

class StorySubscribePopup : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StorySubscribePopup);
    
    enum SUB_POPUP_TYPE
    {
        SUB_STORY = 1,
        SUB_AUDIO = 2,
        SUB_LESSON = 3,
        SUB_AI_SPEAKING_LESSON =4,
    };
    
    static StorySubscribePopup * createSubscribePopup(int _id, int type);
    void onShowHidePopUp(bool isShow);
    void setDelegate(StorySubscribePopupDelegate *_delegate);
private:
    StorySubscribePopupDelegate *delegate = nullptr;
    void didLoadFromCSB(int _id, int type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onOk(cocos2d::Ref *sender);
    void onCancel(cocos2d::Ref *sender);
    void onFree(cocos2d::Ref *sender);
    int _type = 1;
    int action_id = 0;
    
    CC_SYNTHESIZE(std::string,m_startFrom,StartingFrom);
    
private:

    void pushCleverTapEvent(std::string i_clickedType);
};

CREATE_READER_CLASS(StorySubscribePopupReader, StorySubscribePopup);

#endif /* defined(__ALTP__StorySubscribePopup__) */
