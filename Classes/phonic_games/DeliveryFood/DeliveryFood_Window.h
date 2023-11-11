#ifndef MJ_GAME_DELIVERY_FOOD_WINDOW_H
#define MJ_GAME_DELIVERY_FOOD_WINDOW_H

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "PhonicInfoModel.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
#include "PhonicConfig.h"
#include "StoryConfigManager.h"
class DeliveryFood_WindowDelegate {
public:
	virtual void onClickedWindowDelegate(cocos2d::Ref * sender) {};
};

enum class DeliveryFood_WindowState {
	NONE,
	SHOWING,
	OPEN,
	CLOSE,
	IDLE,
	WIN,
	LOSE,
	OFFER
};

class DeliveryFood_Window : public cocos2d::ui::Layout {
public:
	static DeliveryFood_Window * createWindow(mj::PHONIC_INFO info, int character_id = 0);
	cocos2d::ui::Text * getLableText();
	void setDelegate(DeliveryFood_WindowDelegate * delegate);
	void idleWindow(std::function<void()> call_back = nullptr);
	void showWindow(float dt = 0.0f,std::function<void()> call_back = nullptr);
	void openWindow(std::function<void()> call_back = nullptr);
	void closeWindow(std::function<void()> call_back = nullptr);
	void offerFood();
	void playWin(int food_id, std::function<void()> call_back = nullptr);
	void playLose(std::function<void()> call_back = nullptr);
	mj::PHONIC_INFO getInfo() const;
	void setInfo(mj::PHONIC_INFO info);
	DeliveryFood_WindowState getWindowState() const;
protected:
	DeliveryFood_WindowDelegate * _delegate;
private:
	void onClickedWindow(cocos2d::Ref * sender);
	void didLoadFromCSB(mj::PHONIC_INFO info, int character_id);

	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	mj::PHONIC_INFO m_window_info;
	DeliveryFood_WindowState m_window_state;

	cocos2d::ui::Layout * m_layout_root;
	cocos2d::ui::Layout * m_layout_touch;
	cocos2d::ui::Layout * m_layout_offer;

	GAFWrapper * m_gaf_window;
	GAFWrapper * m_gaf_character;
	GAFWrapper * m_gaf_angry_fx;
	
	cocos2d::ui::ImageView * m_img_offer;
	
	cocos2d::ui::Text * m_lbl_offer;
	cocos2d::ui::Text * m_lbl_info;
};

#endif // !MJ_GAME_DELIVERY_FOOD_WINDOW_H
