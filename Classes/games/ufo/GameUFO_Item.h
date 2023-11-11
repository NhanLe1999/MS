
#ifndef GAME_UFO_ITEM_HPP
#define GAME_UFO_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;

class GameUFO_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameUFO_Item);

    struct ItemInfo {
		bool isHasImage = false;
		std::string img_path;
        std::string text;
        std::string audio_path;
		std::string description_path;
    };
	static GameUFO_Item * createItem(std::string str);
    
	std::string text;
	int getFontSize();
	void setFontSize(int sz);
private:
	void didLoadFromCSB(std::string str);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameUFO_Item);

#endif
