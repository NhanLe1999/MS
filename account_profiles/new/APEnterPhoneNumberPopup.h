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

class APEnterPhoneNumberPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	
	CREATE_FUNC(APEnterPhoneNumberPopup);

	static APEnterPhoneNumberPopup* createView();

private:

	void onEnter() override;
	void onExit() override;
    
    void onSend(cocos2d::Ref * sender);

	bool didLoadFromCSB() override;

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
};

CREATE_READER(APEnterPhoneNumberPopup);

