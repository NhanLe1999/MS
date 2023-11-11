
#ifndef MJGameTyping_ItemTap_hpp
#define MJGameTyping_ItemTap_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameTyping_ItemTapDelegate {
public:
    virtual void onFinishMonkey(int finish_type) {};
};
class MJGameTyping_ItemTap : public cocos2d::ui::Layout, public MJGameTyping_ItemTapDelegate {
public:
    CREATE_FUNC(MJGameTyping_ItemTap);
    
    struct ItemTapInfo {
        std::string text;
        std::string audio_path;
        std::string gaf_path;
        std::string img_path;
    };
    
    static MJGameTyping_ItemTap * createGame(ItemTapInfo item_info);
    ItemTapInfo _item_info;
    void setDelegate(MJGameTyping_ItemTapDelegate *_delegate);
    GAFWrapper *gaf_tap;
    
private:
    MJGameTyping_ItemTapDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemTapInfo item_info);
    bool isRunAni = false;
};

CREATE_READER(MJGameTyping_ItemTap);

#endif /* GameLandThePlane_hpp */
