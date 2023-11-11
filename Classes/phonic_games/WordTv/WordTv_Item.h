//
//  MJ_WordTv_Item.h
//

#ifndef WordTv_Item_h
#define WordTv_Item_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"

class WordTv_Item : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(WordTv_Item);
    static WordTv_Item *createItem(mj::GEAR_ITEM item_info);
    
    mj::GEAR_ITEM _item_info;
    void onClicked();
    void setCallbackW(const std::function<void()> afunc);
    void setCallbackC(const std::function<void()> afunc);
    
    void setWrong();
    void setCorrect();
private:
    void didLoadFromCSB(mj::GEAR_ITEM item_info);
    void onEnter();
    
    GAFWrapper *filter;
    cocos2d::ui::ImageView *bgGear;
    cocos2d::ui::ImageView *item;
    std::function<void()> clickedFinishW =nullptr;
    std::function<void()> clickedFinishC =nullptr;
};
#endif /* MJ_WordTv_Item_h */
