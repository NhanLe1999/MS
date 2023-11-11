///////////////////////////////////////////////////////////////////////
//  MsFreeItem_hpp
//  Implementation of the Class MsFreeItem_hpp
//  Created on:      21-04-2022
//  Original author: TruongBX
///////////////////////////////////////////////////////////


#ifndef MsFreeItem_hpp
#define MsFreeItem_hpp
#include <stdio.h>
#include <string>
#include <vector>
#include "cocostudiomacro2.h"
#include "ui/CocosGui.h"		
#include "cocos2d.h"
#include "platform/CCPlatformConfig.h"

#include "MJDefinitions.h"
#include "RemoteImageView.h"
#include "MSCollectionFreeItemDelegate.h"
#include "MSCollectionStoryItem.h"
#include "StoryDataModel.h"
#include "MSEventLogger.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
#define DELAY_CREAT_LESSON 0.05
class MsFreeItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{

public:
	CREATE_FUNC(MsFreeItem);
	MsFreeItem();
	virtual ~MsFreeItem();
	static MsFreeItem* createView(mj::model::FreeItemInfo freeItemInfo, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionFreeItemDelegate* delegate_, int listViewHeight);

private:
	void didLoadFromCSB(mj::model::FreeItemInfo freeItemInfo, ms::f2p::FreeContentOption freeContentOption, ms::delegate::MSCollectionFreeItemDelegate* delegate_, int listViewHeight);
    void onExit() override;   
	void onEnter() override;
	void initView();
	void initdata(std::function<void(bool)> callback = nullptr);
	std::string getAvataUrl();
	void getRealItemInfo();
	std::string getItemsNameByTypes();
	void onClick(cocos2d::Ref* sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name);
	bool checkItemsLearned();
	void checkUnlockDay();
	bool checkItemStoryLearned(mj::model::FreeItemInfo freeItemInfo);
	std::vector<int> getAllStoriesLearn();
	void onLearn();
	void onLearnLesson();
	void onLearnStory();
	void onDownload();
	void setViewByType();
	CC_SYNTHESIZE(ms::f2p::FreeContentOption, _freeContentOption, FreeContentOption);
	CC_SYNTHESIZE(mj::model::FreeItemInfo, _ItemInfo, ItemInfo);
	CC_SYNTHESIZE(mj::model::StoryInfo, _storyItemInfo, StoryItemInfo);
	CC_SYNTHESIZE(mj::model::LessonInfo, _lessonItemInfo, LessonItemInfo);

	CC_SYNTHESIZE(ms::delegate::MSCollectionFreeItemDelegate*, _delegate, Delegate);
	Layout* _root = nullptr, * _back_ground = nullptr, * _date_image = nullptr, * _avata_image = nullptr, * _learned_state;
	Text* _date_title = nullptr, * _date_value = nullptr; 
	std::string avataUrl = "";
	bool isTenItemFree = false;
};
CREATE_READER(MsFreeItem);

#endif // defined(MsFreeItem_hpp)
