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
#include "APPhoneCodePopup.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class APRegisterScreen : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol ,public APPhoneCodePopupDelegate, public cocos2d::ui::EditBoxDelegate
{
public:
	
	CREATE_FUNC(APRegisterScreen);

	static APRegisterScreen* createView(cocos2d::Node* i_loginView);

    void updateSkipUI();
    
private:

	void onRegisterButtonClicked(cocos2d::Ref * sender);

	void onFacebookButtonClicked(cocos2d::Ref * sender);

	void onGmailButtonClicked(cocos2d::Ref * sender);

	void onSkipButton(cocos2d::Ref * sender);

	void onLoginButtonClicked(cocos2d::Ref * sender);

	void onPhoneCodeButtonClicked(cocos2d::Ref * sender);

	void onShowPasswordButtonClicked(cocos2d::Ref * sender);
    void onBack(cocos2d::Ref * sender);

	bool didLoadFromCSB(cocos2d::Node* i_loginView);

    void onExit() override final;

	void changeDisplayLanguage() override final;

    void onPhoneCodeItemSelected(PhoneCodeInfo i_info) override;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;

	virtual void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, cocos2d::ui::EditBoxDelegate::EditBoxEndAction action) override;

	virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;

	virtual void editBoxReturn(cocos2d::ui::EditBox* editBox) override;

private:

	std::string m_mailOrPhoneText;
    
    cocos2d::Node* m_loginView = nullptr;
    
    std::vector< ui::TextField*> m_passwordTextfields;

};

CREATE_READER(APRegisterScreen);

