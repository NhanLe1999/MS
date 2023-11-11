//
//  AccountPopup.hpp
//  MonkeyMath
//
//  Created by To Ngoc Linh on 5/16/18.
//

#ifndef AccountPopup_hpp
#define AccountPopup_hpp

#include "cocostudiomacro2.h"

class AccountPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    struct APPopupInfo {
        std::string title;
        std::string left;
        std::string right;
    };
    
    CREATE_FUNC(AccountPopup);
    static AccountPopup * show(APPopupInfo info, std::function<void()> left_callback = nullptr, std::function<void()> right_callback = nullptr, int z_order = 0);
	static AccountPopup * createView(APPopupInfo info);
	std::function<void()> _left_callback;
	std::function<void()> _right_callback;
	bool _is_left_null_callback = false;
	bool _is_right_null_callback = false;
private:
    void didLoadFromCSB(APPopupInfo info);
    void onEnter() override;
    void doAppear();
    void doDisappear();
    void onLeft(cocos2d::Ref * sender);
    void onRight(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(AccountPopup);

#endif /* AccountPopup_hpp */
