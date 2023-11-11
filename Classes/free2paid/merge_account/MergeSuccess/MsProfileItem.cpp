///////////////////////////////////////////////////////////
//  MsProfileItem.cpp
//  Implementation of the Class MsProfileItem.cpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////

#include "MsProfileItem.h"


INITIALIZE_READER(MsProfileItem);

/*
 */

std::once_flag _MsProfileItem_reader;

MsProfileItem::MsProfileItem(){
	CRASH_AUTO_LOG;

}

MsProfileItem::~MsProfileItem(){
	CRASH_AUTO_LOG;

}

MsProfileItem* MsProfileItem::createView(ap::Profile profileInfo, ms::delegate::MSCollectionProfileItemDelegate* delegate_, bool showDeleteButton)
{
	CRASH_AUTO_LOG;
	CCLOG("MsProfileItem::createView");

	std::call_once(_MsProfileItem_reader, []
		{
			REGISTER_CSB_READER(MsProfileItem);
		});

	auto csbName = mj::helper::getCSBName("csb/free2paid/MsProfileItem.csb");

	auto node = reinterpret_cast<MsProfileItem*>(CSLoader::createNode(csbName));

	if (node)
	{
		node->didLoadFromCSB(profileInfo, delegate_, showDeleteButton);
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;
}


void MsProfileItem::didLoadFromCSB(ap::Profile profileInfo, ms::delegate::MSCollectionProfileItemDelegate* delegate_, bool showDeleteButton)
{
	CRASH_AUTO_LOG;
	CCLOG("MsProfileItem::didLoadFromCSB");

	this->setTouchEnabled(true);
	setIsShowDeleteButton(showDeleteButton);
	setProfileInfo(profileInfo);
	setDelegate(delegate_);
	initView(getIsShowDeleteButton());

}

void MsProfileItem::onExit()
{
	CRASH_AUTO_LOG;

	Layout::onExit();
}

void MsProfileItem::onEnter()
{
	CRASH_AUTO_LOG;

	Layout::onEnter();
}

void MsProfileItem::initView(bool showDeleteButton)
{
	CRASH_AUTO_LOG;
	CCLOG("MsProfileItem::initView");
	this->setVisible(false);
	//Layout* _root = nullptr, * back_ground = nullptr, * date_image = nullptr, * avata_image = nullptr;
	_root = utils::findChild<Layout*>(this, "root");
	_back_ground = utils::findChild<Layout*>(this, "back_ground");
	_deleteButton = utils::findChild<Layout*>(this, "deleteButton");
	_name = utils::findChild<Text*>(this, "lb.item.type.name");
	_imgProfile = utils::findChild<ImageView*>(this, "img_profile");
	if (_deleteButton)
		_deleteButton->setVisible(true);

	if (_name)
	{
		(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE) ? _name->setFontName("fonts/leelawdb.ttf") : _name->setFontName("fonts/Montserrat-Bold.ttf");
		_name->setString(getProfileInfo().name);
	}
	if (_imgProfile)
	{
		_imgProfile->loadTexture(_profileInfo.avatar);
	}
	if (_name)
	{
		_name->setString(_profileInfo.name);
	}
	setHidenDeleteButton(showDeleteButton);
}

void MsProfileItem::setHidenDeleteButton(bool isHiden)
{
	if (auto deleteButton = utils::findChild<Layout*>(this, "deleteButton"))
	{
		deleteButton->setVisible(isHiden);
	}
}

void MsProfileItem::onSelectProfile(cocos2d::Ref* sender)
{
	CRASH_AUTO_LOG;
	CCLOG("MsProfileItem::onSelectProfile");
	_delegate ? _delegate->onSelectedProfile(getProfileInfo().profile_id) : (void) nullptr;
}
void MsProfileItem::onDeleteProfile(cocos2d::Ref* sender)
{

	CRASH_AUTO_LOG;
	_delegate ? _delegate->onDeleteSelectedProfile(getProfileInfo().profile_id) : (void) nullptr;
	//this->removeFromParent();
}
cocos2d::ui::Widget::ccWidgetClickCallback MsProfileItem::onLocateClickCallback(const std::string& callback_name)
{
	CRASH_AUTO_LOG;
	CCLOG("MsProfileItem::onLocateClickCallback");

	std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(MsProfileItem, onSelectProfile),
		CLICK_MAP(MsProfileItem, onDeleteProfile),
	};
	if (function_map.count(callback_name) > 0) {
		return function_map.at(callback_name);
	}
	return nullptr;
}

void MsProfileItem::scalingSize(float ratio)
{
	//ratio *= 0.8;
	auto children = this->getChildren();
	float oldHeight = this->getContentSize().height;
	this->setContentSize(this->getContentSize() * (ratio));
	for (auto child: children)
	{
		//child->setScale(ratio);
		
		child->setContentSize(child->getContentSize()*ratio);
		if (child->getName() != "deleteButton")
		{
			child->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			child->setPositionX(this->getContentSize().width / 2);
		}
		else {
			child->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			child->setScale(0.8);
			child->setPosition(Point(this->getContentSize().width - child->getContentSize().width,
				this->getContentSize().height - child->getContentSize().height));
		}
	}
	//this->setScale(ratio);
	
	
} 

