///////////////////////////////////////////////////////////////////////
//  MSMergeSuccess_hpp
//  Implementation of the Class MSMergeSuccess_hpp
//  Created on:      01-07-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////


#ifndef MSMergeSuccess_hpp
#define MSMergeSuccess_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "cocostudiomacro2.h"
#include "ui/CocosGui.h"		
#include "cocos2d.h"
#include "MJDefinitions.h"
#include "F2PCommon.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "StoryDataModel.h"
#include "MSHome.h"
#include "platform/CCPlatformConfig.h"
#include "MsProfileItem.h"
#include "MSInstallPromotion.h"
#include "StoryWelcomePopUp.h"
#include "APUpdateProfileAPI.h"
#include "APPurchaseManager.h"
#include "APLoadUpdateAPI.h"
#include "APProfilesListAPI.h"
#include "F2PManager.h"

USING_NS_CC;
#define DELAY_CREAT_LESSON 0.05
using namespace ms::delegate;
class MSMergeSuccess : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::MSCollectionProfileItemDelegate
{
public:
	CREATE_FUNC(MSMergeSuccess);
	MSMergeSuccess();
	virtual ~MSMergeSuccess();
	static Scene* createScene();
	static MSMergeSuccess* createTab();

private:
	void initViewContent();
	void didLoadFromCSB();
    void mouseScroll(std::string listviewName = "", ListView * listView = nullptr, float percent = 10, float timeInSec = 1.5, bool attenuated = true);
    void onExit() override;  
	void onEnter() override;
	void onSelectedProfile(int profileId)override;
	void onDeleteSelectedProfile(int profileId)override;
	void initListView();
	MsProfileItem* initProfileItem(ap::Profile profileInfo);
	void APICallBack(bool callAPISuccess);
	void updateProfileData(bool callAPISuccess = false);

	void CallAPILoadUpdate();
	void APILoadUpdateCallBack(bool isSucess, std::string msg);

	void CallAPIProfileList();
	void APIProfileListCallBack(bool isSucess, std::string msg);

	void ShowCreateProfile();

	void CallAPIUpdateProfile();
	void onBackButtonClicked(cocos2d::Ref* sender);
	void getAllProfilefromDB();
	void selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
	/*show alerts */
	/*------------*/

	/*Go to other screen*/
	void gotoMsHome();
	/*------------------*/

	void setProfileItemsListView(cocos2d::ui::ListView* profileItemsListview) {
		CCLOG("MSMergeSuccess::setProfileItemsListView");

		_profileItemsListview = profileItemsListview;
	}
	cocos2d::ui::ListView* getProfileItemsListView() {
		CCLOG("MSMergeSuccess::getProfileItemsListView");

		return _profileItemsListview;
	}

	void setProfileInfoList(vector<ap::Profile> profileList) {
		CCLOG("MSMergeSuccess::setProfileInfoList");

		_profileList = profileList;
	}
	vector<ap::Profile>  getProfileInfoList() {
		CCLOG("MSMergeSuccess::setProfileInfoList");
		return _profileList;
	}
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);

	CC_SYNTHESIZE(int, _selectedProfileId, SelectedProfileId);

	cocos2d::ui::ListView* _profileItemsListview = nullptr;
	vector<ap::Profile>_profileList = {};

};
CREATE_READER(MSMergeSuccess);

#endif // defined(MSFreeContentPage_hpp)
