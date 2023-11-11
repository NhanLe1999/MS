//
//  MJ_BuggiesV2_Item.h
//

#ifndef BuggiesV2_Item_h
#define BuggiesV2_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
class BuggiesV2_ItemDelegate {
public:
    virtual void onItemClicked(cocos2d::Ref *sender) {};
};

class BuggiesV2_Item : public cocos2d::ui::Layout
{
public:
    
    struct CHARACTER_INFO {
        std::string character;
        std::string audio_path;
    };
    
    CREATE_FUNC(BuggiesV2_Item);
    static BuggiesV2_Item *createItem(CHARACTER_INFO item_info);
    void setDelegate(BuggiesV2_ItemDelegate *delegate);
    CHARACTER_INFO item_info;
    void runText();
    cocos2d::ui::Text *text;
    void onMergeCharacter();
    
private:
    void didLoadFromCSB(CHARACTER_INFO item_info);
    void onEnter();
    BuggiesV2_ItemDelegate *delegate = nullptr;
    std::vector<cocos2d::Vec2> list_pos;
    void addStar();
    
};
CREATE_READER(BuggiesV2_Item);
#endif /* MJ_BuggiesV2_Item_h */
