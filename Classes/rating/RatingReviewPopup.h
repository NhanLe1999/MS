//
//  Created by NguyenHoangThienPhuoc@gmail.com
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "popup/PopupBase.h"
#include "ComboBox.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class RatingReviewPopup : public PopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:

	enum class Type 
	{
		HAPPY,
		CONFUSED,
		SAD
	};
	
	CREATE_FUNC(RatingReviewPopup);

	static RatingReviewPopup* createView(Type i_type);

private:

	void onEnter() override;

	void onExit() override;
    
	void onPost(cocos2d::Ref * sender);

	void onFacebookClicked(cocos2d::Ref * sender);
	
	bool didLoadFromCSB(Type i_type) ;

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;

	void setUpUIForMS();

private:
	
	Type m_type = Type::CONFUSED;

	ComboBox *m_comboBox = nullptr;
};

CREATE_READER(RatingReviewPopup);

