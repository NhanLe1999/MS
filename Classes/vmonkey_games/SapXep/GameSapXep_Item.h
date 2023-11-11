#ifndef MJ_GAME_SAP_XEP_ITEM_H__
#define MJ_GAME_SAP_XEP_ITEM_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "ui/CocosGUI.h"

class GameSapXep_ItemDelegate {
public:
	virtual void onClickedItem(cocos2d::Ref * sender) {};
	virtual void onMoveBeginItem(cocos2d::Ref * sender) {};
	virtual void onMoveEndedItem(cocos2d::Ref * sender,bool is_item_moved) {};
};
enum class GameSapXep_ItemTouchType {
	NONE,
	CLICK,
	TOUCH
};
class GameSapXep_Item : public cocos2d::ui::Layout
{
public:
	CREATE_FUNC(GameSapXep_Item);
	static GameSapXep_Item * createItem(mj::PHONIC_INFO info,std::string mask = "");
	mj::PHONIC_INFO getInfo() const;
	void setInfo(const mj::PHONIC_INFO info);
	void setDelegate(GameSapXep_ItemDelegate * delegate) { _delegate = delegate; };
	void showItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void hideItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void backToStart(float time = 0.25f);
	void setTouchHandleType(GameSapXep_ItemTouchType type);
	void setTouchEnable(bool enable);
   
    bool isPaired();
    void setPaired(bool is_paired);
    
	cocos2d::Vec2 _start_pos;
protected :
	GameSapXep_ItemDelegate * _delegate;
private:
	void createTexture();
	void didLoadFromCSB(mj::PHONIC_INFO info, std::string mask);
	bool m_is_moved;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	mj::PHONIC_INFO m_item_info;
	cocos2d::ui::Layout * m_layout_touch;
	cocos2d::ui::ImageView * m_img_item;
    bool m_is_paired = false;

	CC_SYNTHESIZE(bool, flag1, Flag1);

};

#endif // !MJ_GAME_SAP_XEP_ITEM_H__
