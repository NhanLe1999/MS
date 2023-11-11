//
//  PageMenuControl.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __PageMenuControl__
#define __PageMenuControl__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;
using namespace std;

class PageMenuControlDelegate {
    
public:
    virtual void onClickBtPlayPause(bool is_play) {};
    virtual void onClickBtClose() {};
    virtual void onClickBtFavorite(bool i_isFavorited) {};
};

class PageMenuControl : public cocos2d::ui::Layout, public PageMenuControlDelegate,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(PageMenuControl);
    void resetStateMenu();
    void setDelegate(PageMenuControlDelegate *delegate);
    void setClickEnable(bool is_click_enable);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow;

    void onClickButton(cocos2d::Ref *sender);
    void onOverlay(cocos2d::Ref * sender);
    Button *btfavorite;
    bool isPlay = true;
    bool isClickEnable = true;
    bool _isSharing = false;
    Button *btplay;
    Text *text_play;
    string class_name;
    PageMenuControlDelegate *_delegate = nullptr;
    
public:
//    void showAlertView(int index);
    void onShareFacebookCallback(bool succed);
};

CREATE_READER_CLASS(PageMenuControlReader, PageMenuControl);

#endif /* defined(__ALTP__PageMenuControl__) */
