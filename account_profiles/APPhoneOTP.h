//
//  APPhoneOTP.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APPhoneOTP_hpp
#define APPhoneOTP_hpp
#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;
#include "cocostudiomacro2.h"

class APPhoneOTP : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(APPhoneOTP);
    static APPhoneOTP * createView(const std::string & phone_number);
	bool show_close = false;
	bool is_onboard = true;
private:
    void didLoadFromCSB(const std::string & phone_number);
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(bool isVN);
	std::string _phone_number;
	bool is_vn;
    void onNext(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref* sender);
	void onResend(cocos2d::Ref* sender);
	void onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type);
	void insertOTP();
	TextField* textfield;
	float _timer;
	Text* textResend;
	Button* btn_resend;
	Button* btn_send;
	void countDown(float delta = 1);
	std::vector<Text*> text_numbers;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
	void showErrorAlert(std::string msg);
	int index_cusor;
	float count_time_cusor;
	int check = 0;
	void updateCusor(float dt);
};

CREATE_READER(APPhoneOTP);

#endif /* APPhoneOTP_hpp */
