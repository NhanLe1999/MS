#ifndef MJ_DIEN_THE_ITEM_H__
#define MJ_DIEN_THE_ITEM_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "ui/CocosGUI.h"
#include "Math_Func.h"
class DienThe_ItemDelegate {
public:
	virtual void onClickedItem(cocos2d::Ref * sender) {};
	virtual void onMoveBeginItem(cocos2d::Ref * sender) {};
	virtual void onMoveEndedItem(cocos2d::Ref * sender,bool is_item_moved) {};
};
enum class DienThe_ItemTouchType {
	NONE,
	CLICK,
	TOUCH
};
class DienThe_Item : public cocos2d::ui::Layout
{
public:
	CREATE_FUNC(DienThe_Item);
	static DienThe_Item * createItem(math::resource::Image* img_item,std::string mask = "");
	void setDelegate(DienThe_ItemDelegate * delegate) { _delegate = delegate; };
	void showItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void hideItem(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void backToStart(float time = 0.25f);
	void setTouchHandleType(DienThe_ItemTouchType type);
	void setTouchEnable(bool enable);

	cocos2d::Vec2 _start_pos;
    
    std::string getImagePath();
protected :
	DienThe_ItemDelegate * _delegate;
private:
	void createTexture();
	void didLoadFromCSB(math::resource::Image* img_item, std::string mask);
	bool m_is_moved;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	math::resource::Image* _img_item;
	cocos2d::ui::Layout * m_layout_touch;
	cocos2d::ui::ImageView * m_img_item;


};

#endif // !MJ_GAME_DIEN_THE_ITEM_H__
