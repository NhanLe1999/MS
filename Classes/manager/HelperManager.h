//
//  Created by NguyenHoangThienPhuoc on 24/03/2020.
//
#pragma once
#include "BaseSingleton.h"
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "APPhoneCodePopup.h"

using namespace cocos2d;
using namespace cocos2d::ui;
#define HELPER_MANAGER HelperManager::GetInstance()

class HelperManager :public BaseSingleton<HelperManager>
{
public:

	virtual void trimSpace(cocos2d::Node* i_node);

	virtual void removeSpace(cocos2d::Node* i_textField);

	virtual void removeSpace(std::vector<cocos2d::Node*> i_textFields);

	void parseCountryCodesInfomationFromJson();
	
	std::string getProfileAvatarPathFromConfigJson(std::string i_config);

	std::string getFontPath(ui::Text* i_text,std::string i_defaultValue = "");

	virtual void writeLogToFile(std::string log);
    
    std::string convertToDatetimeForClevertap(int i_time);
    
	void addSpriteFramesWithFile(const std::string& plist, const std::string& textureFileName);
    void addSidebarBlack();

	void scrollingViewVibrate();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	virtual void scrollingViewVibrateIOS();
//
//	virtual void forceDismissKeyboardIOS();
#endif
private:
    
    CC_SYNTHESIZE(std::vector<PhoneCodeInfo>,m_phonCodeList,PhoneCodeList);
};


