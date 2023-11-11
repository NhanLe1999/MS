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

class APSecurityQuestionPopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol 
{
public:
	
	enum class RECOVERY_TYPE
	{
		MAIL,
		PHONE
	};

	CREATE_FUNC(APSecurityQuestionPopup);

	static APSecurityQuestionPopup* createView(RECOVERY_TYPE i_type, ReponseData data);

private: //private function

	bool didLoadFromCSB(RECOVERY_TYPE i_type,ReponseData data);

	void changeDisplayLanguage() override final;

	void onContinute(cocos2d::Ref * sender);

	void onAnsClicked(cocos2d::Ref * sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void onAnsCheckboxClicked(Ref *sender, cocos2d::ui::CheckBox::EventType type);

private: //private variables

	ReponseData							m_responseData;

	std::vector<std::vector<ui::Text*>> m_uiAnswers;

	std::vector< ui::Text*>				m_uiQuestions;

	RECOVERY_TYPE						m_type;

	int									m_failedVerificationCount = 0;
};

CREATE_READER(APSecurityQuestionPopup);

