#pragma once
#ifndef StoryParentEditUserInfo_Landscape_hpp
#define StoryParentEditUserInfo_Landscape_hpp
#include "cocostudiomacro2.h"
#include "APProfile.h"

class StoryParentEditUserInfo_Landscape : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(StoryParentEditUserInfo_Landscape);
	static StoryParentEditUserInfo_Landscape * createView();
private:
	void didLoadFromCSB();
	void onEnter() override;
	void onSave(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref * sender);
    void onChangeDisplayLanguage(int lang_id);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callBackName) override;
	void onTextfieldChanged(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);

	void hideError(cocos2d::Node * sender);
	void showError(cocos2d::Node * sender,std::string message);
	void setupTextField(cocos2d::Ref * sender,std::string value);

	ap::UserInfo _info;
	cocos2d::ui::Layout * box_name;
	cocos2d::ui::Layout * box_address;
	cocos2d::ui::Layout * box_email;
	cocos2d::ui::Layout * box_phone;

	cocos2d::ui::TextField * text_mail;
	cocos2d::ui::TextField * text_phone;
    cocos2d::ui::TextField * text_name;
    cocos2d::ui::TextField * text_address;
};

CREATE_READER(StoryParentEditUserInfo_Landscape);

#endif // !StoryParentEditUserInfo_Landscape_hpp
