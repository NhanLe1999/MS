//
//  StoryEndingPage.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryEndingPage__
#define __StoryEndingPage__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "HSBaseLayout.h"
#include "MJRequest.h"

using namespace cocos2d;
using namespace cocos2d::ui;
class StoryEndingPage : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryEndingPage);
    static StoryEndingPage * createPage();
    void runAnimShow();
private:
    void preload();
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    bool isShow;

    void onClickButton(cocos2d::Ref *sender);
    ImageView *bg;
    
    std::vector<Layout *> list_face_rate;
    void addFaceToRate();
    int ani_count_done = 0;
    void onRate(cocos2d::Ref *sender);
    Text *queslb, *suggestlb;
    std::vector<Text *> list_face_text;
    void showTextQues();
    void onSuggestStories();
    bool isTapRate = false;
    Button *btclose;
    cocos2d::ValueVector list_stories_choose;
    ValueVector choose4StoriesSuggest();
    ValueVector getStoriesFromPopular(int num_stories);
    ValueVector getStoriesFromNew(int num_stories);
    ValueVector filterStoriesCHUADOC(ValueVector list_stories, ValueVector list_filter);
    ValueVector filterStoriesWithListStories(ValueVector list_stories, ValueVector list_filter);
    ValueVector list_choose;
    void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest) override;
    void showRatePopup();
    void showFeedbackPopup();
    bool is_hide_face_rate = true;
    void onStoryRecomentClicked(cocos2d::Ref *sender);
    
    void onRecommendTab(cocos2d::Ref * sender);
    
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryEndingPageReader, StoryEndingPage);

#endif /* defined(__ALTP__StoryEndingPage__) */
