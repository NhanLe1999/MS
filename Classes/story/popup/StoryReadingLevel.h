//
//  StoryReadingLevel.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryReadingLevel__
#define __StoryReadingLevel__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "StoryInfoObj.h"
#include "HSBaseLayout.h"

using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryReadingLevel : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryReadingLevel);
    static StoryReadingLevel * createPage();
    void initWithData(bool isPortrait);
    
    void onShowHidePopUp(bool isShow);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    bool isShow = false, isRunning = false;

    void onRate(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);
    
    Text *titlelb;
    
    ui::ScrollView *scroll_content;
    ListView *listview_level;
    void displayListLevel();
    void displayContentOfLevel(int level_id);
    Layout* levelButtonWith(int tag, int level_id, bool isSelect = false);
    void onChooseLevel(cocos2d::Ref *sender);
    int index_selected = 0;
    ValueVector list_levelid;
    
    Label *content_level_lb;
    ValueVector list_level_des;
    StoryInfoObj _storyInfo;
    bool _isPortrait = false;
    
    void displayWelcome();
    void displayReadingLevel();
    bool isNextClicked = false;
    void onNext(cocos2d::Ref *sender);
    Text *welcome_titlelb;
    Label *welcome_contentlb;
    bool isRequest = false;
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryReadingLevelReader, StoryReadingLevel);

#endif /* defined(__ALTP__StoryReadingLevel__) */
