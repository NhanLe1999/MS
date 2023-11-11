//
//	MPBubbleBoom.h
//	Created by mintzy on 04/03/18
//
// 
#ifndef MPBubbleBoom_Item_h
#define MPBubbleBoom_Item_h

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;
class MPBubbleBoom_ItemDelegate {
public:
	virtual void onClickedBubbleBoomDelegate(cocos2d::Ref *sender) {};
	virtual void onChangeBubbleBoomStateDelegate(cocos2d::Ref * sender,int state) {};
};
class MPBubbleBoom_Item :public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(MPBubbleBoom_Item);
	static MPBubbleBoom_Item * createItem(mj::PHONIC_INFO game_info = mj::PHONIC_INFO(),bool bubble_star = false);	
	void setEnableClick(bool clickable);
	void setDelegate(MPBubbleBoom_ItemDelegate * delegate);
	enum BubbleState {
		NONE,
		SHOWED,
		PUSHING,
		FLYING,
		IDLE,
		CLICKED,
		BOOMED,
		FINNISH
	};
	void onBubbleFinishAction(std::string action_name,bool loop);
	void onBubbleBoomed(std::function<void()> callback = nullptr);
	bool isPhonicBubble() const;
	mj::PHONIC_INFO _phonic_info;
	//AutoPlay
	void onAutoClick();
	void showTapGuideGame(std::string gaf_path);
	void setBubbleSize(cocos2d::Size bubble_size);
	bool isBoomed();
	bool getUsed();
	bool isStarBubble();
	void setUsed(bool);
	void resetBubble();
	void setBubbleConfig(bool is_phonic_bubble = true, bool show_text = false, bool upper_case = false, bool newType = false);
private:
	bool is_star_bubble;
	bool is_boomed;
	bool is_used; 
	bool clickable = true;
	bool is_phonic_bubble;
	int bubble_state;
	
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
	void didLoadFromCSB(mj::PHONIC_INFO game_info, bool bubble_star);
	std::string font_name = "fonts/Linotte Regular.otf";//PhonicConfig::getInstance()->getGameFontName();
	cocos2d::ui::Text * lbl_text;
	cocos2d::ui::ImageView * img_bubble;
	cocos2d::ui::ImageView * img_toy;
	cocos2d::ui::Layout* root_layout;
	void fitValueTextSize();
	MPBubbleBoom_ItemDelegate * _delegate;
	void onClickedBubbleBoom(cocos2d::Ref * sender);
	void onChangeBubbleBoomState(cocos2d::Ref * sender,int state);
};

CREATE_READER(MPBubbleBoom_Item);


#endif /* MPBubbleBoom_Item.h*/