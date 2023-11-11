
#ifndef SweetLand_Item_HPP
#define SweetLand_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class SweetLand_ItemDelegate {
public:
	virtual void onClickedDelegate(cocos2d::Ref *sender) {};
};

class SweetLand_Item : public cocos2d::ui::Layout {
public:
	CREATE_FUNC(SweetLand_Item);

	static SweetLand_Item * createItem(int number, int id);
	void playEffect(bool correct);
	void setDelegate(SweetLand_ItemDelegate* delegate);
	
	void setAudio(std::string audio);
	std::string getAudio();
	int id;

	void setTouchOnOff(bool isTrue);
	int getNumber() {
		return _number;
	}

	void onShow(float dt = 0.5f);
	void onHide(float dt = 0.5f);
private:

	bool is_pressed;
	void onEnter() override;
	void didLoadFromCSB(int number, int id);
	void onClickItem(Ref* sender);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 150;
	//    bool _water_splash = false;
	bool _hint;
	std::string _audio;
	int _number;
	
	GAFWrapper * gaf_button;
	SweetLand_ItemDelegate* _delegate;
	Layout* item_layout;
	Text* text;

};


#endif
