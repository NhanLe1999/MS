
#ifndef GameQA_AnswerItem_hpp
#define GameQA_AnswerItem_hpp

#include "cocostudiomacro2.h"
#include "GameData.h"
using namespace cocos2d;
using namespace ui;
using namespace std;

class MJGameQA_AnswerItemDelegate {
public:
    virtual void onChooseItem(Ref* sender) {};
    virtual void onItemClickSpeaker(bool is_reading) {};
};


class MJGameQA_AnswerItem : public cocos2d::ui::Layout,
    public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJGameQA_AnswerItem);
    static MJGameQA_AnswerItem* createFrame(mj::GameObject object, int type);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;

    std::string getObjectName();

    void setDelegate(MJGameQA_AnswerItemDelegate* delegate);
    mj::GameObject _data;

    void setChooseAnswer();
    void setUnChooseAnswer();
    void setRedColorAnswer(bool enable);
    void setBlueColorAnswer();
    void setHighlight();
    void onShow();
    void disableTouch();

private:
    void didLoadFromCSB(mj::GameObject object, int type);
    MJGameQA_AnswerItemDelegate* delegate = nullptr;
    ImageView* getImgWith(string path_img, int type);
    int _type = 0;

    void onChoose(Ref* sender);
    Text* answer_lb = nullptr, * num_lb = nullptr;
    ImageView* thumb = nullptr;
    std::string getStrABCDWithIndex(int index);
    void onClickSpeaker(Ref* sender);
    bool isChooseItem = false;
    Button* btspeaker = nullptr;

};

CREATE_READER(MJGameQA_AnswerItem);

#endif /* GameQA_AnswerItem_hpp */
