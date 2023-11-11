
#ifndef MJGameWaterSpray_Item_hpp
#define MJGameWaterSpray_Item_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameWaterSpray_ItemDelegate {
public:
    virtual void onClickItem(cocos2d::Ref *sender, int type) {};
};
class MJGameWaterSpray_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameWaterSpray_ItemDelegate {
public:
    CREATE_FUNC(MJGameWaterSpray_Item);
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
    
    static MJGameWaterSpray_Item * createGame(ItemInfo item_info, int type);
    ItemInfo _item_info;
    void setDelegate(MJGameWaterSpray_ItemDelegate *_delegate);
    void onEnter() override;
    void onExit() override;
    void onRightAnswer(string rightAnswer);
    void onWrongAnswer(string rightAnswer);
    int type_item = 1;
    int getClickEnable();
    void setClickEnable(bool click_enable);
private:
    bool is_click_enable = false;
    MJGameWaterSpray_ItemDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemInfo item_info, int type);
    
    void onChoose(cocos2d::Ref *sender);
    
};

CREATE_READER(MJGameWaterSpray_Item);

#endif
