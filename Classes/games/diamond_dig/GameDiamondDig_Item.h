
#ifndef GAME_DIAMOND_DIG_ITEM_HPP
#define GAME_DIAMOND_DIG_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;

class GameDiamondDig_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameDiamondDig_Item);

    struct ItemInfo {
		bool isTrueAnswer = false;
        std::string text;
        std::string audio_path;
    };
    
    static GameDiamondDig_Item * createItem(ItemInfo item_info);
    ItemInfo _item_info;
	GAFWrapper *stone = nullptr;
	Text* text;
	void setNewData(ItemInfo item_info);

    void onEnter() override;
	void onRead(bool isRead);
	void onDig();

private:
	void didLoadFromCSB(ItemInfo item_info);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameDiamondDig_Item);

#endif
