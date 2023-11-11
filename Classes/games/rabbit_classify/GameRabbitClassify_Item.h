
#ifndef MJGameSample_Item_hpp
#define MJGameSample_Item_hpp

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

enum ITEM_STATE
{
	IS_NONE = -1,
	IS_RUNNING,
	IS_DRAGING,
	IS_COMMINGBACK,
	IS_REMOVING
};
class MJGameSample_ItemDelegate {
public:
    virtual void onClickItem(cocos2d::Ref *sender) {};
};
class GameRabbitClassify_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameSample_ItemDelegate {
public:
    CREATE_FUNC(GameRabbitClassify_Item);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct ItemInfo {
		std::vector<int> iType;
        std::string text;
        std::string audio_path;
        std::string img_path;
    };
    
    static GameRabbitClassify_Item * createGame(ItemInfo item_info);
    ItemInfo _item_info;
	ITEM_STATE itemState = ITEM_STATE::IS_NONE;
	cocos2d::Vec2 curPos;

    void setDelegate(MJGameSample_ItemDelegate *_delegate);
    void onEnter() override;
private:
    MJGameSample_ItemDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemInfo item_info);
    void onChoose(cocos2d::Ref *sender);
};

CREATE_READER(GameRabbitClassify_Item);

#endif
