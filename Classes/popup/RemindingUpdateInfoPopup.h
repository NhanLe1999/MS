//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "PopupBase.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class RemindingUpdateInfoPopup :public PopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	CREATE_FUNC(RemindingUpdateInfoPopup);

	static RemindingUpdateInfoPopup* createView();

	void setOKButtonCallback(std::function<void()> i_callback);

private:

	bool didLoadFromCSB() override final;

	void onEnter() override;

	void onExit() override;

	void changeDisplayLanguage() override final;

	void onOKButtonClicked(cocos2d::Ref *i_sender);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    
    void pushCleverTapEvent(std::string i_clickedType);

private:
	std::function<void()> m_okButtonCallback = nullptr;
};

CREATE_READER(RemindingUpdateInfoPopup);

