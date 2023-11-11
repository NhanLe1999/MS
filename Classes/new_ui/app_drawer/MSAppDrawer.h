//
//  MSAppDrawer.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSAppDrawer_hpp
#define MSAppDrawer_hpp

#include "cocostudiomacro2.h"
#include "MSSidebarDelegate.h"

class MSAppDrawer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol 
{
public:

	CREATE_FUNC(MSAppDrawer);
    
	static MSAppDrawer * createDrawer();

private:

	~MSAppDrawer();

	void didLoadFromCSB();
    
	void onEnter() override;
    
    void onClose(cocos2d::Ref * sender);
    void onPlay(cocos2d::Ref * sender);
	void onItemClick(cocos2d::Ref * sender);

    void moveBackground(cocos2d::ui::Button * selected, cocos2d::Color3B color);

	void updateHightlightBackground(cocos2d::Node * i_node);

	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;

	void pushCleverTapTrackingData();

private: 
	CC_SYNTHESIZE(ms::SidebarDelegate*, _delegate, Delegate);
	bool isDownloadClicked = false;

	std::string productName = "none";

	std::vector<cocos2d::Node*> m_appContentLayers;

};

CREATE_READER(MSAppDrawer);

#endif /* MSAppDrawer_hpp */
