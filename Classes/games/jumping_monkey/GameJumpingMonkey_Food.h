
#ifndef MJGameJumpingMonkey_Food_hpp
#define MJGameJumpingMonkey_Food_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
class MJGameJumpingMonkey_FoodDelegate {
public:
    virtual void onClickFood(cocos2d::Ref *sender, bool is_answer) {};
};
class MJGameJumpingMonkey_Food : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameJumpingMonkey_FoodDelegate {
public:
    CREATE_FUNC(MJGameJumpingMonkey_Food);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
    static MJGameJumpingMonkey_Food * createGame(int type);
    void setDelegate(MJGameJumpingMonkey_FoodDelegate *_delegate);
    void onEnter() override;
    int _type = 0;
    bool isRunAni = false;
private:
    MJGameJumpingMonkey_FoodDelegate *delegate = nullptr;
    void didLoadFromCSB(int type);
    
    void onFood(cocos2d::Ref *sender);
    
};

CREATE_READER(MJGameJumpingMonkey_Food);

#endif
