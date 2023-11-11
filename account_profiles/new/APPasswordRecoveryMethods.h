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

class APPasswordRecoveryMethods : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	
	enum class RECOVERY_FROM
	{
		EMAIL,
		PHONE
	};

	CREATE_FUNC(APPasswordRecoveryMethods);

	static APPasswordRecoveryMethods* createView(ReponseData data, RECOVERY_FROM  i_from);

private: //private functions

	void onExit() override final;

	void onContinute(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(ReponseData data, RECOVERY_FROM i_from) ;

	void changeDisplayLanguage() override final;

	void onCheckboxClicked(Ref * sender, cocos2d::ui::CheckBox::EventType type);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

private: //private variables

	RECOVERY_FROM	m_from;

	ReponseData		m_responseData;
	
	std::vector<ui::CheckBox*> m_uiCheckboxs;
};

CREATE_READER(APPasswordRecoveryMethods);

