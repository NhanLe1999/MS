
#ifndef DefeatTheDragon_Item_HPP
#define DefeatTheDragon_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class DefeatTheDragon_ItemDelegate {
public:
	virtual void onClickedDelegate(cocos2d::Ref *sender, ui::Text* text, int id) {};
};

class DefeatTheDragon_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(DefeatTheDragon_Item);

	static DefeatTheDragon_Item * createItem(Text* text, bool is_true, int id);
	void playEffect(bool correct);
	void setDelegate(DefeatTheDragon_ItemDelegate* delegate);

	void lockTouch();
	void unLockTouch();

	void setAudio(std::string audio);
	std::string getAudio();

	Text* getText();

	void showFire();
	void breakFire();
	void showStar();
	GAFWrapper* getFire();

	bool isTrue() {
		return is_true;
	}
	int id;

private:
	void onEnter() override;
	void didLoadFromCSB(Text* text, bool is_true, int id);

	void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	//    bool _water_splash = false;
	bool _hint;
	std::string _audio;
	Text* text;
	bool is_true;
	DefeatTheDragon_ItemDelegate* _delegate;
	GAFWrapper* fire_small;
	GAFWrapper* fire_break;
	GAFWrapper* fire_star;
	Layout* item_layout;
	
};

CREATE_READER(DefeatTheDragon_Item);

#endif
