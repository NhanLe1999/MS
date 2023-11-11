
#ifndef GameQA_Item_hpp
#define GameQA_Item_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
class MJGameQA_ItemDelegate {
public:
    virtual void onChooseQuesItem(Ref* sender) {};
};


class MJGameQA_Item : public cocos2d::ui::Layout,
    public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameQA_Item);
    static MJGameQA_Item* createFrame(int item_id);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;

    std::string getObjectName();

    void setDelegate(MJGameQA_ItemDelegate* delegate);
    mj::GameObject _data;

    void setStateItem();
    void setCurrentItem();
    void setChooseItem();
    void setCheckTrueFalseItem(bool isTrue);
    void setNormalItem();
    void setWasChooseQes();
    bool getWasChooseQes();
    void setWasChooseAnswer();
    bool getWasChooseAnswer();
    void setWasChooseCorrectAnswer(bool answerStatus);
    bool getWasChooseCorrectAnswer();
    void setVisibleItem_QARed(string color);

    enum QAITEM_STATE {
        NORMAL_STATE = 1,
        CHOOSE_STATE = 2,
        CHECK_STATE = 3,
    };
    int _state = 1;
    bool _isCheckAnswer = false;
    bool _wasChooseAnswer = false;                      // when choose answer of QA then set true without true or false Answer
    bool _wasChooseCorrectAnswer = false;               
    bool _wasChooseQues = false;
    void showArrowGuide(bool is_show_when_wrong = false, bool active = false);
    void hideArrowGuide();
    bool is_show_arrowguide = false;
    bool is_show_guide_wrong = false;
private:
    void didLoadFromCSB(int item_id);
    MJGameQA_ItemDelegate* delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    int _itemid = 0;
    ImageView* item_bg1 = nullptr, * item_bg2 = nullptr, * _item_bg2_red = nullptr, *_item_bg2_green = nullptr;
    Text* lbnum1 = nullptr, * lbnum2 = nullptr;
    void onChoose(Ref* sender);
    bool is_set_choose = false;

};

CREATE_READER(MJGameQA_Item);

#endif /* GameQA_Item_hpp */
