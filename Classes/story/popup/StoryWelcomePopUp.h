//
//  StoryWelcomePopUp.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryWelcomePopUp__
#define __StoryWelcomePopUp__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryWelcomePopUp : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryWelcomePopUp);
    static StoryWelcomePopUp * createPage();
    void onShowHidePopUp(bool isShow);
    bool _isPortrait = false;
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onSubscribe(cocos2d::Ref *sender);
    void onFree(cocos2d::Ref *sender);
    
    ImageView *_bg;
    Text *content, *title, *titlesub, *titlefree;
    int index_choose = 0;
    string classes_name = "popup";
    std::function<void(bool isShow)> _callbackShowPop = nullptr;
    void setCallbackShowPop();

private:

    void pushCleverTapEvent(std::string i_clickedType);
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryWelcomePopUpReader, StoryWelcomePopUp);

#endif /* defined(__ALTP__StoryWelcomePopUp__) */
