///////////////////////////////////////////////////////////////////////
//  MsGroupFreeItems_hpp
//  Implementation of the Class MsGroupFreeItems_hpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////


#ifndef MsGroupFreeItems_hpp
#define MsGroupFreeItems_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MJDefinitions.h"
#include "MKDefinitions.h"
#include "APProfileManager.h"
#include "MsFreeItem.h"
#include "MSCollectionFreeItemDelegate.h"
#include "MSCollectionGroupFreeItemsDelegate.h"

#include "StoryDataModel.h"
#include "platform/CCPlatformConfig.h"

USING_NS_CC;
#define DELAY_CREAT_LESSON 0.05
enum GroupItemsState
{
    LOCK,
    CANUNLOCK,
    UNLOCKED,
};
class MsGroupFreeItems : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::MSCollectionFreeItemDelegate
{

    
public:
	CREATE_FUNC(MsGroupFreeItems);
	MsGroupFreeItems();
	virtual ~MsGroupFreeItems();
	static MsGroupFreeItems* createView(vector<mj::model::FreeItemInfo> freeItemInfos, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionGroupFreeItemsDelegate* delegate_, int listViewWith = 300);
private:
	void initViewContentOneItem();
	void initViewContentMutileItem();
	void initViewContentTenItemFreeUnlock();
	void didLoadFromCSB(vector<mj::model::FreeItemInfo> freeItemInfos, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionGroupFreeItemsDelegate* delegate_, int listViewWith = 300);
	void initViewContent();
	std::string getItemsNameByTypes();
	void mouseScroll(std::string listviewName = "", ListView * listView = nullptr, float percent = 10, float timeInSec = 1.5, bool attenuated = true);
    void onExit() override;    
	void onEnter() override;
	bool onItemsLearn(int day) override;
	void onLearnLessonFree(mj::model::LessonInfo lessonItemInfo) override;
	void onLearnStory(mj::model::StoryInfo storyItemInfo ) override;
	bool checkItemLearned(mj::model::FreeItemInfo freeItemInfo) override;
	void onsetAvataUrl(std::string url) override;
	void flipCard(Node* card1, Node* card2);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
	void onUnlockItems();
	void onTouch(cocos2d::Ref* sender);
	void setSetLockView();
	void setUnlockView();
	void initListView();
	std::vector<int> getAllStoriesLearn();
	bool checkGroupLearn();
	void changeStateLockUnclock();
	void setDefaultView();
	void checkCanUnLockDay(int day = 0);
	void onSaveAvataUrl();
	void checkItemsUnlockButHavenotLearned();
	MsFreeItem* initFreeItem(mj::model::FreeItemInfo freeItemInfo);
	void setListViewItem(ListView* listViewItem) {
		_listViewItem = listViewItem;
	};
	ListView* getListViewItem() {
		return _listViewItem ;
	};

	void setFreeItems(std::vector<MsFreeItem*> msFreeItems) {
		_msFreeItems = msFreeItems;
	};
	std::vector<MsFreeItem*> getFreeItems() {
		return _msFreeItems;
	};
	std::vector<std::string> getAvataUrl() {
		return _avataUrl;
	}
	CC_SYNTHESIZE(ms::delegate::MSCollectionGroupFreeItemsDelegate*, _delegate, Delegate);

	CC_SYNTHESIZE(ms::f2p::FreeContentOption, _freeContentOption, FreeContentOption);
	CC_SYNTHESIZE(vector<mj::model::FreeItemInfo>, _freeItemInfos, FreeItemInfos);

	CC_SYNTHESIZE(int, _day, Day);
	CC_SYNTHESIZE(bool, _isCanUnlock, IsCanUnlock);
	CC_SYNTHESIZE(bool, _isUnlocked, IsUnlocked);
	CC_SYNTHESIZE(float, _listViewWith, ListViewWith);
	std::vector<std::string> _avataUrl;
	std::vector<MsFreeItem*> _msFreeItems;
	ListView* _listViewItem = nullptr;
	Layout* unLockBackGround = nullptr, *lockBackGround = nullptr;
	Text* _date_title = nullptr, * _date_value = nullptr, *item_type_name = nullptr;
	bool isRunAni = false;
};
CREATE_READER(MsGroupFreeItems);

#endif // defined(MsGroupFreeItems_hpp)
