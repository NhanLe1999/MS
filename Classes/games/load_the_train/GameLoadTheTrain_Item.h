
#ifndef GAME_LOAD_THE_TRAIN_ITEM_HPP
#define GAME_LOAD_THE_TRAIN_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;


class GameLoadTheTrain_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameLoadTheTrain_Item);

    struct ItemInfo {
		std::vector<std::string> texts;
    };
    
    static GameLoadTheTrain_Item * createItem(ItemInfo item_info);
    ItemInfo _item_info;
	std::vector<ImageView*> imgs;
	std::vector<Vec2> poss;
	std::vector<int> colors;
	void loadData(std::vector<std::string> data_);

	void setTop(ImageView* img, int z);
	void highLightItem(std::string trueText);
	void setDone(ImageView* img);
	Vec2 getPosItem(ImageView* img);
	int getColor(ImageView* img);
    void onEnter() override;
	cocos2d::GLProgram* aGL = nullptr;
private:
	void didLoadFromCSB(ItemInfo item_info);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameLoadTheTrain_Item);

#endif
