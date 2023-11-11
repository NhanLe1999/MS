
#ifndef MJGameTyping_ItemAnswer_hpp
#define MJGameTyping_ItemAnswer_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
#include "PhonicConfig.h"
class MJGameTyping_ItemAnswerDelegate {
public:
    virtual void onClickAnswer(cocos2d::Ref *sender) {};
    virtual void onDoneAnswer(cocos2d::Ref *sender) {};
};
class MJGameTyping_ItemAnswer : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameTyping_ItemAnswerDelegate {
public:
    CREATE_FUNC(MJGameTyping_ItemAnswer);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct ItemAnswerInfo {
        std::string text;
        std::string audio_path;
        std::string gaf_path;
        std::string img_path;
    };
    
    struct ItemDistracting {
        std::string text;
        std::string audio_path;
    };    
    
    static MJGameTyping_ItemAnswer * createGame(ItemDistracting item_info);
    void setDelegate(MJGameTyping_ItemAnswerDelegate *_delegate);
    void onEnter() override;
    cocos2d::ui::Text *text;
    void onResultAnswer_True();
    void onResultAnswer_False();
    ItemDistracting getInfo(){
        return _item_info;
    }
private:
    MJGameTyping_ItemAnswerDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemDistracting item_info);
    bool isRunAni = false;
    ItemDistracting _item_info;
    bool is_touch_enable = false;
    void onAnswer(cocos2d::Ref *sender);
};

CREATE_READER(MJGameTyping_ItemAnswer);

#endif 
