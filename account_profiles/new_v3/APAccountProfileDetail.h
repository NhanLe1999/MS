//
//  APAccountProfileDetail.h
//  MJStory
//
//  Created by Hieu Nguyen on 8/8/20.
//

#ifndef APAccountProfileDetail_h
#define APAccountProfileDetail_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "APAccountProfileCard.h"
#include "StoryParentSubPage_Landscape.h"
#include "APProfileDelegate.h"

#define name_account_profile_detail "name_account_profile_detail"

class APAccountProfileDetail : public StoryParentSubPage_Landscape,public APAccountProfileCardDelegate,public ap::delegate::ProfileDelegate {
public:
    CREATE_FUNC(APAccountProfileDetail);
    static APAccountProfileDetail * createPage();
    CC_SYNTHESIZE(cocos2d::Node*, m_delegate, ParentDelegate);
private:
    ~APAccountProfileDetail();
    void didLoadFromCSB() override;
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
    
    //for clevertap tracking.
    bool   m_isEditProfileClicked = false;
    
    bool   m_isProfileEditedSuccessfully = false;
};

CREATE_READER(APAccountProfileDetail);

#endif /* APAccountProfileDetail_h */
