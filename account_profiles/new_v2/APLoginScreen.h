//
//  APEnterPhoneNumberPopup.hpp
//  MJStory
//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "../../account_profiles/new/APPopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APLoginScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol , public cocos2d::ui::EditBoxDelegate {

public:

	CREATE_FUNC(APLoginScreen);

	static APLoginScreen* createView(cocos2d::Node* i_node);

private:

	void onLoginButtonClicked(cocos2d::Ref * sender);

	void onForgotPasswordClicked(cocos2d::Ref * sender);

	void onForgotAccountClicked(cocos2d::Ref * sender);

	void onQuickLoginClicked(cocos2d::Ref * sender);
	
	void onFacebookButtonClicked(cocos2d::Ref * sender);

	void onGmailButtonClicked(cocos2d::Ref * sender);

	void onRegisterButtonClicked(cocos2d::Ref * sender);

	void onShowPasswordButtonClicked(cocos2d::Ref * sender);

	void onBack(cocos2d::Ref * sender);
    
    bool didLoadFromCSB(cocos2d::Node* i_node);

    void onExit() override final;

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	
	void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action)override;
	
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)override;
	
	void editBoxReturn(cocos2d::ui::EditBox* editBox)override;

private:

	std::string m_mailOrPhoneText;
    
    cocos2d::Node* m_loginView = nullptr;

};

CREATE_READER(APLoginScreen);

