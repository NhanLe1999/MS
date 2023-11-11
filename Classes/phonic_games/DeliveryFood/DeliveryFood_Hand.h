#ifndef MJ_GAME_DELIVERY_FOOD_HAND_H
#define MJ_GAME_DELIVERY_FOOD_HAND_H

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "MJMainGBase.h"
class DeliveryFood_HandDelegate {
public:
	virtual void onClickedHand(cocos2d::Ref * sender) {};
	virtual void onMoveBeginHand(cocos2d::Ref * sender) {};
	virtual void onMoveEndedHand(cocos2d::Ref * sender) {};
};
enum class DeliveryFoodHandTouchType {
	NONE,
	CLICK,
	TOUCH
};
class DeliveryFood_Hand : public cocos2d::ui::Layout
{
public:
	static DeliveryFood_Hand * createHand(mj::PHONIC_INFO info, int hand_id = 0);
	mj::PHONIC_INFO getInfo() const;
	void setInfo(const mj::PHONIC_INFO info);
	void idle();
	void setDelegate(DeliveryFood_HandDelegate * delegate) { _delegate = delegate; };
	void showHand(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void hideHand(float dt = 0.25f,std::function<void()> call_back = nullptr);
	void servedFood(cocos2d::Vec2 pos);
	void backToStart();
	int getHandId();
	void setTouchHandleType(DeliveryFoodHandTouchType type);
	void setTouchEnable(bool enable);
	// for auto play
	void setVisibleLayout(bool layout_hand, bool layout_food);
	cocos2d::Vec2 _start_pos;
protected :
	DeliveryFood_HandDelegate * _delegate;
private:
	void didLoadFromCSB(mj::PHONIC_INFO info, int hand_id);
	bool m_is_moved;
	int m_hand_id;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	mj::PHONIC_INFO m_hand_info;	
	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_food;
	cocos2d::ui::Layout * m_layout_touch;
	cocos2d::ui::ImageView * m_img_hand;
	cocos2d::ui::ImageView * m_img_hand_put;
	cocos2d::ui::ImageView * m_img_food;
	cocos2d::ui::ImageView * m_img_food_served;
	cocos2d::ui::Text * m_lbl_info;
	cocos2d::ui::Text * m_lbl_food;

};

#endif // !MJ_GAME_DELIVERY_FOOD_HAND_H
