//
//  MJ_BreakTheEgg_EggItem.h
//  Created by HoiSung on 02/02/18.
//
//

#ifndef MJ_BreakTheEgg_EggItem_h
#define MJ_BreakTheEgg_EggItem_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#include "StoryConfigManager.h"
class BreakTheEgg_EggItemDelegate {
public:
    virtual void onClickedEggDelegate(cocos2d::Ref *sender) {};
    virtual void onChangeEggStateDelegate(int state) {};
};

class BreakTheEgg_EggItem : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(BreakTheEgg_EggItem);
    static BreakTheEgg_EggItem *createItem(mj::PHONIC_INFO item_info, int type_egg, bool is_auto_play = false);
    void showEgg(std::function<void()> callback = nullptr);
    void hideEgg(std::function<void()> callback = nullptr);
    int state = -1;
    enum STATE_EGG {
        IDLE,
        RUNNING,
        SHOWED,
        HIDDEN,
        ACTION,
        BREAKING,
        BROKEN,
        TEXT_SHOWED,
        FINISH,
    };
    
    void setDelegate(BreakTheEgg_EggItemDelegate *delegate);
    void breakEggAction();
    void setEggFinish();
    mj::PHONIC_INFO _item_info;
    void breakEggActionJustOne();
    int egg_type;
private:
    void didLoadFromCSB(mj::PHONIC_INFO item_info, int type_egg, bool is_auto_play);
    void onEnter();
    
    cocos2d::ui::Layout *egg_layout_root, *egg_layout_choose;
    GAFWrapper *egg_gaf, *egg_star;
    
    void onClickedEgg(cocos2d::Ref *sender);
    cocos2d::ui::Text *text_full, *text_phonic;
    std::string font_name = "fonts/phonicfont.ttf";
    void loadItem();
    cocos2d::Vec2 pos_origin;
    cocos2d::Size screen_size;
    cocos2d::ui::ImageView *gio2;
    BreakTheEgg_EggItemDelegate *_delegate;
    void onChangeEggState(int state);
    bool is_break_start = false;
    
    void showTextFull();
    int break_audio_id = -1, phonic_audio_id = -1;
    int count_break = 1;
    int egg_state;
    
};
CREATE_READER(BreakTheEgg_EggItem);
#endif /* MJ_BreakTheEgg_EggItem_h */
