#ifndef CompareAndContrast_Item_HPP
#define CompareAndContrast_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class CompareAndContrast_ItemDelegate {
public:
	virtual void onClickedBox(cocos2d::Ref * sender) {};
	virtual void onMoveBeginBox(cocos2d::Ref * sender) {};
	virtual void onMovingBox(cocos2d::Ref * sender) {};
	virtual void onMoveEndedBox(cocos2d::Ref * sender) {};
};

enum class CompareAndContrastItemTouchType {
	NONE,
	CLICK,
	TOUCH
};

class CompareAndContrast_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(CompareAndContrast_Item);

	static CompareAndContrast_Item * createItem(std::string text, int id,std::string audio);
	void playEffect(bool correct);
	void setDelegate(CompareAndContrast_ItemDelegate* delegate);
	void showItem();

	void backToStart();
	void setTouchHandleType(CompareAndContrastItemTouchType type);
	void setTouchEnable(bool enable);

	void setAudio(std::string audio);
	void playAudio();
	std::string getAudio();

	bool isTrue() {
		return is_true;
	}

	
	int id;
private:
	void onEnter() override;
	void didLoadFromCSB(std::string text, int id, std::string audio);
	//void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	//    bool _water_splash = false;
	bool _hint;
	std::string _audio;
	std::string _text;
	bool is_true;
	CompareAndContrast_ItemDelegate* _delegate;
	Layout* item_layout;
	Text* text;
	bool m_is_moved;
	Vec2 _start_pos;
};

CREATE_READER(CompareAndContrast_Item);

#endif
