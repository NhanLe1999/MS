//
//  APEmailOTP.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 5/30/19.
//

#ifndef APEmailOTP_hpp
#define APEmailOTP_hpp
#include "cocos2d.h"
using namespace cocos2d;
using namespace cocos2d::ui;
#include "cocostudiomacro2.h"

class APEmailOTP : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	enum class OTP_TYPE
	{
		EMAIL,
		PHONE
	};
    CREATE_FUNC(APEmailOTP);
    static APEmailOTP * createView(const std::string & inputText,const OTP_TYPE i_type, const int i_reponseCode);
private:
    void didLoadFromCSB(const std::string & inputText, const OTP_TYPE i_type, int i_reponseCode);
    void onEnter() override;
    void onExit() override;
    void changeDisplayLanguage(bool isVN);
	std::string m_inputText;
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
	void callAPI();
private:
	int			m_responeCode = -1;
	OTP_TYPE	m_OTPType;
};

CREATE_READER(APEmailOTP);

#endif /* APEmailOTP_hpp */
