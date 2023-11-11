#ifndef ChoBoPhanTimTongThe_Item_HPP
#define ChoBoPhanTimTongThe_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class ChoBoPhanTimTongThe_ItemDelegate {
public:
	virtual void onClickedBox(cocos2d::Ref * sender) {};
};



class ChoBoPhanTimTongThe_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(ChoBoPhanTimTongThe_Item);

	static ChoBoPhanTimTongThe_Item * createItem(std::string path,std::string audio, bool is_true);
	void setDelegate(ChoBoPhanTimTongThe_ItemDelegate* delegate);
	void showItem();

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
	void didLoadFromCSB(std::string path, std::string audio, bool is_true);
	//void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	//    bool _water_splash = false;
	bool _hint;
	std::string _audio;
	std::string _text;
	bool is_true;
	ChoBoPhanTimTongThe_ItemDelegate* _delegate;
	Layout* item_layout;

};

CREATE_READER(ChoBoPhanTimTongThe_Item);

#endif
