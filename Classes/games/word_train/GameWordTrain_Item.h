
#ifndef MJGameWordTrain_Item_hpp
#define MJGameWordTrain_Item_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "PhonicConfig.h"
class MJGameWordTrain_ItemDelegate {
public:
//    virtual void onAnswerCorrect(cocos2d::Ref *sender) {};
    virtual void onFinishTurn() {};
    virtual void onAnswerTrue() {};
    virtual void onTouchItemBegan() {};
    virtual void onTouchItemEnded() {};
};
class MJGameWordTrain_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameWordTrain_ItemDelegate {
public:
    CREATE_FUNC(MJGameWordTrain_Item);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct ItemInfo {
        std::string text;
        std::string audio_path;
        std::string img_path;
        int start;
        int end;
        std::string character;
        std::string character_audio_path;
    };
    
    struct ItemDistracting {
        std::string text;
        std::string audio_path;
    };
    
    static MJGameWordTrain_Item * createGame(ItemInfo item_info, ItemDistracting item_distracting);
    ItemInfo _item_info;
    ItemDistracting _item_distracting;
    void setDelegate(MJGameWordTrain_ItemDelegate *_delegate);
    void onEnter() override;
    void onShow(std::function<void()> finishCallback = nullptr);

    cocos2d::Point getGuideStartingPoint();
    cocos2d::Point getGuideStoppingPoint();
    
private:
    MJGameWordTrain_ItemDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemInfo item_info, ItemDistracting item_distracting);
    void onChoose(cocos2d::Ref *sender);
    cocos2d::Rect getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find);
    cocos2d::ui::Text *text_full = nullptr;
    std::vector<cocos2d::ui::Layout *> list_layout_charaters;
    std::vector<std::string> list_audio_path;
    int index_charater_correct = 0, index_choose = 0;
    
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    
    cocos2d::ui::Layout *layout_choose = nullptr, *layout_text = nullptr;
    bool is_choose = false, is_answer_done = true;
    cocos2d::Rect rect;
    Vec2 old_pos_layout;
};

CREATE_READER(MJGameWordTrain_Item);

#endif
