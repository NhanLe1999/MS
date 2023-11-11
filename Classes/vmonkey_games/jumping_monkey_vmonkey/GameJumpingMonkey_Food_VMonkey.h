
#ifndef MJGameJumpingMonkeyVMonkey_Food_hpp
#define MJGameJumpingMonkeyVMonkey_Food_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameJumpingMonkeyVMonkey_FoodDelegate {
public:
    virtual void onClickFood(cocos2d::Ref *sender, bool is_answer) {};
};
class MJGameJumpingMonkeyVMonkey_Food : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameJumpingMonkeyVMonkey_FoodDelegate {
public:
    CREATE_FUNC(MJGameJumpingMonkeyVMonkey_Food);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    static MJGameJumpingMonkeyVMonkey_Food * createGame(int type);
    void setDelegate(MJGameJumpingMonkeyVMonkey_FoodDelegate *_delegate);
    void onEnter() override;
    int _type = 0;
    bool isRunAni = false;
private:
    MJGameJumpingMonkeyVMonkey_FoodDelegate *delegate = nullptr;
    void didLoadFromCSB(int type);
    
    void onFood(cocos2d::Ref *sender);
    
};

CREATE_READER(MJGameJumpingMonkeyVMonkey_Food);

#endif
