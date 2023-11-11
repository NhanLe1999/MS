//
//  APWaitView.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APWaitView_hpp
#define APWaitView_hpp

#include "cocostudiomacro2.h"

class APWaitView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(APWaitView);
	static APWaitView* createView(const std::string& message, bool have_btn = false, bool have_layout = true);
    
private:
	void onEnter() override;
	void onExit() override;
    
    void changeDisplayLanguage(int lang_id);
    void onConfirm(cocos2d::Ref * sender);
    void onSkip(cocos2d::Ref * sender);
    
	void didLoadFromCSB(const std::string & message, bool have_btn, bool have_layout);

	CC_SYNTHESIZE(std::function<void(bool)>,_confirm_callback, OnConfirmCallback);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void addDeviceId();
};

CREATE_READER(APWaitView);

#endif
