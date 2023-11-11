///////////////////////////////////////////////////////////////////////
//  AISpeakingTopic_hpp
//  Implementation of the Class AISpeakingTopic_hpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////


#ifndef MSFreeContentPage_hpp
#define MSFreeContentPage_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "cocostudiomacro2.h"
#include "ui/CocosGui.h"		
#include "cocos2d.h"
#include "MJDefinitions.h"
#include "F2PCommon.h"
#include "MsGroupFreeItems.h"
#include "MJAPI_F2P_TrialContent.h"
#include "MSCollectionGroupFreeItemsDelegate.h"
#include "MSCollectionLessonItemDelegate.h"
#include "MSCollectionLessonItem.h"
#include "MJDefinitions.h"
#include "StoryLanguageManager.h"
#include "StoryDataModel.h"
#include "MSHome.h"
#include "platform/CCPlatformConfig.h"
#include "MSActivityEnd.h"
#include "ShowNewAlert.h"
#include "MSInstallPromotion.h"
#include "StoryWelcomePopUp.h"
#include "MSFree2PaidPermissionPopup.h"
#include "EndLessonScene.h"
#include "EndTrialLessonPopup.h"
#include "MsSubscriptionLandscape.h"
#include "APPurchaseManager.h"
#include "MSLessonManager.h"
#include "MSLessonPreview.h"
#include "Common.h"
#include "MSActivityWrapper.h"
#include "F2PManager.h"
#include "MJDefinitions.h"

USING_NS_CC;
#define DELAY_CREAT_LESSON 0.05
using namespace ms::delegate;
class MSFreeContentPage : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol,
	public MSCollectionGroupFreeItemsDelegate, public ms::delegate::CollectionLessonItem, ms::delegate::PreActivity
{

public:
	CREATE_FUNC(MSFreeContentPage);
	MSFreeContentPage();
	virtual ~MSFreeContentPage();
	static Scene* createScene(ms::f2p::FreeContentOption freeContentOption = ms::f2p::FreeContentOption::DEFAULT);
	static MSFreeContentPage* createTab(ms::f2p::FreeContentOption freeContentOption = ms::f2p::FreeContentOption::DEFAULT);
    void CallAPIFreeContentPage();

private:
	void initViewContent();
	void didLoadFromCSB(ms::f2p::FreeContentOption freeContentOption = ms::f2p::FreeContentOption::DEFAULT);
    void mouseScroll(std::string listviewName = "", ListView * listView = nullptr, float percent = 10, float timeInSec = 1.5, bool attenuated = true);
    void onExit() override;  
	void onEnter() override;
	void onShowNotifyPermissonPopup();
	void initListView();
	MsGroupFreeItems* initFreeGroupView(int day);
	void APICallBack(bool callAPISuccess);
	void initFreeData(bool callAPISuccess = false);
	std::vector<int> getAllStoriesLearn();
	bool checkItemStoryLearned(mj::model::FreeItemInfo freeItemInfo);
	void caculateCountDown();
	void getCountDown();
	void onSaveFreeItemsLearned(std::function<void(bool)> callback = nullptr);

	void onBackButtonClicked(cocos2d::Ref* sender);
	void onSignInButton(cocos2d::Ref* sender);
	void onSignInF2P();
	void onUnlockAll(cocos2d::Ref* sender);
	void pushEventDiscoverUnlock(std::string clickType);
	void onDiscover(cocos2d::Ref* sender);
	void onChangePageList();
	void selectedEventScrollView(Ref* pSender, ui::ScrollView::EventType type);
	bool checkPermissionNotifications();
	void onTouchToGroupItem(bool isTouchToUnLock, int index) override;
	bool onItemsLearn(int day) override;
	void onLearnStory (mj::model::StoryInfo storyItemInfo) override;
	void onLearnLessonFree(mj::model::LessonInfo lessonItemInfo) override;
	void onLearnLesson(mj::model::LessonInfo lesson) override;
	void onContinueLearn(mj::model::ActInfo info);
	bool checkItemLearned(mj::model::FreeItemInfo freeItemInfo) override;
	void onResetLayoutListView(int index) override;
	/*show alerts */
	void showNotifyBeginTrialProgramsAlert(std::function<void()> callback);
	void showNotifyPermissionRequestAlert();
	void showNotifyLessonNotReadyAlertWhenLearned() override;
	void showNotifyLessonNotReadyAlertWhenHaveNotLearned() override;
	void showNotifyEndTrialProgramsAlert();
	void showPopupEndOfTheProgramTenFreeLessons(std::function<void()> callback);
	void moveScrollBar(int percent, int movePercent, bool flag);

	void setLearnLesson(cocos2d::Ref* sender);

	void setKeyDiscoverToPaymentInMSHome();

	void pushEvenPartlyDone7Or14Lessons(std::string done, std::string clickType);

	std::string setKeyUnlockFromToPayment(bool isStart);
	/*------------*/

	/*Go to other screen*/
	void gotoMsHome();
	void gotoEndLessonScene(std::function<void()> onUnlockCallback = nullptr, std::function<void()> onComeBackLaterCallback = nullptr);
	/*------------------*/

	void setRadiusPageList(std::vector<Layout*> radiusPageList) {
		CCLOG("MSFreeContentPage::setRadiusPageList");
		_radiusPageList = radiusPageList;
	}
	std::vector<Layout*> getRadiusPageList() {
		CCLOG("MSFreeContentPage::getRadiusPageList");

		return _radiusPageList;
	}

	void setGroupFreeItems(std::vector<MsGroupFreeItems*> msGroupFreeItems) {
		CCLOG("MSFreeContentPage::setGroupFreeItems");
		_msGroupFreeItems = msGroupFreeItems;
	}
	std::vector<MsGroupFreeItems*> getGroupFreeItems() {
		CCLOG("MSFreeContentPage::getGroupFreeItems");

		return _msGroupFreeItems;
	}


	void setAllFreeItemInfos(std::vector<std::vector<mj::model::FreeItemInfo>> allFreeItemInfos) {
		CCLOG("MSFreeContentPage::setAllFreeItemInfos");
		_allFreeItemInfos = allFreeItemInfos;
	}
	std::vector<std::vector<mj::model::FreeItemInfo>> getAllFreeItemInfos() {
		CCLOG("MSFreeContentPage::getAllFreeItemInfos");

		return _allFreeItemInfos;
	}

	void setFreeItemsListView(cocos2d::ui::ListView* freeItemsListview) {
		CCLOG("MSFreeContentPage::setFreeItemsListView");

		_freeItemsListview = freeItemsListview;
	}
	cocos2d::ui::ListView* getFreeItemsListView() {
		CCLOG("MSFreeContentPage::getFreeItemsListView");

		return _freeItemsListview;
	}

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
	CC_SYNTHESIZE(ms::f2p::FreeContentOption, _freeContentOption, FreeContentOption);

	//CC_SYNTHESIZE(std::vector<MsGroupFreeItems>, _msGroupFreeItems, GroupFreeItems);
	//CC_SYNTHESIZE(cocos2d::ui::ListView*, _freeItemsListview, FreeItemsListView);
	//CC_SYNTHESIZE(std::vector<Layout*>, _radiusPageList, RadiusPageList);
	//CC_SYNTHESIZE(std::vector<MsGroupFreeItems*>, _msGroupFreeItems, GroupFreeItems);
	//CC_SYNTHESIZE(vector<vector<mj::model::FreeItemInfo>> , _allFreeItemInfos, AllFreeItemInfos);
	std::vector<Layout*> _radiusPageList;
	std::vector<MsGroupFreeItems*> _msGroupFreeItems;
	std::vector<std::vector<mj::model::FreeItemInfo>> _allFreeItemInfos;
	cocos2d::ui::ListView* _freeItemsListview = nullptr;
	std::vector<int> _listOfStoryLearned;
	bool _permissionNotify = false;
	int _percent = 0;
	int _currentPercentage = 0;
	int _countLessonForTest = 0;

	std::string _clickTypeOpenOptinTenFree = "";
	std::string _clickTypeFinishOptionTenFree = "";
	std::string _clickType7OptionFree = "";
	std::string _clickType14OptionFree = "";
    std::string _sourcePayment = "";
    std::string _pathLanding = "";
	monkey::iap::FROM _fromPayment;
	bool _isStartDemoPopup = false;

};
CREATE_READER(MSFreeContentPage);

#endif // defined(MSFreeContentPage_hpp)
