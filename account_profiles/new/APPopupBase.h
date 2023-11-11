//
//  APEnterPhoneNumberPopup.hpp
//  MJStory
//
//  Created by NguyenHoangThienPhuoc on 13/02/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "LicenseInfoModel.h"
#include "APWaitView.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "MJHelper.h"
#include "APPasswordAPI.h"
#include "APSendOTPAPI.h"
#include "APLoginAPI.h"
#include "HSAudioEngine.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PREFIX_ERROR_TAG				"<XXX|Error>"
#define	POPUP_ZORDER					1000
#define ENABLE_SECURITY_QUESTION_KEY	"enable_security_question_key"
#define DISALE_SECURITY_QUESTION_EVENT_KEY "disable_security_question_even_key"
#define	PASSWORD_MIN_LENGTH				8
#define	PASSWORD_MAX_LENGTH				30
#define OTP_MIN_LENGTH					4
#define AP_API_SEND_OTP_EMAIL_TYPE		"2"
#define AP_API_SEND_OTP_PHONE_TYPE		"1"
#define HOURS_FOR_SECURITY_QUESTION		"hours_for_security_question"
#define FAILED_VERIFICATION_COUNT_KEY	"failed_verification_count"
#define PHONE_MAX_LENGTH						15
#define PHONE_MAX_LENGTH_WITH_COUNTRY_CODE      14
#define PHONE_MIN_LENGTH                6
#define SHOW_PASS_BUTTON_TAG_IN_PASSWORD_TEXTFIELD 999
#define	USER_NAME_MAX_LENGTH			8
#define	MAX_NUMBER_IN_USER_NAME			6
#define	MAX_CHARACTER_IN_USER_NAME		2

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string k_alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

#define AP_CREATE_VIEW(__TYPE__,flag_name,csb_name) \
std::once_flag flag_##flag_name; \
__TYPE__ * __TYPE__::createView() \
{ \
	std::call_once(flag_##flag_name, [] \
	{ \
	REGISTER_CSB_READER(__TYPE__);\
	});\
	__TYPE__ *pRet = static_cast< __TYPE__ *>(CSLoader::createNodeWithVisibleSize(csb_name)); \
    if (pRet && pRet->didLoadFromCSB()) \
    { \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
        return nullptr; \
    } \
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace ap;
using namespace api;

struct SecurityQuestion
{
	std::string					m_question = "";
	std::vector<std::string>	m_answers;
	std::string					m_rightAnswer = "";
};

struct ReponseData
{
	std::vector<SecurityQuestion>	m_questions;
	std::string						m_email = "";
	std::string						m_phone = "";
	bool							m_isFree = false;
};

class APPopupBase : public cocos2d::ui::Layout 
{
public: 

	enum class AnimationDirection
	{
		LEFT,
		RIGHT,
		TOP,
		DOWN
	};
    
    enum class Flow
    {
        NONE,
        ENTER_INFO,     // luồng Lấy thông tin người dùng
        UPDATE_EMAIL,   // luồng Thêm/Cập nhật địa chỉ email
        UPDATE_PHONE,   // luồng Thêm/Cập nhật số điện thoại
    };

	enum class TextType 
	{
		BOLD,
		ITALIC,
		REGULAR
	};

	static void sAnimationMove(Node *i_node, AnimationDirection i_direction, float i_duration = 0.5f);
    
    static void sAnimationMoveForNodes(std::vector<Node*> i_nodes, AnimationDirection i_direction, float i_duration = 0.5f, float i_delayTime = 0.0f);

protected:

	cocos2d::Size m_visibleSize;
	
	cocos2d::Vec2 m_origin;
    
    Flow m_flow;
	
	std::function<void()> m_onCancelCallBack = nullptr;

protected:

	virtual bool didLoadFromCSB();

	virtual void onCancel(cocos2d::Ref * sender);

	virtual void showingAnimation(cocos2d::Node *i_node, std::function<void()> i_callback = nullptr);

	virtual void hidingAnimation(cocos2d::Node *i_node,std::function<void()> i_callback);

    virtual void trimSpace(cocos2d::Node *i_node);

	virtual void removeSpace(cocos2d::Node* i_textField);

	virtual void removeSpace(std::vector<cocos2d::Node*> i_textFields);

	virtual void removeSpace(std::vector<ui::TextField*> i_textFields);

    virtual void validEmaiInput(ui::TextField* i_textField);

	virtual void validPhoneInput(ui::TextField* i_textField);

	virtual void validNumberInput(ui::TextField* i_textField, std::function<void()> i_callback = nullptr);

	virtual void enableButtonBasedOnValidPhoneNumber(std::string i_phoneNumber,ui::Button* i_button);

	virtual void enableButtonBasedOnValidNumber(std::string i_Number, ui::Button* i_button);
	
	virtual void enableWidgetNodeBasedOnTextLength(std::string i_text,int i_leghth, cocos2d::ui::Widget * i_node);

	virtual void visibleWidgetNodeBasedOnTextLength(std::string i_text, int i_leghth, cocos2d::ui::Widget * i_node);

	virtual void changeDisplayLanguage() = 0;

	virtual void showErrorAlert(std::string msg);

	virtual void showNetworkDownAlert(std::function<void()> i_okButtonCallback = nullptr, std::function<void()> i_cacelButtonCallback = nullptr);
	
	virtual void showSuccessPopup(std::string msg);

	virtual void showFailedPopup(std::string msg);
	
	virtual rapidjson::Document readJsonFromString(const std::string i_jsonFilePath, rapidjson::Type i_type);
	
	virtual void sendOTPAPICall(std::string i_type);

	virtual void removeFromParentWithAnmation(std::function<void()> i_callback = nullptr);

	virtual void runWarningActionOnBackgroundNode(std::string i_bgName = "popup_bg");

	virtual void enableAButtonBasedOnTextfieldLength(cocos2d::ui::Button* i_button, cocos2d::ui::TextField* i_textField, int i_length);

	virtual void enableAButtonBasedOnTextfieldsLength(cocos2d::ui::Button* i_button, std::vector<cocos2d::ui::TextField*> i_textFields, cocos2d::ui::Widget * i_node, int i_length);

	virtual void showWaittingView();

	virtual void showWaittingViewInCocosThread();

	virtual void removeWaittingView();

	virtual int converMillisecondsToHours(long long i_milliseconds);

	virtual bool isUTF8StringPhoneNumberValid(std::string s);

	virtual bool isUTF8StringNumberValid(std::string s);
    
    virtual bool isGMail(std::string s);
	
	virtual void reduceFontSizeOfTextfieldBasedOnLength(cocos2d::ui::TextField* i_textField,const int i_defaultLength);

	virtual void hideVirtualKeyboard(cocos2d::Node* i_textField);

	virtual void hideVirtualKeyboard(std::vector<cocos2d::Node*> i_textField);

	virtual void hideVirtualKeyboard(std::vector<ui::TextField*> i_textFields);

    virtual void animationMoveToLeft(cocos2d::Node *i_node,float i_duration = 0.5f);
    
    virtual void animationMoveToRight(cocos2d::Node *i_node,float i_duration = 0.5f);

    virtual void avoidSpamOnButton(cocos2d::ui::Button *i_button);
    
    virtual void replaceAll(std::string& str, const std::string from, const std::string to);

    virtual void animationMoveForNodes(std::vector<Node*> i_nodes, AnimationDirection i_direction, float i_duration = 0.5f, float i_delayTime = 0.0f);

    virtual void playClickedSoundEffect();
    
    virtual void visibleAllNodes(std::vector<cocos2d::Node*> i_nodes,bool i_visible);

	virtual void visibleBlackLayer(bool i_visible);
    
    virtual bool isContainSpace(const std::string i_input);

	virtual void setFontByLang(Node* i_node, TextType type = TextType::BOLD);

	virtual void setFontByLang(std::vector<Node*> i_nodes, TextType type = TextType::BOLD);

	// [0] Node		: Textfield,Lable,Text or Button. ( we will dynamic_cast the node based on node type).
	// [1] String	: text key in language files.
	// [2] TextType	: BOLD,ITALIC,REGULAR
	virtual void setTextAndFontForNodes(std::vector<std::tuple<Node*, std::string, TextType>> i_nodes);

	virtual void removeChildrenByNames(cocos2d::Node *i_parent, std::vector<std::string> i_name);

	std::string getDisplayTextByKey(std::string i_key);

	virtual bool isThaiLandLanguage();

	virtual bool isDigitString(const std::string& str);

	// This function only disable button
	// and not re-enable.
	virtual void safeDisableButton(cocos2d::Ref *i_button);

	virtual void moveScreenWhenUseTextfiled(Node* i_node, AnimationDirection i_direction = AnimationDirection::TOP);

	//
	// set up the color,max length... for phone textfield
	//
	virtual void setupForPhoneNumberTextfield(cocos2d::ui::TextField *i_phoneNumberTextfield , const int i_maxPhoneLength = PHONE_MAX_LENGTH);

	virtual void setupForPasswordTextfield(cocos2d::ui::TextField *i_passwordTextfield, cocos2d::Node* i_parent = nullptr);

	virtual std::string resizeString(std::string i_string,int length);

	virtual bool isValidUserName(std::string i_userName);

	virtual cocos2d::ui::RichText *createRichTextFromText(cocos2d::ui::Text* i_text,std::string i_xml);

	virtual cocos2d::ui::EditBox *createEditBoxFromTextfield(cocos2d::ui::TextField* i_text,
		ui::EditBox::InputFlag inputFlag = ui::EditBox::InputFlag::LOWERCASE_ALL_CHARACTERS,
		ui::EditBox::InputMode inputMode = ui::EditBox::InputMode::SINGLE_LINE);

	virtual void validPhoneInput(ui::EditBox* i_editbox,std::string text);

};


