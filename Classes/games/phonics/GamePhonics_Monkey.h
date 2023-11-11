
#ifndef MJGamePhonics_Monkey_hpp
#define MJGamePhonics_Monkey_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGamePhonics_MonkeyDelegate {
public:
    virtual void onFinishMonkey(int finish_type) {};
};
class MJGamePhonics_Monkey : public cocos2d::ui::Layout, public MJGamePhonics_MonkeyDelegate {
public:
    CREATE_FUNC(MJGamePhonics_Monkey);
    static MJGamePhonics_Monkey * createGame();
    enum MONKEY_FINISH_TYPE {
        JUMP = 1,
        IDLE = 2,
        POINT_UP = 3,
        POINT_DOWN = 4,
        HAND = 5,
        LISTEN = 6,
        LISTEN_HAND = 7,
        READ = 8,
        MIC_SHOW = 9,
        MIC_HIDE = 10,
        TALK = 11,
    };
    
    void setDelegate(MJGamePhonics_MonkeyDelegate *_delegate);
    GAFWrapper *gaf_monkey;
    void playMonkeyWithAction(int monkey_action_type, bool isloop = false);
    int _monkey_finish_type = 0;
    
private:
    MJGamePhonics_MonkeyDelegate *delegate = nullptr;
    void didLoadFromCSB();    
    bool isRunAni = false;
    std::string getGafSequenceNameByType(int type);
};

CREATE_READER(MJGamePhonics_Monkey);

#endif
