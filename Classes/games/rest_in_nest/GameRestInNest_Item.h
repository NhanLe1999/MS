
#ifndef GAME_REST_IN_NEST_ITEM_HPP
#define GAME_REST_IN_NEST_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
USING_NS_CC;
using namespace ui;

#define REST_IN_NEST_ITEM_SIZE Vec2(Director::getInstance()->getVisibleSize().width,330)

class GameRestInNest_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameRestInNest_Item);

    struct ItemInfo {
        std::string main_text;
		std::string audio_path;
        std::vector<std::string> texts;
    };
    
    static GameRestInNest_Item * createItem();
    ItemInfo _item_info;
	GAFWrapper *stone = nullptr;
	Text* text = nullptr;
	Vector<ImageView*> imgs;
	std::vector<int> numImg;
    float text_sz = 80;
	void reloadInfo(ItemInfo item_info);
	bool onClick(int index);
    bool isRight(int index);
    ImageView* getRightImage(int index);
	void setForFinish();
private:
	void didLoadFromCSB();
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameRestInNest_Item);

#endif
