//
//  StoryParentProfileDetail_Landscape.hpp
//  MJStory
//
//  Created by Minh on 08/07/2019.
//

#ifndef StoryParentProfileDetail_Landscape_hpp
#define StoryParentProfileDetail_Landscape_hpp

#include "cocostudiomacro2.h"
#include "APProfileCard.h"
#include "StoryParentSubPage_Landscape.h"
#include "APProfileDelegate.h"

class StoryParentProfileDetail_Landscape : public StoryParentSubPage_Landscape,public APProfileCardDelegate,public ap::delegate::ProfileDelegate {
public:
    CREATE_FUNC(StoryParentProfileDetail_Landscape);
    static StoryParentProfileDetail_Landscape * createPage();
	CC_SYNTHESIZE(cocos2d::Node*, m_delegate, ParentDelegate);
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    
    void onNewProfile(cocos2d::Ref * sender);
	void onEditProfile(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref *sender);
    void onBack(cocos2d::Ref *sender);
	void onClickProfileIcon(cocos2d::Ref* sender) override;
	void onProfileCreated(int profile_id) override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	cocos2d::ui::ListView* m_lv_profile;
	bool _edit_enabled;

	void pushCleverTapTrackingData(int profileID);

};

CREATE_READER(StoryParentProfileDetail_Landscape);

#endif /* StoryParentProfileDetail_Landscape_hpp */
