//
//  APLoginView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APLoginView_hpp
#define APLoginView_hpp

#include "cocostudiomacro2.h"
#include "StoryAlert.h"

class APLoginView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    
    enum class SCREEN_TYPE
    {
        LOGIN,
        REGISTER
    };
    
    CREATE_FUNC(APLoginView);
    static APLoginView * createView(SCREEN_TYPE i_type);
    
    void onFacebook(cocos2d::Ref * sender,std::string i_newAccessToken = "");
    void onGoogle(cocos2d::Ref * sender,bool isGmail = true, std::string i_newAccessToken = "");
    void onPhone(cocos2d::Ref * sender);
    void onQuickLogin(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    void onSkipLogin(cocos2d::Ref * sender);
	void onLoginSuccess(std::string type);
private:
    void didLoadFromCSB(SCREEN_TYPE i_type);
    void initQuickLoginButton();

    void onEnter() override;
    void onExit() override;
    
    void changeDisplayLanguage(int lang_id);
	void addDeviceId();
    void onClose(cocos2d::Ref * sender);
    
    void onLoggedIn();
    void onSignOut(Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    int _retry = 0;
	bool _is_vn;
	CC_SYNTHESIZE(std::function<void()>, _login_callback, OnLoginCallback);
    
	static void pushCleverTapTrackingEvent(std::string i_loginType,bool i_isSuccess,bool i_isUpgrade, std::string i_failedReason);
private:
    
    CC_SYNTHESIZE(cocos2d::Node* , m_currentScreen, CurrentScreen);

};

CREATE_READER(APLoginView);

#endif /* APLoginView_hpp */
