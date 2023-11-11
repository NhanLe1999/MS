
#ifndef MJGameBuildWall_Item_hpp
#define MJGameBuildWall_Item_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
class MJGameBuildWall_ItemDelegate {
public:
    virtual void onChooseAnswerFinish() {};
    virtual void onChooseWrongAnswer() {};
};
class MJGameBuildWall_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameBuildWall_ItemDelegate {
public:
    CREATE_FUNC(MJGameBuildWall_Item);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    struct ItemInfo {
        std::string text_full;
        std::string text_true;
        std::string text_false;
        std::string audio_path_full;
        std::string audio_path_quest;
        std::string audio_path_true;
        std::string audio_path_false;
        std::string image_path;//kien
        std::vector<math::resource::Sync_Text> sync_text;
    };
    
    static MJGameBuildWall_Item * createGame(ItemInfo item_info, bool is_crocodile_flip);
    ItemInfo _item_info;
    void setDelegate(MJGameBuildWall_ItemDelegate *_delegate);
    void onEnter() override;
    Vec2 pos_goal;
    void setDoneAnswer();
    void onShow(bool isFlip);
    Rect rect_convert, rect;
    cocos2d::ui::Text *text_full;
    bool _isPlayAudio = true;
    bool _isPlayReadText = true;
    
    cocos2d::Vec2 getGuideStartPosition();
    cocos2d::Vec2 getGuideStopPosition();
    void runActionHand(float timeDelay);
    bool checkIsPlayAudio();
    void setCheckIsPlayAudio(bool isPlayAudio);
    bool getTouchableStatus() {
        return _isTouchable;
    }
    void setTouchableStatus(bool touchable) {
        _isTouchable = touchable;
    }
private:
    MJGameBuildWall_ItemDelegate *delegate = nullptr;
    void didLoadFromCSB(ItemInfo item_info, bool is_crocodile_flip);
    bool isRunAni = false;
    cocos2d::Rect getRectOfLetters(std::vector<Sprite *> _letter_arr, std::string str_find);
    std::vector<cocos2d::ui::Scale9Sprite *> list_answer;
    
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    cocos2d::ui::Scale9Sprite *img_choose=nullptr;
    bool is_choose = false, is_answer_done = true;
    Vec2 answer_old_pos;
    cocos2d::ui::Layout *_layout_compare = nullptr;
    int index_answer = -1, audio_index = 0;
    Vec2 pos_layout;
    
    bool crocodile_flip = false;
    void readText(float delayTime);
    int size_text = 45;
    bool play_image;
    ui::Text* _textTrue = nullptr;
    ui::Text* _textFalse = nullptr;
    bool _isTouchable = false;
    
};

CREATE_READER(MJGameBuildWall_Item);

#endif
