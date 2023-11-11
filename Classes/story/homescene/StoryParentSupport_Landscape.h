//
//  StoryParentSupport_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/12/17.
//
//

#ifndef StoryParentSupport_Landscape_hpp
#define StoryParentSupport_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"

class StoryParentSupport_Landscape : public StoryParentSubPage_Landscape
{
public:
    CREATE_FUNC(StoryParentSupport_Landscape);
    static StoryParentSupport_Landscape * createPage();
    void onShow();
private:

    ~StoryParentSupport_Landscape();
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onCall(cocos2d::Ref * sender);
    void onMail(cocos2d::Ref * sender);
    void onJoin(cocos2d::Ref * sender);
    void didLoadFromCSB() override;
    void onBack(Ref *sender);
    void changeSuppportInformation();
    void pushCleverTapTrackingData();
    void onUpdate(cocos2d::Ref *sender);
    bool isNeedToUpdate(const std::string& curr_version, const std::string& newest_version);
private:
    bool isBack = false;

	std::string m_supportProperty = "none";

};

CREATE_READER_CLASS(StoryParentSupport_LandscapeReader, StoryParentSupport_Landscape);

#endif /* StoryParentSupport_Landscape_hpp */
