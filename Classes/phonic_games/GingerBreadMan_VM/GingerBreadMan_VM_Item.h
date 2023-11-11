//
//  MJ_GingerBreadMan_VM_Item.h
//

#ifndef GingerBreadMan_VM_Item_h
#define GingerBreadMan_VM_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
class GingerBreadMan_VM_ItemDelegate {
public:
    virtual void onItemClicked(cocos2d::Ref *sender) {};
};

class GingerBreadMan_VM_Item : public cocos2d::ui::Layout
{
public:
    
    struct GBM_CHARACTER_INFO {
        std::string character;
        std::string audio_path;
		std::vector<int> list_index;
    };
    
    CREATE_FUNC(GingerBreadMan_VM_Item);
    static GingerBreadMan_VM_Item *createItem(GBM_CHARACTER_INFO item_info);
    void setDelegate(GingerBreadMan_VM_ItemDelegate *delegate);
    void setChooseRight();
    void onChooseWrong();
    GBM_CHARACTER_INFO item_info;
    cocos2d::ui::Text *text_lb;
private:
    void didLoadFromCSB(GBM_CHARACTER_INFO item_info);
    void onEnter();
    GingerBreadMan_VM_ItemDelegate *delegate = nullptr;
    void onClicked(cocos2d::Ref *sender);
    cocos2d::ui::ImageView *pie;
};
CREATE_READER(GingerBreadMan_VM_Item);
#endif /* MJ_GingerBreadMan_VM_Item_h */
