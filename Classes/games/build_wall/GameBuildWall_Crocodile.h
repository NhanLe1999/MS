
#ifndef MJGameBuildWall_Crocodile_hpp
#define MJGameBuildWall_Crocodile_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameBuildWall_CrocodileDelegate {
public:
    virtual void onCrocodileActionFinish(int action_type) {};
};
class MJGameBuildWall_Crocodile : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameBuildWall_CrocodileDelegate {
public:
    CREATE_FUNC(MJGameBuildWall_Crocodile);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    enum CROCODILE_ACTION {
        IDLE = 1,
        WRONG = 2,
        JUMP = 3,
        RUN = 4,
        STOP = 5,
        SMASH = 6,
        VIC = 7,
    };
    
    static MJGameBuildWall_Crocodile * createGame();
    void setDelegate(MJGameBuildWall_CrocodileDelegate *_delegate);
    void onEnter() override;
    GAFWrapper *crocodile = nullptr;
    void playMonkeyWithAction(int monkey_action_type, bool isloop);
    Vec2 pos_1, pos_2;
    void setFlipCrocodile(bool is_flipX);
    bool is_flip = false;
private:
    MJGameBuildWall_CrocodileDelegate *delegate = nullptr;
    void didLoadFromCSB();
    bool isRunAni = false;
    
    int _monkey_finish_type = 0;
    std::string getGafSequenceNameByType(int type);
};

CREATE_READER(MJGameBuildWall_Crocodile);

#endif
