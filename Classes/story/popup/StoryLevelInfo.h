//
//  StoryLevelInfo.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryLevelInfo__
#define __StoryLevelInfo__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "StoryInfoObj.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class StoryLevelInfo : public cocos2d::ui::Layout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryLevelInfo);
    static StoryLevelInfo * createPage();
    void initWithData(StoryInfoObj storyInfo, bool isPortrait);
    
    void onShowHidePopUp(bool isShow);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExitTransitionDidStart();
    bool isShow = false, isRunning = false;

    void onRate(cocos2d::Ref *sender);
    void onClose(cocos2d::Ref *sender);
    
    Text *lexile_lb, *word_lb, *page_lb, *lexile_value_lb, *word_value_lb, *page_value_lb;
    ImageView *thumb;
    
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
    string classes_name = "levelinfo";
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryLevelInfoReader, StoryLevelInfo);

#endif /* defined(__ALTP__StoryLevelInfo__) */
