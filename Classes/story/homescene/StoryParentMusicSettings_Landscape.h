//
//  StoryParentMusicSettings_Landscape.hpp
//  MJStory
//
//  Created by AnhB Nguyen on 11/1/18.
//

#ifndef StoryParentMusicSettings_Landscape_hpp
#define StoryParentMusicSettings_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"

class StoryParentMusicSettings_Landscape : public StoryParentSubPage_Landscape{
public:
    CREATE_FUNC(StoryParentMusicSettings_Landscape);
    static StoryParentMusicSettings_Landscape * createPage();
    void onShow();
private:
	~StoryParentMusicSettings_Landscape();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    
    void onSwitchedButton(cocos2d::Ref * sender);
    void didLoadFromCSB() override;
    
    void onBack(Ref *sender);
    bool isBack = false;
    bool isSwitch = true;
	void pushCleverTapEvents();
	bool m_beforeState =false;
};

CREATE_READER_CLASS(StoryParentMusicSettings_LandscapeReader, StoryParentMusicSettings_Landscape);

#endif /* StoryParentMusicSettings_Landscape_hpp */
