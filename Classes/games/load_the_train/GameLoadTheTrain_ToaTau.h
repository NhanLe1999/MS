
#ifndef GAME_LOAD_THE_TRAIN_TOA_TAU_HPP
#define GAME_LOAD_THE_TRAIN_TOA_TAU_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;

#define TOA_TAU_CONTENSIZE cocos2d::Size(202,130)

class GameLoadTheTrain_ToaTau : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameLoadTheTrain_ToaTau);

    struct ItemInfo {
		std::string text;
    };
    
    static GameLoadTheTrain_ToaTau * createItem(ItemInfo item_info);
    ItemInfo _item_info;
	GAFWrapper *toa = nullptr;

	void loadString(std::string s);
	void loadColor(int color);
	void showToa(bool isShow);
	Vec2 getToaPos();

    void onEnter() override;
private:
	void didLoadFromCSB(ItemInfo item_info);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameLoadTheTrain_ToaTau);

#endif
