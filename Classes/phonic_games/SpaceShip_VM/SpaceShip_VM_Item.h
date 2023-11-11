//
//  MJ_SpaceShip_VM_Item.h
//

#ifndef SpaceShip_VM_Item_h
#define SpaceShip_VM_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
class SpaceShip_VM_ItemDelegate {
public:
    virtual void onItemClicked(cocos2d::Ref *sender) {};
};

class SpaceShip_VM_Item : public cocos2d::ui::Layout
{
public:
    struct METEORITE_INFO {
        std::string character;
        std::string audio_path;
    };
    
    CREATE_FUNC(SpaceShip_VM_Item);
    static SpaceShip_VM_Item *createMeteorite(METEORITE_INFO item_info);
    static SpaceShip_VM_Item *createShip();
    void setDelegate(SpaceShip_VM_ItemDelegate *delegate);
    METEORITE_INFO item_info;
    void playSequence(std::string sequence, bool looped = false, std::function<void()> callback = nullptr);
    void shoot(cocos2d::Vec2 pos_shoot, std::function<void()> callback = nullptr);
    void explosive(std::function<void()> callback = nullptr);
    void onChooseWrong();
    cocos2d::ui::Layout *click_layout;
    cocos2d::ui::Text *text;
private:
    void didLoad_Meteorite(METEORITE_INFO item_info);
    void didLoad_Ship();
    void onEnter();
    SpaceShip_VM_ItemDelegate *delegate = nullptr;
    GAFWrapper *item_gaf;
    void onClickItem(cocos2d::Ref *sender);
    
};
CREATE_READER(SpaceShip_VM_Item);
#endif /* MJ_SpaceShip_VM_Item_h */
