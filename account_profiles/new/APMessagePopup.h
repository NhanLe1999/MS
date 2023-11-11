//
//  APEnterPhoneNumberPopup.hpp
//  MJStory
//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "APPopupBase.h"
using namespace cocos2d;
using namespace cocos2d::ui;

class APMessagePopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol 
{

public:

	enum class MESSAGE_TYPE
	{
		SUCCESS,
		UNSUCCESS
	};

	CREATE_FUNC(APMessagePopup);

	static APMessagePopup* createView(MESSAGE_TYPE i_type = MESSAGE_TYPE::SUCCESS,std::string i_msg = "");

private:

	void onFinish(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(MESSAGE_TYPE i_type, std::string i_msg) ;

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
};

CREATE_READER(APMessagePopup);

