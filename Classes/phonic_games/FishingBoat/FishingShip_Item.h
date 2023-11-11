
#ifndef FishingShip_Item_HPP
#define FishingShip_Item_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class FishingShip_ItemDelegate {
public:
    virtual void onClickedDelegate(cocos2d::Ref *sender, ui::Widget::TouchEventType type) {};
};
class FishingShip_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(FishingShip_Item);
    static FishingShip_Item * createItem(mj::PHONIC_INFO item_info);
    void playEffect(bool correct);
    void setDelegate(FishingShip_ItemDelegate* delegate);
    void setSplash(bool is_set);
    bool getSplash();
    
    void setHint(bool is_hint);
    bool getHint();
	mj::PHONIC_INFO getInfo();
private:
    void onEnter() override;
	void didLoadFromCSB(mj::PHONIC_INFO item_info);
    
    void onClickItem(Ref* sender, ui::Widget::TouchEventType type);
    std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
    int font_sz = 80;
    bool _water_splash = false;
    bool _hint = false;
    FishingShip_ItemDelegate* _delegate;

	mj::PHONIC_INFO item_info;

};

CREATE_READER(FishingShip_Item);

#endif
