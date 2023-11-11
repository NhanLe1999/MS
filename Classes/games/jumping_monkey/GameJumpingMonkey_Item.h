
#ifndef MJGameJumpingMonkey_Item_hpp
#define MJGameJumpingMonkey_Item_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameJumpingMonkey_ItemDelegate {
public:
    virtual void onClickItem(cocos2d::Ref *sender, int type) {};
};
class MJGameJumpingMonkey_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameJumpingMonkey_ItemDelegate {
public:
    CREATE_FUNC(MJGameJumpingMonkey_Item);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct ItemInfo {
        std::string text;
        std::string audio_path;
        std::string img_path;
    };
    
    enum Item_Type {
        TEXT_TYPE = 1,
        IMG_TYPE = 2
    };
    
    static MJGameJumpingMonkey_Item * createGame(ItemInfo item_info, int type);
    ItemInfo _item_info;
    void setDelegate(MJGameJumpingMonkey_ItemDelegate *_delegate);
    void onEnter() override;
    void onRightAnswer();
    void onWrongAnswer();
    int type_item = 1;
    bool is_click_enable = false;
    
private:
    MJGameJumpingMonkey_ItemDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemInfo item_info, int type);
    bool isRunAni = false;
    void onChoose(cocos2d::Ref *sender);
};

CREATE_READER(MJGameJumpingMonkey_Item);

#endif
