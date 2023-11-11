#ifndef MJ_DIEN_THE_SLOT_H__
#define MJ_DIEN_THE_SLOT_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "GAFWrapper.h"
#include "cocostudiomacro2.h"
#include "DienThe_Item.h"

class DienThe_SlotDelegate {
public:
	virtual void onClickedSlotDelegate(cocos2d::Ref * sender) {};
};



class DienThe_Slot : public cocos2d::ui::Layout {
public:
	enum class DienThe_SlotState {
		NONE,
		SHOWED,
		PAIRED,
		HIDED
	};
	CREATE_FUNC(DienThe_Slot);
	static DienThe_Slot * createSlot(int order, std::string custom_background = "");
	void setDelegate(DienThe_SlotDelegate * delegate);
	void onShow(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void onHide(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void setLinker(DienThe_Item * linker);
	DienThe_Item * getLinker();
	void removeLinker();

	cocos2d::Vec2 _start_pos;
protected:
	DienThe_SlotDelegate * _delegate;
private:
	void onClickedSlot(cocos2d::Ref * sender);
	void didLoadFromCSB(int order, std::string custom_background = "");

	std::string font_name = "fonts/FatMarker.ttf";
	DienThe_SlotState m_slot_state;

	cocos2d::ui::Layout * m_layout_touch;

	cocos2d::ui::ImageView * m_background;
	DienThe_Item * m_linker;
};


#endif // !MJDIEN_THESLOT_H__
