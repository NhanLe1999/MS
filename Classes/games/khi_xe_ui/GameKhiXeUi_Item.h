
#ifndef GAME_KHI_XE_UI_ITEM_HPP
#define GAME_KHI_XE_UI_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
enum HOUSE_TYPE
{
	HT_NONE = -1,
	HT_SMALL,
	HT_MEDIUM,
	HT_BIG
};

class GameKhiXeUi_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameKhiXeUi_Item);

    struct ItemInfo {
		bool isCrash = false;
        std::string text;
        std::string audio_path;
		int pair;
		cocos2d::ui::Layout* aLayout = nullptr;
		cocos2d::ui::Text* textItem = nullptr;
		cocos2d::ui::ImageView* imgCrash = nullptr;
    };
    
    static GameKhiXeUi_Item * createGame(std::vector<ItemInfo> item_infos);
    std::vector<ItemInfo> _item_infos;
	HOUSE_TYPE house_type;
	std::string img_crash_path;
	GAFWrapper *boom;
	Layout* boomPanel = nullptr;
	Layout* mainLayout = nullptr;

    void onEnter() override;
	void playBoom(int posBoom);
private:
	void didLoadFromCSB(std::vector<ItemInfo> item_infos);
};

CREATE_READER(GameKhiXeUi_Item);

#endif
