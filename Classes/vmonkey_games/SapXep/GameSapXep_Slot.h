#ifndef MJ_GAME_SAP_XEP_SLOT_H__
#define MJ_GAME_SAP_XEP_SLOT_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "GAFWrapper.h"
#include "cocostudiomacro2.h"
#include "GameSapXep_Item.h"

class GameSapXep_SlotDelegate {
public:
	virtual void onClickedSlotDelegate(cocos2d::Ref * sender) {};
};



class GameSapXep_Slot : public cocos2d::ui::Layout {
public:
	enum class GameSapXep_SlotState {
		NONE,
		SHOWED,
		PAIRED,
		HIDED
	};
	CREATE_FUNC(GameSapXep_Slot);
	static GameSapXep_Slot * createSlot(int order, std::string custom_background = "");
	void setDelegate(GameSapXep_SlotDelegate * delegate);
	void onShow(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void onHide(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void setLinker(GameSapXep_Item * linker);
	GameSapXep_Item * getLinker();
	void removeLinker();

	cocos2d::Vec2 _start_pos;
protected:
	GameSapXep_SlotDelegate * _delegate;
private:
	void onClickedSlot(cocos2d::Ref * sender);
	void didLoadFromCSB(int order, std::string custom_background = "");

	std::string font_name = "fonts/FatMarker.ttf";
	GameSapXep_SlotState m_slot_state;

	cocos2d::ui::Layout * m_layout_touch;

	cocos2d::ui::ImageView * m_background;
	cocos2d::ui::Text * m_lbl_order;
	GameSapXep_Item * m_linker;
};


#endif // !MJ_GAME_SAP_XEP_SLOT_H__
