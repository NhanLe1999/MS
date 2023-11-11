//
//  StoryFeedback.hpp
//  MJQuiz
//
//  Created by MonkeyStories.
//
//

#ifndef StoryFeedback_hpp
#define StoryFeedback_hpp

#include <stdio.h>
#include "CocoStudioMacro.h"
#include "HSBaseLayout.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d;
using namespace ui;

class StoryFeedbackDelegate {
public:
    virtual void onSentFeedback() {};
};


class StoryFeedback : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryFeedback);
    static StoryFeedback * createPage();
    void onShowHidePopUp(bool isShow);
    void setDelegate(StoryFeedbackDelegate *delegate);
private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    cocos2d::ui::Widget::ccWidgetEventCallback onLocateEventCallback(const std::string &callback_name) override;
    
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;

    void initPage();
    void onClose(cocos2d::Ref*sender);
    ui::TextField *textfield;
    ui::Text *content_label;
    void onTextfieldChanged(cocos2d::Ref *pSender, int event);
    
    void onSend(cocos2d::Ref *sender);
    vector<string> explode(const string& str, const char& ch);
    StoryFeedbackDelegate *_delegate;
};
CREATE_READER_CLASS(StoryFeedbackReader, StoryFeedback);

#endif /* StoryFeedback_hpp */
