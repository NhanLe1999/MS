#pragma once
#ifndef APProfileItem_hpp
#define APProfileItem_hpp

#include "cocostudiomacro2.h"

#include "APProfile.h"

class APProfileItemDelegate
{
public:
	virtual void onClickSelectItem(cocos2d::Ref * sender) {};
};

class APProfileItem : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
	CREATE_FUNC(APProfileItem);
	static APProfileItem* createItem(const ap::Profile profile, bool selected = true);
	void setItemSelected(bool selected);
	void setDelegate(APProfileItemDelegate *delegate);
	void setProfile(ap::Profile profile);
	ap::Profile getProfile();
private:
	void didLoadFromCSB(const ap::Profile profile, bool selected);
	ap::Profile m_profile;
	bool m_is_selected;
	cocos2d::ui::ImageView* m_img_avatar;
	cocos2d::ui::Text* m_lbl_name;
	cocos2d::ui::Button* m_btn_select;

	APProfileItemDelegate* _delegate;
	void onSelect(cocos2d::Ref * sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &name) override;
};

CREATE_READER(APProfileItem);
#endif // !APProfileItem_hpp
