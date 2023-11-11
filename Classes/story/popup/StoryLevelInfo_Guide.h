//
//  StoryLevelInfo_Guide.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryLevelInfo_Guide__
#define __StoryLevelInfo_Guide__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "StoryInfoObj.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryLevelInfo_Guide : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryLevelInfo_Guide);
    static StoryLevelInfo_Guide * createPage();
    void initWithData(StoryInfoObj storyInfo, bool isPortrait);
    void onShowHidePopUp(bool isShow);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;
    void onClose(cocos2d::Ref *sender);
    bool _isPortrait = false;
    Text *content, *title;
    void onLevelInfo(cocos2d::Ref *sender);
    StoryInfoObj _storyInfo;
    bool isClickLevelInfo = false;
    
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryLevelInfo_GuideReader, StoryLevelInfo_Guide);

#endif /* defined(__ALTP__StoryLevelInfo_Guide__) */
