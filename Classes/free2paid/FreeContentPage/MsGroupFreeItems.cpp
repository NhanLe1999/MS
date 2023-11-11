///////////////////////////////////////////////////////////
//  MsGroupFreeItems.cpp
//  Implementation of the Class MsGroupFreeItems.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MsGroupFreeItems.h"

INITIALIZE_READER(MsGroupFreeItems);

/*
 * 1. New, Popular
 * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
 */

std::once_flag _MsGroupFreeItems_reader;

MsGroupFreeItems::MsGroupFreeItems(){
	CRASH_AUTO_LOG;

}

MsGroupFreeItems::~MsGroupFreeItems(){
	CRASH_AUTO_LOG;

}

MsGroupFreeItems* MsGroupFreeItems::createView(vector<mj::model::FreeItemInfo> freeItemInfos, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionGroupFreeItemsDelegate* delegate_, int listViewWith)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::createView");
	std::call_once(_MsGroupFreeItems_reader, []
		{
			REGISTER_CSB_READER(MsGroupFreeItems);
		});

	auto csbName = mj::helper::getCSBName("csb/free2paid/MsGroupFreeItems.csb");

	auto node = reinterpret_cast<MsGroupFreeItems*>(CSLoader::createNode(csbName));

	if (node)
	{
		node->didLoadFromCSB(freeItemInfos, freeContentOption, delegate_, listViewWith);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}

void MsGroupFreeItems::didLoadFromCSB(vector<mj::model::FreeItemInfo> freeItemInfos, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionGroupFreeItemsDelegate* delegate_, int listViewWith)
{
	CRASH_AUTO_LOG;

	CCLOG("MsGroupFreeItems::didLoadFromCSB");
	setFreeContentOption(freeContentOption);
	setDelegate(delegate_);
	setDay(freeItemInfos.at(0)._day);
	setListViewWith(listViewWith);
	setFreeItemInfos(freeItemInfos);
	initViewContent();

	auto myScreenContentSize = Director::getInstance()->getVisibleSize();
}


void MsGroupFreeItems::initViewContentOneItem()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initViewContentOneItem");
	MK_SAFELY_REMOVE(this, "back_ground_lock_multi_item");
	MK_SAFELY_REMOVE(this, "back_ground_unlock_multi_item");
	unLockBackGround = utils::findChild<Layout*>(this, "back_ground_unlock_one_item");
	lockBackGround = utils::findChild<Layout*>(this, "back_ground_lock_one_item");
	this->setContentSize(lockBackGround->getContentSize());

}

void MsGroupFreeItems::initViewContentMutileItem()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initViewContentMutileItem");
	MK_SAFELY_REMOVE(this, "back_ground_lock_one_item");
	MK_SAFELY_REMOVE(this, "back_ground_unlock_one_item");
	unLockBackGround = utils::findChild<Layout*>(this, "back_ground_unlock_multi_item");
	lockBackGround = utils::findChild<Layout*>(this, "back_ground_lock_multi_item");
	this->setContentSize(lockBackGround->getContentSize());

}

void MsGroupFreeItems::initViewContentTenItemFreeUnlock()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initViewContentTenItemFreeUnlock");
	MK_SAFELY_REMOVE(this, "back_ground_lock_multi_item");
	MK_SAFELY_REMOVE(this, "back_ground_unlock_multi_item");
	MK_SAFELY_REMOVE(this, "back_ground_lock_one_item");
	unLockBackGround = utils::findChild<Layout*>(this, "back_ground_unlock_one_item");
}

void MsGroupFreeItems::initViewContent()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initViewContent");
	//setIsUnlocked(false);
	if (getFreeContentOption() == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
	{
		initViewContentMutileItem();
	}
	else if(getFreeContentOption() == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
	{
		initViewContentTenItemFreeUnlock();
	}
	else
	{
		initViewContentOneItem();
	}
	setDefaultView();
	this->setTouchEnabled(true);

	LANGUAGE_MANAGER->changeDisplayLanguage(this);

	if (auto listView = utils::findChild<ListView*>(unLockBackGround, "free_items_list"))
	{
		listView->setScrollBarEnabled(true);
		listView->setTouchEnabled(true);
		setListViewItem(listView);

	}

	if (lockBackGround)
	{
		setSetLockView();
	}
	initListView();
	//if (unLockBackGround)
	//{
	//	setUnlockView();
	//}
}

std::string MsGroupFreeItems::getItemsNameByTypes()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::getItemsNameByTypes");
	std::string itemName = LANGUAGE_MANAGER->getDisplayTextByKey("name.stories");
	if (getFreeItemInfos().size() > 0)
	{
		ms::MsTypes::DocumentsType type = getFreeItemInfos().at(0)._type;
		switch (type)
		{
		case ms::MsTypes::DocumentsType::STORY:
		{
			itemName = LANGUAGE_MANAGER->getDisplayTextByKey("name.stories");
			break;
		}
			
		case ms::MsTypes::DocumentsType::PHONICS:
		{
			itemName = LANGUAGE_MANAGER->getDisplayTextByKey("name.phonic");
			break;
		}
		case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
		{
			itemName = LANGUAGE_MANAGER->getDisplayTextByKey("name.reading_comprehension");
			break;
		}
		default:
			break;
		}
	}
	return itemName.c_str();
}
void MsGroupFreeItems::mouseScroll(std::string listviewName, ListView* listView, float percent, float timeInSec, bool attenuated)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::mouseScroll");
    ListView* list;
    if(listviewName!="")
        list = utils::findChild<ListView*>(this, listviewName);
    else
    {
        list = listView;
    }
    if (!list)
        return;

        int _percent = list->getScrolledPercentHorizontal();
        _percent += percent;
        list->scrollToPercentHorizontal(_percent, timeInSec, attenuated);

}

void MsGroupFreeItems::onExit()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onExit");
	if(this->isScheduled("waitingForBeginTrialPopupClose"))
		this->unschedule("waitingForBeginTrialPopupClose");
	Layout::onExit();
}

void MsGroupFreeItems::onEnter()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onEnter");
	Layout::onEnter();
	setDefaultView();
	if (lockBackGround)
	{
		setSetLockView();
	}

	if (getFreeContentOption() == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
	{
		changeStateLockUnclock();
	}
	else
	{
		checkCanUnLockDay();
	}
}

bool MsGroupFreeItems::onItemsLearn(int day)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onItemsLearn");
	checkCanUnLockDay(day);
	if (getFreeItemInfos().size() <= 0)
		return false;
	if (!lockBackGround || !unLockBackGround)
		return false;
	if (day == getFreeItemInfos().at(0)._day)
		setUnlockView();
	return true;
}

void MsGroupFreeItems::onLearnLessonFree(mj::model::LessonInfo lessonItemInfo)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onLearnLessonFree");
	onSaveAvataUrl();
	_delegate ? _delegate->onLearnLessonFree(lessonItemInfo) : (void) nullptr;

}

void MsGroupFreeItems::onLearnStory(mj::model::StoryInfo storyItemInfo)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onLearnStory");
	onSaveAvataUrl();
	_delegate ? _delegate->onLearnStory(storyItemInfo) : (void) nullptr;

}

bool MsGroupFreeItems::checkItemLearned(mj::model::FreeItemInfo freeItemInfo)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::checkItemLearned");
	bool isLearned = false;
	if(_delegate)
		isLearned = _delegate->checkItemLearned(freeItemInfo);
	return isLearned;
}

void MsGroupFreeItems::onsetAvataUrl(std::string url)
{
	_avataUrl.push_back(url);
}

void MsGroupFreeItems::flipCard(Node* card1, Node* card2) {
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::flipCard");
	isRunAni = true;
	auto duration = 0.5f;

	if(getDay()>= MJDEFAULT->getIntegerForKey(key_f2p_save_unlocked_items, 0))
		MJDEFAULT->setIntegerForKey(key_f2p_save_unlocked_items, getDay());
	this->runAction(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([=] {
			card2->setVisible(false);
			card1->runAction(Sequence::create(ScaleTo::create(duration, 0, 1.0), CallFunc::create([=] {
				card1->setVisible(false);
				card2->setVisible(true);
				setUnlockView();
				}), NULL));
			}),
		NULL));
}

cocos2d::ui::Widget::ccWidgetClickCallback MsGroupFreeItems::onLocateClickCallback(const std::string& callback_name)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::onLocateClickCallback");
	std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(MsGroupFreeItems, onTouch),
	};
	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}
	return nullptr;
}
void MsGroupFreeItems::onUnlockItems()
{
	CRASH_AUTO_LOG;
	checkItemsUnlockButHavenotLearned();
	int groupDay = getFreeItemInfos().at(0)._day;

	vector<int>unlockedDays = {};
	if (getIsUnlocked())
	{
		setUnlockView();
	}
	else
	{
		int unlocked = MJDEFAULT->getIntegerForKey(key_f2p_save_unlocked_items, 0);
		if (getIsCanUnlock())// group co the unlock
		{
			if (unlocked >= getDay())
			{
				setUnlockView();
				if (lockBackGround && unLockBackGround)
				{
					lockBackGround->setVisible(false);
					unLockBackGround->setVisible(true);
				}

			}
			else
			{
				unlockedDays = MJDEFAULT->getArrayIntegerForKey(key_f2p_save_unlock_day_number, unlockedDays);
				if (std::find(unlockedDays.begin(), unlockedDays.end(), groupDay) != unlockedDays.end()) // group da unlock va duoc luu gia tri vao MJDefault
				{

				}
				else // group da unlock nhung duoc luu gia tri vao MJDefault
				{
					unlockedDays.push_back(groupDay);
					MJDEFAULT->setArrayIntegerForKey(key_f2p_save_unlock_day_number, unlockedDays);
				}

				if (lockBackGround && unLockBackGround)
				{
					flipCard(lockBackGround, unLockBackGround);
				}
			}

		}
	}
	

}

void MsGroupFreeItems::onTouch(cocos2d::Ref* sender)
{
	int groupDay = getFreeItemInfos().at(0)._day;

	vector<int>unlockedDays = {};

	if (!getIsCanUnlock())// group co the unlock
	{
		bool isHaveItemsHavanotLearn = (MJDEFAULT->getBoolForKey(key_f2p_save_free_items_learned_on_day, false));
		//int currentDay = mj::helper::getTimeInDays();
		//int lastItemsLearnedDate = MJDEFAULT->getIntegerForKey(key_f2p_save_last_date_of_learned_items, currentDay);
		//if (currentDay == lastItemsLearnedDate)
		//{
		if (!isHaveItemsHavanotLearn) {
			// 1. truong hop khi be da hoan thanh bai hoc cua ngay hien tai, 
			_delegate ? _delegate->showNotifyLessonNotReadyAlertWhenLearned() : (void) nullptr;
		}
		else
		{
			// 2. truong hop khi be chua hoan thanh bai hoc cua ngay hien tai
			_delegate ? _delegate->showNotifyLessonNotReadyAlertWhenHaveNotLearned() : (void) nullptr;

		}
		//}

	}

}

void MsGroupFreeItems::setSetLockView()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::setSetLockView");
	if (!lockBackGround)
	{
		return;
	}
	if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.value"))
	{
		itemValue->setString(StringUtils::format("%02d", getDay()));
	}
	if (auto itemName = utils::findChild<Text*>(lockBackGround, "lb.item.type.name"))
	{
		(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? itemName->setFontName("fonts/leelawdb.ttf") : itemName->setFontName("fonts/Montserrat-ExtraBold.ttf");
		itemName->setString(getItemsNameByTypes());
	}

	if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.title"))
	{
		if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE)
		{
			itemValue->setFontSize(itemValue->getFontSize() - 2);
		}
	}


	ms::MsTypes::DocumentsType lockBackGroudType = ms::MsTypes::DocumentsType::STORY;
	if (getFreeItemInfos().size() > 0)
		lockBackGroudType = getFreeItemInfos().at(getFreeItemInfos().size() - 1)._type;

	if (auto backGround = utils::findChild<Layout*>(lockBackGround, "back_ground"))
	{
		switch (lockBackGroudType)
		{
		case ms::MsTypes::DocumentsType::STORY:
		{
			if (getFreeContentOption() != ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
				backGround->setBackGroundImage("free2paid/freeItems/blue_light_background_lock.png");
			else
			{
				backGround->setBackGroundImage("free2paid/freeItems/blue_background.png");
			}

			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.title"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.value"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.item.type.name"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
			}
			break;
		}

		case ms::MsTypes::DocumentsType::PHONICS:
		{
			if (getFreeContentOption() != ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
				backGround->setBackGroundImage("free2paid/freeItems/green_light_background_lock.png");
			else
			{
				backGround->setBackGroundImage("free2paid/freeItems/green_background.png");
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.title"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.value"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.item.type.name"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
			}
			break;
		}
		case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
		{
			if (getFreeContentOption() != ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
				backGround->setBackGroundImage("free2paid/freeItems/violet_light_background_lock.png");
			else
			{
				backGround->setBackGroundImage("free2paid/freeItems/violet_background.png");
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.title"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.date.value"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
			}
			if (auto itemValue = utils::findChild<Text*>(lockBackGround, "lb.item.type.name"))
			{
				itemValue->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
			}
			break;
		}
		default:
			break;
		}
	}
	
}

void MsGroupFreeItems::setUnlockView()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::setUnlockView");
	//if (lockBackGround)
	//	lockBackGround->setVisible(false);
	if (auto itemName = utils::findChild<Text*>(unLockBackGround, "lb.item.type.name"))
	{
		(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? itemName->setFontName("fonts/leelawdb.ttf") : itemName->setFontName("fonts/Montserrat-ExtraBold.ttf");
		itemName->setString(getItemsNameByTypes());
	}

	if (auto itemValue = utils::findChild<Text*>(unLockBackGround, "lb.date.value"))
	{
		itemValue->setString(StringUtils::format("%02d", getDay()));
	}

	this->setContentSize(unLockBackGround->getContentSize());
	unLockBackGround->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	unLockBackGround->setPosition(Vec2(0, 0));
	_delegate ? _delegate->onTouchToGroupItem(true, getDay()) : (void) nullptr;

}

void MsGroupFreeItems::initListView()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initListView");
	if (!getListViewItem())
		return;
	getListViewItem()->setVisible(true);
	getListViewItem()->setOpacity(255);
	getListViewItem()->setZOrder(INT_MAX);
	getListViewItem()->setDirection(ScrollView::Direction::HORIZONTAL);
	getListViewItem()->setItemsMargin(getListViewItem()->getContentSize().width/5);
	getListViewItem()->setMagneticType(ListView::MagneticType::LEFT);
	getListViewItem()->setScrollBarEnabled(false);
	getListViewItem()->setTouchEnabled(false);
	getListViewItem()->removeAllChildren();
	Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
		for (int i = 0; i < getFreeItemInfos().size(); i++)
		{
			mj::model::FreeItemInfo freeItemInfo = getFreeItemInfos().at(i);
			MsFreeItem* msFreeItem = initFreeItem(freeItemInfo);
			if (!msFreeItem)
				continue;
			//msGroupFreeItems->setVisible(false);
			_msFreeItems.push_back(msFreeItem);

		}
		for (MsFreeItem* msFreeItem : getFreeItems())
		{
			msFreeItem->setVisible(true);
			getListViewItem()->pushBackCustomItem(msFreeItem);
		}
		getListViewItem()->requestDoLayout();
		getListViewItem()->jumpToLeft();
		});
}
std::vector<int> MsGroupFreeItems::getAllStoriesLearn()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::getAllStoriesLearn");
	std::vector<int> listIds =  _delegate->getAllStoriesLearn() ;
	if(_delegate)
		listIds = _delegate->getAllStoriesLearn();
	return 	listIds;
}
bool MsGroupFreeItems::checkGroupLearn()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::checkGroupLearn");
	bool result = false;
	for (auto freeItem: getFreeItemInfos())
	{

	}
	return false;
}

void MsGroupFreeItems::changeStateLockUnclock()
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::changeStateLockUnclock");
	this->runAction(Sequence::create(DelayTime::create(1.0f),
		CallFunc::create([=] {
			if (getIsUnlocked())
			{
				if (unLockBackGround)
				{
					unLockBackGround->setVisible(true);
				}
				if (lockBackGround)
				{
					lockBackGround->setVisible(false);
				}
			}
			else
			{
				if (lockBackGround)
				{
					lockBackGround->setVisible(true);
				}
				if (unLockBackGround)
				{
					if (getFreeContentOption() == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
					{
						unLockBackGround->setVisible(true);
						setUnlockView();
					}
					else
					{
						unLockBackGround->setVisible(false);
					}
				}
			}
			onUnlockItems();
			}),
		NULL));


}

void MsGroupFreeItems::setDefaultView()
{
	if (unLockBackGround)
	{
		unLockBackGround->setVisible(false);
	}
	if (lockBackGround)
	{
		lockBackGround->setVisible(true);
	}
}

void MsGroupFreeItems::checkCanUnLockDay(int day)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::checkCanUnLockDay");
	// neu qua ngay cua item la ngay hien tai, user co the unlock item cua ngay do
	// trang thai co the unlock nay la IsCanUnlock
	// trang thai da unlock hay chua la IsUnlocked, duoc danh dau bang viec user da click vao groupItem cua ngay hom do chua.
	vector<int>unlockedDays = {};

	// check boi ngoai le khong co item con
	if (getFreeItemInfos().empty())
	{
		setIsCanUnlock(false);
		return;
	}
	int unlockday = MJDEFAULT->getIntegerForKey(key_f2p_save_learned_items_last_day, 1);
	int groupDay = getFreeItemInfos().at(0)._day;

	// check boi ngoai le trang thai da hoc cua item con
    
	if (unlockday >= groupDay)// neu bai hoc cua ngay nay da hoc, unlock group item nay
	{
			setIsCanUnlock(true);
			setIsUnlocked(true);
	}
	else if (unlockday == groupDay - 1)
	{
		// neu la ngay tiep theo
#if defined(COCOS2D_DEBUG) && (COCOS2D_DEBUG > 0)
		int currentDay = mj::helper::getTimeInMinutes();
#else
		int currentDay = mj::helper::getTimeInDays();
#endif
		int dayLearnedLastItem = MJDEFAULT->getIntegerForKey(key_f2p_save_last_date_of_learned_items, currentDay);
		if (currentDay >= dayLearnedLastItem + 1)
		{
			setIsCanUnlock(true);
			setIsUnlocked(false);
		}
		else
		{
			setIsCanUnlock(false);
			setIsUnlocked(false);
		}
	}
	else
	{
		setIsCanUnlock(false);
		setIsUnlocked(false);
	}


	if (groupDay == 1 || day == groupDay)
	{
		setIsCanUnlock(true);
		setIsUnlocked(false);
	}

	changeStateLockUnclock();
}

void MsGroupFreeItems::onSaveAvataUrl()
{
	MJDEFAULT->setArrayStringForKey(key_f2p_save_items_learned_avata_url, getAvataUrl());

}

void MsGroupFreeItems::checkItemsUnlockButHavenotLearned()
{
	// ham nay xu ly viec item da duoc mo khoa, nhung chua hoan thanh bai hoc 
	if(getIsCanUnlock()&& !getIsUnlocked())
	{
		for (auto item:getFreeItemInfos() )
		{
			if (!checkItemLearned(item)) // neu co bai hoc nao chua duoc hoc, nghia la bai hoc hom do chua hoan thanh, user se khong duoc hoc bai tiep theo
			{
				MJDEFAULT->setBoolForKey(key_f2p_save_free_items_learned_on_day, true);
				break;
			}
		}
	}
}

MsFreeItem* MsGroupFreeItems::initFreeItem(mj::model::FreeItemInfo freeItemInfo)
{
	CRASH_AUTO_LOG;
	CCLOG("MsGroupFreeItems::initFreeItem");
	MsFreeItem* msFreeItem = MsFreeItem::createView(freeItemInfo, getFreeContentOption(), this, getListViewItem()->getContentSize().height);

	return msFreeItem;
}
