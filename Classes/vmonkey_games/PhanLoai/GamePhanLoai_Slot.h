#ifndef MJ_GAME_PHAN_LOAI_SLOT_H__
#define MJ_GAME_PHAN_LOAI_SLOT_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "GAFWrapper.h"
#include "cocostudiomacro2.h"
#include "GamePhanLoai_Item.h"

class GamePhanLoai_SlotDelegate {
public:
	virtual void onClickedSlotDelegate(cocos2d::Ref * sender) {};
};



class GamePhanLoai_Slot : public cocos2d::ui::Layout {
public:
	const static uint8_t SHOW_TEXT = 0x01;
	const static uint8_t SHOW_ICON = 0x02;
	const static uint8_t SHOW_BACKGROUND = 0x04;
	const static uint8_t SHOW_CUSTOM_BG = 0x08;

	enum class GamePhanLoai_SlotState {
		NONE,
		SHOWED,
		PAIRED,
		HIDED
	};
	CREATE_FUNC(GamePhanLoai_Slot);
	static GamePhanLoai_Slot * createSlot(mj::PHONIC_INFO info, std::string custom_background = "");
	void setDelegate(GamePhanLoai_SlotDelegate * delegate);
	void onShow(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void onHide(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void setCompareValue(int value);
	void setSlotSize(cocos2d::Size size);
	void setSlotEnabled(int size_type, uint8_t mode);
	int getCompareValue();
	mj::PHONIC_INFO _info;
protected:
	GamePhanLoai_SlotDelegate * _delegate;
	uint8_t _slot_mode;
private:
	void onClickedSlot(cocos2d::Ref * sender);
	void didLoadFromCSB(mj::PHONIC_INFO info, std::string custom_background = "");
	void fitValueTextSize();
	std::string font_name = "fonts/Montserrat-SemiBold.ttf";
	GamePhanLoai_SlotState m_slot_state;

	cocos2d::ui::Layout * m_layout_touch = nullptr;
	cocos2d::ui::ImageView * m_character = nullptr;

	cocos2d::ui::ImageView * m_background = nullptr;
	cocos2d::ui::ImageView * m_custom_background = nullptr;
	cocos2d::ui::ImageView * m_bg_title = nullptr;
	
	cocos2d::ui::Text * m_title = nullptr;
	int m_compare_value;
	
};


#endif // !MJ_GAME_PHAN_LOAI_SLOT_H__
