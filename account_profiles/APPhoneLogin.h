//
//  APPhoneLogin.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APPhoneLogin_hpp
#define APPhoneLogin_hpp
#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;
#include "cocostudiomacro2.h"

class APPhoneLogin : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APPhoneLogin);
    static APPhoneLogin * createView(const bool & is_confirm_otp = false);
	void onVerifyOTPSuccess();
	bool is_onboard = false;
private:
	bool _is_confirm_otp;
    void didLoadFromCSB(const bool & is_confirm_otp = false);
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(bool isVN);
    void onNext(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref* sender);
	void onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type);
	void showErrorAlert(std::string msg);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
	TextField* txt_phone;
	Button* btn_send;
	void update(float delta = 1);
};

CREATE_READER(APPhoneLogin);

#endif /* APPhoneLogin_hpp */
