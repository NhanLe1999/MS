///////////////////////////////////////////////////////////////////////
//  MsProfileItem
//  Implementation of the Class MsProfileItem
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////


#ifndef MsProfileItem_hpp
#define MsProfileItem_hpp
#include <stdio.h>
#include <string>
#include <vector>
#include "cocostudiomacro2.h"
#include "ui/CocosGui.h"		
#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"

#include "MJDefinitions.h"
#include "RemoteImageView.h"
#include "MSCollectionProfileItemDelegate.h"
#include "StoryDataModel.h"
#include "MSEventLogger.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
#define DELAY_CREAT_LESSON 0.05


using namespace ms::delegate;
class MsProfileItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(MsProfileItem);
	MsProfileItem();
	virtual ~MsProfileItem();
	static MsProfileItem* createView(ap::Profile profileInfo, ms::delegate::MSCollectionProfileItemDelegate* delegate_, bool showDeleteButton = false);
	void setHidenDeleteButton(bool isHiden);
private:
	void didLoadFromCSB(ap::Profile profileInfo, ms::delegate::MSCollectionProfileItemDelegate* delegate_, bool showDeleteButton = false);
    void onExit() override;   
	void onEnter() override;
	void initView(bool showDeleteButton = false);
	void onSelectProfile(cocos2d::Ref* sender);
	void onDeleteProfile(cocos2d::Ref* sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
public:
	void scalingSize(float ratio);


	CC_SYNTHESIZE(ap::Profile, _profileInfo, ProfileInfo);

	CC_SYNTHESIZE(ms::delegate::MSCollectionProfileItemDelegate*, _delegate, Delegate);
	CC_SYNTHESIZE(bool, _isShowDeleteButton, IsShowDeleteButton);
	//CC_SYNTHESIZE(std::vector)

	Layout* _root = nullptr, * _back_ground = nullptr, * _deleteButton = nullptr;
	Text* _name = nullptr;
	ImageView* _imgProfile = nullptr;
	std::string avataUrl = "";
	bool _isSelected = false;
	

};
CREATE_READER(MsProfileItem);

#endif // defined(MsFreeItem_hpp)
