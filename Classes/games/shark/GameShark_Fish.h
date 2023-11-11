
#ifndef MJGameShark_Fish_hpp
#define MJGameShark_Fish_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameShark_FishDelegate {
public:
    virtual void onFish(cocos2d::Ref *sender) {};
    virtual void onSharkRun(cocos2d::Ref *sender) {};
};
class MJGameShark_Fish : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameShark_FishDelegate {
public:
    CREATE_FUNC(MJGameShark_Fish);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct FishInfo {
        std::string text;
        std::string audio_path;
        std::string img_path;
    };
    
    static MJGameShark_Fish * createFish(FishInfo item_info);
    static MJGameShark_Fish * createShark();
    FishInfo _item_info;
    void setDelegate(MJGameShark_FishDelegate *_delegate);
    GAFWrapper *gaf_fish = nullptr;
    void run();
    void onEnter() override;
    void sharkRunToGoal(Vec2 pos_goal, bool fish_flip);
    void fishRunWhenTap(Vec2 fish_tap_to, float duration = 0.f);
    bool flipx = false;
    float time_shark = 0;
    Vec2 pos_tap_to;
private:
    MJGameShark_FishDelegate *delegate = nullptr;
    void didLoadFromCSB(FishInfo item_info);
    void didLoadFromCSB(bool is_shark = true);
    bool isRunAni = false;
    bool isShark = false;
    void onTapFish(cocos2d::Ref *sender);
    cocos2d::ui::Text *text = nullptr;
    int fish_id = 0;
    
};

CREATE_READER(MJGameShark_Fish);

#endif
