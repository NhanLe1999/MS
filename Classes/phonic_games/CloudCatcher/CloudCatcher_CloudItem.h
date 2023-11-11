//
//	CloudCatcher_CloudItem.h
//	Created by mintzy on 10/05/28
//
// 
#ifndef CloudCatcher_CloudItem_h
#define CloudCatcher_CloudItem_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;
class CloudCatcher_CloudItemDelegate {
public:
	virtual void onClickedCloudDelegate(cocos2d::Ref *sender) {};
	virtual void onChangeCloudStateDelegate(cocos2d::Ref * sender,int state) {};
};
class CloudCatcher_CloudItem :public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	enum CloudType {
		TEXT_PHONIC,
		PICTURE
	};
	enum CloudState {
		SHOWED,
		MOVING,
		IDLE,
		IDLE2,
		CLICKED,
		CATCHERED,
		FINNISH
	};
	CREATE_FUNC(CloudCatcher_CloudItem);
	static CloudCatcher_CloudItem * createItem(mj::PHONIC_INFO game_info,CloudType type = CloudType::TEXT_PHONIC,bool upper_case = false);	
	void setEnableClick(bool clickable);
	void setDelegate(CloudCatcher_CloudItemDelegate * delegate);


	mj::PHONIC_INFO _phonic_info;
	//AutoPlay
	void onCatcherCloud(function<void()> callback = nullptr);
	void onAutoClick();
	void showTapGuideGame(std::string gaf_path); 
	void hideTapGuideGame();
	CloudType getCloudType();
	bool isCatcher();
	bool isUsed();
	void setUsed(bool used);
	cocos2d::ui::Text * getLabelPhonic();
	cocos2d::ui::ImageView * getImagePhonic();
private: 
	bool is_catcher = false;
	bool clickable = true;
	bool is_used = false;
	CloudType type;
	int cloud_state;
	GAFWrapper * gaf_cloud;
	cocos2d::ui::ImageView * img_phonic;
	cocos2d::ui::Text * lbl_phonic;
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
	void didLoadFromCSB(mj::PHONIC_INFO game_info, CloudType type, bool upper_case);
	std::string font_name = "fonts/Linotte Regular.otf";//PhonicConfig::getInstance()->getGameFontName();

	cocos2d::ui::Layout* root_layout;
	CloudCatcher_CloudItemDelegate * _delegate;
	void onClickedCloud(cocos2d::Ref * sender);
	void onChangeCloudState(cocos2d::Ref * sender,int state);
};

CREATE_READER(CloudCatcher_CloudItem);


#endif /* CloudCatcher_CloudItem.h*/