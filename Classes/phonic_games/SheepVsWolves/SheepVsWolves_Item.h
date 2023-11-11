#ifndef MJ_GAME_SHEEP_VS_WOLVES_ITEM_H_
#define MJ_GAME_SHEEP_VS_WOLVES_ITEM_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UILayout.h"
#include "GAFWrapper.h"
#include "SheepVsWolvesInfoModel.h"

enum class SheepVsWolvesType {
	SHEEP_ONFARM,
	SHEEP,
	WOLVES
};
class SheepVsWolves_ItemDelegate {
public:
	virtual void onClickSheepVsWolvesDelegate(cocos2d::Ref * sender) {};
};
class SheepVsWolves_Item :public cocos2d::ui::Layout {
public:
	CREATE_FUNC(SheepVsWolves_Item);
	static SheepVsWolves_Item * createItem(sheepvswolves::PHONIC_INFO info,SheepVsWolvesType type = SheepVsWolvesType::SHEEP);
	SheepVsWolvesType getSheepType() const;
	sheepvswolves::PHONIC_INFO getInfo() const;
	void setSheepFlipped(bool flipX = false , bool flipY = false);
	void playSheepSequence(std::string sequenceName, bool loop = false, std::function<void(gaf::GAFObject * obj,const std::string &sqName)> call_back = nullptr);
	void setDelegate(SheepVsWolves_ItemDelegate * delegate);
	cocos2d::ui::Text * getLableInfo();
	void setSheepMissing(bool missing);
	void setEnableClick(bool enable);
	bool isClickEnabled();
protected:
	SheepVsWolves_ItemDelegate * _delegate{ nullptr };
private:
	void didLoadFromCSB(sheepvswolves::PHONIC_INFO info,SheepVsWolvesType type);
	SheepVsWolvesType m_type;
	cocos2d::Size sheep_size;
	sheepvswolves::PHONIC_INFO m_sheep_info;
	cocos2d::ui::Layout * m_layout_root{ nullptr };
	cocos2d::ui::Layout * m_layout_touch{ nullptr };
	cocos2d::ui::Text * m_lbl_info{ nullptr };
	cocos2d::ui::ImageView * m_img_sheepplacement{ nullptr };
	GAFWrapper * m_gaf_sheep_wolves{ nullptr };
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
};


#endif // !MJ_GAME_SHEEP_VS_WOLVES_ITEM_H_
