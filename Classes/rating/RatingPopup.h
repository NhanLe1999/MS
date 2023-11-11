//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "popup/PopupBase.h"
#include "RatingReviewPopup.h"

using namespace cocos2d;
using namespace cocos2d::ui;

class RatingPopup :public PopupBase, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	
	CREATE_FUNC(RatingPopup);

	static RatingPopup* createView();

private:

	void onEnter() override;

	void onExit() override;
    
	void onHappyClicked(cocos2d::Ref * sender);
	
	void onConfusedClicked(cocos2d::Ref * sender);

	void onSadClicked(cocos2d::Ref * sender);
	
	void addRatingReviewPopup(RatingReviewPopup::Type i_type);

	bool didLoadFromCSB();

	void changeDisplayLanguage() override final;

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callBackName) override;
    
    void pushCleverTapEvent(std::string i_clickedType);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    void doRate();
#endif
};

CREATE_READER(RatingPopup);

