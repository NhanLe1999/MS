//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "LicenseInfoModel.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "MJHelper.h"

#define PHONE_MAX_LENGTH                13
#define PHONE_MIN_LENGTH                6
using namespace cocos2d;
using namespace cocos2d::ui;

class PopupBase : public cocos2d::ui::Layout 
{
protected:

	cocos2d::Size m_visibleSize;
	
	cocos2d::Vec2 m_origin;
	
	std::function<void()> m_onCancelCallBack = nullptr;

protected:

	virtual bool didLoadFromCSB();

	virtual void onCancel(cocos2d::Ref * sender);

	virtual void showingAnimation(cocos2d::Node *i_node, std::function<void()> i_callback = nullptr);

	virtual void hidingAnimation(cocos2d::Node *i_node,std::function<void()> i_callback);

	virtual void validPhoneInput(ui::TextField* i_textField);

	virtual void validNumberInput(ui::TextField* i_textField, std::function<void()> i_callback = nullptr);

	virtual void enableButtonBasedOnValidPhoneNumber(std::string i_phoneNumber,ui::Button* i_button);

	virtual void enableButtonBasedOnValidNumber(std::string i_Number, ui::Button* i_button);
	
	virtual void enableWidgetNodeBasedOnTextLength(std::string i_text,int i_leghth, cocos2d::ui::Widget * i_node);

	virtual void visibleWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget * i_node);

	virtual void changeDisplayLanguage() = 0;

	virtual void showErrorAlert(std::string msg);

	virtual void showSuccessPopup(std::string msg);

	virtual void showFailedPopup(std::string msg);
	
	virtual void removeFromParentWithAnmation(std::function<void()> i_callback = nullptr);

	virtual void runWarningActionOnBackgroundNode(std::string i_bgName = "popup_bg");

	virtual void enableAButtonBasedOnTextfieldLength(cocos2d::ui::Button* i_button, cocos2d::ui::TextField* i_textField, int i_length);

	virtual void enableAButtonBasedOnTextfieldsLength(cocos2d::ui::Button* i_button, std::vector<cocos2d::ui::TextField*> i_textFields, cocos2d::ui::Widget * i_node, int i_length);

	virtual void showWaittingView();

	virtual void removeWaittingView();

	virtual int converMillisecondsToHours(long long i_milliseconds);

	virtual bool isUTF8StringPhoneNumberValid(std::string s);

	virtual bool isUTF8StringNumberValid(std::string s);
	
	virtual void reduceFontSizeOfTextfieldBasedOnLength(cocos2d::ui::TextField* i_textField,const int i_defaultLength);

	virtual void hideVirtualKeyboard(cocos2d::ui::TextField* i_textField);

	virtual void hideVirtualKeyboard(std::vector<cocos2d::ui::TextField*> i_textField);

	virtual void replaceString(std::string &i_source, std::string i_subString,std::string i_newSubString);
};


