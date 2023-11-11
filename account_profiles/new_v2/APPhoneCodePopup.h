//
//  APResetPasswordPopup.hpp
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

struct PhoneCodeInfo
{
    std::string id;
    std::string name;
    std::string code;
    bool        selected = false;
    
    PhoneCodeInfo(){}
    
    PhoneCodeInfo(std::string i_id,std::string i_name, std::string i_code,bool i_selected)
    :id(i_id),name(i_name),code(i_code),selected(i_selected)
    {
        
        
    }
};

class APPhoneCodePopupDelegate
{
public:
    virtual void onPhoneCodeItemSelected(PhoneCodeInfo i_info) = 0;
};

class APPhoneCodePopup : public APPopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	CREATE_FUNC(APPhoneCodePopup);

	static APPhoneCodePopup* createView();

private:

    ~APPhoneCodePopup();
    
    void onItemClicked(cocos2d::Ref * sender);

	bool didLoadFromCSB();
    
    void onExit() override final;

	void updateListView(std::string);
	
	void changeDisplayLanguage() override final;

	void onTextfieldChanged(cocos2d::Ref* pSender, cocos2d::ui::TextField::EventType type);

	std::vector<ui::Widget*> list_item;

	ui::TextField* textfield_search;
    
	ui::ListView* listview;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

    CC_SYNTHESIZE(APPhoneCodePopupDelegate*, m_delegate, Delegate);
    
};

CREATE_READER(APPhoneCodePopup);

