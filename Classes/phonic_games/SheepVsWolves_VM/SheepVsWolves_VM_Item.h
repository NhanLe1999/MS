#ifndef MJ_GAME_SHEEP_VS_WOLVES_VM_ITEM_H_
#define MJ_GAME_SHEEP_VS_WOLVES_VM_ITEM_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "GAFWrapper.h"
#include "SheepVsWolves_VMInfoModel.h"

enum class SheepVsWolves_VMType {
	SHEEP_ONFARM,
	SHEEP,
	WOLVES
};
class SheepVsWolves_VM_ItemDelegate {
public:
	virtual void onClickSheepVsWolves_VMDelegate(cocos2d::Ref * sender) {};
};
class SheepVsWolves_VM_Item :public cocos2d::ui::Layout {
public:
	CREATE_FUNC(SheepVsWolves_VM_Item);
	static SheepVsWolves_VM_Item * createItem(SheepVsWolvesVM::PHONIC_INFO info,SheepVsWolves_VMType type = SheepVsWolves_VMType::SHEEP);
	SheepVsWolves_VMType getSheepType() const;
	SheepVsWolvesVM::PHONIC_INFO getInfo() const;
	void setSheepFlipped(bool flipX = false , bool flipY = false);
	void playSheepSequence(std::string sequenceName, bool loop = false, std::function<void(gaf::GAFObject * obj,const std::string &sqName)> call_back = nullptr);
	void setDelegate(SheepVsWolves_VM_ItemDelegate * delegate);
	cocos2d::ui::Text * getLableInfo();
	void setSheepMissing(bool missing);
	void setEnableClick(bool enable);
	bool isClickEnabled();
protected:
	SheepVsWolves_VM_ItemDelegate * _delegate{ nullptr };
private:
	void didLoadFromCSB(SheepVsWolvesVM::PHONIC_INFO info,SheepVsWolves_VMType type);
	SheepVsWolves_VMType m_type;
	cocos2d::Size sheep_size;
	SheepVsWolvesVM::PHONIC_INFO m_sheep_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_touch{ nullptr };
	cocos2d::ui::Text * m_lbl_info{ nullptr };
	cocos2d::ui::ImageView * m_img_sheepplacement{ nullptr };
	GAFWrapper * m_gaf_sheep_wolves{ nullptr };
	std::string font_name = "fonts/danhvan.ttf";
};


#endif // !MJ_GAME_SHEEP_VS_WOLVES_ITEM_H_
