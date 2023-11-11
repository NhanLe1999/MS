
#ifndef WaterBalloon_Item_HPP
#define WaterBalloon_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;
class WaterBalloon_ItemDelegate {
public:
	virtual void onClickedDelegate(cocos2d::Ref *sender, int id) {};
};

class WaterBalloon_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
	CREATE_FUNC(WaterBalloon_Item);
	struct Word {
		std::string text;
		std::string audio_1;
		std::string audio_2;
		bool is_right;
	};
	static WaterBalloon_Item * createItem(WaterBalloon_Item::Word word, int id);
	void playEffect(bool correct);
	void setDelegate(WaterBalloon_ItemDelegate* delegate);
	

	void setAudio(std::string audio);
	std::string getAudio();

	bool isTrue() {
		return is_true;
	}
	int id;
	void lockBalloon();
	void unLockBalloon();
	Text* getText() {
		return text;
	}
    void setFontSize(int size);
    
private:
	void onEnter() override;
	void didLoadFromCSB(WaterBalloon_Item::Word word, int id);
	void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 60;
	//    bool _water_splash = false;
	bool _hint;
	std::string _audio;
	WaterBalloon_Item::Word word;
	bool is_true;
	WaterBalloon_ItemDelegate* _delegate;
	Layout* item_layout;
	Text* text;
};

CREATE_READER(WaterBalloon_Item);

#endif
