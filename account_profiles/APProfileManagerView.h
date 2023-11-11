#ifndef APProfileManagerView_hpp
#define APProfileManagerView_hpp

#include "cocostudiomacro2.h"

class APProfileManagerView : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(APProfileManagerView);
	static APProfileManagerView * createView();
private:
	void didLoadFromCSB();
	void onEnter() override;
	void onExit() override;
	
	void onClickAddLicense(cocos2d::Ref * sender);
	void onClickEditProfile(cocos2d::Ref * sender);
	void onClickDetailProfile(cocos2d::Ref * sender);
	void onClickSignOut(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref * sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	cocos2d::ui::TextField * txt_name;
	cocos2d::ui::TextField * txt_email;
	cocos2d::ui::TextField * txt_phone;
	cocos2d::ui::TextField * txt_address;
};

CREATE_READER(APProfileManagerView);
#endif // !APProfileManagerView_hpp
