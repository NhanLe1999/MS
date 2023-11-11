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

class GettingUserInfoManager :public BaseSingleton<GettingUserInfoManager>
{
public:
    
    int getRemindingUpdateInfoPopupCount() {return m_remindingUpdateInfoPopupCount;};

	void parsePopupTitlesFromSever(char * i_jsonStr);

	void showGettingUserInfoPopup(bool isSkipButtonEnable = true,bool isRewardPopupShow = false);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool hasRemindingUpdateInfoPopupShow();

	void showRemindingUpdateInfoPopup(std::function<void()> i_callback = nullptr);

	void increaseRemindingUpdateInfoPopupCount();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::string getRemaidingUpdateInfoPopupTitle();

	std::string getGettingUserInfoPopupTitle();

	std::string getRewardPopupStringFromServer();

	std::unordered_map<std::string, std::string> getLanguageListFromServer() 
	{
		return m_languageList;
	}

	void setRemindingUpdateInfoPopupCount(int i_value);

	void startCalculationTime();

	void deleteData();

	void show();

	void setFirebaseDataLoaded(bool i_value);

    std::string getLanguageKey();

private:
    
	int m_remindingUpdateInfoPopupCount		= 0;

	std::unordered_map<std::string, std::string> m_languageList;

	bool m_isFirebaseDataLoaded = false;
    
    CC_SYNTHESIZE(std::vector<PhoneCodeInfo>,m_phonCodeList,PhoneCodeList);
};


