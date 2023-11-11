///////////////////////////////////////////////////////////
//  MsFreeItem.cpp
//  Implementation of the Class MsFreeItem.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MsFreeItem.h"


INITIALIZE_READER(MsFreeItem);

/*
 * 1. New, Popular
 * 2. Trong các topic, hiện các audiobook có quality cao nhất. Nếu có cùng quality score thì ưu tiên truyện chưa được nghe. Cùng truyện chưa được nghe, ưu tiên truyện có publish_date mới nhất (tối đa 10)
 */

std::once_flag _MsFreeItem_reader;

MsFreeItem::MsFreeItem(){
	CRASH_AUTO_LOG;

}

MsFreeItem::~MsFreeItem(){
	CRASH_AUTO_LOG;

}

MsFreeItem* MsFreeItem::createView(mj::model::FreeItemInfo freeItemInfo, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionFreeItemDelegate* delegate_, int listViewHeight)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::createView");

	std::call_once(_MsFreeItem_reader, []
		{
			REGISTER_CSB_READER(MsFreeItem);
		});

	auto csbName = mj::helper::getCSBName("csb/free2paid/MsFreeItem.csb");

	auto node = reinterpret_cast<MsFreeItem*>(CSLoader::createNode(csbName));

	if (node)
	{
		node->didLoadFromCSB(freeItemInfo, freeContentOption, delegate_, listViewHeight);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}


void MsFreeItem::didLoadFromCSB(mj::model::FreeItemInfo freeItemInfo, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionFreeItemDelegate* delegate_, int listViewHeight)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::didLoadFromCSB");

	this->setTouchEnabled(true);
	setFreeContentOption(freeContentOption);
	setItemInfo(freeItemInfo);
	setDelegate(delegate_);
	if (freeContentOption == ms::f2p::FreeContentOption::MULTILE_ITEM_ONE_DAY)
	{
		int height = this->getContentSize().height;
		float scale = (float)(listViewHeight - 50) / (float)height;
		this->setScale(scale);
	}
	else if(freeContentOption == ms::f2p::FreeContentOption::TEN_ITEM_FREE)
	{
		isTenItemFree = true;
		int height = this->getContentSize().height;
		float scale = (float)(listViewHeight) / (float)height;
		this->setScale(scale * 0.9);
		if (auto date = utils::findChild<Layout*>(this, "date_image"))
		{
			date->setVisible(false);
		}
	}
	else
	{
		int height = this->getContentSize().height;
		float scale = (float)(listViewHeight) / (float)height;
		this->setScale(scale*0.9);
	}
	initView();
}

void MsFreeItem::onExit()
{
	CRASH_AUTO_LOG;

	Layout::onExit();
}

void MsFreeItem::onEnter()
{
	CRASH_AUTO_LOG;

	Layout::onEnter();
}

void MsFreeItem::initView()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::initView");
	this->setVisible(false);
	checkUnlockDay();
	//Layout* _root = nullptr, * back_ground = nullptr, * date_image = nullptr, * avata_image = nullptr;
	_root = utils::findChild<Layout*>(this, "root");
	_back_ground = utils::findChild<Layout*>(this, "back_ground");
	_date_image = utils::findChild<Layout*>(this, "date_image");
	_avata_image = utils::findChild<Layout*>(this, "avata_image");

	 _date_title = utils::findChild<Text*>(this, "lb.date.title");
	 if (LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_ID_CODE)
	 {
		 _date_title->setFontSize(_date_title->getFontSize() - 2);
	 }

	_date_value = utils::findChild<Text*>(this, "lb.date.value");

	_learned_state = utils::findChild<Layout*>(this, "learned_state");
	setViewByType();
	initdata([=](bool isSuccess) {
		
		});


}

void MsFreeItem::initdata(std::function<void(bool)> callback)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::initdata");

	
	LANGUAGE_MANAGER->changeDisplayLanguage(this);
	if (auto name = utils::findChild<Text*>(this, "lb.item.type.name"))
	{
		(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? name->setFontName("fonts/leelawdb.ttf") : name->setFontName("fonts/Montserrat-ExtraBold.ttf");
		name->setString(getItemsNameByTypes());
	}
	if (auto day = utils::findChild<Text*>(this, "lb.date.value"))
	{
		day->setString(StringUtils::format("%02d", getItemInfo()._day));
	}
	getRealItemInfo();
	std::string url = getAvataUrl();

	getItemInfo()._url = url;
	_delegate ? _delegate->onsetAvataUrl(url) : (void) nullptr;
	if (!url.empty())
	{
		auto avata = RemoteImageView::createImage(url, [=](RemoteImageView* img, bool is_success) {}, RemoteImageView::Type::OTHER);
		if (_avata_image)
		{
			_avata_image->setVisible(false);
			_avata_image->getParent()->addChild(avata, _avata_image->getZOrder());
			avata->setContentSize(_avata_image->getContentSize());
			avata->setAnchorPoint(_avata_image->getAnchorPoint());
			avata->setPosition(isTenItemFree? (_avata_image->getPosition() + Vec2(0, 30)) : _avata_image->getPosition());
			
			avata->setVisible(true);
			if (auto cloud = utils::findChild<Layout*>(this, "cloud_image"))
			{
				cloud->setZOrder(avata->getZOrder() + 1);
			}
		}
	}
	this->setVisible(true);

}

std::string MsFreeItem::getAvataUrl()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::getAvataUrl");

	ms::MsTypes::DocumentsType itemType = getItemInfo()._type;
	std::string url = "";
	auto itemInfo = getItemInfo();
	switch (itemType)
	{
	case ms::MsTypes::DocumentsType::STORY:
		{
			url = getStoryItemInfo().thumbnail_url;
			break;
		}	
	case ms::MsTypes::DocumentsType::PHONICS:
		{
		}
	case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
		{
			url = getLessonItemInfo().thumbnail_url;
			break;
		}

	default:
		break;
	}
	return url;
}
void MsFreeItem::getRealItemInfo()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::getRealItemInfo");

	ms::MsTypes::DocumentsType type = getItemInfo()._type;

	switch (type)
	{
	case ms::MsTypes::DocumentsType::STORY:
	{
		setStoryItemInfo(StoryDataModel::getInstance()->getStoryById(getItemInfo()._id));
		break;
	}
	case ms::MsTypes::DocumentsType::PHONICS:
	{
	}
	case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
	{
		setLessonItemInfo(StoryDataModel::getInstance()->getLessonsById(getItemInfo()._id));
		break;
	}

	default:
		break;
	}
}
std::string MsFreeItem::getItemsNameByTypes()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::getItemsNameByTypes");

	std::string itemName = LANGUAGE_MANAGER->getDisplayTextByKey("name.stories");

	ms::MsTypes::DocumentsType type = getItemInfo()._type;
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
	return itemName.c_str();
}

void MsFreeItem::onClick(cocos2d::Ref* sender)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::onClick");

	onLearn();
}

cocos2d::ui::Widget::ccWidgetClickCallback MsFreeItem::onLocateClickCallback(const std::string& callback_name)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::onLocateClickCallback");

	std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(MsFreeItem, onClick),
	};
	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}
	return nullptr;
}

bool MsFreeItem::checkItemsLearned()
{
	CCLOG("MsFreeItem::checkItemsLearned");

	bool isLearned = false;
	if (_delegate)
		isLearned = _delegate->checkItemLearned(getItemInfo());
	return isLearned;
}


void MsFreeItem::checkUnlockDay()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::checkUnlockDay");

	bool unlock = false;

	if (checkItemsLearned())
	{
		if (auto date_image = utils::findChild<Layout*>(this, "date_image"))
		{
			if (auto date_title = utils::findChild<Text*>(this, "lb.date.title"))
			{
				date_title->setVisible(false);
			}
			if (auto date_value = utils::findChild<Text*>(this, "lb.date.value"))
			{
				date_value->setVisible(false);

			}
			if (auto learned_state = utils::findChild<Layout*>(this, "learned_state"))
			{
				learned_state->setVisible(true);
				learned_state->setBackGroundImage("free2paid/freeItems/check_icon.png");
			}

		}
		unlock = true;
	}
	else
	{
		if (auto date_image = utils::findChild<Layout*>(this, "date_image"))
		{
			if (auto date_title = utils::findChild<Text*>(this, "lb.date.title"))
			{
				date_title->setVisible(true);
			}
			if (auto date_value = utils::findChild<Text*>(this, "lb.date.value"))
			{
				date_value->setVisible(true);

			}
			if (auto learned_state = utils::findChild<Layout*>(this, "learned_state"))
			{
				learned_state->setVisible(false);
			}

		}
		unlock = false;
	}
	if(unlock && _delegate)
        _delegate->onItemsLearn(getItemInfo()._day);
}

bool MsFreeItem::checkItemStoryLearned(mj::model::FreeItemInfo freeItemInfo)
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::checkItemStoryLearned");

	std::vector<int> _listIdOfStoryLearned = getAllStoriesLearn();
	if (_listIdOfStoryLearned.size() <= 0)
		return false;
	for (int id : _listIdOfStoryLearned)
	{
		if (id == freeItemInfo._id)
			return true;
	}
	return false;
}

std::vector<int> MsFreeItem::getAllStoriesLearn()
{
	CCLOG("MsFreeItem::getAllStoriesLearn");

	std::vector<int> listIds;
	if (_delegate)
		listIds = _delegate->getAllStoriesLearn();
	return 	listIds;
}

void MsFreeItem::onLearn()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::onLearn");

	ms::MsTypes::DocumentsType type = getItemInfo()._type;

	MJDEFAULT->setIntegerForKey(key_f2p_current_learning_item_id, getItemInfo()._id);

	switch (type)
	{
	case ms::MsTypes::DocumentsType::STORY:
	{
		onLearnStory();
		break;
	}
	case ms::MsTypes::DocumentsType::PHONICS:
	{
	}
	case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
	{
		onLearnLesson();

		break;
	}

	default:
		break;
	}
}

void MsFreeItem::onLearnLesson()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::onLearnLesson");


	if (getLessonItemInfo().lesson_id == 0)
		return;
	_delegate ? _delegate->onLearnLessonFree(getLessonItemInfo()) : (void) nullptr;
}

void MsFreeItem::onLearnStory()
{
	CRASH_AUTO_LOG;
	CCLOG("MsFreeItem::onLearnStory");

	if (getStoryItemInfo().story_id == 0)
		return;
	_delegate ? _delegate->onLearnStory(getStoryItemInfo()) : (void) nullptr;

}

void MsFreeItem::onDownload()
{
	CRASH_AUTO_LOG;

}

void MsFreeItem::setViewByType()
{
	ms::MsTypes::DocumentsType type= getItemInfo()._type;
	switch (type)
	{
	case ms::MsTypes::DocumentsType::STORY:
	{		
		if (auto value = utils::findChild<Text*>(this, "lb.item.type.name"))
			{
			value->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
			}
		_back_ground->setBackGroundImage(isTenItemFree ? "free2paid/freeItems/free_blue_backgound.png" : "free2paid/freeItems/blue_light_background.png");
		_date_title->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));
		_date_value->setTextColor(Color4B(mj::helper::colorFromHex(0x01C4F8)));


		//_learned_state = utils::findChild<Layout*>(this, "learned_state");
		break;
	}
	case ms::MsTypes::DocumentsType::CATEGORY:
		break;
	case ms::MsTypes::DocumentsType::AUDIOBOOK_CATEGORY:
		break;
	case ms::MsTypes::DocumentsType::AUDIOBOOK:
		break;
	case ms::MsTypes::DocumentsType::LESSON:
		break;
	case ms::MsTypes::DocumentsType::PHONICS:
	{
		if (auto value = utils::findChild<Text*>(this, "lb.item.type.name"))
		{
			value->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
		}
		_back_ground->setBackGroundImage(isTenItemFree ? "free2paid/freeItems/free_green_backgound.png" : "free2paid/freeItems/green_light_background.png");
		_date_title->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
		_date_value->setTextColor(Color4B(mj::helper::colorFromHex(0x65BA12)));
		break;
	}
	case ms::MsTypes::DocumentsType::READING_COMPREHENTION:
	{
		if (auto value = utils::findChild<Text*>(this, "lb.item.type.name"))
		{
			value->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
		}
		_back_ground->setBackGroundImage(isTenItemFree ? "free2paid/freeItems/free_violet_backgound.png" : "free2paid/freeItems/violet_light_background.png");
		_date_title->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
		_date_value->setTextColor(Color4B(mj::helper::colorFromHex(0x7666D2)));
		break;
	}
	case ms::MsTypes::DocumentsType::AI_SPEAKING:
		break;
	default:
		break;
	}

}
