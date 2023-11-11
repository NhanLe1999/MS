#ifndef MJ_GAME_PHAN_LOAI_ITEM_H__
#define MJ_GAME_PHAN_LOAI_ITEM_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "ui/CocosGUI.h"

class GamePhanLoai_ItemDelegate {
public:
	virtual void onClickedItem(cocos2d::Ref * sender) {};
	virtual void onMoveBeginItem(cocos2d::Ref * sender) {};
	virtual void onMoveEndedItem(cocos2d::Ref * sender,bool is_item_moved) {};
};
enum class GamePhanLoai_ItemTouchType {
	NONE,
	CLICK,
	TOUCH
};
class GamePhanLoai_Item : public cocos2d::ui::Layout
{
public:
	CREATE_FUNC(GamePhanLoai_Item);
	static GamePhanLoai_Item * createItem(mj::PHONIC_INFO info,std::string mask = "");
	mj::PHONIC_INFO getInfo() const;
	void setInfo(const mj::PHONIC_INFO info);
	void setDelegate(GamePhanLoai_ItemDelegate * delegate) { _delegate = delegate; };
	void showItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void hideItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void backToStart(float time = 0.25f);
	void setTouchHandleType(GamePhanLoai_ItemTouchType type);
	void setTouchEnable(bool enable);
	cocos2d::Vec2 _start_pos;
	void setCompareValue(std::vector<int> values);
	int isContainValue(int value);
protected :
	GamePhanLoai_ItemDelegate * _delegate;
private:
	void createTexture();
	void didLoadFromCSB(mj::PHONIC_INFO info, std::string mask);
	bool m_is_moved;
	std::string font_name = "fonts/Montserrat-SemiBold.ttf";
	mj::PHONIC_INFO m_item_info;
	cocos2d::ui::Layout * m_layout_touch;
	cocos2d::ui::ImageView * m_img_item;
	std::vector<int> m_compare_value;
};

#endif // !MJ_GAME_PHAN_LOAI_ITEM_H__
