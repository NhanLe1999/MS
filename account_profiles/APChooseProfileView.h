//
//  APChooseProfileView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APChooseProfileView_hpp
#define APChooseProfileView_hpp

#include "cocostudiomacro2.h"
#include "APProfileCard.h"
#include "APProfileDelegate.h"


class APChooseProfileView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol,public APProfileCardDelegate, public ap::delegate::ProfileDelegate {
public:
    CREATE_FUNC(APChooseProfileView);
    static APChooseProfileView * createView();
    
    void onNewProfile(cocos2d::Ref * sender);
    void hideCreateNewProfilesButton();
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    
    void changeDisplayLanguage();
	void onClose(cocos2d::Ref *sender);
    void onLock(cocos2d::Ref *sender);
	void onClickProfileCard(cocos2d::Ref* sender) override;
	void onProfileCreated(int profile_id) override;
	void onProfileChoosed(int profile_id, bool is_new);
    void goHome();
    void showWaittingView();
    void removeWaittingView();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;


	//CC_SYNTHESIZE(ap::delegate::ChooseProfileDelegate *, _delegate, Delegate);
	cocos2d::ui::ListView* m_lv_profile;
	bool _edit_enabled;
    bool _is_welcome_new;
};

CREATE_READER(APChooseProfileView);

#endif /* APChooseProfileView_hpp */
